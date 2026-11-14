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
#include <Windows.h>
#include "../Core/Settings.h"

// The inputs of the wheel (InputDeviceWheelEnable) and of a second device of a game in M2Emulator:
// the joystick events (SDL) are written to the memory of the emulated game, or sent as keys to the
// window of the emulator
class M2EmulatorInputs
{
public:
	explicit M2EmulatorInputs(const HWND& window)
		: hWndM2(window)
	{
	}
	virtual ~M2EmulatorInputs() = default;

	// Called by the inputs thread every 16 ms
	virtual void Update() = 0;

protected:
	const HWND& hWndM2; // the window of the game
};
