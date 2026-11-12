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
#include "CrazyTaxi.h"
static UINT8 oldGroundEffect;
static bool Collision;

static int SpringStrength = GetSettingInt(TEXT("SpringStrength"), 0);
static int DisableConstantEffects = GetSettingInt(TEXT("DisableConstantEffects"), 0);
static int EngineRumbleEffect = GetSettingInt(TEXT("EngineRumbleEffect"), 0);
static int OriginalRumbleEffect = GetSettingInt(TEXT("OriginalRumbleEffect"), 0);
static int EnableGroundEffects = GetSettingInt(TEXT("EnableGroundEffects"), 0);
static int FFBGrassSinePeriod = GetSettingInt(TEXT("FFBGrassSinePeriod"), 0);
static int FFBUnderwaterSinePeriod = GetSettingInt(TEXT("FFBUnderwaterSinePeriod"), 0);
static int FFBShorelineSinePeriod = GetSettingInt(TEXT("FFBShorelineSinePeriod"), 0);
static int FFBGrassFadeSinePeriod = GetSettingInt(TEXT("FFBGrassFadeSinePeriod"), 0);
static int FFBUnderwaterFadeSinePeriod = GetSettingInt(TEXT("FFBUnderwaterFadeSinePeriod"), 0);
static int FFBShorelineFadeSinePeriod = GetSettingInt(TEXT("FFBShorelineFadeSinePeriod"), 0);
static int FFBFootpathSinePeriod = GetSettingInt(TEXT("FFBFootpathSinePeriod"), 0);
static int FFBFootpathFadeSinePeriod = GetSettingInt(TEXT("FFBFootpathFadeSinePeriod"), 0);
static int FFBSandSinePeriod = GetSettingInt(TEXT("FFBSandSinePeriod"), 0);
static int FFBSandFadeSinePeriod = GetSettingInt(TEXT("FFBSandFadeSinePeriod"), 0);
static int FFBStairsSinePeriod = GetSettingInt(TEXT("FFBStairsSinePeriod"), 0);
static int FFBStairsFadeSinePeriod = GetSettingInt(TEXT("FFBStairsFadeSinePeriod"), 0);
static int FFBSubwaySinePeriod = GetSettingInt(TEXT("FFBSubwaySinePeriod"), 0);
static int FFBSubwayFadeSinePeriod = GetSettingInt(TEXT("FFBSubwayFadeSinePeriod"), 0);
static int FFBSandFriction = GetSettingInt(TEXT("FFBSandFriction"), 0);
static int FFBUnderwaterFriction = GetSettingInt(TEXT("FFBUnderwaterFriction"), 0);
static int FFBShorelineFriction = GetSettingInt(TEXT("FFBShorelineFriction"), 0);

void CrazyTaxi::FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers) {

	UINT8 InGame = helpers->ReadByte(0x12AA314, true);
	INT_PTR Timer = helpers->ReadIntPtr(0x12A41A4, true);
	UINT8 GroundEffect = helpers->ReadByte(0x1AB0D74, true);
	UINT8 OriginalRumble = helpers->ReadByte(0x12EF93B, true);
	UINT8 CrazyMoves1 = helpers->ReadByte(0x12EF892, true);
	UINT8 CrazyMoves2 = helpers->ReadByte(0x12EF893, true);
	float Speed = helpers->ReadFloat32(0x106A010, true);
	float LeftWall = helpers->ReadFloat32(0x126214C, true);
	float RightWall = helpers->ReadFloat32(0x12621D0, true);

	helpers->log("got value: ");
	std::string ffs = std::to_string(GroundEffect);
	helpers->log((char*)ffs.c_str());

	ApplyDamper(triggers);

	if (!InGame && Timer)
	{
		double percentForce = Speed / 10.0;

		if (percentForce > 1.0)
			percentForce = 1.0;

		double percentForceGround = percentForce / 2.0;

		helpers->WriteByte(0x1C363A, 0x00, true); //Disable native rumble/ffb

		triggers->Springi(SpringStrength / 100);

		if (EngineRumbleEffect)
		{
			if (Speed < 0.001)
				triggers->Rumble(0, 0.012, 100);
			else
				triggers->Rumble(0, 0.012 + percentForceGround / 7.0, 100);
		}

		if (OriginalRumbleEffect)
		{
			if (OriginalRumble)
				triggers->Rumble(0, percentForce, 100);

			if (CrazyMoves1 & 0x10)
				triggers->Rumble(0, 1.0, 100);

			if (CrazyMoves2)
				triggers->Rumble(0, 1.0, 100);
		}

		if (OriginalRumbleEffect && CrazyMoves1 & 0x10 || OriginalRumbleEffect && CrazyMoves2) {}
		else
		{
			switch (GroundEffect)
			{
			case 0x00: //Driving on road
				break;
			case 0x03: //Driving on grass
				if (EnableGroundEffects && Speed > 0.001)
				{
					triggers->Rumble(percentForceGround, percentForceGround, 100);
					triggers->Sine(FFBGrassSinePeriod, FFBGrassFadeSinePeriod, percentForceGround, 100);
				}
				break;
			case 0x05: //Driving on shoreline
				if (EnableGroundEffects && Speed > 0.001)
				{
					triggers->Rumble(percentForceGround, percentForceGround, 100);
					triggers->Sine(FFBShorelineSinePeriod, FFBShorelineFadeSinePeriod, percentForceGround, 100);
					triggers->Friction(FFBShorelineFriction / 100.0);
				}
				break;
			case 0x09: //Driving on sand
				if (EnableGroundEffects && Speed > 0.001)
				{
					triggers->Rumble(percentForceGround, percentForceGround, 100);
					triggers->Sine(FFBSandSinePeriod, FFBSandFadeSinePeriod, percentForceGround, 100);
					triggers->Friction(FFBSandFriction / 100.0);
				}
				break;
			case 0x0A: //Driving on footpath
				if (EnableGroundEffects && Speed > 0.001)
				{
					triggers->Rumble(percentForceGround, percentForceGround, 100);
					triggers->Sine(FFBFootpathSinePeriod, FFBFootpathFadeSinePeriod, percentForceGround, 100);
				}
				break;
			case 0x0E: //Driving underwater
				if (EnableGroundEffects && Speed > 0.001)
				{
					triggers->Rumble(percentForceGround, percentForceGround, 100);
					triggers->Sine(FFBUnderwaterSinePeriod, FFBUnderwaterFadeSinePeriod, percentForceGround, 100);
					triggers->Friction(FFBUnderwaterFriction / 100.0);
				}
				break;
			case 0x11: //Driving stairs
			case 0x13:
				if (EnableGroundEffects && Speed > 0.001)
				{
					triggers->Rumble(0, percentForceGround, 100);
					triggers->Sine(FFBStairsSinePeriod, FFBStairsFadeSinePeriod, percentForceGround, 100);
				}
				break;
			case 0x12: //Driving stairs
			case 0x14:
				if (EnableGroundEffects && Speed > 0.001)
				{
					triggers->Rumble(percentForceGround, 0, 100);
					triggers->Sine(FFBStairsSinePeriod, FFBStairsFadeSinePeriod, percentForceGround, 100);
				}
				break;
			case 0x15: //Driving subway indoors
				if (EnableGroundEffects && Speed > 0.001)
				{
					triggers->Rumble(percentForceGround, 0, 100);
					triggers->Sine(FFBSubwaySinePeriod, FFBSubwayFadeSinePeriod, percentForceGround, 100);
				}
				break;
			case 0xFF: //Colliding with cars/walls
				if (RightWall > 0)
				{
					triggers->Rumble(0, percentForce, 100);

					if (!DisableConstantEffects)
						triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
				}
				else if (LeftWall > 0)
				{
					triggers->Rumble(percentForce, 0, 100);

					if (!DisableConstantEffects)
						triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
				}
				else
				{
					if (GroundEffect != oldGroundEffect)
					{
						if (!Collision)
							Collision = true;
					}
				}
				break;
			}
		}

		if (Collision)
		{
			triggers->Sine(150, 0, percentForce, 100);
			triggers->Rumble(percentForce, percentForce, 100);
			Collision = false;
		}

		oldGroundEffect = GroundEffect;
	}
}