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

// Sega Rally 3 (Sega Europa-R) without TeknoParrot: the game writes the force of the wheel with Out32
// of inpout32.dll, whose first bytes the plugin replaces with a jump to its own function. The value
// is the one of the drive board: 16 to 31 from the left (16 the strongest), 1 to 15 from the right
// (1 the strongest). DllMain.cpp also patches the function at 0x57B2F0 to return 1 when the plugin
// is loaded.
class SegaRally3Other : public ffb::Game
{
public:
	SegaRally3Other();
	void Update() override;

private:
	// Bytes of Out32 replaced: a jump to Out32Hook (5 bytes), then NOPs
	static const int HookLength = 6;

	static SegaRally3Other* instance;
	// Replaces Out32(port, value) of inpout32.dll: never calls it
	static int __stdcall Out32Hook(DWORD port, DWORD data);

	bool hooked = false;
};
