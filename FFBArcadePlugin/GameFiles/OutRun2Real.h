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
#include "OutRun2Fake.h"

// OutRun 2 SP SDX, the force feedback of the game (see OutRun2Game): the plugin writes a jump from the
// function the game sends its drive board commands to, to FfbFunction. A force command plays a constant
// force or a vibration by speed (SpeedStrength), the other commands the spring of SpringStrength. The
// thread plays a vibration at each change of gear in a race.
class OutRun2Real : public OutRun2Game
{
public:
	OutRun2Real();
	void Update() override;

private:
	// The function of the game the drive board commands are sent to
	static const DWORD DriveBoardFunction = 0x08105A48;

	static OutRun2Real* instance;
	// Replaces the function at DriveBoardFunction: command 0x7B is a force; any other command (0x7D,
	// said to be unused, included) plays the spring of SpringStrength, read again at each command
	static signed int __cdecl FfbFunction(unsigned __int8 command, unsigned __int8 unknown1, unsigned __int8 force, char unknown2);
	// Writes the jump from DriveBoardFunction to FfbFunction
	void WriteJump();
	// The forces of the game:
	// 0x0B hitting the right wall hard, 0x10 scraping the left side of the car against a rail,
	// 0x04 back from the grass or the sand to the road on the right side: force from the right
	// 0x1B hitting the left wall hard, 0x14 back to the road on the left side, 0x00 over the grass,
	// the sand...: force from the left
	// 0x02 driving on the grass: vibration
	void PlayForce(__int8 force);
	void ThreadLoop() override;

	UINT8 lastGear = 0;
};
