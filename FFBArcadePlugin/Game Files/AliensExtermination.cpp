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
#include "../Common Files/Devices.h"
#include "SDL.h"
#include <string>
#include <Windows.h>
static bool init = false;

static int EscapeKeyExitViaPlugin = GetSettingInt(TEXT("EscapeKeyExitViaPlugin"), 0);
static int configFeedbackLength = GetSettingInt(TEXT("FeedbackLength"), 120);
static int HowtoRumbleGunEffect = GetSettingInt(TEXT("HowtoRumbleGunEffect"), 0);
static int HowtoRumbleFlameEffect = GetSettingInt(TEXT("HowtoRumbleFlameEffect"), 0);
static int HowtoRumbleHealthEffect = GetSettingInt(TEXT("HowtoRumbleHealthEffect"), 0);
static int Gun1pStrength = GetSettingInt(TEXT("Gun1pStrength"), 0);
static int Flame1pStrength = GetSettingInt(TEXT("Flame1pStrength"), 0);
static int Health1pStrength = GetSettingInt(TEXT("Health1pStrength"), 0);
static int Gun2pStrength = GetSettingInt(TEXT("Gun2pStrength"), 0);
static int Flame2pStrength = GetSettingInt(TEXT("Flame2pStrength"), 0);
static int Health2pStrength = GetSettingInt(TEXT("Health2pStrength"), 0);

static bool Ammo1pBool = false;
static bool Ammo2pBool = false;
static bool Flame1pBool = false;
static bool Flame2pBool = false;
static bool Health1pBool = false;
static bool Health2pBool = false;

void AliensExtermination::FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers) {

	UINT8 Ammo1pA = helpers->ReadByte(0x4E7AEA0, true);
	UINT8 Ammo1pB = helpers->ReadByte(0x4E7AEA1, true);
	float Flame1p = helpers->ReadFloat32(0x556CB30, true);
	float Health1p = helpers->ReadFloat32(0x556C884, true);
	UINT8 Ammo2pA = helpers->ReadByte(0x4E7AEA2, true);
	UINT8 Ammo2pB = helpers->ReadByte(0x4E7AEA3, true);
	float Flame2p = helpers->ReadFloat32(0x556CEC4, true);
	float Health2p = helpers->ReadFloat32(0x556CC18, true);
	UINT8 Shoot1p = helpers->ReadByte(0x556ACBC, true);
	UINT8 Shoot2p = helpers->ReadByte(0x556AFD8, true);

	HWND hWnd = FindWindowA(0, ("GLUT"));

	if (GetAsyncKeyState((VK_ESCAPE)) && (EscapeKeyExitViaPlugin == 1))
	{
		if (hWnd > NULL)
		{
			ExitProcess(0);
		}
	}

	if (!init)
	{
		OpenHapticDevice2();
		init = true;
	}
	
	static UINT8 oldAmmo = 0;
	UINT8 newAmmo = Ammo1pA;

	static UINT8 oldAmmo2 = 0;
	UINT8 newAmmo2 = Ammo2pA;

	static float oldFlame = 0;
	float newFlame = Flame1p;

	static float oldFlame2 = 0;
	float newFlame2 = Flame2p;

	static float oldHealth = 0;
	float newHealth = Health1p;

	static float oldHealth2 = 0;
	float newHealth2 = Health2p;

	static UINT8 oldShoot = 0;
	UINT8 newShoot = Shoot1p;

	static UINT8 oldShoot2 = 0;
	UINT8 newShoot2 = Shoot2p;
	
	UINT32 length = configFeedbackLength;

	if (oldAmmo != newAmmo)
	{
		if (Ammo1pBool)
		{
			if (Ammo1pB >= 0 && Ammo1pA > 0)
			{
				RumbleWithMode(triggers->Rumble, HowtoRumbleGunEffect, Gun1pStrength / 100.0, length);
			}
		}

		if (Ammo1pA == 0 && Ammo1pB == 0)
			Ammo1pBool = false;
		else
			Ammo1pBool = true;
	}

	if (oldShoot != newShoot)
	{
		if ((Ammo1pA == 0 && Ammo1pB == 0) || (Ammo1pA == 10 && Ammo1pB == 10))
		{ 
			if (Shoot1p & 0x10)
			{
				RumbleWithMode(triggers->Rumble, HowtoRumbleGunEffect, Gun1pStrength / 100.0, length);
			}
		}
	}

	if (oldAmmo2 != newAmmo2)
	{
		if (Ammo2pBool)
		{
			if (Ammo2pB >= 0 && Ammo2pA > 0)
			{
				RumbleWithMode(triggers->RumbleDevice2, HowtoRumbleGunEffect, Gun2pStrength / 100.0, length);
			}
		}

		if (Ammo2pA == 0 && Ammo2pB == 0)
			Ammo2pBool = false;
		else
			Ammo2pBool = true;
	}

	if (oldShoot2 != newShoot2)
	{
		if ((Ammo2pA == 0 && Ammo2pB == 0) || (Ammo2pA == 10 && Ammo2pB == 10))
		{ 
			if (Shoot2p & 0x10)
			{
				RumbleWithMode(triggers->RumbleDevice2, HowtoRumbleGunEffect, Gun2pStrength / 100.0, length);
			}
		}
	}

	if (oldFlame != newFlame)
	{
		if (Flame1pBool)
		{
			if (Flame1p > 0)
			{
				RumbleWithMode(triggers->Rumble, HowtoRumbleFlameEffect, Flame1pStrength / 100.0, length);
			}
		}

		if ((Flame1p <= 0) )
			Flame1pBool = false;
		else
			Flame1pBool = true;
	}

	if (oldFlame2 != newFlame2)
	{
		if (Flame2pBool)
		{
			if (Flame2p > 0)
			{
				RumbleWithMode(triggers->RumbleDevice2, HowtoRumbleFlameEffect, Flame2pStrength / 100.0, length);
			}
		}

		if ((Flame2p <= 0))
			Flame2pBool = false;
		else
			Flame2pBool = true;
	}

	if (oldHealth != newHealth)
	{
		if (Health1pBool)
		{
			if (Health1p > 0)
			{
				RumbleWithMode(triggers->Rumble, HowtoRumbleHealthEffect, Health1pStrength / 100.0, length);
			}
		}

		if ((Health1p == 0))
			Health1pBool = false;
		else
			Health1pBool = true;
	}

	if (oldHealth2 != newHealth2)
	{
		if (Health2pBool)
		{
			if (Health2p > 0)
			{
				RumbleWithMode(triggers->RumbleDevice2, HowtoRumbleHealthEffect, Health2pStrength / 100.0, length);
			}
		}

		if ((Health2p == 0))
			Health2pBool = false;
		else
			Health2pBool = true;
	}

	oldAmmo = newAmmo;
	oldAmmo2 = newAmmo2;
	oldFlame = newFlame;
	oldFlame2 = newFlame2;
	oldHealth = newHealth;
	oldHealth2 = newHealth2;
	oldShoot = newShoot;
	oldShoot2 = newShoot2;
}
