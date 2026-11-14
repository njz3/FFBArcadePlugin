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

#include "Transformers.h"
#include "../Core/Devices.h"

using namespace ffb;

bool Transformers::StopsShots(UINT8 gun)
{
	return (gun == 0x3) || (gun == 0x90) || (gun == 0x93) || (gun == 0x02) || (gun == 0x12);
}

void Transformers::Update()
{
	INT_PTR players = Memory::Read(Players);
	INT_PTR players1p = Memory::Read<INT_PTR>(players + 0x34);
	INT_PTR player1 = Memory::Read<INT_PTR>(players1p + 0x39C);
	UINT8 health1p = Memory::Read<UINT8>(player1 + 0x54);

	INT_PTR players2p = Memory::Read<INT_PTR>(players + 0x20);
	INT_PTR players2p2 = Memory::Read<INT_PTR>(players2p + 0x398);
	INT_PTR player2 = Memory::Read<INT_PTR>(players2p2 + 0x1C);
	UINT8 health2p = Memory::Read<UINT8>(player2 + 0x310);

	UINT8 gun = Memory::Read(Gun);
	UINT8 trigger1p = Memory::Read(Trigger1p);
	UINT8 trigger2p = Memory::Read(Trigger2p);

	if (!devicesOpen)
	{
		OpenHapticDevice2();
		devicesOpen = true;
	}

	UINT32 length_ms = feedbackLength;

	if (gun == 0x4B)
	{
		shooting1p = true;
		shooting2p = true;
	}
	else if (gun == 0x42)
	{
		shooting1p = true;
		shooting2p = false;
	}
	else if (gun == 0x09)
	{
		shooting1p = false;
		shooting2p = true;
	}

	// the rumble of the last shot is played even when the shots stop
	if (shooting1p)
	{
		if (trigger1p == 0)
			shooting1p = false;
		if (StopsShots(gun))
			shooting1p = false;

		RumbleWithMode(wheel, howtoRumbleGunEffect, gun1pStrength / 100.0, length_ms);
	}

	if (shooting2p)
	{
		if (trigger2p == 0)
			shooting2p = false;
		if (StopsShots(gun))
			shooting2p = false;

		RumbleWithMode(device2, howtoRumbleGunEffect, gun2pStrength / 100.0, length_ms);
	}

	if (lastHealth1p != health1p)
	{
		if (health1p != 100)
			RumbleWithMode(wheel, howtoRumbleHealthEffect, health1pStrength / 100.0, length_ms);
	}

	if (lastHealth2p != health2p)
	{
		if (health2p != 100)
			RumbleWithMode(device2, howtoRumbleHealthEffect, health2pStrength / 100.0, length_ms);
	}

	lastHealth1p = health1p;
	lastHealth2p = health2p;
}
