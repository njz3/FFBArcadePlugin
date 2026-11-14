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
#include "M2EmulatorDaytonaInputs.h"
#include "M2EmulatorIndy500Inputs.h"
#include "M2EmulatorSegaRallyInputs.h"

// M2Emulator (Sega Model 2): the game is found by the title of its window. The force is read in the
// memory of the emulated game: the command of the Model 2 drive board, or the motor value of Sega
// Rally. With InputDeviceWheelEnable, a thread writes the inputs of the wheel to the emulated memory,
// and the old inputs functions of the emulator are replaced by an empty one. Settings per game with
// the suffixes SRally, Daytona, Indy500, STCC, OverRev and SuperGT. Daytona USA has two hacks:
// DaytonaForcePanoramicAttract and DaytonaAIMultiplayerHack (AI cars in a 2 cabinets link).
class M2Emulator : public ffb::Game
{
public:
	void Update() override;

private:
	// the titles of the windows of the games, 1 to 20 (Sega Rally: 1, 14, 15; Daytona USA: 2, 7 to 13;
	// Indy 500: 3, 16, 17; Sega Touring Car: 4, 18, 19; Over Rev: 5, 20; Super GT: 6)
	static const char* const Titles[21];

	// replaces the inputs functions of the emulator (hookAddressM2A, B and C)
	static void OldInputs();
	static bool __stdcall ExitHook(UINT exitCode);
	// Replaces the first `length` bytes at `at` by a jump to `to`, the rest by NOP
	static void HookWithJump(INT_PTR at, void* to, int length);
	static DWORD WINAPI InputLoop(LPVOID game);
	void UpdateInputs();

	int InputDeviceWheelEnable = ffb::Settings::Int(L"InputDeviceWheelEnable", 0);
	int DaytonaAIMultiplayerHack = ffb::Settings::Int(L"DaytonaAIMultiplayerHack", 0);
	int DaytonaForcePanoramicAttract = ffb::Settings::Int(L"DaytonaForcePanoramicAttract", 0);
	int EnableOutputs = ffb::Settings::Int(L"EnableOutputs", 0);

	HWND hWnd[21] = {};  // the window of each game, null if it is not open
	HWND hWndM2 = NULL;  // the window of the game
	char romnameM2Buffer[256] = {};
	INT_PTR hookAddressM2A = 0;
	INT_PTR hookAddressM2B = 0;
	INT_PTR hookAddressM2C = 0;
	UINT8 OldFFB = 0; // last motor value of Sega Rally
	UINT8 ff = 0;     // drive board command, read at the end of a loop for the next one
	bool init = false;
	bool inputinit = false;
	bool CustomStrengthInit = false;
	bool outputinit = false;

	M2EmulatorSegaRallyInputs segaRallyInputs{ hWndM2 };
	M2EmulatorDaytonaInputs daytonaInputs{ hWndM2 };
	M2EmulatorIndy500Inputs indy500Inputs{ hWndM2 };
};
