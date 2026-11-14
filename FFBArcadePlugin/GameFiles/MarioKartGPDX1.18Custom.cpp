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

#include "MarioKartGPDX1.18Custom.h"
#include "../Core/Hooks.h"

using namespace ffb;

MarioKartGPDX118Custom* MarioKartGPDX118Custom::instance = nullptr;
int(__stdcall* MarioKartGPDX118Custom::playerCostumeSelectionRightOriginal)(DWORD* a1) = nullptr;
int(__stdcall* MarioKartGPDX118Custom::playerCostumeSelectionLeftOriginal)(DWORD* a1) = nullptr;
int(__stdcall* MarioKartGPDX118Custom::kartCostumeSelectionOriginal)(int a1, int a2, int a3) = nullptr;
DWORD*(__stdcall* MarioKartGPDX118Custom::playerSelectionRightOriginal)(int a1) = nullptr;
DWORD*(__stdcall* MarioKartGPDX118Custom::playerSelectionLeftOriginal)(int a1) = nullptr;
DWORD*(__stdcall* MarioKartGPDX118Custom::modeSelectionRightOriginal)(int a1) = nullptr;
DWORD*(__stdcall* MarioKartGPDX118Custom::modeSelectionLeftOriginal)(int a1) = nullptr;
int(__stdcall* MarioKartGPDX118Custom::speedSelectionRightOriginal)(int a1) = nullptr;
int(__stdcall* MarioKartGPDX118Custom::speedSelectionLeftOriginal)(int a1) = nullptr;
int(__stdcall* MarioKartGPDX118Custom::cupSelectionRightOriginal)(int a1, int a2, int a3) = nullptr;
int(__stdcall* MarioKartGPDX118Custom::cupSelectionLeftOriginal)(int a1, int a2, int a3) = nullptr;
int(__stdcall* MarioKartGPDX118Custom::roundSelectionRightOriginal)(int a1) = nullptr;
int(__stdcall* MarioKartGPDX118Custom::roundSelectionLeftOriginal)(int a1) = nullptr;
int(__stdcall* MarioKartGPDX118Custom::itemSelectionRightOriginal)(int a1) = nullptr;
int(__stdcall* MarioKartGPDX118Custom::itemSelectionLeftOriginal)(int a1) = nullptr;
int(__stdcall* MarioKartGPDX118Custom::item2SelectionRightOriginal)(int a1) = nullptr;
int(__stdcall* MarioKartGPDX118Custom::item2SelectionLeftOriginal)(int a1) = nullptr;
int(__stdcall* MarioKartGPDX118Custom::item3SelectionRightOriginal)(int a1) = nullptr;
int(__stdcall* MarioKartGPDX118Custom::item3SelectionLeftOriginal)(int a1) = nullptr;

MarioKartGPDX118Custom::MarioKartGPDX118Custom()
{
	instance = this;
}

int __stdcall MarioKartGPDX118Custom::PlayerCostumeSelectionRightHook(DWORD* a1)
{
	instance->menuEffect = true;
	return playerCostumeSelectionRightOriginal(a1);
}

int __stdcall MarioKartGPDX118Custom::PlayerCostumeSelectionLeftHook(DWORD* a1)
{
	instance->menuEffect = true;
	return playerCostumeSelectionLeftOriginal(a1);
}

int __stdcall MarioKartGPDX118Custom::KartCostumeSelectionHook(int a1, int a2, int a3)
{
	instance->menuEffect = true;
	return kartCostumeSelectionOriginal(a1, a2, a3);
}

DWORD* __stdcall MarioKartGPDX118Custom::PlayerSelectionRightHook(int a1)
{
	instance->menuEffect = true;
	return playerSelectionRightOriginal(a1);
}

DWORD* __stdcall MarioKartGPDX118Custom::PlayerSelectionLeftHook(int a1)
{
	instance->menuEffect = true;
	return playerSelectionLeftOriginal(a1);
}

DWORD* __stdcall MarioKartGPDX118Custom::ModeSelectionRightHook(int a1)
{
	instance->menuEffect = true;
	return modeSelectionRightOriginal(a1);
}

DWORD* __stdcall MarioKartGPDX118Custom::ModeSelectionLeftHook(int a1)
{
	instance->menuEffect = true;
	return modeSelectionLeftOriginal(a1);
}

int __stdcall MarioKartGPDX118Custom::SpeedSelectionRightHook(int a1)
{
	instance->menuEffect = true;
	return speedSelectionRightOriginal(a1);
}

int __stdcall MarioKartGPDX118Custom::SpeedSelectionLeftHook(int a1)
{
	instance->menuEffect = true;
	return speedSelectionLeftOriginal(a1);
}

int __stdcall MarioKartGPDX118Custom::CupSelectionRightHook(int a1, int a2, int a3)
{
	instance->menuEffect = true;
	return cupSelectionRightOriginal(a1, a2, a3);
}

int __stdcall MarioKartGPDX118Custom::CupSelectionLeftHook(int a1, int a2, int a3)
{
	instance->menuEffect = true;
	return cupSelectionLeftOriginal(a1, a2, a3);
}

int __stdcall MarioKartGPDX118Custom::RoundSelectionRightHook(int a1)
{
	instance->menuEffect = true;
	return roundSelectionRightOriginal(a1);
}

int __stdcall MarioKartGPDX118Custom::RoundSelectionLeftHook(int a1)
{
	instance->menuEffect = true;
	return roundSelectionLeftOriginal(a1);
}

int __stdcall MarioKartGPDX118Custom::ItemSelectionRightHook(int a1)
{
	instance->menuEffect = true;
	return itemSelectionRightOriginal(a1);
}

int __stdcall MarioKartGPDX118Custom::ItemSelectionLeftHook(int a1)
{
	instance->menuEffect = true;
	return itemSelectionLeftOriginal(a1);
}

int __stdcall MarioKartGPDX118Custom::Item2SelectionRightHook(int a1)
{
	instance->menuEffect = true;
	return item2SelectionRightOriginal(a1);
}

int __stdcall MarioKartGPDX118Custom::Item2SelectionLeftHook(int a1)
{
	instance->menuEffect = true;
	return item2SelectionLeftOriginal(a1);
}

int __stdcall MarioKartGPDX118Custom::Item3SelectionRightHook(int a1)
{
	instance->menuEffect = true;
	return item3SelectionRightOriginal(a1);
}

int __stdcall MarioKartGPDX118Custom::Item3SelectionLeftHook(int a1)
{
	instance->menuEffect = true;
	return item3SelectionLeftOriginal(a1);
}

void MarioKartGPDX118Custom::Update()
{
	INT_PTR shake = Memory::Read(Shake);
	INT_PTR kart = Memory::Read(Kart);
	UINT8 terrain = Memory::Read<UINT8>(kart + Terrain);
	UINT8 onGround = Memory::Read<UINT8>(kart + OnGround);
	UINT8 race = Memory::Read(Race);
	float speed = Memory::Read<float>(kart + Speed);
	UINT8 coins = Memory::Read(Coins);
	UINT8 weapon = Memory::Read(Weapon);
	UINT8 steering = Memory::Read(Steering);
	INT_PTR boostPointer = Memory::Read(BoostPointer);
	UINT8 drift = Memory::Read<UINT8>(kart + Drift);
	UINT8 boost = Memory::Read<UINT8>(boostPointer + Boost);

	DWORD gameStateBase = Memory::Read(GameStateBase);
	UINT8 gameState = Memory::Read<UINT8>(gameStateBase + GameState);

	Log::Printf("got value: %lld", (long long)shake);

	if (!hooked && menuRumble)
	{
		hooked = true;

		Hooks::Initialize();
		Hooks::Create(Memory::Address(Base::Module, PlayerCostumeSelectionRight), PlayerCostumeSelectionRightHook, &playerCostumeSelectionRightOriginal);
		Hooks::Create(Memory::Address(Base::Module, PlayerCostumeSelectionLeft), PlayerCostumeSelectionLeftHook, &playerCostumeSelectionLeftOriginal);
		Hooks::Create(Memory::Address(Base::Module, KartCostumeSelection), KartCostumeSelectionHook, &kartCostumeSelectionOriginal);
		Hooks::Create(Memory::Address(Base::Module, PlayerSelectionRight), PlayerSelectionRightHook, &playerSelectionRightOriginal);
		Hooks::Create(Memory::Address(Base::Module, PlayerSelectionLeft), PlayerSelectionLeftHook, &playerSelectionLeftOriginal);
		Hooks::Create(Memory::Address(Base::Module, ModeSelectionRight), ModeSelectionRightHook, &modeSelectionRightOriginal);
		Hooks::Create(Memory::Address(Base::Module, ModeSelectionLeft), ModeSelectionLeftHook, &modeSelectionLeftOriginal);
		Hooks::Create(Memory::Address(Base::Module, SpeedSelectionRight), SpeedSelectionRightHook, &speedSelectionRightOriginal);
		Hooks::Create(Memory::Address(Base::Module, SpeedSelectionLeft), SpeedSelectionLeftHook, &speedSelectionLeftOriginal);
		Hooks::Create(Memory::Address(Base::Module, CupSelectionRight), CupSelectionRightHook, &cupSelectionRightOriginal);
		Hooks::Create(Memory::Address(Base::Module, CupSelectionLeft), CupSelectionLeftHook, &cupSelectionLeftOriginal);
		Hooks::Create(Memory::Address(Base::Module, RoundSelectionRight), RoundSelectionRightHook, &roundSelectionRightOriginal);
		Hooks::Create(Memory::Address(Base::Module, RoundSelectionLeft), RoundSelectionLeftHook, &roundSelectionLeftOriginal);
		Hooks::Create(Memory::Address(Base::Module, ItemSelectionRight), ItemSelectionRightHook, &itemSelectionRightOriginal);
		Hooks::Create(Memory::Address(Base::Module, ItemSelectionLeft), ItemSelectionLeftHook, &itemSelectionLeftOriginal);
		Hooks::Create(Memory::Address(Base::Module, Item2SelectionRight), Item2SelectionRightHook, &item2SelectionRightOriginal);
		Hooks::Create(Memory::Address(Base::Module, Item2SelectionLeft), Item2SelectionLeftHook, &item2SelectionLeftOriginal);
		Hooks::Create(Memory::Address(Base::Module, Item3SelectionRight), Item3SelectionRightHook, &item3SelectionRightOriginal);
		Hooks::Create(Memory::Address(Base::Module, Item3SelectionLeft), Item3SelectionLeftHook, &item3SelectionLeftOriginal);
		Hooks::Enable();
	}

	ApplyDamper();

	if (menuRumble && !race)
	{
		DWORD menuBase = Memory::Read(MenuBase);
		DWORD menu = Memory::Read<int>(menuBase + Menu);

		DWORD banapass = Memory::Read<int>(gameStateBase + Banapass);

		if (gameState != oldGameState)
			menuPosition = 0;

		switch (gameState)
		{
		case 0x09: // Banapass Selection Screen
			menuPosition = Memory::Read<UINT8>(banapass + 0x30);
			break;
		case 0x13: // Camera Costume Selection Screen
			menuPosition = Memory::Read<UINT8>(menu + 0x294);
			break;
		case 0x2D: // After Race Change Selection Screen
			menuPosition = Memory::Read<UINT8>(menu + 0xE0);
			break;
		case 0x2F: // After Race Continue Selection Screen
			menuPosition = Memory::Read<UINT8>(menu + 0xE8);
			break;
		}

		if (menuPosition != oldMenuPosition && gameState == oldGameState)
		{
			double percentForce = menuRumbleStrength / 100.0;
			wheel.Sine(60, 0, percentForce, 100);
			wheel.Rumble(percentForce, percentForce, 100);
		}

		if (menuEffect)
		{
			menuEffect = false;
			double percentForce = menuRumbleStrength / 100.0;
			wheel.Sine(60, 0, percentForce, 100);
			wheel.Rumble(percentForce, percentForce, 100);
		}
	}

	if (constantEffectForSteering && race == 1)
	{
		if (steering >= 0 && steering < 128)
		{
			double percentForce = ((128 - steering) / (constantEffectForSteeringStrength / 1.0));
			wheel.ConstantWithRumble(Direction::FromLeft, percentForce);
		}
		else if (steering > 127 && steering < 256)
		{
			double percentForce = ((steering - 127) / (constantEffectForSteeringStrength / 1.0));
			wheel.ConstantWithRumble(Direction::FromRight, percentForce);
		}
	}

	if ((mainShakeRumble) && (4194308 == shake) && (race == 1))
	{
		// Large Shake when hitting walls, other karts or getting hit by items
		double percentForce = ((mainShakeRumbleStrength) / 100.0);
		wheel.Rumble(percentForce, percentForce, 500);
		wheel.Sine(200, 200, percentForce, 200);
	}

	if ((boostRumble == 1) && (boost == 1) && (race == 1))
	{
		// Shake when Boost
		double percentForce = ((boostRumbleStrength) / 100.0);
		wheel.Rumble(percentForce, percentForce, 100);
		wheel.Sine(60, 60, percentForce, 100);
	}

	if ((driftRumble == 1) && (drift == 1) && (steering >= 0) && (steering < 128) && (race == 1))
	{
		// Drift Effect including steering left
		double percentForce = (((128 - steering) / 128.0) * (driftRumbleControllerStrengthMultiplier / 100.0));
		wheel.Rumble(percentForce, 0, 100);
		wheel.Friction(percentForce);
	}

	if ((driftRumble == 1) && (drift == 1) && (steering > 127) && (steering < 256) && (race == 1))
	{
		// Drift Effect including steering right
		double percentForce = (((steering - 127) / 128.0) * (driftRumbleControllerStrengthMultiplier / 100.0));
		wheel.Rumble(0, percentForce, 100);
		wheel.Friction(percentForce);
	}

	if ((hitGroundRumble == 1) && (oldHitGround != onGround) && (onGround == 1) && (race == 1))
	{
		// Shake when hitting ground
		double percentForce = ((hitGroundRumbleStrength) / 100.0);
		wheel.Rumble(percentForce, percentForce, 100);
		wheel.Constant(Direction::FromLeft, percentForce);
		Sleep(50);
		wheel.Constant(Direction::FromRight, percentForce);
	}

	if ((weaponRumble == 1) && (oldWeapon != weapon) && (race == 1))
	{
		// Shake when picking up new weapons or using them
		double percentForce = ((weaponRumbleStrength) / 100.0);
		wheel.Rumble(percentForce, percentForce, 300);
		wheel.Sine(80, 50, percentForce, 100);
	}

	if ((coinRumble == 1) && (oldCoins != coins) && (race == 1))
	{
		// Shake when picking up coins
		double percentForce = ((coinRumbleStrength) / 100.0);
		wheel.Rumble(percentForce, percentForce, 200);
		wheel.Sine(50, 50, percentForce, 100);
	}

	if ((dirtRumble == 1) && (3 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1))
	{
		// small friction when driving on dirt while moving
		double percentForce = ((dirtRumbleStrength) / 100.0);
		wheel.Rumble(percentForce, 0, 100);
		wheel.Friction(percentForce);
	}

	if ((speedBumpRumble == 1) && (10 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1))
	{
		// Small constant when hitting bumps
		double percentForce = ((speedBumpRumbleStrength) / 100.0);
		wheel.Rumble(percentForce, percentForce, 50);
		wheel.Constant(Direction::FromRight, percentForce);
		wheel.Constant(Direction::FromRight, 0);
	}

	if ((grassRumble == 1) && (4 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1))
	{
		// Wheel rumbles while driving on grass
		double percentForce = ((grassRumbleStrength) / 100.0);
		wheel.Rumble(0, percentForce, 50);
		wheel.Sine(50, 50, percentForce, 100);
	}

	if ((carpetRumble == 1) && (9 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1))
	{
		// Wheel rumbles while driving on carpet
		double percentForce = ((carpetRumbleStrength) / 100.0);
		wheel.Rumble(0, percentForce, 50);
		wheel.Sine(50, 50, percentForce, 100);
	}

	if ((waterRumble == 1) && (7 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1) && (steering >= 0) && (steering < 128))
	{
		// wheel hard to turn while driving through water
		double percentForce = ((waterRumbleWheelStrength) / 100.0);
		// 128 - steering / 128.0, not (128 - steering) / 128.0 as for the drift
		double percentForce1 = ((128 - steering / 128.0) * (waterRumbleControllerStrengthMultiplier / 100.0));
		wheel.Rumble(percentForce1, 0, 100);
		wheel.Friction(percentForce);
	}

	if ((waterRumble == 1) && (7 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1) && (steering > 127))
	{
		double percentForce = ((waterRumbleWheelStrength) / 100.0);
		double percentForce1 = ((steering - 127 / 128.0) * (waterRumbleControllerStrengthMultiplier / 100.0));
		wheel.Rumble(0, percentForce1, 100);
		wheel.Friction(percentForce);
	}

	if ((tileRumble == 1) && (12 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1))
	{
		// Wheel rumbles lightly when driving over tiles
		double percentForce = ((tileRumbleStrength) / 100.0);
		wheel.Rumble(0, percentForce, 150);
		wheel.Friction(percentForce);
	}

	if ((sandRumble == 1) && (14 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1))
	{
		// Wheel rumbles lightly when driving over sand
		double percentForce = ((sandRumbleStrength) / 100.0);
		wheel.Rumble(percentForce, 0, 50);
		wheel.Sine(70, 70, percentForce, 100);
	}

	if ((roughTrackRumble == 1) && (11 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1))
	{
		// Wheel rumbles lightly when driving over rough part of track
		double percentForce = ((roughTrackRumbleStrength) / 100.0);
		wheel.Rumble(0, percentForce, 100);
		wheel.Sine(40, 50, percentForce, 100);
	}

	if ((bridgeRumble == 1) && (8 == terrain) && (race == 1) && (onGround == 1) && (speed > 0.1))
	{
		// Wheel rumbles moderately when driving over wooden bridges
		double percentForce = ((bridgeRumbleStrength) / 100.0);
		wheel.Rumble(percentForce, percentForce, 100);
		wheel.Sine(120, 120, percentForce, 200);
	}

	oldCoins = coins;
	oldWeapon = weapon;
	oldHitGround = onGround;
	oldMenuPosition = menuPosition;
	oldGameState = gameState;
}
