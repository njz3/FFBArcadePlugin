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

#include "MarioKartGPDXUSACustom.h"

using namespace ffb;

void MarioKartGPDXUSACustom::Update()
{
	int constantEffectForSteering = Settings::Int(L"ConstantEffectForSteering", 0);
	int constantEffectForSteeringStrength = Settings::Int(L"ConstantEffectForSteeringStrength", 0);
	int weaponRumble = Settings::Int(L"WeaponRumble", 0);
	int weaponRumbleStrength = Settings::Int(L"WeaponRumbleStrength", 0);
	int coinRumble = Settings::Int(L"CoinRumble", 0);
	int coinRumbleStrength = Settings::Int(L"CoinRumbleStrength", 0);
	int driftRumble = Settings::Int(L"DriftRumble", 0);
	int driftRumbleControllerStrengthMultiplier = Settings::Int(L"DriftRumbleControllerStrengthMultiplier", 0);
	int hitGroundRumble = Settings::Int(L"HitGroundRumble", 0);
	int hitGroundRumbleStrength = Settings::Int(L"HitGroundRumbleStrength", 0);
	int boostRumble = Settings::Int(L"BoostRumble", 0);
	int boostRumbleStrength = Settings::Int(L"BoostRumbleStrength", 0);
	int mainShakeRumble = Settings::Int(L"MainShakeRumble", 0);
	int mainShakeRumbleStrength = Settings::Int(L"MainShakeRumbleStrength", 0);
	int dirtRumble = Settings::Int(L"DirtRumble", 0);
	int dirtRumbleStrength = Settings::Int(L"DirtRumbleStrength", 0);
	int grassRumble = Settings::Int(L"GrassRumble", 0);
	int grassRumbleStrength = Settings::Int(L"GrassRumbleStrength", 0);
	int sandRumble = Settings::Int(L"SandRumble", 0);
	int sandRumbleStrength = Settings::Int(L"SandRumbleStrength", 0);
	int waterRumble = Settings::Int(L"WaterRumble", 0);
	int waterRumbleWheelStrength = Settings::Int(L"WaterRumbleWheelStrength", 0);
	int waterRumbleControllerStrengthMultiplier = Settings::Int(L"WaterRumbleControllerStrengthMultiplier", 0);
	int tileRumble = Settings::Int(L"TileRumble", 0);
	int tileRumbleStrength = Settings::Int(L"TileRumbleStrength", 0);
	int carpetRumble = Settings::Int(L"CarpetRumble", 0);
	int carpetRumbleStrength = Settings::Int(L"CarpetRumbleStrength", 0);
	int speedBumpRumble = Settings::Int(L"SmallBumpRumble", 0);
	int speedBumpRumbleStrength = Settings::Int(L"SmallBumpRumbleStrength", 0);
	int roughTrackRumble = Settings::Int(L"RoughTrackRumble", 0);
	int roughTrackRumbleStrength = Settings::Int(L"RoughTrackRumbleStrength", 0);
	int bridgeRumble = Settings::Int(L"BridgeRumble", 0);
	int bridgeRumbleStrength = Settings::Int(L"BridgeRumbleStrength", 0);

	INT_PTR shake = Memory::Read(Shake);
	INT_PTR kart = Memory::Read(Kart);
	UINT8 terrain = Memory::Read<int>(kart + Terrain);
	UINT8 onGround = Memory::Read<int>(kart + OnGround);
	UINT8 race = Memory::Read(Race);
	float speed = Memory::Read<float>(kart + Speed);
	UINT8 coins = Memory::Read(Coins);
	UINT8 weapon = Memory::Read(Weapon);
	INT_PTR steeringPointer = Memory::Read(SteeringPointer);
	UINT8 steering = Memory::Read<UINT8>(steeringPointer + Steering);
	INT_PTR boostPointer = Memory::Read(BoostPointer);
	UINT8 drift = Memory::Read<UINT8>(kart + Drift);
	UINT8 boost = Memory::Read<UINT8>(boostPointer + Boost);

	ApplyDamper();

	if ((constantEffectForSteering == 1) && (race == 1))
	{
		if ((steering >= 0) && (steering < 128))
		{
			double percentForce = ((128 - steering) / (constantEffectForSteeringStrength / 1.0));
			wheel.LeftRight(percentForce, 0, 100);
			wheel.Constant(Direction::FromLeft, percentForce);
		}
		else if ((steering > 127) && (steering < 256))
		{
			double percentForce = ((steering - 127) / (constantEffectForSteeringStrength / 1.0));
			wheel.LeftRight(0, percentForce, 100);
			wheel.Constant(Direction::FromRight, percentForce);
		}
	}
	if ((mainShakeRumble == 1) && (4194308 == shake) && (race == 1))
	{
		// Large Shake when hitting walls, other karts or getting hit by items
		double percentForce = ((mainShakeRumbleStrength) / 100.0);
		wheel.LeftRight(percentForce, percentForce, 500);
		wheel.Sine(200, 200, percentForce, 500);
	}
	else if ((boostRumble == 1) && (boost == 1) && (race == 1))
	{
		// Shake when Boost
		double percentForce = ((boostRumbleStrength) / 100.0);
		wheel.LeftRight(percentForce, percentForce, 100);
		wheel.Sine(60, 60, percentForce, 100);
	}
	else if ((driftRumble == 1) && (drift == 1) && (steering >= 0) && (steering < 128) && (race == 1))
	{
		// Drift Effect including steering left
		double percentForce = (((128 - steering) / 128.0) * (driftRumbleControllerStrengthMultiplier / 100.0));
		wheel.LeftRight(percentForce, 0, 100);
		wheel.Friction(percentForce);
	}
	else if ((driftRumble == 1) && (drift == 1) && (steering > 127) && (steering < 256) && (race == 1))
	{
		// Drift Effect including steering right
		double percentForce = (((steering - 127) / 128.0) * (driftRumbleControllerStrengthMultiplier / 100.0));
		wheel.LeftRight(0, percentForce, 100);
		wheel.Friction(percentForce);
	}
	else if ((hitGroundRumble == 1) && (oldHitGround != onGround) && (onGround == 1) && (race == 1))
	{
		// Shake when hitting ground
		double percentForce = ((hitGroundRumbleStrength) / 100.0);
		wheel.LeftRight(percentForce, percentForce, 100);
		wheel.Constant(Direction::FromLeft, percentForce);
		Sleep(50);
		wheel.Constant(Direction::FromRight, percentForce);
	}
	else if ((weaponRumble == 1) && (oldWeapon != weapon) && (race == 1))
	{
		// Shake when picking up new weapons or using them
		double percentForce = ((weaponRumbleStrength) / 100.0);
		wheel.LeftRight(percentForce, percentForce, 300);
		wheel.Sine(80, 50, percentForce, 100);
	}
	else if ((coinRumble == 1) && (oldCoins != coins) && (race == 1))
	{
		// Shake when picking up coins
		double percentForce = ((coinRumbleStrength) / 100.0);
		wheel.LeftRight(percentForce, percentForce, 200);
		wheel.Sine(50, 50, percentForce, 100);
	}
	else if ((dirtRumble == 1) && (3 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1))
	{
		// small friction when driving on dirt while moving
		double percentForce = ((dirtRumbleStrength) / 100.0);
		wheel.LeftRight(percentForce, 0, 100);
		wheel.Friction(percentForce);
	}
	else if ((speedBumpRumble == 1) && (10 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1))
	{
		// Small constant when hitting bumps
		double percentForce = ((speedBumpRumbleStrength) / 100.0);
		wheel.LeftRight(percentForce, percentForce, 50);
		wheel.Constant(Direction::FromRight, percentForce);
		wheel.Constant(Direction::FromRight, 0);
	}
	else if ((grassRumble == 1) && (4 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1))
	{
		// Wheel rumbles while driving on grass
		double percentForce = ((grassRumbleStrength) / 100.0);
		wheel.LeftRight(0, percentForce, 50);
		wheel.Sine(50, 50, percentForce, 100);
	}
	else if ((carpetRumble == 1) && (9 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1))
	{
		// Wheel rumbles while driving on carpet
		double percentForce = ((carpetRumbleStrength) / 100.0);
		wheel.LeftRight(0, percentForce, 50);
		wheel.Sine(50, 50, percentForce, 100);
	}
	else if ((waterRumble == 1) && (7 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1) && (steering >= 0) && (steering < 128))
	{
		// wheel hard to turn while driving through water
		double percentForce = ((waterRumbleWheelStrength) / 100.0);
		// 128 - steering / 128.0, not (128 - steering) / 128.0 as for the drift
		double percentForce1 = ((128 - steering / 128.0) * (waterRumbleControllerStrengthMultiplier / 100.0));
		wheel.LeftRight(percentForce1, 0, 100);
		wheel.Friction(percentForce);
	}
	else if ((waterRumble == 1) && (7 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1) && (steering > 127))
	{
		double percentForce = ((waterRumbleWheelStrength) / 100.0);
		double percentForce1 = ((steering - 127 / 128.0) * (waterRumbleControllerStrengthMultiplier / 100.0));
		wheel.LeftRight(0, percentForce1, 100);
		wheel.Friction(percentForce);
	}
	else if ((tileRumble == 1) && (12 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1))
	{
		// Wheel rumbles lightly when driving over tiles
		double percentForce = ((tileRumbleStrength) / 100.0);
		wheel.LeftRight(0, percentForce, 150);
		wheel.Friction(percentForce);
	}
	else if ((sandRumble == 1) && (14 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1))
	{
		// Wheel rumbles lightly when driving over sand
		double percentForce = ((sandRumbleStrength) / 100.0);
		wheel.LeftRight(percentForce, 0, 50);
		wheel.Sine(70, 70, percentForce, 100);
	}
	else if ((roughTrackRumble == 1) && (11 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1))
	{
		// Wheel rumbles lightly when driving over rough part of track
		double percentForce = ((roughTrackRumbleStrength) / 100.0);
		wheel.LeftRight(0, percentForce, 100);
		wheel.Sine(40, 50, percentForce, 100);
	}
	else if ((bridgeRumble == 1) && (8 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1))
	{
		// Wheel rumbles moderately when driving over wooden bridges
		double percentForce = ((bridgeRumbleStrength) / 100.0);
		wheel.LeftRight(percentForce, percentForce, 100);
		wheel.Sine(180, 150, percentForce, 200);
	}
	oldCoins = coins;
	oldWeapon = weapon;
	oldHitGround = onGround;
}
