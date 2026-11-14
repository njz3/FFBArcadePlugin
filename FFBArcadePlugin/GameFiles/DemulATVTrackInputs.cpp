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

#include "DemulATVTrackInputs.h"
#include <string>
#include <windows.h>
#include "SDL.h"
#include "../Core/Devices.h"
#include "../Core/Memory.h"
#include "../Core/Settings.h"
#include "../Core/PluginState.h"


void DemulATVTrackInputs::Update()
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
	std::string view("ViewButton");
	std::string start("StartButton");
	std::string volup("VolumeUp");
	std::string voldown("VolumeDown");
	std::string emergency("EmergencyStop");
	std::string exit2("ExitButtonDevice2");
	std::string test2("TestButtonDevice2");
	std::string service2("ServiceButtonDevice2");
	std::string coin2("CoinButtonDevice2");
	std::string view2("ViewButtonDevice2");
	std::string start2("StartButtonDevice2");
	std::string volup2("VolumeUpDevice2");
	std::string voldown2("VolumeDownDevice2");
	std::string emergency2("EmergencyStopDevice2");
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

	UINT8 brakeread = ffb::Memory::Read<UINT8>(BrakeAddress);
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
						SteeringValue = (128 + (e.jaxis.value + WHEEL_DEAD_ZONE) / 255);
					}
					//Right of dead zone
					else if (e.jaxis.value > WHEEL_DEAD_ZONE)
					{
						e.jaxis.value = e.jaxis.value + WHEEL_DEAD_ZONE;
						SteeringValue = (127 + (e.jaxis.value - WHEEL_DEAD_ZONE) / 255);
					}
					else if ((e.jaxis.value < WHEEL_DEAD_ZONE) && (e.jaxis.value > -WHEEL_DEAD_ZONE))
					{
						SteeringValue = 0x7F;
					}

					ffb::Memory::Write<UINT8>(SteeringAddress, -SteeringValue - 1);
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
								BrakeValue = ((e.jaxis.value + ACCL_DEAD_ZONE) / 128);
							}
							else if ((e.jaxis.value < ACCL_DEAD_ZONE) && (e.jaxis.value > -ACCL_DEAD_ZONE))
							{
								ffb::Memory::Write<UINT8>(AcclAddress, 0x00);
								BrakeValue = 0x00;
							}

							if (BrakeValue > 0)
							{
								if (!brakepressed)
								{
									ffb::Memory::Write<UINT8>(BrakeAddress, brakeread -= 0x10);
									brakepressed = true;
								}
							}
							else
							{
								if (brakepressed)
								{
									ffb::Memory::Write<UINT8>(BrakeAddress, brakeread += 0x10);
									brakepressed = false;
								}
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
								BrakeValue = 127 - e.jaxis.value;
							}
							else if (e.jaxis.value > BRAKE_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								BrakeValue = 128 - e.jaxis.value;
							}

							if (BrakeValue > 0)
							{
								if (!brakepressed)
								{
									ffb::Memory::Write<UINT8>(BrakeAddress, brakeread -= 0x10);
									brakepressed = true;
								}
							}
							else
							{
								if (brakepressed)
								{
									ffb::Memory::Write<UINT8>(BrakeAddress, brakeread += 0x10);
									brakepressed = false;
								}
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
								BrakeValue = ((-e.jaxis.value + ACCL_DEAD_ZONE) / 128);
							}
							else if ((e.jaxis.value < ACCL_DEAD_ZONE) && (e.jaxis.value > -ACCL_DEAD_ZONE))
							{
								ffb::Memory::Write<UINT8>(AcclAddress, 0xFF);
								BrakeValue = 0x00;
							}

							if (BrakeValue > 0)
							{
								if (!brakepressed)
								{
									ffb::Memory::Write<UINT8>(BrakeAddress, brakeread -= 0x10);
									brakepressed = true;
								}
							}
							else
							{
								if (brakepressed)
								{
									ffb::Memory::Write<UINT8>(BrakeAddress, brakeread += 0x10);
									brakepressed = false;
								}
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
								BrakeValue = 128 + e.jaxis.value;
							}
							else if (e.jaxis.value > BRAKE_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								BrakeValue = 127 + e.jaxis.value;
							}

							if (BrakeValue > 0)
							{
								if (!brakepressed)
								{
									ffb::Memory::Write<UINT8>(BrakeAddress, brakeread -= 0x10);
									brakepressed = true;
								}
							}
							else
							{
								if (brakepressed)
								{
									ffb::Memory::Write<UINT8>(BrakeAddress, brakeread += 0x10);
									brakepressed = false;
								}
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
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x08);
						testbuttonA = false;
					}
					if (servicebuttonA)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x02);
						servicebuttonA = false;
					}
					if (startbuttonA)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread += 0x80);
						startbuttonA = false;
					}
					if (volumeupA)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x01);
						volumeupA = false;
					}
					if (volumedownA)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x04);
						volumedownA = false;
					}
					if (emergencystopA)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread -= 0x40);
						emergencystopA = false;
					}
					if (coinA)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x20);
						coinA = false;
					}
				}
				else if (e.jhat.which == joystick_index2)
				{
					if (testbuttonA)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x08);
						testbuttonA = false;
					}
					if (servicebuttonA)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x02);
						servicebuttonA = false;
					}
					if (startbuttonA)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread += 0x80);
						startbuttonA = false;
					}
					if (volumeupA)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x01);
						volumeupA = false;
					}
					if (volumedownA)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x04);
						volumedownA = false;
					}
					if (emergencystopA)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread -= 0x40);
						emergencystopA = false;
					}
					if (coinA)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x20);
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
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x08);
						testbuttonA = true;
					}
					if (dpdup.compare(service) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x02);
						servicebuttonA = true;
					}
					if (dpdup.compare(coin) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
						coinA = true;
					}
					if (dpdup.compare(emergency) == 0)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread += 0x40);
						emergencystopA = true;
					}
					if (dpdup.compare(start) == 0)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread -= 0x80);
						startbuttonA = true;
					}
					if (dpdup.compare(volup) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x01);
						volumeupA = true;
					}
					if (dpdup.compare(voldown) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x04);
						volumedownA = true;
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
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x08);
						testbuttonA = true;
					}
					if (dpdup2.compare(service2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x02);
						servicebuttonA = true;
					}
					if (dpdup2.compare(coin2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
						coinA = true;
					}
					if (dpdup2.compare(emergency2) == 0)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread += 0x40);
						emergencystopA = true;
					}
					if (dpdup2.compare(start2) == 0)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread -= 0x80);
						startbuttonA = true;
					}
					if (dpdup2.compare(volup2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x01);
						volumeupA = true;
					}
					if (dpdup2.compare(voldown2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x04);
						volumedownA = true;
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
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x08);
						testbuttonA = true;
					}
					if (dpddown.compare(service) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x02);
						servicebuttonA = true;
					}
					if (dpddown.compare(coin) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
						coinA = true;
					}
					if (dpddown.compare(emergency) == 0)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread += 0x40);
						emergencystopA = true;
					}
					if (dpddown.compare(start) == 0)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread -= 0x80);
						startbuttonA = true;
					}
					if (dpddown.compare(volup) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x01);
						volumeupA = true;
					}
					if (dpddown.compare(voldown) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x04);
						volumedownA = true;
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
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x08);
						testbuttonA = true;
					}
					if (dpddown2.compare(service2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x02);
						servicebuttonA = true;
					}
					if (dpddown2.compare(coin2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
						coinA = true;
					}
					if (dpddown2.compare(emergency2) == 0)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread += 0x40);
						emergencystopA = true;
					}
					if (dpddown2.compare(start2) == 0)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread -= 0x80);
						startbuttonA = true;
					}
					if (dpddown2.compare(volup2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x01);
						volumeupA = true;
					}
					if (dpddown2.compare(voldown2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x04);
						volumedownA = true;
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
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x08);
						testbuttonA = true;
					}
					if (dpdleft.compare(service) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x02);
						servicebuttonA = true;
					}
					if (dpdleft.compare(coin) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
						coinA = true;
					}
					if (dpdleft.compare(emergency) == 0)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread += 0x40);
						emergencystopA = true;
					}
					if (dpdleft.compare(start) == 0)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread -= 0x80);
						startbuttonA = true;
					}
					if (dpdleft.compare(volup) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x01);
						volumeupA = true;
					}
					if (dpdleft.compare(voldown) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x04);
						volumedownA = true;
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
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x08);
						testbuttonA = true;
					}
					if (dpdleft2.compare(service2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x02);
						servicebuttonA = true;
					}
					if (dpdleft2.compare(coin2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
						coinA = true;
					}
					if (dpdleft2.compare(emergency2) == 0)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread += 0x40);
						emergencystopA = true;
					}
					if (dpdleft2.compare(start2) == 0)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread -= 0x80);
						startbuttonA = true;
					}
					if (dpdleft2.compare(volup2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x01);
						volumeupA = true;
					}
					if (dpdleft2.compare(voldown2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x04);
						volumedownA = true;
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
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x08);
						testbuttonA = true;
					}
					if (dpdright.compare(service) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x02);
						servicebuttonA = true;
					}
					if (dpdright.compare(coin) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
						coinA = true;
					}
					if (dpdright.compare(emergency) == 0)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread += 0x40);
						emergencystopA = true;
					}
					if (dpdright.compare(start) == 0)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread -= 0x80);
						startbuttonA = true;
					}
					if (dpdright.compare(volup) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x01);
						volumeupA = true;
					}
					if (dpdright.compare(voldown) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x04);
						volumedownA = true;
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
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x08);
						testbuttonA = true;
					}
					if (dpdright2.compare(service2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x02);
						servicebuttonA = true;
					}
					if (dpdright2.compare(coin2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
						coinA = true;
					}
					if (dpdright2.compare(emergency2) == 0)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread += 0x40);
						emergencystopA = true;
					}
					if (dpdright2.compare(start2) == 0)
					{
						ffb::Memory::Write<UINT8>(BrakeAddress, brakeread -= 0x80);
						startbuttonA = true;
					}
					if (dpdright2.compare(volup2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x01);
						volumeupA = true;
					}
					if (dpdright2.compare(voldown2) == 0)
					{
						ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x04);
						volumedownA = true;
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
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x08);
				}
				if (e.jbutton.button == ExitButton)
				{
					system("taskkill /f /im demul.exe");
				}
				if (e.jbutton.button == ServiceButton)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x02);
				}
				if (e.jbutton.button == CreditButton)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
				}
				if (e.jbutton.button == EmergencyStop)
				{
					ffb::Memory::Write<UINT8>(BrakeAddress, brakeread += 0x40);
				}
				if (e.jbutton.button == StartButton)
				{
					ffb::Memory::Write<UINT8>(BrakeAddress, brakeread -= 0x80);
				}
				if (e.jbutton.button == VolumeUp)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x01);
				}
				if (e.jbutton.button == VolumeDown)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x04);
				}
			}
			else if (e.jbutton.which == joystick_index2)
			{
				if (e.jbutton.button == TestButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x08);
				}
				if (e.jbutton.button == ExitButtonDevice2)
				{
					system("taskkill /f /im demul.exe");
				}
				if (e.jbutton.button == ServiceButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x02);
				}
				if (e.jbutton.button == CreditButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x20);
				}
				if (e.jbutton.button == EmergencyStopDevice2)
				{
					ffb::Memory::Write<UINT8>(BrakeAddress, brakeread += 0x40);
				}
				if (e.jbutton.button == StartButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(BrakeAddress, brakeread -= 0x80);
				}
				if (e.jbutton.button == VolumeUpDevice2)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x01);
				}
				if (e.jbutton.button == VolumeDownDevice2)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread -= 0x04);
				}
			}
		}
		if (e.type == SDL_JOYBUTTONUP)
		{
			if (e.jbutton.which == joystick_index1)
			{
				if (e.jbutton.button == TestButton)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x08);
				}
				if (e.jbutton.button == ServiceButton)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x02);
				}
				if (e.jbutton.button == CreditButton)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x20);
				}
				if (e.jbutton.button == EmergencyStop)
				{
					ffb::Memory::Write<UINT8>(BrakeAddress, brakeread -= 0x40);
				}
				if (e.jbutton.button == StartButton)
				{
					ffb::Memory::Write<UINT8>(BrakeAddress, brakeread += 0x80);
				}
				if (e.jbutton.button == VolumeUp)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x01);
				}
				if (e.jbutton.button == VolumeDown)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x04);
				}
			}
			else if (e.jbutton.which == joystick_index2)
			{
				if (e.jbutton.button == TestButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x08);
				}
				if (e.jbutton.button == ServiceButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x02);
				}
				if (e.jbutton.button == CreditButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x20);
				}
				if (e.jbutton.button == EmergencyStopDevice2)
				{
					ffb::Memory::Write<UINT8>(BrakeAddress, brakeread -= 0x40);
				}
				if (e.jbutton.button == StartButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(BrakeAddress, brakeread += 0x80);
				}
				if (e.jbutton.button == VolumeUpDevice2)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x01);
				}
				if (e.jbutton.button == VolumeDownDevice2)
				{
					ffb::Memory::Write<UINT8>(ServiceTestAddress, servicetestread += 0x04);
				}
			}
		}
	}
}
