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
#include "../Hardware/SegaInitialD.h"

// Initial D The Arcade v2.31 (Sega ALLS): the command of the drive board is read in the game's memory,
// with a strength of 16 bits (a full force is 32767)
class InitialDTA231 : public ffb::Game
{
public:
	void Update() override;

private:
	// Drive board command: byte 0 the command, byte 1 its parameter (see ffb::SegaInitialDDriveBoard)
	static constexpr ffb::GameValue<int> DriveBoardCommand{ ffb::Base::Module, 0x3D35838 };
	// Strength of the command, 0 to 32767 (bytes 2 and 3 of the command)
	static constexpr ffb::GameValue<WORD> Strength{ ffb::Base::Module, 0x3D3583A };

	ffb::SegaInitialDDriveBoard driveBoard{ wheel, { 32767.0, 32767, 127.0, 100, 0xFFFFFFFF } };
};
