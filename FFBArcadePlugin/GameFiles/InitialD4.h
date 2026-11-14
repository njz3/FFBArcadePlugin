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

// Initial D Arcade Stage 4, 4 Japan and 5 (Sega Lindbergh): the command of the drive board is read
// in the game's memory, at an absolute address, and played with the usual drive board scaling
class InitialDLindberghGame : public ffb::Game
{
public:
	void Update() override;

protected:
	// driveBoardAddress: absolute address of the drive board command: byte 0 the command, byte 1 its
	// parameter, byte 2 the strength (see ffb::SegaInitialDDriveBoard).
	// applyDamper: the damper of the EnableDamper setting is played at each update
	InitialDLindberghGame(INT_PTR driveBoardAddress, bool applyDamper);

private:
	ffb::GameValue<int> driveBoardCommand;
	bool applyDamper;
	ffb::SegaInitialDDriveBoard driveBoard{ wheel };
};

// Initial D Arcade Stage 4 (export version), which plays no damper
class InitialD4 : public InitialDLindberghGame
{
public:
	InitialD4() : InitialDLindberghGame(0x089AE898, false) {}
};
