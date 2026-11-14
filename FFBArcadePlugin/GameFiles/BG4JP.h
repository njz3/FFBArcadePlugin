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

// Battle Gear 4 Tuned, Japan version v2.07 (Taito Type X+, TeknoParrot): in a race, a spring, and effects
// for the contacts of the car (walls, other cars), the boost and the gear shifts, read in the game's
// memory; their strength is the speed / 180 km/h, up to 1.
// The values are read as ints, of which the first byte is kept.
class BG4JP : public ffb::Game
{
public:
	void Update() override;

private:
	// != 0 in a race
	static constexpr ffb::GameValue<int> InRace{ ffb::Base::Module, 0x4A9508 };
	// Read, not used
	static constexpr ffb::GameValue<int> GroundContact{ ffb::Base::Module, 0x42EBB1 };
	// Bits: 0x02 boost, 0x10 a wall on the left (force from the left), 0x20 on the right (logged)
	static constexpr ffb::GameValue<int> WallContact{ ffb::Base::Module, 0x42EBB2 };
	// Bits: 0x01 a car, 0x08 a car on the left (force from the left), 0x02 on the right
	static constexpr ffb::GameValue<int> CarContact{ ffb::Base::Module, 0x42EBB3 };
	// Changes at a gear shift; 0 no effect
	static constexpr ffb::GameValue<int> ShiftEffect{ ffb::Base::Module, 0x42ECF0 };
	// Read, not used
	static constexpr ffb::GameValue<int> CurrentView{ ffb::Base::Module, 0x3F2654 };
	// Speed of the car (km/h)
	static constexpr ffb::GameValue<float> Speed{ ffb::Base::Module, 0x3F3000 };

	int springStrength = ffb::Settings::Int(L"SpringStrength", 100);
	int gearChangeStrength = ffb::Settings::Int(L"GearChangeStrength", 100);
	int gearChangeSinePeriod = ffb::Settings::Int(L"GearSinePeriod", 100);
	int enableBoostEffect = ffb::Settings::Int(L"EnableBoostEffect", 0);
	int enableGearShiftEffect = ffb::Settings::Int(L"EnableGearShiftEffect", 0);
	int boostSinePeriod = ffb::Settings::Int(L"BoostSinePeriod", 0);
	int boostFadeSinePeriod = ffb::Settings::Int(L"BoostFadeSinePeriod", 0);

	UINT8 lastShiftEffect = 0;
};
