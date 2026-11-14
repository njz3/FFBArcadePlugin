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

// Sega World Drivers Championship 2018 (Sega, ALLS, TeknoParrot): the game keeps the last command of its
// drive board, like the one of Initial D, in its memory, at an address that depends on the version of the
// game (1.03 or 2.30). The version is found by the name of the game, "SDDS", at an address of each version;
// the commands are played once it is found. Escape quits the game when EscapeKeyExitViaPlugin is 1.
class SWDC : public ffb::Game
{
public:
	void Update() override;

private:
	// The name of the game, "SDDS" (4 characters), in version 1.03 and in version 2.30
	static constexpr ffb::GameValue<int> Name103{ ffb::Base::Module, 0x8E5CBD0 };
	static constexpr ffb::GameValue<int> Name230{ ffb::Base::Module, 0x9BC7B20 };
	// Command of the drive board: byte 0 the command, byte 1 its parameter, byte 2 its strength
	static constexpr ffb::GameValue<int> DriveBoardCommand103{ ffb::Base::Module, 0x8E5CCF4 };
	static constexpr ffb::GameValue<int> DriveBoardCommand230{ ffb::Base::Module, 0x9BC7B18 };

	// True when the 4 bytes of the value are the characters of the name of the game
	static bool IsGameName(DWORD value);

	int escapeKeyExitViaPlugin = ffb::Settings::Int(L"EscapeKeyExitViaPlugin", 0);

	bool versionFound = false;
	bool version103 = false;
	bool version230 = false;
	DWORD command = 0; // last command read
	ffb::SegaInitialDDriveBoard driveBoard{ wheel };
};
