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

// Crazy Taxi (Sega, PC Steam version): during a ride, the plugin turns the rumble of the game off and
// plays the ground the taxi drives on, its collisions, its crazy moves and the rumble of the game, read
// in the game's memory.
class CrazyTaxi : public ffb::Game
{
public:
	void Update() override;

private:
	// 0 during a ride (with Timer not 0)
	static constexpr ffb::GameValue<UINT8> InGame{ ffb::Base::Module, 0x12AA314 };
	// Timer of the ride: not 0 during a ride
	static constexpr ffb::GameValue<INT_PTR> Timer{ ffb::Base::Module, 0x12A41A4 };
	// Ground under the taxi: 0x00 road, 0x03 grass, 0x05 shoreline, 0x09 sand, 0x0A footpath,
	// 0x0E underwater, 0x11 and 0x13 stairs (second motor), 0x12 and 0x14 stairs (first motor),
	// 0x15 subway, 0xFF a collision with a car or a wall
	static constexpr ffb::GameValue<UINT8> Ground{ ffb::Base::Module, 0x1AB0D74 };
	// Not 0 while the game rumbles (OriginalRumbleEffect)
	static constexpr ffb::GameValue<UINT8> OriginalRumble{ ffb::Base::Module, 0x12EF93B };
	// Crazy moves: bit 0x10 of the first byte, or the second byte not 0
	static constexpr ffb::GameValue<UINT8> CrazyMoves1{ ffb::Base::Module, 0x12EF892 };
	static constexpr ffb::GameValue<UINT8> CrazyMoves2{ ffb::Base::Module, 0x12EF893 };
	// Speed of the taxi: the strength of the effects is speed / 10, up to 1
	static constexpr ffb::GameValue<float> Speed{ ffb::Base::Module, 0x106A010 };
	// > 0 in a collision on the left / on the right
	static constexpr ffb::GameValue<float> LeftWall{ ffb::Base::Module, 0x126214C };
	static constexpr ffb::GameValue<float> RightWall{ ffb::Base::Module, 0x12621D0 };
	// Byte of the game's code set to 0 during a ride: turns the rumble/FFB of the game off
	static constexpr ffb::GameValue<UINT8> NativeRumble{ ffb::Base::Module, 0x1C363A };

	int springStrength = ffb::Settings::Int(L"SpringStrength", 0);
	int disableConstantEffects = ffb::Settings::Int(L"DisableConstantEffects", 0);
	int engineRumbleEffect = ffb::Settings::Int(L"EngineRumbleEffect", 0);
	int originalRumbleEffect = ffb::Settings::Int(L"OriginalRumbleEffect", 0);
	int enableGroundEffects = ffb::Settings::Int(L"EnableGroundEffects", 0);
	int grassSinePeriod = ffb::Settings::Int(L"FFBGrassSinePeriod", 0);
	int underwaterSinePeriod = ffb::Settings::Int(L"FFBUnderwaterSinePeriod", 0);
	int shorelineSinePeriod = ffb::Settings::Int(L"FFBShorelineSinePeriod", 0);
	int grassFadeSinePeriod = ffb::Settings::Int(L"FFBGrassFadeSinePeriod", 0);
	int underwaterFadeSinePeriod = ffb::Settings::Int(L"FFBUnderwaterFadeSinePeriod", 0);
	int shorelineFadeSinePeriod = ffb::Settings::Int(L"FFBShorelineFadeSinePeriod", 0);
	int footpathSinePeriod = ffb::Settings::Int(L"FFBFootpathSinePeriod", 0);
	int footpathFadeSinePeriod = ffb::Settings::Int(L"FFBFootpathFadeSinePeriod", 0);
	int sandSinePeriod = ffb::Settings::Int(L"FFBSandSinePeriod", 0);
	int sandFadeSinePeriod = ffb::Settings::Int(L"FFBSandFadeSinePeriod", 0);
	int stairsSinePeriod = ffb::Settings::Int(L"FFBStairsSinePeriod", 0);
	int stairsFadeSinePeriod = ffb::Settings::Int(L"FFBStairsFadeSinePeriod", 0);
	int subwaySinePeriod = ffb::Settings::Int(L"FFBSubwaySinePeriod", 0);
	int subwayFadeSinePeriod = ffb::Settings::Int(L"FFBSubwayFadeSinePeriod", 0);
	int sandFriction = ffb::Settings::Int(L"FFBSandFriction", 0);
	int underwaterFriction = ffb::Settings::Int(L"FFBUnderwaterFriction", 0);
	int shorelineFriction = ffb::Settings::Int(L"FFBShorelineFriction", 0);

	UINT8 lastGround = 0; // ground of the last loop of a ride
	bool collision = false;
};
