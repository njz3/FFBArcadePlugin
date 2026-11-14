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

#include "HOTD4.h"
#include "../Common Files/Devices.h"

using namespace ffb;

extern HINSTANCE gl_cgGLDll;

DWORD WINAPI HOTD4::RunningLoop(LPVOID game)
{
	while (true)
	{
		((HOTD4*)game)->ThreadLoop();
		Sleep(16);
	}
}

void HOTD4::ThreadLoop()
{
	INT_PTR cgGL = (INT_PTR)gl_cgGLDll;
	INT_PTR player1 = Memory::Read(PointerChain<INT_PTR, 3>{ { Base::Absolute, cgGL + PlayersPointer }, { 0x11C, 0x50, 0x20 } });
	UINT8 health1p = Memory::Read<UINT8>(player1 + HealthOffset);
	UINT8 bullets1p = Memory::Read<UINT8>(player1 + BulletsOffset);
	UINT8 grenade1p = Memory::Read<UINT8>(player1 + GrenadeOffset);
	INT_PTR player2 = Memory::Read(PointerChain<INT_PTR, 3>{ { Base::Absolute, cgGL + PlayersPointer }, { 0x11C, 0x50, 0x38 } });
	UINT8 health2p = Memory::Read<UINT8>(player2 + HealthOffset);
	UINT8 bullets2p = Memory::Read<UINT8>(player2 + BulletsOffset);
	UINT8 grenade2p = Memory::Read<UINT8>(player2 + GrenadeOffset);
	UINT8 state1p = Memory::Read<UINT8>(player1 + StateOffset);
	UINT8 state2p = Memory::Read<UINT8>(player2 + StateOffset);
	INT_PTR startButtons = Memory::Read(PointerChain<INT_PTR, 2>{ { Base::Absolute, cgGL + StartButtonsPointer }, { 0x130, 0x234 } });
	UINT8 startButton1p = Memory::Read<UINT8>(startButtons + StartButton1pOffset);
	UINT8 startButton2p = Memory::Read<UINT8>(startButtons + StartButton2pOffset);

	int feedbackLength = Settings::Int(L"FeedbackLength", 120);
	int howtoRumbleBulletEffect = Settings::Int(L"HowtoRumbleBulletEffect", 0);
	int howtoRumbleGrenadeEffect = Settings::Int(L"HowtoRumbleGrenadeEffect", 0);
	int howtoRumbleHealthEffect = Settings::Int(L"HowtoRumbleHealthEffect", 0);
	int bullet1pStrength = Settings::Int(L"Bullet1pStrength", 0);
	int grenade1pStrength = Settings::Int(L"Grenade1pStrength", 0);
	int health1pStrength = Settings::Int(L"Health1pStrength", 0);
	int bullet2pStrength = Settings::Int(L"Bullet2pStrength", 0);
	int grenade2pStrength = Settings::Int(L"Grenade2pStrength", 0);
	int health2pStrength = Settings::Int(L"Health2pStrength", 0);

	UINT32 length_ms = feedbackLength;
	if ((health1p == 0) && (state1p == 11))
	{
		if (!noHealth1pPlayed)
		{
			RumbleWithMode(wheel, howtoRumbleHealthEffect, health1pStrength / 100.0, length_ms);
			noHealth1pPlayed = true;
		}
	}

	if ((health2p == 0) && (state2p == 11))
	{
		if (!noHealth2pPlayed)
		{
			RumbleWithMode(device2, howtoRumbleHealthEffect, health2pStrength / 100.0, length_ms);
			noHealth2pPlayed = true;
		}
	}

	if ((state1p == 3) && (startButton1p != 0x80))
	{
		if (lastHealth1p != health1p)
			RumbleWithMode(wheel, howtoRumbleHealthEffect, health1pStrength / 100.0, length_ms);
		if (lastBullets1p != bullets1p)
			RumbleWithMode(wheel, howtoRumbleBulletEffect, bullet1pStrength / 100.0, length_ms);
		if (grenade1p == 0x42)
			RumbleWithMode(wheel, howtoRumbleGrenadeEffect, grenade1pStrength / 100.0, length_ms);
		noHealth1pPlayed = false;
	}

	if ((state2p == 3) && (startButton2p != 0x80))
	{
		if (lastHealth2p != health2p)
			RumbleWithMode(device2, howtoRumbleHealthEffect, health2pStrength / 100.0, length_ms);
		if (lastBullets2p != bullets2p)
			RumbleWithMode(device2, howtoRumbleBulletEffect, bullet2pStrength / 100.0, length_ms);
		if (grenade2p == 0x42)
			RumbleWithMode(device2, howtoRumbleGrenadeEffect, grenade2pStrength / 100.0, length_ms);
		noHealth2pPlayed = false;
	}

	lastHealth1p = health1p;
	lastBullets1p = bullets1p;
	lastHealth2p = health2p;
	lastBullets2p = bullets2p;
}

void HOTD4::Update()
{
	if (!started)
	{
		CreateThread(NULL, 0, RunningLoop, this, 0, NULL);
		OpenHapticDevice2();
		started = true;
	}
}
