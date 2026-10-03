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

#include <string>
#include "ButtonRumble.h"
#include "../Common Files/Devices.h"
#include "SDL.h"
#include <Windows.h>
static bool init = false;
extern int joystick_index1;
extern int joystick_index2;
extern SDL_Event e;

static int ShowButtonNumbersForSetup = GetSettingInt(TEXT("ShowButtonNumbersForSetup"), 0);
static int BothRumbleMotor = GetSettingInt(TEXT("BothRumbleMotor"), 0);
static int LeftRumbleMotor = GetSettingInt(TEXT("LeftRumbleMotor"), 0);
static int RightRumbleMotor = GetSettingInt(TEXT("RightRumbleMotor"), 0);
static int RumbleStrength = GetSettingInt(TEXT("RumbleStrength"), 0);
static int RumbleLength = GetSettingInt(TEXT("RumbleLength"), 0);
static int Button1Rumble = GetSettingInt(TEXT("Button1Rumble"), 0);
static int Button2Rumble = GetSettingInt(TEXT("Button2Rumble"), 0);
static int Button3Rumble = GetSettingInt(TEXT("Button3Rumble"), 0);
static int Button4Rumble = GetSettingInt(TEXT("Button4Rumble"), 0);
static int Button5Rumble = GetSettingInt(TEXT("Button5Rumble"), 0);
static int Button6Rumble = GetSettingInt(TEXT("Button6Rumble"), 0);
static int Button7Rumble = GetSettingInt(TEXT("Button7Rumble"), 0);
static int Button8Rumble = GetSettingInt(TEXT("Button8Rumble"), 0);
static int Button9Rumble = GetSettingInt(TEXT("Button9Rumble"), 0);
static int Button10Rumble = GetSettingInt(TEXT("Button10Rumble"), 0);
static int Button1Device2Rumble = GetSettingInt(TEXT("Button1Device2Rumble"), 0);
static int Button2Device2Rumble = GetSettingInt(TEXT("Button2Device2Rumble"), 0);
static int Button3Device2Rumble = GetSettingInt(TEXT("Button3Device2Rumble"), 0);
static int Button4Device2Rumble = GetSettingInt(TEXT("Button4Device2Rumble"), 0);
static int Button5Device2Rumble = GetSettingInt(TEXT("Button5Device2Rumble"), 0);
static int Button6Device2Rumble = GetSettingInt(TEXT("Button6Device2Rumble"), 0);
static int Button7Device2Rumble = GetSettingInt(TEXT("Button7Device2Rumble"), 0);
static int Button8Device2Rumble = GetSettingInt(TEXT("Button8Device2Rumble"), 0);
static int Button9Device2Rumble = GetSettingInt(TEXT("Button9Device2Rumble"), 0);
static int Button10Device2Rumble = GetSettingInt(TEXT("Button10Device2Rumble"), 0);

void ButtonRumble::FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers) {

	if (!init)
	{
		OpenHapticDevice2();
		init = true;
	}
	
	while (SDL_WaitEvent(&e) != 0)
	{
		if (ShowButtonNumbersForSetup)
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

		double percentForce = RumbleStrength / 100.0;
		UINT32 length_ms = RumbleLength;

		if (e.type == SDL_JOYBUTTONDOWN)
		{
			if (e.jaxis.which == joystick_index1)
			{
				if (BothRumbleMotor && !LeftRumbleMotor && !RightRumbleMotor)
				{
					if (e.jbutton.button == Button1Rumble || e.jbutton.button == Button2Rumble || e.jbutton.button == Button3Rumble || e.jbutton.button == Button4Rumble || e.jbutton.button == Button5Rumble || e.jbutton.button == Button6Rumble || e.jbutton.button == Button7Rumble || e.jbutton.button == Button8Rumble || e.jbutton.button == Button9Rumble || e.jbutton.button == Button10Rumble)
					{
						triggers->Rumble(percentForce, percentForce, length_ms);
					}
				}

				if (!BothRumbleMotor && LeftRumbleMotor && !RightRumbleMotor)
				{
					if (e.jbutton.button == Button1Rumble || e.jbutton.button == Button2Rumble || e.jbutton.button == Button3Rumble || e.jbutton.button == Button4Rumble || e.jbutton.button == Button5Rumble || e.jbutton.button == Button6Rumble || e.jbutton.button == Button7Rumble || e.jbutton.button == Button8Rumble || e.jbutton.button == Button9Rumble || e.jbutton.button == Button10Rumble)
					{
							triggers->Rumble(0, percentForce, length_ms);
					}
				}

				if (!BothRumbleMotor && !LeftRumbleMotor && RightRumbleMotor)
				{
					if (e.jbutton.button == Button1Rumble || e.jbutton.button == Button2Rumble || e.jbutton.button == Button3Rumble || e.jbutton.button == Button4Rumble || e.jbutton.button == Button5Rumble || e.jbutton.button == Button6Rumble || e.jbutton.button == Button7Rumble || e.jbutton.button == Button8Rumble || e.jbutton.button == Button9Rumble || e.jbutton.button == Button10Rumble)
					{
						triggers->Rumble(percentForce, 0, length_ms);
					}
				}
			}

			if (e.jaxis.which == joystick_index2)
			{
				if (BothRumbleMotor && !LeftRumbleMotor && !RightRumbleMotor)
				{
					if (e.jbutton.button == Button1Device2Rumble || e.jbutton.button == Button2Device2Rumble || e.jbutton.button == Button3Device2Rumble || e.jbutton.button == Button4Device2Rumble || e.jbutton.button == Button5Device2Rumble || e.jbutton.button == Button6Device2Rumble || e.jbutton.button == Button7Device2Rumble || e.jbutton.button == Button8Device2Rumble || e.jbutton.button == Button9Device2Rumble || e.jbutton.button == Button10Device2Rumble)
					{
						triggers->RumbleDevice2(percentForce, percentForce, length_ms);
					}
				}

				if (!BothRumbleMotor && LeftRumbleMotor && !RightRumbleMotor)
				{
					if (e.jbutton.button == Button1Device2Rumble || e.jbutton.button == Button2Device2Rumble || e.jbutton.button == Button3Device2Rumble || e.jbutton.button == Button4Device2Rumble || e.jbutton.button == Button5Device2Rumble || e.jbutton.button == Button6Device2Rumble || e.jbutton.button == Button7Device2Rumble || e.jbutton.button == Button8Device2Rumble || e.jbutton.button == Button9Device2Rumble || e.jbutton.button == Button10Device2Rumble)
					{
						triggers->RumbleDevice2(0, percentForce, length_ms);
					}
				}

				if (!BothRumbleMotor && !LeftRumbleMotor && RightRumbleMotor)
				{
					if (e.jbutton.button == Button1Device2Rumble || e.jbutton.button == Button2Device2Rumble || e.jbutton.button == Button3Device2Rumble || e.jbutton.button == Button4Device2Rumble || e.jbutton.button == Button5Device2Rumble || e.jbutton.button == Button6Device2Rumble || e.jbutton.button == Button7Device2Rumble || e.jbutton.button == Button8Device2Rumble || e.jbutton.button == Button9Device2Rumble || e.jbutton.button == Button10Device2Rumble)
					{
						triggers->RumbleDevice2(percentForce, 0, length_ms);
					}
				}
			}
		}

		if (e.type == SDL_JOYBUTTONUP)
		{
			if (e.jaxis.which == joystick_index1)
			{
				if (e.jbutton.button == Button1Rumble || e.jbutton.button == Button2Rumble || e.jbutton.button == Button3Rumble || e.jbutton.button == Button4Rumble || e.jbutton.button == Button5Rumble || e.jbutton.button == Button6Rumble || e.jbutton.button == Button7Rumble || e.jbutton.button == Button8Rumble || e.jbutton.button == Button9Rumble || e.jbutton.button == Button10Rumble)
				{
					triggers->Rumble(0, 0, length_ms);
				}
			}

			if (e.jaxis.which == joystick_index2)
			{
				if (e.jbutton.button == Button1Device2Rumble || e.jbutton.button == Button2Device2Rumble || e.jbutton.button == Button3Device2Rumble || e.jbutton.button == Button4Device2Rumble || e.jbutton.button == Button5Device2Rumble || e.jbutton.button == Button6Device2Rumble || e.jbutton.button == Button7Device2Rumble || e.jbutton.button == Button8Device2Rumble || e.jbutton.button == Button9Device2Rumble || e.jbutton.button == Button10Device2Rumble)
				{
					triggers->RumbleDevice2(0, 0, length_ms);
				}
			}
		}
	}
}	