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

#include <stdlib.h>
#include "Daytona3.h"

using namespace ffb;

Daytona3Game::Daytona3Game(const Addresses& addresses)
	: addresses(addresses)
{
}

void Daytona3Game::PlayDriveBoard()
{
	UINT8 command = Memory::Read(GameValue<UINT8>{ Base::Module, addresses.driveBoardCommand });
	UINT8 gameState = Memory::Read(GameValue<UINT8>{ Base::Module, addresses.gameState });
	DWORD trackData = Memory::Read(GameValue<int>{ Base::Module, addresses.trackData });
	UINT8 track = Memory::Read<UINT8>(trackData + 0x4);

	// the rolls are the other way on the tracks 2 and 4, in a race
	bool reversed = (track == 2 || track == 4) && gameState == 0x16;

	if (command > 0x80 && command <= 0x8F) // spring (meaning unknown)
		wheel.Spring((144 - command) / 15.0);

	if (command > 0x90 && command <= 0x9F) // Roll Right
		wheel.ConstantWithRumble(reversed ? Direction::FromRight : Direction::FromLeft, (160 - command) / 15.0);

	if (command > 0xA0 && command <= 0xAF) // Roll Left
		wheel.ConstantWithRumble(reversed ? Direction::FromLeft : Direction::FromRight, (176 - command) / 15.0);

	if (command > 0xB0 && command <= 0xBF) // Shaking across grass etc
	{
		double strength = (192 - command) / 15.0;
		wheel.Sine(180, 0, strength, 100);
		wheel.Rumble(strength, strength, 100);
	}

	if (command > 0xC0 && command <= 0xCF) // Spring + Rumble on wheel while racing
	{
		wheel.Spring((207 - command) / 15.0);
		wheel.Sine(40, 0, (command - 192) / 96.0, 100);
	}

	// 0xD1-0xDF: wheel loose as tyres spin (no effect)

	if (command > 0xE0 && command <= 0xEF) // Big crash ?
		wheel.Spring((command - 224) / 15.0);

	if (command > 0xF0 && command <= 0xFF) // Menu only
		wheel.Spring((command - 240) / 15.0);
}

void Daytona3::Update()
{
	HWND window = FindWindowA(0, ("Daytona Championship USA"));

	if (hideCursor)
		ShowCursor(false);

	if (GetAsyncKeyState(VK_ESCAPE) && escapeKeyExitViaPlugin)
	{
		if (window > NULL)
		{
			system("taskkill /f /im InpWrapper.exe");
			ExitProcess(0);
		}
	}

	ApplyDamper();

	if (originalFFB)
	{
		PlayDriveBoard();
	}
	else
	{
		UINT8 ff = Memory::Read(WheelForce);

		if (ff > 15)
			wheel.ConstantWithRumble(Direction::FromLeft, (31 - ff) / 15.0);
		else if (ff > 0)
			wheel.ConstantWithRumble(Direction::FromRight, (16 - ff) / 15.0);
	}
}
