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
#include <vector>
#include "../Hardware/OutputBoard.h"

// A game of MAME, Supermodel or Flycast whose force feedback the plugin plays
struct EmulatorGame
{
	std::vector<const char*> roms;      // MAME rom names, Supermodel game names, Flycast game titles (with their new line)
	const wchar_t* suffix;              // of its settings in FFBPlugin.ini: MinForce<suffix>...
	unsigned fields;                    // the settings it has (ffb::ForceProfile::Field)
	ffb::OutputBoard* (*createBoard)(); // its force feedback hardware
};

// The game of a rom name, null if none
const EmulatorGame* FindEmulatorGame(const char* romName);
