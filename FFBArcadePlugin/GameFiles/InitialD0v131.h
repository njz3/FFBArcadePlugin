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

// Initial D Zero (Sega ALLS): the command of the drive board is read in the game's memory (byte 0 the
// command, byte 1 its parameter, byte 2 the strength, see ffb::SegaInitialDDriveBoard). The plugin
// also loads OutputBlaster.dll (EnableOutputs setting) and ends the game with the Escape key
// (EscapeKeyExitViaPlugin setting).
class InitialDZeroGame : public ffb::Game
{
public:
	void Update() override;

protected:
	// driveBoardOffset: offset of the drive board command in the game's executable.
	// loadOutputs: OutputBlaster.dll is loaded at the first update
	InitialDZeroGame(INT_PTR driveBoardOffset, const ffb::SegaInitialDDriveBoard::Scaling& scaling, bool loadOutputs);

	// The usual scaling with vibrations of 200 ms
	static constexpr ffb::SegaInitialDDriveBoard::Scaling LongVibrationScaling{ 127.0, 128, 127.0, 200, 0xFFFFFFFF };

private:
	ffb::GameValue<int> driveBoardCommand;
	ffb::SegaInitialDDriveBoard driveBoard;
	bool loadOutputs;
	bool outputsLoaded = false;
	int escapeKeyExitViaPlugin = ffb::Settings::Int(L"EscapeKeyExitViaPlugin", 0);
};

// Initial D Zero v1.31: OutputBlaster.dll is loaded when EnableOutputs is not 0
class InitialD0 : public InitialDZeroGame
{
public:
	InitialD0() : InitialDZeroGame(0x168317C, LongVibrationScaling, ffb::Settings::Int(L"EnableOutputs", 0) != 0) {}
};
