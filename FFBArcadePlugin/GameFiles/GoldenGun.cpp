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

#include "GoldenGun.h"
#include "../Core/Devices.h"

using namespace ffb;

void GoldenGun::Update()
{
	INT_PTR player1 = Memory::Read(Player1);
	UINT8 health1p = Memory::Read<UINT8>(player1 + HealthOffset);
	UINT8 ammo1p = Memory::Read<UINT8>(player1 + AmmoOffset);
	UINT8 state1p = Memory::Read<UINT8>(player1 + StateOffset);
	UINT8 startButton1p = Memory::Read(StartButton1p);

	INT_PTR player2 = Memory::Read(Player2);
	UINT8 health2p = Memory::Read<UINT8>(player2 + HealthOffset);
	UINT8 ammo2p = Memory::Read<UINT8>(player2 + AmmoOffset);
	UINT8 state2p = Memory::Read<UINT8>(player2 + StateOffset);
	UINT8 startButton2p = Memory::Read(StartButton2p);

	if (!devicesOpen)
	{
		OpenHapticDevice2();
		devicesOpen = true;
	}

	UINT32 length = feedbackLength;
	if (!noHealth1pPlayed)
	{
		if ((health1p == 0) && (state1p == 12 || state1p == 14))
		{
			RumbleWithMode(wheel, howtoRumbleHealthEffect, health1pStrength / 100.0, length);
			noHealth1pPlayed = true;
		}
	}

	if (state1p == 3)
	{
		if (startButton1p != 0x80)
		{
			if (lastHealth1p != health1p)
			{
				RumbleWithMode(wheel, howtoRumbleHealthEffect, health1pStrength / 100.0, length);
				noHealth1pPlayed = false;
			}
		}

		if (lastAmmo1p != ammo1p)
			RumbleWithMode(wheel, howtoRumbleGunEffect, gun1pStrength / 100.0, length);
	}

	if (!noHealth2pPlayed)
	{
		if ((health2p == 0) && (state2p == 12 || state2p == 14))
		{
			RumbleWithMode(device2, howtoRumbleHealthEffect, health2pStrength / 100.0, length);
			noHealth2pPlayed = true;
		}
	}

	if (state2p == 3)
	{
		if (startButton2p != 0x80)
		{
			if (lastHealth2p != health2p)
				RumbleWithMode(device2, howtoRumbleHealthEffect, health2pStrength / 100.0, length);
		}

		if (lastAmmo2p != ammo2p)
			RumbleWithMode(device2, howtoRumbleGunEffect, gun2pStrength / 100.0, length);
	}

	lastHealth1p = health1p;
	lastAmmo1p = ammo1p;
	lastHealth2p = health2p;
	lastAmmo2p = ammo2p;
}
