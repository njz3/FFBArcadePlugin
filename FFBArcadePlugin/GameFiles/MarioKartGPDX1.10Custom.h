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

#pragma once
#include "../Core/Game.h"

// Mario Kart Arcade GP DX 1.10 (Namco, TeknoParrot), effects made by the plugin from values of the game's
// memory during a race (shake, terrain, steering, drift, boost, coins, items), each one enabled by a
// setting with its strength (%).
class MarioKartGPDX110Custom : public ffb::Game
{
public:
	void Update() override;

private:
	// 4194308 when hitting walls, other karts or getting hit by items
	static constexpr ffb::GameValue<INT_PTR> Shake{ ffb::Base::Module, 0xA46974 };
	// Pointer to the kart, holding at:
	static constexpr ffb::GameValue<INT_PTR> Kart{ ffb::Base::Module, 0x00A416E4 };
	static const int Terrain = 0x628;  // UINT8: 3 dirt, 4 grass, 7 water, 8 wooden bridge, 9 carpet, 10 bump, 11 rough track, 12 tiles, 14 sand
	static const int OnGround = 0x658; // UINT8: 1 on the ground, else flying
	static const int Speed = 0x558;    // float: speed of the kart
	static const int Drift = 0x674;    // UINT8: 1 drifting
	// 1 during a race only
	static constexpr ffb::PointerChain<UINT8, 5> Race{ { ffb::Base::Module, 0x00A309A0 }, { 0x304, 0xE8, 0x64, 0x38, 0x4C4 } };
	// Change when picking up coins
	static constexpr ffb::GameValue<UINT8> Coins{ ffb::Base::Module, 0xA39690 };
	// Change when picking up an item box or using an item
	static constexpr ffb::GameValue<UINT8> Weapon{ ffb::Base::Module, 0xA4528D };
	// Steering, 0 to 255: < 128 left
	static constexpr ffb::GameValue<UINT8> Steering{ ffb::Base::Module, 0xA4652D };
	// Pointer to a structure with a byte at 0x3A4 that is 1 during a boost
	static constexpr ffb::GameValue<INT_PTR> BoostPointer{ ffb::Base::Module, 0x00A2E284 };
	static const int Boost = 0x3A4;

	int constantEffectForSteering = ffb::Settings::Int(L"ConstantEffectForSteering", 0);
	int constantEffectForSteeringStrength = ffb::Settings::Int(L"ConstantEffectForSteeringStrength", 0);
	int weaponRumble = ffb::Settings::Int(L"WeaponRumble", 0);
	int weaponRumbleStrength = ffb::Settings::Int(L"WeaponRumbleStrength", 0);
	int coinRumble = ffb::Settings::Int(L"CoinRumble", 0);
	int coinRumbleStrength = ffb::Settings::Int(L"CoinRumbleStrength", 0);
	int driftRumble = ffb::Settings::Int(L"DriftRumble", 0);
	int driftRumbleControllerStrengthMultiplier = ffb::Settings::Int(L"DriftRumbleControllerStrengthMultiplier", 0);
	int hitGroundRumble = ffb::Settings::Int(L"HitGroundRumble", 0);
	int hitGroundRumbleStrength = ffb::Settings::Int(L"HitGroundRumbleStrength", 0);
	int boostRumble = ffb::Settings::Int(L"BoostRumble", 0);
	int boostRumbleStrength = ffb::Settings::Int(L"BoostRumbleStrength", 0);
	int mainShakeRumble = ffb::Settings::Int(L"MainShakeRumble", 0);
	int mainShakeRumbleStrength = ffb::Settings::Int(L"MainShakeRumbleStrength", 0);
	int dirtRumble = ffb::Settings::Int(L"DirtRumble", 0);
	int dirtRumbleStrength = ffb::Settings::Int(L"DirtRumbleStrength", 0);
	int grassRumble = ffb::Settings::Int(L"GrassRumble", 0);
	int grassRumbleStrength = ffb::Settings::Int(L"GrassRumbleStrength", 0);
	int sandRumble = ffb::Settings::Int(L"SandRumble", 0);
	int sandRumbleStrength = ffb::Settings::Int(L"SandRumbleStrength", 0);
	int waterRumble = ffb::Settings::Int(L"WaterRumble", 0);
	int waterRumbleWheelStrength = ffb::Settings::Int(L"WaterRumbleWheelStrength", 0);
	int waterRumbleControllerStrengthMultiplier = ffb::Settings::Int(L"WaterRumbleControllerStrengthMultiplier", 0);
	int tileRumble = ffb::Settings::Int(L"TileRumble", 0);
	int tileRumbleStrength = ffb::Settings::Int(L"TileRumbleStrength", 0);
	int carpetRumble = ffb::Settings::Int(L"CarpetRumble", 0);
	int carpetRumbleStrength = ffb::Settings::Int(L"CarpetRumbleStrength", 0);
	int speedBumpRumble = ffb::Settings::Int(L"SmallBumpRumble", 0);
	int speedBumpRumbleStrength = ffb::Settings::Int(L"SmallBumpRumbleStrength", 0);
	int roughTrackRumble = ffb::Settings::Int(L"RoughTrackRumble", 0);
	int roughTrackRumbleStrength = ffb::Settings::Int(L"RoughTrackRumbleStrength", 0);
	int bridgeRumble = ffb::Settings::Int(L"BridgeRumble", 0);
	int bridgeRumbleStrength = ffb::Settings::Int(L"BridgeRumbleStrength", 0);

	// values of the previous update
	int oldCoins = 0;
	int oldWeapon = 0;
	int oldHitGround = 0;
};
