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

// Mario Kart Arcade GP DX 1.18 (Namco, TeknoParrot), effects made by the plugin from values of the game's
// memory during a race (shake, terrain, steering, drift, boost, coins, items), each one enabled by a
// setting with its strength (%). With MenuRumble, the menus rumble when their cursor moves (read in the
// game's memory, or seen by hooks of the selection functions of the menus).
class MarioKartGPDX118Custom : public ffb::Game
{
public:
	MarioKartGPDX118Custom();
	void Update() override;

private:
	// 4194308 when hitting walls, other karts or getting hit by items
	static constexpr ffb::GameValue<INT_PTR> Shake{ ffb::Base::Module, 0xAC72F4 };
	// Pointer to the kart, holding at:
	static constexpr ffb::GameValue<INT_PTR> Kart{ ffb::Base::Module, 0xAC2A30 };
	static const int Terrain = 0x66C;  // UINT8: 3 dirt, 4 grass, 7 water, 8 wooden bridge, 9 carpet, 10 bump, 11 rough track, 12 tiles, 14 sand
	static const int OnGround = 0x69C; // UINT8: 1 on the ground, else flying
	static const int Speed = 0x580;    // float: speed of the kart
	static const int Drift = 0x6B8;    // UINT8: 1 drifting
	// 1 during a race only
	static constexpr ffb::PointerChain<UINT8, 5> Race{ { ffb::Base::Module, 0xAAFAB8 }, { 0x44, 0x70, 0x108, 0x98, 0x4C4 } };
	// Change when picking up coins
	static constexpr ffb::GameValue<UINT8> Coins{ ffb::Base::Module, 0xAB9110 };
	// Change when picking up an item box or using an item
	static constexpr ffb::GameValue<UINT8> Weapon{ ffb::Base::Module, 0xAC5C0D };
	// Steering, 0 to 255: < 128 left
	static constexpr ffb::GameValue<UINT8> Steering{ ffb::Base::Module, 0xAC6EAD };
	// Pointer to a structure with a byte at 0x3A4 that is 1 during a boost
	static constexpr ffb::GameValue<INT_PTR> BoostPointer{ ffb::Base::Module, 0xAAD0BC };
	static const int Boost = 0x3A4;
	// Pointer (32 bits) to the state of the game, holding at:
	static constexpr ffb::GameValue<int> GameStateBase{ ffb::Base::Module, 0xAAEE58 };
	static const int GameState = 0x44; // UINT8: screen of the game (see the cursors below)
	static const int Banapass = 0x38;  // pointer (32 bits) to the Banapass screen, its cursor (UINT8) at 0x30
	// Pointer (32 bits) to a pointer (32 bits) at 0x10 to the menu screens, holding their cursors (UINT8)
	static constexpr ffb::GameValue<int> MenuBase{ ffb::Base::Module, 0xAAEEB0 };
	static const int Menu = 0x10;

	// Selection functions of the menus (relative to the module), hooked with MenuRumble
	static const INT_PTR PlayerCostumeSelectionRight = 0x386940;
	static const INT_PTR PlayerCostumeSelectionLeft = 0x3867E0;
	static const INT_PTR KartCostumeSelection = 0x3D1250;
	static const INT_PTR PlayerSelectionRight = 0x379650;
	static const INT_PTR PlayerSelectionLeft = 0x379540;
	static const INT_PTR ModeSelectionRight = 0x3A7A50;
	static const INT_PTR ModeSelectionLeft = 0x3A79D0;
	static const INT_PTR SpeedSelectionRight = 0x377B30;
	static const INT_PTR SpeedSelectionLeft = 0x377AD0;
	static const INT_PTR CupSelectionRight = 0x3908D0;
	static const INT_PTR CupSelectionLeft = 0x390800;
	static const INT_PTR RoundSelectionRight = 0x3B0160;
	static const INT_PTR RoundSelectionLeft = 0x3B00D0;
	static const INT_PTR ItemSelectionRight = 0x39D460;
	static const INT_PTR ItemSelectionLeft = 0x39D3C0;
	static const INT_PTR Item2SelectionRight = 0x39C640;
	static const INT_PTR Item2SelectionLeft = 0x39C550;
	static const INT_PTR Item3SelectionRight = 0x39EF70;
	static const INT_PTR Item3SelectionLeft = 0x39EEA0;

	// The detours set menuEffect, then call the function
	static MarioKartGPDX118Custom* instance;
	static int(__stdcall* playerCostumeSelectionRightOriginal)(DWORD* a1);
	static int(__stdcall* playerCostumeSelectionLeftOriginal)(DWORD* a1);
	static int(__stdcall* kartCostumeSelectionOriginal)(int a1, int a2, int a3);
	static DWORD*(__stdcall* playerSelectionRightOriginal)(int a1);
	static DWORD*(__stdcall* playerSelectionLeftOriginal)(int a1);
	static DWORD*(__stdcall* modeSelectionRightOriginal)(int a1);
	static DWORD*(__stdcall* modeSelectionLeftOriginal)(int a1);
	static int(__stdcall* speedSelectionRightOriginal)(int a1);
	static int(__stdcall* speedSelectionLeftOriginal)(int a1);
	static int(__stdcall* cupSelectionRightOriginal)(int a1, int a2, int a3);
	static int(__stdcall* cupSelectionLeftOriginal)(int a1, int a2, int a3);
	static int(__stdcall* roundSelectionRightOriginal)(int a1);
	static int(__stdcall* roundSelectionLeftOriginal)(int a1);
	static int(__stdcall* itemSelectionRightOriginal)(int a1);
	static int(__stdcall* itemSelectionLeftOriginal)(int a1);
	static int(__stdcall* item2SelectionRightOriginal)(int a1);
	static int(__stdcall* item2SelectionLeftOriginal)(int a1);
	static int(__stdcall* item3SelectionRightOriginal)(int a1);
	static int(__stdcall* item3SelectionLeftOriginal)(int a1);
	static int __stdcall PlayerCostumeSelectionRightHook(DWORD* a1);
	static int __stdcall PlayerCostumeSelectionLeftHook(DWORD* a1);
	static int __stdcall KartCostumeSelectionHook(int a1, int a2, int a3);
	static DWORD* __stdcall PlayerSelectionRightHook(int a1);
	static DWORD* __stdcall PlayerSelectionLeftHook(int a1);
	static DWORD* __stdcall ModeSelectionRightHook(int a1);
	static DWORD* __stdcall ModeSelectionLeftHook(int a1);
	static int __stdcall SpeedSelectionRightHook(int a1);
	static int __stdcall SpeedSelectionLeftHook(int a1);
	static int __stdcall CupSelectionRightHook(int a1, int a2, int a3);
	static int __stdcall CupSelectionLeftHook(int a1, int a2, int a3);
	static int __stdcall RoundSelectionRightHook(int a1);
	static int __stdcall RoundSelectionLeftHook(int a1);
	static int __stdcall ItemSelectionRightHook(int a1);
	static int __stdcall ItemSelectionLeftHook(int a1);
	static int __stdcall Item2SelectionRightHook(int a1);
	static int __stdcall Item2SelectionLeftHook(int a1);
	static int __stdcall Item3SelectionRightHook(int a1);
	static int __stdcall Item3SelectionLeftHook(int a1);

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
	int menuRumble = ffb::Settings::Int(L"MenuRumble", 0);
	int menuRumbleStrength = ffb::Settings::Int(L"MenuRumbleStrength", 0);

	bool hooked = false;
	bool menuEffect = false; // set by the detours
	UINT8 menuPosition = 0;  // cursor of the menu, kept on the screens without one
	// values of the previous update
	UINT8 oldMenuPosition = 0;
	UINT8 oldGameState = 0;
	int oldCoins = 0;
	int oldWeapon = 0;
	int oldHitGround = 0;
};
