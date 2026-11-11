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

// MAME's network output module ("-output network") is a TCP server on port 8000. It sends
// text lines ending with '\r' to every client:
//   mame_start = <rom name>     when a game starts, and to each client that connects
//   <output name> = <value>     each time an output changes
//   pause = 0|1
//   mame_stop = 1               when the game stops
// MAME uses only one output module, so with "-output network" nothing arrives through
// MAME64.dll. This client turns the lines into the calls the Windows protocol makes, so the
// game code does not care which protocol MAME uses.

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MAMENetworkOutputs.h"

#pragma comment(lib, "ws2_32.lib")

// handlers of the Windows protocol, in MAMESupermodel.cpp
int __stdcall mame_copydata(int id, const char* name);
int __stdcall mame_stop(void);
int __stdcall mame_updatestate(const char* id, int state);

static SOCKET ConnectToMame()
{
	// MAME listens on the IPv6 wildcard address, which on Windows does not accept IPv4
	// connections, so try IPv6 first
	static const char* const hosts[] = { "::1", "127.0.0.1" };
	for (const char* host : hosts)
	{
		addrinfo hints = {};
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_NUMERICHOST;
		addrinfo* address = NULL;
		if (getaddrinfo(host, "8000", &hints, &address) != 0)
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

static void HandleLine(char* line)
{
	char* separator = strstr(line, " = ");
	if (separator == NULL)
		return;
	*separator = '\0';
	const char* name = line;
	const char* value = separator + 3;

	if (strcmp(name, "mame_start") == 0)
	{
		// the Windows protocol gives the rom name as "game = <rom name>" with id 0
		char game[300];
		sprintf_s(game, "game = %.250s", value);
		mame_copydata(0, game);
	}
	else if (strcmp(name, "mame_stop") == 0)
	{
		mame_stop();
	}
	else if (strcmp(name, "pause") != 0) // the Windows protocol does not pass pauses either
	{
		mame_updatestate(name, (int)strtol(value, NULL, 10));
	}
}

static DWORD WINAPI NetworkOutputsThread(LPVOID)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	static char buffer[4096];
	for (;;)
	{
		// MAME opens the port only when started with "-output network", and again for each
		// game it runs, so keep trying
		SOCKET s = ConnectToMame();
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
			used += received;

			size_t start = 0;
			for (size_t i = 0; i < used; i++)
			{
				if (buffer[i] == '\r' || buffer[i] == '\n')
				{
					buffer[i] = '\0';
					if (i > start)
						HandleLine(buffer + start);
					start = i + 1;
				}
			}
			memmove(buffer, buffer + start, used - start);
			used -= start;
			if (used == sizeof(buffer) - 1) // a line longer than the buffer: drop it
				used = 0;
		}
		closesocket(s);
		Sleep(1000);
	}
}

void StartMameNetworkOutputs()
{
	CreateThread(NULL, 0, NetworkOutputsThread, NULL, 0, NULL);
}
