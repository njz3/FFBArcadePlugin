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

// Mario Kart Arcade GP DX 1.00 (Namco, TeknoParrot), effects made by the plugin: the shake of the kart
// and the terrain it drives on are read in the game's memory, during a race.
class MarioKartGPDX100Custom : public ffb::Game
{
public:
	void Update() override;

private:
	// 4194308 when hitting walls, other karts or getting hit by items
	static constexpr ffb::GameValue<int> Shake{ ffb::Base::Module, 0x564C5F };
	// Pointer (32 bits) to the kart, holding at:
	static constexpr ffb::GameValue<int> Kart{ ffb::Base::Module, 0x559B68 };
	static const int Terrain = 0x5F8;  // int: 3 dirt, 4 grass, 7 water, 8 wooden bridge, 10 bump, 11 rough track, 12 tiles, 14 sand
	static const int OnGround = 0x628; // int: 1 on the ground, else flying
	static const int Moving = 0x520;   // int: 1065353216 (1.0f) when the kart moves
	// Acceleration, 0 to 255 (read, unused)
	static constexpr ffb::GameValue<int> Acceleration{ ffb::Base::Module, 0x563860 };
	// Pointer (32 bits) to a chain of pointers (32 bits) at 0x1F0, 0x18 and 0x7C leading to an int at
	// 0x164 that is 1 during a race only
	static constexpr ffb::GameValue<int> Race{ ffb::Base::Module, 0x5532C4 };
	static constexpr int RaceChain[] = { 0x1F0, 0x18, 0x7C };
	static const int InRace = 0x164;
};
