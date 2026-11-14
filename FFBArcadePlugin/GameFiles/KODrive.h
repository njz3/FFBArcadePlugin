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

// KO Drive (TeknoParrot): the game keeps the last command of its drive board, a Sega drive board like
// the one of Initial D, in its memory; the plugin plays it at each loop.
class KODrive : public ffb::Game
{
public:
	void Update() override;

private:
	// Command of the drive board: byte 0 the command, byte 1 its parameter, byte 2 its strength
	static constexpr ffb::GameValue<int> DriveBoardCommand{ ffb::Base::Module, 0x7261F4 };

	// Strengths up to 24, a constant force from the left is (128 - strength) / 24, vibration periods
	// are parameter / 128 * 120 ms and the vibrations last 200 ms
	ffb::SegaInitialDDriveBoard driveBoard{ wheel, { 24.0, 128, 128.0, 200, 0xFFFFFFFF } };
};
