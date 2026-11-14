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

// R-Tuned: Ultimate Street Racing (Sega, Lindbergh, TeknoParrot): the plugin turns the FFB of the game
// on, then plays its crash, spring and boost states, read in the game's memory.
class RTuned : public ffb::Game
{
public:
	void Update() override;

private:
	// 0xFF in a crash (vibration), 0x04 centering spring (SpringStrength)
	static constexpr ffb::GameValue<UINT8> SpringCrash{ ffb::Base::Absolute, 0x8519D18 };
	// Strength of the boost vibration, played as value / 400
	static constexpr ffb::GameValue<UINT8> BoostStrength{ ffb::Base::Absolute, 0x8519D10 };
	// 0x7F while boosting
	static constexpr ffb::GameValue<UINT8> Boost{ ffb::Base::Absolute, 0x8519D20 };
	// FFB state of the game: set from 0x01 to 0x09 by the plugin, which turns the FFB on
	static constexpr ffb::GameValue<UINT8> FFBState{ ffb::Base::Absolute, 0x8519C58 };

	int springStrength = ffb::Settings::Int(L"SpringStrength", 0);
};
