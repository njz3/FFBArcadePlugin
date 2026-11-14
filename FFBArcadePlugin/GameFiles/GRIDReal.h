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
#include <Xinput.h>

// GRID (Codemasters, TeknoParrot), with the force feedback of the game: the plugin replaces the game
// function enabling its FFB (to enable it, and the rumble with FFBOrRumble) and Out32 of inpout32.dll, to
// which the game writes the force of the wheel (port 0x1020): every 5th write is played, 16 to 31 from the
// left (strongest at 16), 1 to 15 from the right (strongest at 1). With FFBOrRumble the force is not
// played, the rumble of the game (XInputSetState) is.
class GRIDReal : public ffb::Game
{
public:
	GRIDReal();
	void Update() override;

private:
	// Function of the game enabling its FFB, relative to the game executable: its object gets the FFB
	// enabled (BYTE) at +92 and the rumble enabled (BYTE) at +12
	static const INT_PTR EnableFFB = 0x79CDE0;

	static GRIDReal* instance;
	static int(__stdcall* out32Original)(DWORD device, DWORD data);
	static int(__fastcall* enableFFBOriginal)(int object, double unknown);
	static int __stdcall Out32Hook(DWORD device, DWORD data);
	static int __fastcall EnableFFBHook(int object, double unknown);
	static DWORD WINAPI XInputSetStateHook(DWORD userIndex, XINPUT_VIBRATION* vibration);

	bool hooked = false;
	int ffbCounter = 0; // writes to the port 0x1020 since the last one played
};
