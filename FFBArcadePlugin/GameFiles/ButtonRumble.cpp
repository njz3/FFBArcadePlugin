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

#include <stdio.h>
#include "ButtonRumble.h"
#include "../Core/Devices.h"
#include "SDL.h"
#include "../Core/PluginState.h"


using namespace ffb;

static const wchar_t* const Device1ButtonKeys[] = {
	L"Button1Rumble", L"Button2Rumble", L"Button3Rumble", L"Button4Rumble", L"Button5Rumble",
	L"Button6Rumble", L"Button7Rumble", L"Button8Rumble", L"Button9Rumble", L"Button10Rumble",
};

static const wchar_t* const Device2ButtonKeys[] = {
	L"Button1Device2Rumble", L"Button2Device2Rumble", L"Button3Device2Rumble", L"Button4Device2Rumble", L"Button5Device2Rumble",
	L"Button6Device2Rumble", L"Button7Device2Rumble", L"Button8Device2Rumble", L"Button9Device2Rumble", L"Button10Device2Rumble",
};

ButtonRumble::ButtonRumble()
{
	for (int i = 0; i < ButtonCount; i++)
	{
		device1Buttons[i] = Settings::Int(Device1ButtonKeys[i], 0);
		device2Buttons[i] = Settings::Int(Device2ButtonKeys[i], 0);
	}
}

bool ButtonRumble::IsOneOf(int button, const int (&buttons)[ButtonCount])
{
	for (int rumbleButton : buttons)
	{
		if (button == rumbleButton)
			return true;
	}
	return false;
}

void ButtonRumble::ButtonDown(Device& device, const int (&buttons)[ButtonCount], int button, double strength, UINT32 length_ms)
{
	if (bothRumbleMotor && !leftRumbleMotor && !rightRumbleMotor)
	{
		if (IsOneOf(button, buttons))
			device.Rumble(strength, strength, length_ms);
	}

	if (!bothRumbleMotor && leftRumbleMotor && !rightRumbleMotor)
	{
		if (IsOneOf(button, buttons))
			device.Rumble(0, strength, length_ms);
	}

	if (!bothRumbleMotor && !leftRumbleMotor && rightRumbleMotor)
	{
		if (IsOneOf(button, buttons))
			device.Rumble(strength, 0, length_ms);
	}
}

void ButtonRumble::Update()
{
	if (!initialized)
	{
		OpenHapticDevice2();
		initialized = true;
	}

	// e is the event of the plugin, also read by its FFB strength adjustment
	while (SDL_WaitEvent(&e) != 0)
	{
		if (showButtonNumbersForSetup)
		{
			if (e.type == SDL_JOYBUTTONDOWN)
			{
				if (e.jaxis.which == joystick_index1 || e.jaxis.which == joystick_index2)
				{
					if (e.jbutton.button >= 0)
					{
						char buff[100];
						sprintf_s(buff, "Button %d Pressed", e.jbutton.button);
						MessageBoxA(NULL, buff, "", NULL);
					}
				}
			}
		}

		double percentForce = rumbleStrength / 100.0;
		UINT32 length_ms = rumbleLength;

		if (e.type == SDL_JOYBUTTONDOWN)
		{
			if (e.jaxis.which == joystick_index1)
				ButtonDown(wheel, device1Buttons, e.jbutton.button, percentForce, length_ms);

			if (e.jaxis.which == joystick_index2)
				ButtonDown(device2, device2Buttons, e.jbutton.button, percentForce, length_ms);
		}

		if (e.type == SDL_JOYBUTTONUP)
		{
			if (e.jaxis.which == joystick_index1)
			{
				if (IsOneOf(e.jbutton.button, device1Buttons))
					wheel.Rumble(0, 0, length_ms);
			}

			if (e.jaxis.which == joystick_index2)
			{
				if (IsOneOf(e.jbutton.button, device2Buttons))
					device2.Rumble(0, 0, length_ms);
			}
		}
	}
}
