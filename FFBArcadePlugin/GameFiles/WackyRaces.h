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

// Wacky Races (Taito Type X2, TeknoParrot): the game sends the force of the wheel to its I/O board with
// the lamps, in a word read in the game's memory. Once the lamp bits removed, the word is one of 30
// levels of force: 16 (strongest) to 30 (weakest) from the right, 1 (strongest) to 15 (weakest) from the left.
class WackyRaces : public ffb::Game
{
public:
	void Update() override;

private:
	// Pointer (32 bits) to the output of the I/O board, the word of the force and the lamps at 0x45
	static constexpr ffb::GameValue<int> Output{ ffb::Base::Module, 0x7E00590 };
	static const int ForceAndLamps = 0x45;
	// Bits of the lamps in the word
	static constexpr int Lamps[] = { 16, 1024, 512, 128, 8, 256, 16384 };

	// Level of force of a word without the lamps, 0 if none
	static int ForceLevel(int word);
};
