/*This file is part of FFB Arcade Plugin.
FFB Arcade Plugin is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
FFB Arcade Plugin is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with FFB Arcade Plugin.If not, see < https://www.gnu.org/licenses/>.
*/

// The emulators' network outputs: a TCP server that sends text lines "name = value" to every
// client. MAME, Supermodel and Flycast use one output protocol at a time, so when another program
// (BackForceFeeder, MAMEHooker...) needs the network outputs, nothing arrives through MAME64.dll.
// This client turns the lines into the calls the Windows protocol makes, so the game code does
// not care which protocol the emulator uses.
//
//   MAME ("-output network"): TCP port 8000, lines ending with '\r', values in decimal.
//     "mame_start = <rom name>" when a game starts and to each client that connects,
//     "pause = 0|1", "mame_stop = 1" when the game stops.
//   Supermodel (njz3): the same, but m3ffb and m2ffb (the drive board's command byte) are in
//     hexadecimal, and the game is also announced by a UDP broadcast on port 8001:
//     "mame_start = <game>\r" then "tcp = <TCP port>\r". BackForceFeeder listens for that
//     broadcast to find the TCP port; so does this client.
//   Flycast: TCP port 8000, lines ending with '\n', every value in hexadecimal, the game sent
//     as "game = <game id>" before the first output.

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MAMENetworkOutputs.h"
#include "../Common Files/GameIds.h"

#pragma comment(lib, "ws2_32.lib")

// handlers of the Windows protocol, in MAMESupermodel.cpp
int __stdcall mame_copydata(int id, const char* name);
int __stdcall mame_stop(void);
int __stdcall mame_updatestate(const char* id, int state);

static int GameId;
// the TCP port to connect to: 8000, or what the last UDP broadcast announced
static volatile LONG TcpPort = 8000;

static SOCKET ConnectToEmulator(int port)
{
	// MAME listens on the IPv6 wildcard address, which on Windows does not accept IPv4
	// connections, so try IPv6 first; Supermodel and Flycast listen on IPv4
	static const char* const hosts[] = { "::1", "127.0.0.1" };
	char service[8];
	sprintf_s(service, "%d", port);
	for (const char* host : hosts)
	{
		addrinfo hints = {};
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_NUMERICHOST;
		addrinfo* address = NULL;
		if (getaddrinfo(host, service, &hints, &address) != 0)
			continue;
		SOCKET s = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
		bool connected = s != INVALID_SOCKET && connect(s, address->ai_addr, (int)address->ai_addrlen) == 0;
		freeaddrinfo(address);
		if (connected)
			return s;
		if (s != INVALID_SOCKET)
			closesocket(s);
	}
	return INVALID_SOCKET;
}

static bool ValueIsHex(const char* name)
{
	if (GameId == FLYCAST)
		return true;
	return strcmp(name, "m3ffb") == 0 || strcmp(name, "m2ffb") == 0;
}

static void HandleLine(char* line)
{
	char* separator = strstr(line, " = ");
	if (separator == NULL)
		return;
	*separator = '\0';
	const char* name = line;
	const char* value = separator + 3;

	if (strcmp(name, "mame_start") == 0 || strcmp(name, "game") == 0)
	{
		// the Windows protocol gives the game's name as "game = <name>" with id 0
		char game[300];
		sprintf_s(game, "game = %.250s", value);
		mame_copydata(0, game);
	}
	else if (strcmp(name, "mame_stop") == 0)
	{
		if (strtol(value, NULL, 10) != 0) // Supermodel also sends "mame_stop = 0"
			mame_stop();
	}
	else if (strcmp(name, "pause") != 0) // the Windows protocol does not pass pauses either
	{
		mame_updatestate(name, (int)strtoul(value, NULL, ValueIsHex(name) ? 16 : 10));
	}
}

// Cuts the bytes received into lines ending with '\r' or '\n' and hands each one to handle.
// Returns how many bytes of an unfinished line are left at the start of the buffer.
static size_t SplitLines(char* buffer, size_t used, size_t size, void (*handle)(char* line))
{
	size_t start = 0;
	for (size_t i = 0; i < used; i++)
	{
		if (buffer[i] == '\r' || buffer[i] == '\n')
		{
			buffer[i] = '\0';
			if (i > start)
				handle(buffer + start);
			start = i + 1;
		}
	}
	memmove(buffer, buffer + start, used - start);
	used -= start;
	if (used == size - 1) // a line longer than the buffer: drop it
		used = 0;
	return used;
}

static DWORD WINAPI TcpThread(LPVOID)
{
	static char buffer[4096];
	for (;;)
	{
		// the emulator opens its server only with its network outputs on, and again for each game
		// it runs, so keep trying
		SOCKET s = ConnectToEmulator(TcpPort);
		if (s == INVALID_SOCKET)
		{
			Sleep(1000);
			continue;
		}

		size_t used = 0;
		for (;;)
		{
			int received = recv(s, buffer + used, (int)(sizeof(buffer) - 1 - used), 0);
			if (received <= 0)
				break;
			used = SplitLines(buffer, used + received, sizeof(buffer), HandleLine);
		}
		closesocket(s);
		Sleep(1000);
	}
}

static void HandleBroadcastLine(char* line)
{
	char* separator = strstr(line, " = ");
	if (separator == NULL)
		return;
	*separator = '\0';
	if (strcmp(line, "tcp") == 0)
	{
		int port = (int)strtol(separator + 3, NULL, 10);
		if (port > 0 && port < 65536)
			InterlockedExchange(&TcpPort, port);
	}
	// the game's name ("mame_start") comes again through TCP once connected
}

static DWORD WINAPI UdpThread(LPVOID lpParam)
{
	int port = (int)(INT_PTR)lpParam;
	static char buffer[1024];
	for (;;)
	{
		// Only one program can hold the port: BackForceFeeder, when it runs on the same PC, may
		// have it, so keep trying. Without the broadcast, the TCP port stays 8000.
		SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		sockaddr_in local = {};
		local.sin_family = AF_INET;
		local.sin_addr.s_addr = htonl(INADDR_ANY);
		local.sin_port = htons((u_short)port);
		if (s == INVALID_SOCKET || bind(s, (sockaddr*)&local, sizeof(local)) != 0)
		{
			if (s != INVALID_SOCKET)
				closesocket(s);
			Sleep(5000);
			continue;
		}
		for (;;)
		{
			int received = recvfrom(s, buffer, (int)sizeof(buffer) - 1, 0, NULL, NULL);
			if (received <= 0)
				break;
			buffer[received] = '\n';
			SplitLines(buffer, received + 1, sizeof(buffer), HandleBroadcastLine);
		}
		closesocket(s);
		Sleep(1000);
	}
}

void StartMameNetworkOutputs(int gameId)
{
	GameId = gameId;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return;
	CreateThread(NULL, 0, TcpThread, NULL, 0, NULL);
	// the port of the broadcast that announces the TCP port, 0 to not listen for it
	int udpPort = GetPrivateProfileInt(TEXT("Settings"), TEXT("NetworkOutputsUdpPort"), 8001, TEXT(".\\FFBPlugin.ini"));
	if (udpPort > 0 && udpPort < 65536)
		CreateThread(NULL, 0, UdpThread, (LPVOID)(INT_PTR)udpPort, 0, NULL);
}
