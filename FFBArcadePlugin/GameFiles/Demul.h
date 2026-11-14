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
#include "DemulATVTrackInputs.h"
#include "DemulFasterThanSpeedInputs.h"
#include "DemulInitialDInputs.h"
#include "DemulMaximumSpeedInputs.h"
#include "DemulNascarInputs.h"
#include "DemulSmashingDriveInputs.h"

// Demul (Sega Naomi, Atomiswave): the game is found by the title of the Demul window (with "FPS" or
// "spg"). The force is read in the memory of the emulated game, found by pattern scans, by a thread of
// the game every 16 ms. With InputDeviceWheelEnable, the inputs of the wheel are written to the
// emulated memory, once their addresses are found, and padDemul's own inputs are removed (NOPs, for
// the Demul versions of 2018-04-28 and 2011-11-17, told apart by the CRC of their PE header).
// Settings per game with the suffixes NascarRacing, InitialDDemul, SmashingDrive, MaximumSpeed,
// FasterSpeed and ATVTrack.
class Demul : public ffb::Game
{
public:
	void Update() override;

private:
	// the force of each game, played by its thread every 16 ms
	int InitialDFFBLoop();
	int SmashingDriveFFBLoop();
	int ATVTrackFFBLoop();
	int FasterThanSpeedFFBLoop();
	int MaximumSpeedFFBLoop();
	int NascarFFBLoop();
	static DWORD WINAPI NascarRunningLoop(LPVOID game);
	static DWORD WINAPI InitialDRunningLoop(LPVOID game);
	static DWORD WINAPI SmashingDriveRunningLoop(LPVOID game);
	static DWORD WINAPI ATVTrackRunningLoop(LPVOID game);
	static DWORD WINAPI FasterThanSpeedRunningLoop(LPVOID game);
	static DWORD WINAPI MaximumSpeedRunningLoop(LPVOID game);

	int InputDeviceWheelEnable = ffb::Settings::Int(L"InputDeviceWheelEnable", 0);

	HINSTANCE gl_padDemul = NULL;
	HWND hWnd = NULL;
	bool VolumeMute = false;
	bool CRCinit = false;
	bool NOPinit = false;
	bool InputFind = false;
	bool Demul180428 = false;
	bool Demul111117 = false;
	bool NascarRunning = false;
	bool InitialDRunning = false;
	bool SmashingDriveRunning = false;
	bool MaximumSpeedRunning = false;
	bool FasterThanSpeedRunning = false;
	bool ATVTrackRunning = false;
	bool FFBGameInit = false;
	bool KickStartWait = false;
	bool WindowSearch = false;
	INT_PTR FFBAddress = 0;
	int ffnascar = 0;
	int fffaster = 0;
	char* aAddy = nullptr;  // last match of the pattern scans of the inputs
	char* aAddy2 = nullptr; // last match of the pattern scans of the force

	ffb::SegaInitialDDriveBoard driveBoard{ wheel };

	DemulInputAddresses inputAddresses;
	DemulNascarInputs nascarInputs{ inputAddresses };
	DemulInitialDInputs initialDInputs{ inputAddresses };
	DemulSmashingDriveInputs smashingDriveInputs{ inputAddresses };
	DemulATVTrackInputs atvTrackInputs{ inputAddresses };
	DemulFasterThanSpeedInputs fasterThanSpeedInputs{ inputAddresses };
	DemulMaximumSpeedInputs maximumSpeedInputs{ inputAddresses };
};
