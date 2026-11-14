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

#include "AliensExtermination.h"
#include "../Core/Devices.h"

using namespace ffb;

void AliensExtermination::Update()
{
	UINT8 ammo1pA = Memory::Read(Ammo1pA);
	UINT8 ammo1pB = Memory::Read(Ammo1pB);
	float flame1p = Memory::Read(Flame1p);
	float health1p = Memory::Read(Health1p);
	UINT8 ammo2pA = Memory::Read(Ammo2pA);
	UINT8 ammo2pB = Memory::Read(Ammo2pB);
	float flame2p = Memory::Read(Flame2p);
	float health2p = Memory::Read(Health2p);
	UINT8 shoot1p = Memory::Read(Shoot1p);
	UINT8 shoot2p = Memory::Read(Shoot2p);

	HWND hWnd = FindWindowA(0, ("GLUT"));

	if (GetAsyncKeyState((VK_ESCAPE)) && (escapeKeyExitViaPlugin == 1))
	{
		if (hWnd > NULL)
		{
			ExitProcess(0);
		}
	}

	if (!devicesOpen)
	{
		OpenHapticDevice2();
		devicesOpen = true;
	}

	UINT32 length = feedbackLength;

	if (lastAmmo != ammo1pA)
	{
		if (hadAmmo1p)
		{
			if (ammo1pB >= 0 && ammo1pA > 0)
				RumbleWithMode(wheel, howtoRumbleGunEffect, gun1pStrength / 100.0, length);
		}

		if (ammo1pA == 0 && ammo1pB == 0)
			hadAmmo1p = false;
		else
			hadAmmo1p = true;
	}

	if (lastShoot != shoot1p)
	{
		if ((ammo1pA == 0 && ammo1pB == 0) || (ammo1pA == 10 && ammo1pB == 10))
		{
			if (shoot1p & 0x10)
				RumbleWithMode(wheel, howtoRumbleGunEffect, gun1pStrength / 100.0, length);
		}
	}

	if (lastAmmo2 != ammo2pA)
	{
		if (hadAmmo2p)
		{
			if (ammo2pB >= 0 && ammo2pA > 0)
				RumbleWithMode(device2, howtoRumbleGunEffect, gun2pStrength / 100.0, length);
		}

		if (ammo2pA == 0 && ammo2pB == 0)
			hadAmmo2p = false;
		else
			hadAmmo2p = true;
	}

	if (lastShoot2 != shoot2p)
	{
		if ((ammo2pA == 0 && ammo2pB == 0) || (ammo2pA == 10 && ammo2pB == 10))
		{
			if (shoot2p & 0x10)
				RumbleWithMode(device2, howtoRumbleGunEffect, gun2pStrength / 100.0, length);
		}
	}

	if (lastFlame != flame1p)
	{
		if (hadFlame1p)
		{
			if (flame1p > 0)
				RumbleWithMode(wheel, howtoRumbleFlameEffect, flame1pStrength / 100.0, length);
		}

		if (flame1p <= 0)
			hadFlame1p = false;
		else
			hadFlame1p = true;
	}

	if (lastFlame2 != flame2p)
	{
		if (hadFlame2p)
		{
			if (flame2p > 0)
				RumbleWithMode(device2, howtoRumbleFlameEffect, flame2pStrength / 100.0, length);
		}

		if (flame2p <= 0)
			hadFlame2p = false;
		else
			hadFlame2p = true;
	}

	if (lastHealth != health1p)
	{
		if (hadHealth1p)
		{
			if (health1p > 0)
				RumbleWithMode(wheel, howtoRumbleHealthEffect, health1pStrength / 100.0, length);
		}

		if (health1p == 0)
			hadHealth1p = false;
		else
			hadHealth1p = true;
	}

	if (lastHealth2 != health2p)
	{
		if (hadHealth2p)
		{
			if (health2p > 0)
				RumbleWithMode(device2, howtoRumbleHealthEffect, health2pStrength / 100.0, length);
		}

		if (health2p == 0)
			hadHealth2p = false;
		else
			hadHealth2p = true;
	}

	lastAmmo = ammo1pA;
	lastAmmo2 = ammo2pA;
	lastFlame = flame1p;
	lastFlame2 = flame2p;
	lastHealth = health1p;
	lastHealth2 = health2p;
	lastShoot = shoot1p;
	lastShoot2 = shoot2p;
}
