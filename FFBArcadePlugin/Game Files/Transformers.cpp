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
#include "Transformers.h"
#include "../Common Files/Devices.h"
#include "SDL.h"
static EffectTriggers* myTriggers;
static EffectConstants* myConstants;
static Helpers* myHelpers;
static bool init = false;
static bool Player1Shoot = false;
static bool Player2Shoot = false;

static int configFeedbackLength = GetSettingInt(TEXT("FeedbackLength"), 120);
static int HowtoRumbleGunEffect = GetSettingInt(TEXT("HowtoRumbleGunEffect"), 0);
static int HowtoRumbleHealthEffect = GetSettingInt(TEXT("HowtoRumbleHealthEffect"), 0);
static int Gun1pStrength = GetSettingInt(TEXT("Gun1pStrength"), 0);
static int Health1pStrength = GetSettingInt(TEXT("Health1pStrength"), 0);
static int Gun2pStrength = GetSettingInt(TEXT("Gun2pStrength"), 0);
static int Health2pStrength = GetSettingInt(TEXT("Health2pStrength"), 0);

void Transformers::FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers) {

	INT_PTR Base1p = helpers->ReadIntPtr(0x1A3CC1C, true);
	INT_PTR Base11p = helpers->ReadIntPtr(Base1p + 0x34, false);
	INT_PTR Base21p = helpers->ReadIntPtr(Base11p + 0x39C, false);
	UINT8 Health1p = helpers->ReadByte(Base21p + 0x54, false);

	INT_PTR Base12p = helpers->ReadIntPtr(Base1p + 0x20, false);
	INT_PTR Base22p = helpers->ReadIntPtr(Base12p + 0x398, false);
	INT_PTR Base32p = helpers->ReadIntPtr(Base22p + 0x1C, false);
	UINT8 Health2p = helpers->ReadByte(Base32p + 0x310, false);

	UINT8 Gun = helpers->ReadByte(0xD8CE14, false);
	UINT8 Trigger1p = helpers->ReadByte(0xD8D110, false);
	UINT8 Trigger2p = helpers->ReadByte(0xD8D112, false);

	static UINT8 oldHealth1p = 0;
	static UINT8 oldHealth2p = 0;
	UINT8 newHealth1p = Health1p;
	UINT8 newHealth2p = Health2p;
	
	if (!init)
	{
		OpenHapticDevice2();
		init = true;
	}
	
	
	UINT32 length_ms = configFeedbackLength;

	if (Gun == 0x4B)
	{
		Player1Shoot = true;
		Player2Shoot = true;
	}
	else if (Gun == 0x42)
	{
		Player1Shoot = true;
		Player2Shoot = false;
	}
	else if (Gun == 0x09)
	{
		Player1Shoot = false;
		Player2Shoot = true;
	}

	if (Player1Shoot)
	{
		if (Trigger1p == 0)
		{
			Player1Shoot = false;
		}

		if ((Gun == 0x3) || (Gun == 0x90) || (Gun == 0x93) || (Gun == 0x02) || (Gun == 0x12))
		{
			Player1Shoot = false;
		}

		double percentForce = ((Gun1pStrength) / 100.0);
		RumbleWithMode(triggers->Rumble, HowtoRumbleGunEffect, percentForce, length_ms);
	}

	if (Player2Shoot)
	{
		if (Trigger2p == 0)
		{
			Player2Shoot = false;
		}

		if ((Gun == 0x3) || (Gun == 0x90) || (Gun == 0x93) || (Gun == 0x02) || (Gun == 0x12))
		{
			Player2Shoot = false;
		}

		double percentForce = ((Gun2pStrength) / 100.0);
		RumbleWithMode(triggers->RumbleDevice2, HowtoRumbleGunEffect, percentForce, length_ms);
	}
		
	if (oldHealth1p != newHealth1p)
	{
		if (Health1p != 100)
		{
			double percentForce = ((Health1pStrength) / 100.0);
			RumbleWithMode(triggers->Rumble, HowtoRumbleHealthEffect, percentForce, length_ms);
		}		
	}

	if (oldHealth2p != newHealth2p)
	{
		if (Health2p != 100)
		{
			double percentForce = ((Health2pStrength) / 100.0);
			RumbleWithMode(triggers->RumbleDevice2, HowtoRumbleHealthEffect, percentForce, length_ms);
		}		
	}

	oldHealth1p = newHealth1p;
	oldHealth2p = newHealth2p;
}
