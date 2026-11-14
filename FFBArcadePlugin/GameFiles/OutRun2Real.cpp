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

#include "OutRun2Real.h"

using namespace ffb;

extern SDL_Event e;

OutRun2Real* OutRun2Real::instance = nullptr;

OutRun2Real::OutRun2Real()
{
	instance = this;
}

void OutRun2Real::PlayForce(__int8 force)
{
	Memory::Read(Speed); // read, not used

	double percentForce = speedStrength / 100.0;

	if ((force == 0x10) || (force == 0x0B) || (force == 0x04))
		wheel.ConstantWithRumble(Direction::FromRight, percentForce);
	if ((force == 0x00) || (force == 0x1B) || (force == 0x14))
		wheel.ConstantWithRumble(Direction::FromLeft, percentForce);
	if (force == 0x02)
	{
		wheel.Rumble(percentForce, percentForce, 100);
		wheel.Sine(70, 80, percentForce, 200);
	}
}

signed int __cdecl OutRun2Real::FfbFunction(unsigned __int8 command, unsigned __int8 unknown1, unsigned __int8 force, char unknown2)
{
	int springStrength = Settings::Int(L"SpringStrength", 0);
	if (command == 0x7B)
		instance->PlayForce(force);
	else
		instance->wheel.KeptSpring(springStrength / 100.0);
	return 0;
}

// Written by hand, without changing the protection of the code
void OutRun2Real::WriteJump()
{
	DWORD from = DriveBoardFunction;
	DWORD to = (DWORD)(void*)FfbFunction;
	DWORD relative = to - from - 5;
	*(BYTE*)from = 0xE9;
	*(DWORD*)(from + 1) = relative;
}

void OutRun2Real::Update()
{
	StartThread();

	// SDL_WaitEvent returns when there is an event: the loop does not end
	while (SDL_WaitEvent(&e) != 0)
	{
		// at each event
		WriteJump();
		HandleEvent(e);
	}
}

void OutRun2Real::ThreadLoop()
{
	UINT8 gear = Memory::Read(Gear);
	UINT8 inRace = Memory::Read(InRace);
	float speed = Memory::Read(Speed);

	speedStrength = SpeedStrength(speed);

	ApplyDamper();

	if ((lastGear != gear) && (inRace == 1) && (speed >= 0.1))
	{
		wheel.Sine(240, 320, 0.1, 250);
		wheel.Rumble(0.1, 0.1, 100);
	}

	lastGear = gear;
}
