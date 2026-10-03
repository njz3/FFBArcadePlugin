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
#include "Rambo.h"
#include "../Common Files/Devices.h"
#include "SDL.h"
static EffectTriggers* myTriggers;
static EffectConstants* myConstants;
static Helpers* myHelpers;
extern HINSTANCE gl_cgGLDll;
static bool init = false;

static int configFeedbackLength = GetSettingInt(TEXT("FeedbackLength"), 120);
static int HowtoRumbleGunEffect = GetSettingInt(TEXT("HowtoRumbleGunEffect"), 0);
static int HowtoRumbleRageEffect = GetSettingInt(TEXT("HowtoRumbleRageEffect"), 0);
static int HowtoRumbleHealthEffect = GetSettingInt(TEXT("HowtoRumbleHealthEffect"), 0);
static int Gun1pStrength = GetSettingInt(TEXT("Gun1pStrength"), 0);
static int Rage1pStrength = GetSettingInt(TEXT("Rage1pStrength"), 0);
static int Health1pStrength = GetSettingInt(TEXT("Health1pStrength"), 0);
static int Gun2pStrength = GetSettingInt(TEXT("Gun2pStrength"), 0);
static int Rage2pStrength = GetSettingInt(TEXT("Rage2pStrength"), 0);
static int Health2pStrength = GetSettingInt(TEXT("Health2pStrength"), 0);

void Rambo::FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers) {

	INT_PTR Base1p = helpers->ReadIntPtr((INT_PTR)gl_cgGLDll + 0x1C4E8, false);
	INT_PTR Base11p = helpers->ReadIntPtr(Base1p + 0x5BC, false);
	INT_PTR Base21p = helpers->ReadIntPtr(Base11p + 0x77C, false);
	INT_PTR Base31p = helpers->ReadIntPtr(Base21p + 0x0, false);
	INT_PTR Base41p = helpers->ReadIntPtr(Base31p + 0x20, false);
	UINT8 Health1p = helpers->ReadByte(Base41p + 0x3C, false);
	UINT8 Ammo1p = helpers->ReadByte(Base41p + 0x400, false);
	float Rage1p = helpers->ReadFloat32(Base41p + 0x68, false);

	INT_PTR Base2p = helpers->ReadIntPtr((INT_PTR)gl_cgGLDll + 0x9144, false);
	INT_PTR Base12p = helpers->ReadIntPtr(Base2p + 0x4F4, false);
	INT_PTR Base22p = helpers->ReadIntPtr(Base12p + 0x278, false);
	INT_PTR Base32p = helpers->ReadIntPtr(Base22p + 0x1B0, false);
	INT_PTR Base42p = helpers->ReadIntPtr(Base32p + 0x38, false);
	UINT8 Health2p = helpers->ReadByte(Base42p + 0x3C, false);
	UINT8 Ammo2p = helpers->ReadByte(Base42p + 0x400, false);
	float Rage2p = helpers->ReadFloat32(Base42p + 0x68, false);

	static UINT8 oldHealth1p = 0;
	static UINT8 oldAmmo1p = 0;
	static UINT8 oldHealth2p = 0;
	static UINT8 oldAmmo2p = 0;
	static float oldRage1p = 0;
	static float oldRage2p = 0;
	UINT8 newHealth1p = Health1p;
	UINT8 newAmmo1p = Ammo1p;
	UINT8 newHealth2p = Health2p;
	UINT8 newAmmo2p = Ammo2p;
	float newRage1p = Rage1p;
	float newRage2p = Rage2p;

	if (!init)
	{
		OpenHapticDevice2();
		init = true;
	}

	UINT32 length_ms = configFeedbackLength;
	if (oldHealth1p != newHealth1p)
	{
		RumbleWithMode(triggers->Rumble, HowtoRumbleHealthEffect, Health1pStrength / 100.0, length_ms);
	}

	if (oldHealth2p != newHealth2p)
	{
		RumbleWithMode(triggers->RumbleDevice2, HowtoRumbleHealthEffect, Health2pStrength / 100.0, length_ms);
	}

	if (oldAmmo1p != newAmmo1p)
	{
		RumbleWithMode(triggers->Rumble, HowtoRumbleGunEffect, Gun1pStrength / 100.0, length_ms);
	}

	if (oldAmmo2p != newAmmo2p)
	{
		RumbleWithMode(triggers->RumbleDevice2, HowtoRumbleGunEffect, Gun2pStrength / 100.0, length_ms);
	}

	if (oldRage1p != newRage1p)
	{
		RumbleWithMode(triggers->Rumble, HowtoRumbleRageEffect, Rage1pStrength / 100.0, length_ms);
	}

	if (oldRage2p != newRage2p)
	{
		RumbleWithMode(triggers->RumbleDevice2, HowtoRumbleRageEffect, Rage2pStrength / 100.0, length_ms);
	}

	oldHealth1p = newHealth1p;
	oldAmmo1p = newAmmo1p;
	oldRage1p = newRage1p;
	oldHealth2p = newHealth2p;
	oldAmmo2p = newAmmo2p;
	oldRage2p = newRage2p;
}
