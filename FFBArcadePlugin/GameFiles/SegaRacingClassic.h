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

// Sega Racing Classic (Sega): the command of the drive board is read in the game's memory
class SegaRacingClassic : public ffb::Game
{
public:
	void Update() override;

private:
	// Drive board command, its strength in the low 3 bits (0 the strongest):
	// 0x98-0x9F roll right (force from the left), 0xA8-0xAF roll left (force from the right),
	// 0xB8-0xBF uncentering (vibration), 0xC0-0xC7 centering (spring), 0xD8-0xDF clutch (friction)
	static constexpr ffb::GameValue<UINT8> DriveBoardCommand{ ffb::Base::Absolute, 0x834C19 };
};
