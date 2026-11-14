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

// Mario Kart Arcade GP DX USA (Namco, TeknoParrot), effects made by the plugin from values of the game's
// memory during a race (shake, terrain, steering, drift, boost, coins, items), each one enabled by a
// setting with its strength (%). Unlike the other versions, only the first effect that applies plays
// (besides the constant force of the steering), and the rumble is played as the LeftRight effect. The
// settings are read again at each update.
class MarioKartGPDXUSACustom : public ffb::Game
{
public:
	void Update() override;

private:
	// 4194308 when hitting walls, other karts or getting hit by items
	static constexpr ffb::GameValue<INT_PTR> Shake{ ffb::Base::Module, 0xB96B74 };
	// Pointer to the kart, holding at:
	static constexpr ffb::GameValue<INT_PTR> Kart{ ffb::Base::Module, 0xB4F324 };
	static const int Terrain = 0x628;  // int, its low byte: 3 dirt, 4 grass, 7 water, 8 wooden bridge, 9 carpet, 10 bump, 11 rough track, 12 tiles, 14 sand
	static const int OnGround = 0x658; // int, its low byte: 1 on the ground, else flying
	static const int Speed = 0x558;    // float: speed of the kart
	static const int Drift = 0x674;    // UINT8: 1 drifting
	// 1 during a race only
	static constexpr ffb::PointerChain<UINT8, 5> Race{ { ffb::Base::Module, 0xB486BC }, { 0x2E0, 0xAC, 0xC, 0xC, 0x330 } };
	// Change when picking up coins
	static constexpr ffb::GameValue<UINT8> Coins{ ffb::Base::Module, 0xB4E330 };
	// Change when picking up an item box or using an item
	static constexpr ffb::GameValue<UINT8> Weapon{ ffb::Base::Module, 0xB9548D };
	// Pointer to a structure with the steering at 0x125 (UINT8, 0 to 255: < 128 left)
	static constexpr ffb::GameValue<INT_PTR> SteeringPointer{ ffb::Base::Module, 0xB4F2E0 };
	static const int Steering = 0x125;
	// Pointer to a structure with a byte at 0x3A4 that is 1 during a boost
	static constexpr ffb::GameValue<INT_PTR> BoostPointer{ ffb::Base::Module, 0xB441C8 };
	static const int Boost = 0x3A4;

	// values of the previous update
	int oldCoins = 0;
	int oldWeapon = 0;
	int oldHitGround = 0;
};
