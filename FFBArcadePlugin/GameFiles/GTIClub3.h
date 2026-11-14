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

// GTI Club Supermini Festa (Konami, TeknoParrot): the force and the vibration of the wheel are read in
// the game's memory, and played out of the menus.
class GTIClub3 : public ffb::Game
{
public:
	void Update() override;

private:
	// Force of the wheel: 0x01 to 0x7F from the left (0x7F the strongest), 0x81 to 0xFF from the right
	// (0x81 the strongest)
	static constexpr ffb::GameValue<UINT8> WheelForce{ ffb::Base::Absolute, 0x918CBC };
	// Strength of the vibration: 0x01 to 0x3F
	static constexpr ffb::GameValue<UINT8> Vibration{ ffb::Base::Absolute, 0x918CBD };
	// The byte after them, only logged
	static constexpr ffb::GameValue<UINT8> LoggedValue{ ffb::Base::Absolute, 0x918CBE };
	// Not 0 in the menus
	static constexpr ffb::GameValue<UINT8> Menu{ ffb::Base::Module, 0x518E8E };
};
