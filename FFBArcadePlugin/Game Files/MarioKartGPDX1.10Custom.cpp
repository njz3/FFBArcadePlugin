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
#include "MarioKartGPDX1.10Custom.h"

static int ConstantEffectForSteering = GetSettingInt(TEXT("ConstantEffectForSteering"), 0);
static int ConstantEffectForSteeringStrength = GetSettingInt(TEXT("ConstantEffectForSteeringStrength"), 0);
static int WeaponRumble = GetSettingInt(TEXT("WeaponRumble"), 0);
static int WeaponRumbleStrength = GetSettingInt(TEXT("WeaponRumbleStrength"), 0);
static int CoinRumble = GetSettingInt(TEXT("CoinRumble"), 0);
static int CoinRumbleStrength = GetSettingInt(TEXT("CoinRumbleStrength"), 0);
static int DriftRumble = GetSettingInt(TEXT("DriftRumble"), 0);
static int DriftRumbleControllerStrengthMultiplier = GetSettingInt(TEXT("DriftRumbleControllerStrengthMultiplier"), 0);
static int HitGroundRumble = GetSettingInt(TEXT("HitGroundRumble"), 0);
static int HitGroundRumbleStrength = GetSettingInt(TEXT("HitGroundRumbleStrength"), 0);
static int BoostRumble = GetSettingInt(TEXT("BoostRumble"), 0);
static int BoostRumbleStrength = GetSettingInt(TEXT("BoostRumbleStrength"), 0);
static int MainShakeRumble = GetSettingInt(TEXT("MainShakeRumble"), 0);
static int MainShakeRumbleStrength = GetSettingInt(TEXT("MainShakeRumbleStrength"), 0);
static int DirtRumble = GetSettingInt(TEXT("DirtRumble"), 0);
static int DirtRumbleStrength = GetSettingInt(TEXT("DirtRumbleStrength"), 0);
static int GrassRumble = GetSettingInt(TEXT("GrassRumble"), 0);
static int GrassRumbleStrength = GetSettingInt(TEXT("GrassRumbleStrength"), 0);
static int SandRumble = GetSettingInt(TEXT("SandRumble"), 0);
static int SandRumbleStrength = GetSettingInt(TEXT("SandRumbleStrength"), 0);
static int WaterRumble = GetSettingInt(TEXT("WaterRumble"), 0);
static int WaterRumbleWheelStrength = GetSettingInt(TEXT("WaterRumbleWheelStrength"), 0);
static int WaterRumbleControllerStrengthMultiplier = GetSettingInt(TEXT("WaterRumbleControllerStrengthMultiplier"), 0);
static int TileRumble = GetSettingInt(TEXT("TileRumble"), 0);
static int TileRumbleStrength = GetSettingInt(TEXT("TileRumbleStrength"), 0);
static int CarpetRumble = GetSettingInt(TEXT("CarpetRumble"), 0);
static int CarpetRumbleStrength = GetSettingInt(TEXT("CarpetRumbleStrength"), 0);
static int SpeedBumpRumble = GetSettingInt(TEXT("SmallBumpRumble"), 0);
static int SpeedBumpRumbleStrength = GetSettingInt(TEXT("SmallBumpRumbleStrength"), 0);
static int RoughTrackRumble = GetSettingInt(TEXT("RoughTrackRumble"), 0);
static int RoughTrackRumbleStrength = GetSettingInt(TEXT("RoughTrackRumbleStrength"), 0);
static int BridgeRumble = GetSettingInt(TEXT("BridgeRumble"), 0);
static int BridgeRumbleStrength = GetSettingInt(TEXT("BridgeRumbleStrength"), 0);

void MarioKartGPDX110Custom::FFBLoop(EffectConstants *constants, Helpers *helpers, EffectTriggers* triggers) {

	INT_PTR ff1 = helpers->ReadIntPtr(0xA46974, true); //shake
	INT_PTR ff2 = helpers->ReadIntPtr(0x00A416E4, true);
	UINT8 ff3 = helpers->ReadByte(ff2 + 0x628, false); // terrain data
	UINT8 ff5 = helpers->ReadByte(ff2 + 0x658, false); //kart flying or on ground
	INT_PTR ff6 = helpers->ReadIntPtr(0x00A309A0, true);
	INT_PTR ff7 = helpers->ReadIntPtr(ff6 + 0x304, false);
	INT_PTR ff8 = helpers->ReadIntPtr(ff7 + 0xE8, false);
	INT_PTR ff9 = helpers->ReadIntPtr(ff8 + 0x64, false);
	INT_PTR ff10 = helpers->ReadIntPtr(ff9 + 0x38, false);
	UINT8 ff11 = helpers->ReadByte(ff10 + 0x4C4, false); // 1 during race only
	float Speed = helpers->ReadFloat32(ff2 + 0x558, false); //Speed of Kart
	UINT8 ff13 = helpers->ReadByte(0xA39690, true); //picking up coins
	UINT8 ff14 = helpers->ReadByte(0xA4528D, true); //picking up weapon box
	UINT8 Wheel = helpers->ReadByte(0xA4652D, true); //0-255 steering
	INT_PTR ff16 = helpers->ReadIntPtr(0x00A2E284, true);
	UINT8 ff17 = helpers->ReadByte(ff2 + 0x674, false); // Drift
	UINT8 ff18 = helpers->ReadByte(ff16 + 0x3A4, false); // Boost

	int static oldcoins = 0;
	int newcoins = ff13;
	int static oldweapon = 0;
	int newweapon = ff14;
	int static oldhitground = 0;
	int newhitground = ff5;
	helpers->log("got value: ");
	std::string ffs = std::to_string(ff1);
	helpers->log((char *)ffs.c_str()); helpers->log("got value: ");

	ApplyDamper(triggers);
	
	UINT32 length_ms = 100;
	if ((ConstantEffectForSteering == 1) && (ff11 == 1))
	{
		if ((Wheel >= 0) && (Wheel < 128))
		{
			double percentForce = ((128 - Wheel) / (ConstantEffectForSteeringStrength / 1.0));
			length_ms = 100;
			triggers->Rumble(percentForce, 0, length_ms);
			triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
		}
		else if ((Wheel > 127) && (Wheel < 256))
		{
			double percentForce = ((Wheel - 127) / (ConstantEffectForSteeringStrength / 1.0));
			length_ms = 100;
			triggers->Rumble(0, percentForce, length_ms);
			triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
		}
	}
	if ((MainShakeRumble == 1) && (4194308 == ff1) && (ff11 == 1))
	{
		// Large Shake when hitting walls, other karts or getting hit by items
		double percentForce = ((MainShakeRumbleStrength) / 100.0);
		length_ms = 500;
		triggers->Rumble(percentForce, percentForce, length_ms);
		triggers->Sine(200, 200, percentForce, length_ms);
	}

	if ((BoostRumble == 1) && (ff18 == 1) && (ff11 == 1))
	{
		// Shake when Boost
		double percentForce = ((BoostRumbleStrength) / 100.0);
		length_ms = 100;
		triggers->Rumble(percentForce, percentForce, length_ms);
		triggers->Sine(60, 60, percentForce, length_ms);
	}

	if ((DriftRumble == 1) && (ff17 == 1) && (Wheel >= 0) && (Wheel < 128) && (ff11 == 1))
	{
		// Drift Effect including steering left
		double percentForce = (((128 - Wheel) / 128.0) * (DriftRumbleControllerStrengthMultiplier / 100.0));
		length_ms = 100;
		triggers->Rumble(percentForce, 0, length_ms);
		triggers->Friction(percentForce);
	}
	
	if ((DriftRumble == 1) && (ff17 == 1) && (Wheel > 127) && (Wheel < 256) && (ff11 == 1))
	{
		// Drift Effect including steering right
		double percentForce = (((Wheel - 127) / 128.0) * (DriftRumbleControllerStrengthMultiplier / 100.0));
		length_ms = 100;
		triggers->Rumble(0, percentForce, length_ms);
		triggers->Friction(percentForce);
	}
	
	if ((HitGroundRumble == 1) && (oldhitground != newhitground) && (ff5 == 1) && (ff11 == 1))
	{
		// Shake when hitting ground
		double percentForce = ((HitGroundRumbleStrength) / 100.0);
		length_ms = 100;
		triggers->Rumble(percentForce, percentForce, length_ms);
		triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
		Sleep(50);
		triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
	}
	
	if ((WeaponRumble == 1) && (oldweapon != newweapon) && (ff11 == 1))
	{
		// Shake when picking up new weapons or using them
		double percentForce = ((WeaponRumbleStrength) / 100.0);
		length_ms = 300;
		triggers->Rumble(percentForce, percentForce, length_ms);
		triggers->Sine(80, 50, percentForce, length_ms);
	}
	
	if ((CoinRumble == 1) && (oldcoins != newcoins) && (ff11 == 1))
	{
		// Shake when picking up coins
		double percentForce = ((CoinRumbleStrength) / 100.0);
		length_ms = (200);
		triggers->Rumble(percentForce, percentForce, length_ms);
		triggers->Sine(50, 50, percentForce, length_ms);
	}
	
	if ((DirtRumble == 1) && (3 == ff3) && (ff11 == 1) && (ff5 == 1) && (Speed > 0.1))
	{
		// small friction when driving on dirt while moving
		double percentForce = ((DirtRumbleStrength) / 100.0);
		length_ms = (100);
		triggers->Rumble(percentForce, 0, length_ms);
		triggers->Friction(percentForce);
	}
	
	if ((SpeedBumpRumble == 1) && (10 == ff3) && (ff11 == 1) && (ff5 == 1) && (Speed > 0.1))
	{
		//	Small constant when hitting bumps
		double percentForce = ((SpeedBumpRumbleStrength) / 100.0);
		length_ms = (50);
		triggers->Rumble(percentForce, percentForce, length_ms);
		triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
		triggers->Constant(constants->DIRECTION_FROM_RIGHT, 0);
	}
	
	if ((GrassRumble == 1) && (4 == ff3) && (ff11 == 1) && (ff5 == 1) && (Speed > 0.1))
	{
		// Wheel rumbles while driving on grass
		double percentForce = ((GrassRumbleStrength) / 100.0);
		length_ms = (50);
		triggers->Rumble(0, percentForce, length_ms);
		triggers->Sine(50, 50, percentForce, length_ms);
	}
	
	if ((CarpetRumble == 1) && (9 == ff3) && (ff11 == 1) && (ff5 == 1) && (Speed > 0.1))
	{
		// Wheel rumbles while driving on carpet
		double percentForce = ((CarpetRumbleStrength) / 100.0);
		length_ms = (50);
		triggers->Rumble(0, percentForce, length_ms);
		triggers->Sine(50, 50, percentForce, length_ms);
	}
	
	if ((WaterRumble == 1) && (7 == ff3) && (ff11 == 1) && (ff5 == 1) && (Speed > 0.1) && (Wheel >= 0) && (Wheel < 128))
	{
		//wheel hard to turn while driving through water
		double percentForce = ((WaterRumbleWheelStrength) / 100.0);
		double percentForce1 = ((128 - Wheel / 128.0) * (WaterRumbleControllerStrengthMultiplier / 100.0));
		length_ms = (100);
		triggers->Rumble(percentForce1, 0, length_ms);
		triggers->Friction(percentForce);
	}
	
	if ((WaterRumble == 1) && (7 == ff3) && (ff11 == 1) && (ff5 == 1) && (Speed > 0.1) && (Wheel > 127))
	{
		double percentForce = ((WaterRumbleWheelStrength) / 100.0);
		double percentForce1 = ((Wheel - 127 / 128.0) * (WaterRumbleControllerStrengthMultiplier / 100.0));
		length_ms = (100);
		triggers->Rumble(0, percentForce1, length_ms);
		triggers->Friction(percentForce);
	}
	
	if ((TileRumble == 1) && (12 == ff3) && (ff11 == 1) && (ff5 == 1) && (Speed > 0.1))
	{
		//Wheel rumbles lightly when driving over tiles
		double percentForce = ((TileRumbleStrength) / 100.0);
		length_ms = (150);
		triggers->Rumble(0, percentForce, length_ms);
		triggers->Friction(percentForce);
	}
	
	if ((SandRumble == 1) && (14 == ff3) && (ff11 == 1) && (ff5 == 1) && (Speed > 0.1))
	{
		//Wheel rumbles lightly when driving over sand
		double percentForce = ((SandRumbleStrength) / 100.0);
		length_ms = (50);
		triggers->Rumble(percentForce, 0, length_ms);
		triggers->Sine(70, 70, percentForce, length_ms);
	}
	
	if ((RoughTrackRumble == 1) && (11 == ff3) && (ff11 == 1) && (ff5 == 1) && (Speed > 0.1))
	{
		//Wheel rumbles lightly when driving over rough part of track
		double percentForce = ((RoughTrackRumbleStrength) / 100.0);
		length_ms = (100);
		triggers->Rumble(0, percentForce, length_ms);
		triggers->Sine(40, 50, percentForce, length_ms);
	}
	
	if ((BridgeRumble == 1) && (8 == ff3) && (ff11 == 1) && (ff5 == 1) && (Speed > 0.1))
	{
		//Wheel rumbles moderately when driving over wooden bridges
		double percentForce = ((BridgeRumbleStrength) / 100.0);
		length_ms = (100);
		triggers->Rumble(percentForce, percentForce, length_ms);
		triggers->Sine(120, 120, percentForce, length_ms);
	}
	oldcoins = newcoins;
	oldweapon = newweapon;
	oldhitground = newhitground;
}
