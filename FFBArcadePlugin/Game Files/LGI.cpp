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
#include "LGI.h"
#include "../Common Files/Devices.h"
#include "SDL.h"
#include <Windows.h>
static bool init = false;

void LGI::FFBLoop(EffectConstants *constants, Helpers *helpers, EffectTriggers* triggers) {	
	INT_PTR ff = helpers->ReadIntPtr(0x0063BF5C, /* isRelativeOffset */ true);
	UINT8 ff1 = helpers->ReadByte(ff + 0x44, /* isRelativeOffset */ false);
	INT_PTR health1p1 = helpers->ReadIntPtr(0x00820024, /* isRelativeOffset*/ true);
	INT_PTR health1p2 = helpers->ReadIntPtr(health1p1 + 0x4, /* isRelativeOffset */ false);
	INT_PTR health1p3 = helpers->ReadIntPtr(health1p2 + 0x58, /* isRelativeOffset */ false);
	INT_PTR health2p3 = helpers->ReadIntPtr(health1p2 + 0x5C, /* isRelativeOffset */ false);
	float health1p = helpers->ReadFloat32(health1p3 + 0x14, /* isRelativeOffset */ false); //1p health
	float health2p = helpers->ReadFloat32(health2p3 + 0x14, /* isRelativeOffset */ false); //2p health
	helpers->log("got value: ");
	std::string ffs = std::to_string(ff1);
	helpers->log((char *)ffs.c_str());
	float static oldFloat1 = 0.0;
	float static oldFloat2 = 0.0;
	float newFloat1 = health1p;
	float newFloat2 = health2p;

	int configFeedbackLength = GetSettingInt(TEXT("FeedbackLength"), 120);
	int HowtoRumbleKnockEffect = GetSettingInt(TEXT("HowtoRumbleKnockEffect"), 0);
	int HowtoRumbleMotorEffect = GetSettingInt(TEXT("HowtoRumbleMotorEffect"), 0);
	int HowtoRumbleHealthEffect = GetSettingInt(TEXT("HowtoRumbleHealthEffect"), 0);
	int Knock1pStrength = GetSettingInt(TEXT("Knock1pStrength"), 0);
	int Motor1pStrength = GetSettingInt(TEXT("Motor1pStrength"), 0);
	int Health1pStrength = GetSettingInt(TEXT("Health1pStrength"), 0);
	int Knock2pStrength = GetSettingInt(TEXT("Knock2pStrength"), 0);
	int Motor2pStrength = GetSettingInt(TEXT("Motor2pStrength"), 0);
	int Health2pStrength = GetSettingInt(TEXT("Health2pStrength"), 0);

	if (!init)
	{
		OpenHapticDevice2();
		init = true;
	}

	UINT32 length_ms = configFeedbackLength;
	if ((oldFloat1 != newFloat1) && (health1p != 0x01))
	{
		double percentForce = ((Health1pStrength) / 100.0);
		RumbleWithMode(triggers->Rumble, HowtoRumbleHealthEffect, percentForce, length_ms);
	}
		
	if ((oldFloat2 != newFloat2) && (health2p != 0x01))
	{
		double percentForce = ((Health2pStrength) / 100.0);
		RumbleWithMode(triggers->RumbleDevice2, HowtoRumbleHealthEffect, percentForce, length_ms);
	}
		
	if (ff1 & 0x20)
	{
		double percentForce = ((Knock1pStrength) / 100.0);
		RumbleWithMode(triggers->Rumble, HowtoRumbleKnockEffect, percentForce, length_ms);
	}
		
	if (ff1 & 0x40)
	{
		double percentForce = ((Motor1pStrength) / 100.0);
		RumbleWithMode(triggers->Rumble, HowtoRumbleMotorEffect, percentForce, length_ms);
	}
		
	if (ff1 & 0x04)
	{
		double percentForce = ((Knock2pStrength) / 100.0);
		RumbleWithMode(triggers->RumbleDevice2, HowtoRumbleKnockEffect, percentForce, length_ms);
	}
		
	if (ff1 & 0x08)
	{
		double percentForce = ((Motor2pStrength) / 100.0);
		RumbleWithMode(triggers->RumbleDevice2, HowtoRumbleMotorEffect, percentForce, length_ms);
	}
	oldFloat1 = newFloat1;
	oldFloat2 = newFloat2;
}
