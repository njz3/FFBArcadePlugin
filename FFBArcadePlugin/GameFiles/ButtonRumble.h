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

// Button rumble (any game): the buttons of the settings rumble the first device (Button1Rumble to
// Button10Rumble) or the second one (Button1Device2Rumble to Button10Device2Rumble) while they are held,
// at RumbleStrength (%) for RumbleLength ms. The motors are those of the one setting set among
// BothRumbleMotor, LeftRumbleMotor and RightRumbleMotor. ShowButtonNumbersForSetup shows the number of
// each button pressed in a message box. The plugin waits for the joystick events in the FFB loop.
class ButtonRumble : public ffb::Game
{
public:
	ButtonRumble();
	void Update() override;

private:
	static const int ButtonCount = 10;

	// A button pressed on the device: rumble if it is one of the buttons
	void ButtonDown(ffb::Device& device, const int (&buttons)[ButtonCount], int button, double strength, UINT32 length_ms);
	static bool IsOneOf(int button, const int (&buttons)[ButtonCount]);

	int showButtonNumbersForSetup = ffb::Settings::Int(L"ShowButtonNumbersForSetup", 0);
	int bothRumbleMotor = ffb::Settings::Int(L"BothRumbleMotor", 0);
	int leftRumbleMotor = ffb::Settings::Int(L"LeftRumbleMotor", 0);
	int rightRumbleMotor = ffb::Settings::Int(L"RightRumbleMotor", 0);
	int rumbleStrength = ffb::Settings::Int(L"RumbleStrength", 0);
	int rumbleLength = ffb::Settings::Int(L"RumbleLength", 0);
	int device1Buttons[ButtonCount]; // Button1Rumble...
	int device2Buttons[ButtonCount]; // Button1Device2Rumble...

	bool initialized = false;
};
