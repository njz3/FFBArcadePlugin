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

// Showdown (Sega, TeknoParrot): the plugin replaces Out32 of inpout32.dll, to which the game writes the
// force of the wheel (port 0x378): 16 to 31 from the left (strongest at 16), 1 to 15 from the right
// (strongest at 1). Looked at, not used: a function of the game getting the XINPUT_VIBRATION of the
// rumble (game executable + 0x92E000).
class Showdown : public ffb::Game
{
public:
	Showdown();
	void Update() override;

private:
	static Showdown* instance;
	static int(__stdcall* out32Original)(DWORD device, DWORD data);
	static int __stdcall Out32Hook(DWORD device, DWORD data);

	bool hooked = false;
};
