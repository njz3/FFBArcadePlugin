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

// Sega Race TV (Sega Lindbergh): the force of the wheel is read in the game's memory, once the plugin
// has enabled the I/O calls of the game
class SegaRaceTV : public ffb::Game
{
public:
	void Update() override;

private:
	// Set to 1 at the first update: I/O calls enabled
	static constexpr ffb::GameValue<INT_PTR> IoCallsEnabled{ ffb::Base::Module, 0x8b3fe90 };
	// Force of the wheel: 1 to 127 from the left (1 the strongest), 129 and above from the right
	// (255 a full force), 0 and 128 no force
	static constexpr ffb::GameValue<int> WheelForce{ ffb::Base::Absolute, 0x8b44210 };

	bool ioEnabled = false;
};
