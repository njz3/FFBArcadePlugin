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
#include "HOTD4.h"
#include "../Common Files/Devices.h"
#include "SDL.h"
#include <Windows.h>
static EffectTriggers *myTriggers;
static EffectConstants *myConstants;
static Helpers *myHelpers;
extern HINSTANCE gl_cgGLDll;
static bool HealthA = false;
static bool HealthB = false;
static bool init = false;

static int ThreadLoop()
{
	INT_PTR Base1p = myHelpers->ReadIntPtr((INT_PTR)gl_cgGLDll + 0x9164, false);
	INT_PTR Base11p = myHelpers->ReadIntPtr(Base1p + 0x11C, false);
	INT_PTR Base21p = myHelpers->ReadIntPtr(Base11p + 0x50, false);
	INT_PTR Base31p = myHelpers->ReadIntPtr(Base21p + 0x20, false);
	UINT8 Health1p = myHelpers->ReadByte(Base31p + 0x3C, false);
	UINT8 Bullet1p = myHelpers->ReadByte(Base31p + 0x274, false);
	UINT8 GrenadeExplode1p = myHelpers->ReadByte(Base31p + 0x2B3, false);
	INT_PTR Base2p = myHelpers->ReadIntPtr((INT_PTR)gl_cgGLDll + 0x9164, false);
	INT_PTR Base12p = myHelpers->ReadIntPtr(Base2p + 0x11C, false);
	INT_PTR Base22p = myHelpers->ReadIntPtr(Base12p + 0x50, false);
	INT_PTR Base32p = myHelpers->ReadIntPtr(Base22p + 0x38, false);
	UINT8 Health2p = myHelpers->ReadByte(Base32p + 0x3C, false);
	UINT8 Bullet2p = myHelpers->ReadByte(Base32p + 0x274, false);
	UINT8 GrenadeExplode2p = myHelpers->ReadByte(Base32p + 0x2B3, false);
	UINT8 IngameValue1p = myHelpers->ReadByte(Base31p + 0x38, false);
	UINT8 IngameValue2p = myHelpers->ReadByte(Base32p + 0x38, false);
	INT_PTR StartBase = myHelpers->ReadIntPtr((INT_PTR)gl_cgGLDll + 0x3C40, false);
	INT_PTR StartBase1 = myHelpers->ReadIntPtr(StartBase + 0x130, false);
	INT_PTR StartBase2 = myHelpers->ReadIntPtr(StartBase1 + 0x234, false);
	UINT8 StartButton1p = myHelpers->ReadByte(StartBase2 + 0x530, false);
	UINT8 StartButton2p = myHelpers->ReadByte(StartBase2 + 0x538, false);

	UINT8 static oldHealth1p = 0;
	UINT8 static oldBullet1p = 0;
	UINT8 static oldHealth2p = 0;
	UINT8 static oldBullet2p = 0;
	UINT8 newHealth1p = Health1p;
	UINT8 newBullet1p = Bullet1p;
	UINT8 newHealth2p = Health2p;
	UINT8 newBullet2p = Bullet2p;

	int configFeedbackLength = GetSettingInt(TEXT("FeedbackLength"), 120);
	int HowtoRumbleBulletEffect = GetSettingInt(TEXT("HowtoRumbleBulletEffect"), 0);
	int HowtoRumbleGrenadeEffect = GetSettingInt(TEXT("HowtoRumbleGrenadeEffect"), 0);
	int HowtoRumbleHealthEffect = GetSettingInt(TEXT("HowtoRumbleHealthEffect"), 0);
	int Bullet1pStrength = GetSettingInt(TEXT("Bullet1pStrength"), 0);
	int Grenade1pStrength = GetSettingInt(TEXT("Grenade1pStrength"), 0);
	int Health1pStrength = GetSettingInt(TEXT("Health1pStrength"), 0);
	int Bullet2pStrength = GetSettingInt(TEXT("Bullet2pStrength"), 0);
	int Grenade2pStrength = GetSettingInt(TEXT("Grenade2pStrength"), 0);
	int Health2pStrength = GetSettingInt(TEXT("Health2pStrength"), 0);

	UINT32 length_ms = configFeedbackLength;
	if ((Health1p == 0) && (IngameValue1p == 11))
	{
		if (!HealthA)
		{
			double percentForce = ((Health1pStrength) / 100.0);
			RumbleWithMode(myTriggers->Rumble, HowtoRumbleHealthEffect, percentForce, length_ms);
			HealthA = true;
		}
	}

	if ((Health2p == 0) && (IngameValue2p == 11))
	{
		if (!HealthB)
		{
			double percentForce = ((Health2pStrength) / 100.0);
			RumbleWithMode(myTriggers->RumbleDevice2, HowtoRumbleHealthEffect, percentForce, length_ms);
			HealthB = true;
		}
	}

	if ((IngameValue1p == 3) && (StartButton1p != 0x80))
	{
		if (oldHealth1p != newHealth1p)
		{
			double percentForce = ((Health1pStrength) / 100.0);
			RumbleWithMode(myTriggers->Rumble, HowtoRumbleHealthEffect, percentForce, length_ms);
		}
		if (oldBullet1p != newBullet1p)
		{
			double percentForce = ((Bullet1pStrength) / 100.0);
			RumbleWithMode(myTriggers->Rumble, HowtoRumbleBulletEffect, percentForce, length_ms);
		}
		if (GrenadeExplode1p == 0x42)
		{
			double percentForce = ((Grenade1pStrength) / 100.0);
			RumbleWithMode(myTriggers->Rumble, HowtoRumbleGrenadeEffect, percentForce, length_ms);
		}
		HealthA = false;
	}

	if ((IngameValue2p == 3) && (StartButton2p != 0x80))
	{
		if (oldHealth2p != newHealth2p)
		{
			double percentForce = ((Health2pStrength) / 100.0);
			RumbleWithMode(myTriggers->RumbleDevice2, HowtoRumbleHealthEffect, percentForce, length_ms);
		}
		if (oldBullet2p != newBullet2p)
		{
			double percentForce = ((Bullet2pStrength) / 100.0);
			RumbleWithMode(myTriggers->RumbleDevice2, HowtoRumbleBulletEffect, percentForce, length_ms);
		}
		if (GrenadeExplode2p == 0x42)
		{
			double percentForce = ((Grenade2pStrength) / 100.0);
			RumbleWithMode(myTriggers->RumbleDevice2, HowtoRumbleGrenadeEffect, percentForce, length_ms);
		}
		HealthB = false;
	}
	oldHealth1p = newHealth1p;
	oldBullet1p = newBullet1p;
	oldHealth2p = newHealth2p;
	oldBullet2p = newBullet2p;

	return 0;
}

static DWORD WINAPI RunningLoop(LPVOID lpParam)
{
	while (true)
	{
		ThreadLoop();
		Sleep(16);
	}
}

void HOTD4::FFBLoop(EffectConstants *constants, Helpers *helpers, EffectTriggers* triggers) {

	if (!init)
	{
		myTriggers = triggers;
		myConstants = constants;
		myHelpers = helpers;
		CreateThread(NULL, 0, RunningLoop, NULL, 0, NULL);

		OpenHapticDevice2();
		init = true;
	}
	
	myTriggers = triggers;
	myConstants = constants;
	myHelpers = helpers;
}