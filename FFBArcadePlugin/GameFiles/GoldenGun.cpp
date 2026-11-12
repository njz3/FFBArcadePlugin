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

#include <string>
#include "GoldenGun.h"
#include "../Common Files/Devices.h"
#include "SDL.h"
static bool init = false;
static bool health1pbool = false;
static bool health2pbool = false;

static int configFeedbackLength = GetSettingInt(TEXT("FeedbackLength"), 120);
static int HowtoRumbleGunEffect = GetSettingInt(TEXT("HowtoRumbleGunEffect"), 0);
static int HowtoRumbleHealthEffect = GetSettingInt(TEXT("HowtoRumbleHealthEffect"), 0);
static int Gun1pStrength = GetSettingInt(TEXT("Gun1pStrength"), 0);
static int Health1pStrength = GetSettingInt(TEXT("Health1pStrength"), 0);
static int Gun2pStrength = GetSettingInt(TEXT("Gun2pStrength"), 0);
static int Health2pStrength = GetSettingInt(TEXT("Health2pStrength"), 0);

void GoldenGun::FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers) {

	INT_PTR Base1p = helpers->ReadIntPtr(0x27034A0, true);
	INT_PTR Base11p = helpers->ReadIntPtr(Base1p + 0x24, false);
	INT_PTR Base21p = helpers->ReadIntPtr(Base11p + 0x34, false);
	UINT8 Health1p = helpers->ReadByte(Base21p + 0x3C, false);
	UINT8 Ammo1p = helpers->ReadByte(Base21p + 0x6C, false);
	UINT8 Ingame1p = helpers->ReadByte(Base21p + 0x38, false);
	UINT8 StartButton1p = helpers->ReadByte(0x478F8C, true);

	INT_PTR Base2p = helpers->ReadIntPtr(0x2703388, true);
	INT_PTR Base12p = helpers->ReadIntPtr(Base2p + 0x28, false);
	INT_PTR Base22p = helpers->ReadIntPtr(Base12p + 0x38, false);
	UINT8 Health2p = helpers->ReadByte(Base22p + 0x3C, false);
	UINT8 Ammo2p = helpers->ReadByte(Base22p + 0x6C, false);
	UINT8 Ingame2p = helpers->ReadByte(Base22p + 0x38, false);
	UINT8 StartButton2p = helpers->ReadByte(0x478F94, true);

	static UINT8 oldHealth1p = 0;
	static UINT8 oldAmmo1p = 0;
	static UINT8 oldHealth2p = 0;
	static UINT8 oldAmmo2p = 0;

	UINT8 newHealth1p = Health1p;
	UINT8 newAmmo1p = Ammo1p;
	UINT8 newHealth2p = Health2p;
	UINT8 newAmmo2p = Ammo2p;

	if (!init)
	{
		OpenHapticDevice2();
		init = true;
	}

	UINT32 length = configFeedbackLength;
	if (!health1pbool)
	{
		if ((Health1p == 0) && (Ingame1p == 12 || Ingame1p == 14))
		{
			RumbleWithMode(triggers->Rumble, HowtoRumbleHealthEffect, Health1pStrength / 100.0, length);
			health1pbool = true;
		}
	}

	if (Ingame1p == 3)
	{
		if (StartButton1p != 0x80)
		{
			if (oldHealth1p != newHealth1p)
			{
				RumbleWithMode(triggers->Rumble, HowtoRumbleHealthEffect, Health1pStrength / 100.0, length);
				health1pbool = false;
			}
		}
		
		if (oldAmmo1p != newAmmo1p)
		{
			RumbleWithMode(triggers->Rumble, HowtoRumbleGunEffect, Gun1pStrength / 100.0, length);
		}
	}

	if (!health2pbool)
	{
		if ((Health2p == 0) && (Ingame2p == 12 || Ingame2p == 14))
		{
			UINT32 length_ms = configFeedbackLength;
			RumbleWithMode(triggers->RumbleDevice2, HowtoRumbleHealthEffect, Health2pStrength / 100.0, length_ms);
			health2pbool = true;
		}
	}

	if (Ingame2p == 3)
	{
		if (StartButton2p != 0x80)
		{
			if (oldHealth2p != newHealth2p)
			{
				RumbleWithMode(triggers->RumbleDevice2, HowtoRumbleHealthEffect, Health2pStrength / 100.0, length);
			}
		}
		
		if (oldAmmo2p != newAmmo2p)
		{
			RumbleWithMode(triggers->RumbleDevice2, HowtoRumbleGunEffect, Gun2pStrength / 100.0, length);
		}
	}

	oldHealth1p = newHealth1p;
	oldAmmo1p = newAmmo1p;
	oldHealth2p = newHealth2p;
	oldAmmo2p = newAmmo2p;
}
