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

#include "DemulFasterThanSpeedInputs.h"
#include <string>
#include <windows.h>
#include "SDL.h"
#include "../Core/Devices.h"
#include "../Core/Memory.h"
#include "../Core/Settings.h"

extern int joystick_index1;
extern int joystick_index2;
extern SDL_Event e;

void DemulFasterThanSpeedInputs::Update()
{
	if (!Inputsinit)
	{
		OpenInputDevice2();
		Inputsinit = true;
	}

	// Dpad stuff here to set as any button
	char DpadUpChar[256];
	char DpadDownChar[256];
	char DpadLeftChar[256];
	char DpadRightChar[256];
	char DpadUpCharDevice2[256];
	char DpadDownCharDevice2[256];
	char DpadLeftCharDevice2[256];
	char DpadRightCharDevice2[256];
	strcpy_s(DpadUpChar, ffb::Settings::String("DpadUp").c_str());
	strcpy_s(DpadDownChar, ffb::Settings::String("DpadDown").c_str());
	strcpy_s(DpadLeftChar, ffb::Settings::String("DpadLeft").c_str());
	strcpy_s(DpadRightChar, ffb::Settings::String("DpadRight").c_str());
	strcpy_s(DpadUpCharDevice2, ffb::Settings::String("DpadUpDevice2").c_str());
	strcpy_s(DpadDownCharDevice2, ffb::Settings::String("DpadDownDevice2").c_str());
	strcpy_s(DpadLeftCharDevice2, ffb::Settings::String("DpadLeftDevice2").c_str());
	strcpy_s(DpadRightCharDevice2, ffb::Settings::String("DpadRightDevice2").c_str());
	std::string exit("ExitButton");
	std::string test("TestButton");
	std::string service("ServiceButton");
	std::string coin("CoinButton");
	std::string boost("BoostButton");
	std::string start("StartButton");
	std::string sftup("ShiftUp");
	std::string sftdown("ShiftDown");
	std::string exit2("ExitButtonDevice2");
	std::string test2("TestButtonDevice2");
	std::string service2("ServiceButtonDevice2");
	std::string coin2("CoinButtonDevice2");
	std::string boost2("BoostButtonDevice2");
	std::string start2("StartButtonDevice2");
	std::string sftup2("ShiftUpDevice2");
	std::string sftdown2("ShiftDownDevice2");
	std::string dpdup(DpadUpChar);
	std::string dpddown(DpadDownChar);
	std::string dpdleft(DpadLeftChar);
	std::string dpdright(DpadRightChar);
	std::string dpdup2(DpadUpCharDevice2);
	std::string dpddown2(DpadDownCharDevice2);
	std::string dpdleft2(DpadLeftCharDevice2);
	std::string dpdright2(DpadRightCharDevice2);

	const int WHEEL_DEAD_ZONE = (SteeringDeadzone * 100);
	const int ACCL_DEAD_ZONE = (1 + PedalDeadzone * 100);
	const int BRAKE_DEAD_ZONE = (1 + PedalDeadzone * 100);
	const int SETUP_DEAD_ZONE = 20000;

	UINT8 shiftupdownread = ffb::Memory::Read<UINT8>(ShiftUpDownAddress);
	UINT8 coinread = ffb::Memory::Read<UINT8>(CoinAddress);
	UINT8 servicetestread = ffb::Memory::Read<UINT8>(ServiceTestAddress);

	while (SDL_WaitEvent(&e) != 0)
	{
		if ((e.type == SDL_JOYAXISMOTION) && (ShowAxisForSetup == 0))
		{
			if (e.jaxis.which == joystick_index1)
			{
				if (e.jaxis.axis == InputDeviceWheelSteeringAxis)
				{
					//Left of dead zone
					if (e.jaxis.value < -WHEEL_DEAD_ZONE)
					{
						e.jaxis.value = e.jaxis.value - WHEEL_DEAD_ZONE;
						ffb::Memory::Write<UINT8>(SteeringAddress, (128 + (e.jaxis.value + WHEEL_DEAD_ZONE) / 255));
					}
					//Right of dead zone
					else if (e.jaxis.value > WHEEL_DEAD_ZONE)
					{
						e.jaxis.value = e.jaxis.value + WHEEL_DEAD_ZONE;
						ffb::Memory::Write<UINT8>(SteeringAddress, (127 + (e.jaxis.value - WHEEL_DEAD_ZONE) / 255));
					}
					else if ((e.jaxis.value < WHEEL_DEAD_ZONE) && (e.jaxis.value > -WHEEL_DEAD_ZONE))
					{
						ffb::Memory::Write<UINT8>(SteeringAddress, 0x7F);
					}
				}
				if (InputDeviceWheelReverseAxis == 1)
				{
					if (InputDeviceCombinedPedals == 1)
					{
						if (e.jaxis.axis == InputDeviceWheelAcclAxis)
						{
							if (e.jaxis.value < -ACCL_DEAD_ZONE)
							{
								e.jaxis.value = e.jaxis.value + ACCL_DEAD_ZONE;
								ffb::Memory::Write<UINT8>(AcclAddress, (UINT8)((-e.jaxis.value + ACCL_DEAD_ZONE) / 128.5));
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = e.jaxis.value - ACCL_DEAD_ZONE;
								ffb::Memory::Write<UINT8>(BrakeAddress, ((e.jaxis.value + ACCL_DEAD_ZONE) / 128));
							}
							else if ((e.jaxis.value < ACCL_DEAD_ZONE) && (e.jaxis.value > -ACCL_DEAD_ZONE))
							{
								ffb::Memory::Write<UINT8>(AcclAddress, 0x00);
								ffb::Memory::Write<UINT8>(BrakeAddress, 0x00);
							}
						}
					}
					else
					{
						if (e.jaxis.axis == InputDeviceWheelAcclAxis)
						{
							if (e.jaxis.value < -ACCL_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								ffb::Memory::Write<UINT8>(AcclAddress, 127 - e.jaxis.value);
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								ffb::Memory::Write<UINT8>(AcclAddress, 128 - e.jaxis.value);
							}
						}
						else if (e.jaxis.axis == InputDeviceWheelBrakeAxis)
						{
							if (e.jaxis.value < -BRAKE_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								ffb::Memory::Write<UINT8>(BrakeAddress, 127 - e.jaxis.value);
							}
							else if (e.jaxis.value > BRAKE_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								ffb::Memory::Write<UINT8>(BrakeAddress, 128 - e.jaxis.value);
							}
						}
					}
				}
				else
				{
					if (InputDeviceCombinedPedals == 1)
					{
						if (e.jaxis.axis == InputDeviceWheelAcclAxis)
						{
							if (e.jaxis.value < -ACCL_DEAD_ZONE)
							{
								e.jaxis.value = e.jaxis.value - ACCL_DEAD_ZONE;
								ffb::Memory::Write<UINT8>(AcclAddress, ((e.jaxis.value + ACCL_DEAD_ZONE) / 128));
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = e.jaxis.value + ACCL_DEAD_ZONE;
								ffb::Memory::Write<UINT8>(BrakeAddress, ((-e.jaxis.value + ACCL_DEAD_ZONE) / 128));
							}
							else if ((e.jaxis.value < ACCL_DEAD_ZONE) && (e.jaxis.value > -ACCL_DEAD_ZONE))
							{
								ffb::Memory::Write<UINT8>(AcclAddress, 0xFF);
								ffb::Memory::Write<UINT8>(BrakeAddress, 0xFF);
							}
						}
					}
					else
					{
						if (e.jaxis.axis == InputDeviceWheelAcclAxis)
						{
							if (e.jaxis.value < -ACCL_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								ffb::Memory::Write<UINT8>(AcclAddress, 128 + e.jaxis.value);
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								ffb::Memory::Write<UINT8>(AcclAddress, 127 + e.jaxis.value);
							}
						}
						if (e.jaxis.axis == InputDeviceWheelBrakeAxis)
						{
							if (e.jaxis.value < -BRAKE_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								ffb::Memory::Write<UINT8>(BrakeAddress, 128 + e.jaxis.value);
							}
							else if (e.jaxis.value > BRAKE_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								ffb::Memory::Write<UINT8>(BrakeAddress, 127 + e.jaxis.value);
							}
						}
					}
				}
			}
		}

		if (ShowAxisForSetup == 1)
		{
			if (e.type == SDL_JOYAXISMOTION)
			{
				if (e.jaxis.which == joystick_index1)
				{
					if (e.jaxis.axis >= 0)
					{
						if (e.jaxis.value < -SETUP_DEAD_ZONE)
						{
							e.jaxis.value = e.jaxis.value - SETUP_DEAD_ZONE;
							char buff[100];
							sprintf_s(buff, "Axis %d Moved", e.jaxis.axis);
							MessageBoxA(NULL, buff, "", NULL);
						}
						else if (e.jaxis.value > SETUP_DEAD_ZONE)
						{
							char buff[100];
							sprintf_s(buff, "Axis %d Moved", e.jaxis.axis);
							MessageBoxA(NULL, buff, "", NULL);
						}
					}
				}
			}
		}

		if (ShowButtonNumbersForSetup == 1)
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

		if (e.jhat.type == SDL_JOYHATMOTION)
		{
			if (e.jhat.value == SDL_HAT_CENTERED)
			{
				if (e.jhat.which == joystick_index1)
				{
					if (testbuttonA)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x40);
						testbuttonA = false;
					}
					if (servicebuttonA)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x20);
						servicebuttonA = false;
					}
					if (boostbuttonA)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread += 0x04);
						boostbuttonA = false;
					}
					if (startbuttonA)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread += 0x08);
						startbuttonA = false;
					}
					if (shiftupA)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread += 0x10);
						shiftupA = false;
					}
					if (shiftdownA)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread += 0x20);
						shiftdownA = false;
					}
					if (coinA)
					{
						ffb::Memory::Write<UINT8>(CoinAddress, coinread += 0x01);
						coinA = false;
					}
				}
				else if (e.jhat.which == joystick_index2)
				{
					if (testbuttonA)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x40);
						testbuttonA = false;
					}
					if (servicebuttonA)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x20);
						servicebuttonA = false;
					}
					if (boostbuttonA)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread += 0x04);
						boostbuttonA = false;
					}
					if (startbuttonA)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread += 0x08);
						startbuttonA = false;
					}
					if (shiftupA)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread += 0x10);
						shiftupA = false;
					}
					if (shiftdownA)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread += 0x20);
						shiftdownA = false;
					}
					if (coinA)
					{
						ffb::Memory::Write<UINT8>(CoinAddress, coinread += 0x01);
						coinA = false;
					}
				}
			}
			if (e.jhat.value == SDL_HAT_UP)
			{
				if (e.jhat.which == joystick_index1)
				{
					if (dpdup.compare(exit) == 0)
					{
						system("taskkill /f /im demul.exe");
					}
					if (dpdup.compare(test) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x40);
						testbuttonA = true;
					}
					if (dpdup.compare(service) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
						servicebuttonA = true;
					}
					if (dpdup.compare(coin) == 0)
					{
						ffb::Memory::Write<UINT8>(CoinAddress, coinread -= 0x01);
						coinA = true;
					}
					if (dpdup.compare(boost) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x04);
						boostbuttonA = true;
					}
					if (dpdup.compare(start) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x08);
						startbuttonA = true;
					}
					if (dpdup.compare(sftup) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x10);
						shiftupA = true;
					}
					if (dpdup.compare(sftdown) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x20);
						shiftdownA = true;
					}
				}
				else if (e.jhat.which == joystick_index2)
				{
					if (dpdup2.compare(exit2) == 0)
					{
						system("taskkill /f /im demul.exe");
					}
					if (dpdup2.compare(test2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x40);
						testbuttonA = true;
					}
					if (dpdup2.compare(service2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
						servicebuttonA = true;
					}
					if (dpdup2.compare(coin2) == 0)
					{
						ffb::Memory::Write<UINT8>(CoinAddress, coinread -= 0x01);
						coinA = true;
					}
					if (dpdup2.compare(boost2) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x04);
						boostbuttonA = true;
					}
					if (dpdup2.compare(start2) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x08);
						startbuttonA = true;
					}
					if (dpdup2.compare(sftup2) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x10);
						shiftupA = true;
					}
					if (dpdup2.compare(sftdown2) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x20);
						shiftdownA = true;
					}
				}
			}
			if (e.jhat.value == SDL_HAT_DOWN)
			{
				if (e.jhat.which == joystick_index1)
				{
					if (dpddown.compare(exit) == 0)
					{
						system("taskkill /f /im demul.exe");
					}
					if (dpddown.compare(test) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x40);
						testbuttonA = true;
					}
					if (dpddown.compare(service) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
						servicebuttonA = true;
					}
					if (dpddown.compare(coin) == 0)
					{
						ffb::Memory::Write<UINT8>(CoinAddress, coinread -= 0x01);
						coinA = true;
					}
					if (dpddown.compare(boost) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x04);
						boostbuttonA = true;
					}
					if (dpddown.compare(start) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x08);
						startbuttonA = true;
					}
					if (dpddown.compare(sftup) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x10);
						shiftupA = true;
					}
					if (dpddown.compare(sftdown) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x20);
						shiftdownA = true;
					}
				}
				else if (e.jhat.which == joystick_index2)
				{
					if (dpddown2.compare(exit2) == 0)
					{
						system("taskkill /f /im demul.exe");
					}
					if (dpddown2.compare(test2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x40);
						testbuttonA = true;
					}
					if (dpddown2.compare(service2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
						servicebuttonA = true;
					}
					if (dpddown2.compare(coin2) == 0)
					{
						ffb::Memory::Write<UINT8>(CoinAddress, coinread -= 0x01);
						coinA = true;
					}
					if (dpddown2.compare(boost2) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x04);
						boostbuttonA = true;
					}
					if (dpddown2.compare(start2) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x08);
						startbuttonA = true;
					}
					if (dpddown2.compare(sftup2) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x10);
						shiftupA = true;
					}
					if (dpddown2.compare(sftdown2) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x20);
						shiftdownA = true;
					}
				}
			}
			if (e.jhat.value == SDL_HAT_LEFT)
			{
				if (e.jhat.which == joystick_index1)
				{
					if (dpdleft.compare(exit) == 0)
					{
						system("taskkill /f /im demul.exe");
					}
					if (dpdleft.compare(test) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x40);
						testbuttonA = true;
					}
					if (dpdleft.compare(service) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
						servicebuttonA = true;
					}
					if (dpdleft.compare(coin) == 0)
					{
						ffb::Memory::Write<UINT8>(CoinAddress, coinread -= 0x01);
						coinA = true;
					}
					if (dpdleft.compare(boost) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x04);
						boostbuttonA = true;
					}
					if (dpdleft.compare(start) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x80);
						startbuttonA = true;
					}
					if (dpdleft.compare(sftup) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x10);
						shiftupA = true;
					}
					if (dpdleft.compare(sftdown) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x20);
						shiftdownA = true;
					}
				}
				else if (e.jhat.which == joystick_index2)
				{
					if (dpdleft2.compare(exit2) == 0)
					{
						system("taskkill /f /im demul.exe");
					}
					if (dpdleft2.compare(test2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x40);
						testbuttonA = true;
					}
					if (dpdleft2.compare(service2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
						servicebuttonA = true;
					}
					if (dpdleft2.compare(coin2) == 0)
					{
						ffb::Memory::Write<UINT8>(CoinAddress, coinread -= 0x01);
						coinA = true;
					}
					if (dpdleft2.compare(boost2) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x04);
						boostbuttonA = true;
					}
					if (dpdleft2.compare(start2) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x80);
						startbuttonA = true;
					}
					if (dpdleft2.compare(sftup2) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x10);
						shiftupA = true;
					}
					if (dpdleft2.compare(sftdown2) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x20);
						shiftdownA = true;
					}
				}
			}
			if (e.jhat.value == SDL_HAT_RIGHT)
			{
				if (e.jhat.which == joystick_index1)
				{
					if (dpdright.compare(exit) == 0)
					{
						system("taskkill /f /im demul.exe");
					}
					if (dpdright.compare(test) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x40);
						testbuttonA = true;
					}
					if (dpdright.compare(service) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
						servicebuttonA = true;
					}
					if (dpdright.compare(coin) == 0)
					{
						ffb::Memory::Write<UINT8>(CoinAddress, coinread -= 0x01);
						coinA = true;
					}
					if (dpdright.compare(boost) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x04);
						boostbuttonA = true;
					}
					if (dpdright.compare(start) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x80);
						startbuttonA = true;
					}
					if (dpdright.compare(sftup) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x10);
						shiftupA = true;
					}
					if (dpdright.compare(sftdown) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x20);
						shiftdownA = true;
					}
				}
				else if (e.jhat.which == joystick_index2)
				{
					if (dpdright2.compare(exit2) == 0)
					{
						system("taskkill /f /im demul.exe");
					}
					if (dpdright2.compare(test2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x40);
						testbuttonA = true;
					}
					if (dpdright2.compare(service2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
						servicebuttonA = true;
					}
					if (dpdright2.compare(coin2) == 0)
					{
						ffb::Memory::Write<UINT8>(CoinAddress, coinread -= 0x01);
						coinA = true;
					}
					if (dpdright2.compare(boost2) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x04);
						boostbuttonA = true;
					}
					if (dpdright2.compare(start2) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x80);
						startbuttonA = true;
					}
					if (dpdright2.compare(sftup2) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x10);
						shiftupA = true;
					}
					if (dpdright2.compare(sftdown2) == 0)
					{
						ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x20);
						shiftdownA = true;
					}
				}
			}
		}

		if (e.type == SDL_JOYBUTTONDOWN)
		{
			if (e.jbutton.which == joystick_index1)
			{
				if (e.jbutton.button == TestButton)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x40);
				}
				if (e.jbutton.button == ExitButton)
				{
					system("taskkill /f /im demul.exe");
				}
				if (e.jbutton.button == ServiceButton)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
				}
				if (e.jbutton.button == CreditButton)
				{
					ffb::Memory::Write<UINT8>(CoinAddress, coinread -= 0x01);
				}
				if (e.jbutton.button == BoostButton)
				{
					ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x04);
				}
				if (e.jbutton.button == StartButton)
				{
					ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x08);
				}
				if (e.jbutton.button == ShiftUp)
				{
					ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x10);
				}
				if (e.jbutton.button == ShiftDown)
				{
					ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x20);
				}
			}
			else if (e.jbutton.which == joystick_index2)
			{
				if (e.jbutton.button == TestButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x40);
				}
				if (e.jbutton.button == ExitButtonDevice2)
				{
					system("taskkill /f /im demul.exe");
				}
				if (e.jbutton.button == ServiceButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
				}
				if (e.jbutton.button == CreditButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(CoinAddress, coinread -= 0x01);
				}
				if (e.jbutton.button == BoostButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x04);
				}
				if (e.jbutton.button == StartButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x08);
				}
				if (e.jbutton.button == ShiftUpDevice2)
				{
					ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x10);
				}
				if (e.jbutton.button == ShiftDownDevice2)
				{
					ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread -= 0x20);
				}
			}
		}
		if (e.type == SDL_JOYBUTTONUP)
		{
			if (e.jbutton.which == joystick_index1)
			{
				if (e.jbutton.button == TestButton)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x40);
				}
				if (e.jbutton.button == ServiceButton)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x20);
				}
				if (e.jbutton.button == CreditButton)
				{
					ffb::Memory::Write<UINT8>(CoinAddress, coinread += 0x01);
				}
				if (e.jbutton.button == BoostButton)
				{
					ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread += 0x04);
				}
				if (e.jbutton.button == StartButton)
				{
					ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread += 0x08);
				}
				if (e.jbutton.button == ShiftUp)
				{
					ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread += 0x10);
				}
				if (e.jbutton.button == ShiftDown)
				{
					ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread += 0x20);
				}
			}
			else if (e.jbutton.which == joystick_index2)
			{
				if (e.jbutton.button == TestButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x40);
				}
				if (e.jbutton.button == ServiceButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x20);
				}
				if (e.jbutton.button == CreditButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(CoinAddress, coinread += 0x01);
				}
				if (e.jbutton.button == BoostButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread += 0x04);
				}
				if (e.jbutton.button == StartButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread += 0x08);
				}
				if (e.jbutton.button == ShiftUpDevice2)
				{
					ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread += 0x10);
				}
				if (e.jbutton.button == ShiftDownDevice2)
				{
					ffb::Memory::Write<UINT8>(ShiftUpDownAddress, shiftupdownread += 0x20);
				}
			}
		}
	}
}
