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

// Ford Racing, other version (TeknoParrot): the plugin replaces Out32 of inpout32.dll, to which the game
// writes the force of the wheel (port 0x378), by a jump written over its first bytes once inpout32.dll is
// loaded: 0xFFFF0014 to 0xFFFF001E from the right (strongest at 0xFFFF0014), 6 to 15 from the left
// (strongest at 6).
class FordRacingOther : public ffb::Game
{
public:
	FordRacingOther();
	void Update() override;

private:
	static FordRacingOther* instance;
	static int __stdcall Out32Hook(DWORD device, DWORD data);
	// Writes a jump to `detour` over the first `length` bytes (at least 5) of `toHook`, the others NOPs
	static bool Hook(void* toHook, void* detour, int length);

	bool hooked = false;
	INT_PTR jumpBack = 0; // the address after the replaced bytes (unused)
};
