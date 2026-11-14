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

#include "M2EmulatorIndy500Inputs.h"
#include <string>
#include <windows.h>
#include "SDL.h"
#include "../Core/Devices.h"
#include "../Core/Memory.h"
#include "../Core/Settings.h"

extern int joystick_index1;
extern int joystick_index2;
extern SDL_Event e;

void M2EmulatorIndy500Inputs::Update()
{
	if (!Inputsinit)
	{
		OpenInputDevice2();

		imageBase = (INT_PTR)GetModuleHandleA(0);
		SteeringAddress = imageBase + 0x1AA70C;
		AcclAddress = imageBase + 0x1AA70D;
		BrakeAddress = imageBase + 0x1AA70E;
		Button1Address = imageBase + 0x1AA875;
		Button2Address = imageBase + 0x1AA874;

		ffb::Memory::Write<UINT8>(Button1Address, 0xFF);
		ffb::Memory::Write<UINT8>(Button2Address, 0xFF);
		ffb::Memory::Write<UINT8>(SteeringAddress, 0x80);
		ffb::Memory::Write<UINT8>(AcclAddress, 0x00);
		ffb::Memory::Write<UINT8>(BrakeAddress, 0x00);
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
	char KeyBrdExitChar[256];
	char KeyBrdTestChar[256];
	char KeyBrdServiceChar[256];
	char KeyBrdCoin1Char[256];
	char KeyBrdCoin2Char[256];
	char KeyBrdVR1Char[256];
	char KeyBrdVR2Char[256];
	char KeyBrdVR3Char[256];
	char KeyBrdVR4Char[256];
	char KeyBrdStartChar[256];
	char KeyBrdShift1Char[256];
	char KeyBrdShift2Char[256];
	char KeyBrdShift3Char[256];
	char KeyBrdShift4Char[256];
	char KeyBrdShiftUpChar[256];
	char KeyBrdShiftDownChar[256];
	strcpy_s(DpadUpChar, ffb::Settings::String("DpadUp").c_str());
	strcpy_s(DpadDownChar, ffb::Settings::String("DpadDown").c_str());
	strcpy_s(DpadLeftChar, ffb::Settings::String("DpadLeft").c_str());
	strcpy_s(DpadRightChar, ffb::Settings::String("DpadRight").c_str());
	strcpy_s(DpadUpCharDevice2, ffb::Settings::String("DpadUpDevice2").c_str());
	strcpy_s(DpadDownCharDevice2, ffb::Settings::String("DpadDownDevice2").c_str());
	strcpy_s(DpadLeftCharDevice2, ffb::Settings::String("DpadLeftDevice2").c_str());
	strcpy_s(DpadRightCharDevice2, ffb::Settings::String("DpadRightDevice2").c_str());
	strcpy_s(KeyBrdExitChar, ffb::Settings::String("ExitButtonKeyBrd").c_str());
	strcpy_s(KeyBrdTestChar, ffb::Settings::String("TestButtonKeyBrd").c_str());
	strcpy_s(KeyBrdServiceChar, ffb::Settings::String("ServiceButtonKeyBrd").c_str());
	strcpy_s(KeyBrdCoin1Char, ffb::Settings::String("Coin1ButtonKeyBrd").c_str());
	strcpy_s(KeyBrdCoin2Char, ffb::Settings::String("Coin2ButtonKeyBrd").c_str());
	strcpy_s(KeyBrdVR1Char, ffb::Settings::String("VR1ButtonKeyBrd").c_str());
	strcpy_s(KeyBrdVR2Char, ffb::Settings::String("VR2ButtonKeyBrd").c_str());
	strcpy_s(KeyBrdVR3Char, ffb::Settings::String("VR3ButtonKeyBrd").c_str());
	strcpy_s(KeyBrdVR4Char, ffb::Settings::String("VR4ButtonKeyBrd").c_str());
	strcpy_s(KeyBrdStartChar, ffb::Settings::String("StartButtonKeyBrd").c_str());
	strcpy_s(KeyBrdShift1Char, ffb::Settings::String("Shift1ButtonKeyBrd").c_str());
	strcpy_s(KeyBrdShift2Char, ffb::Settings::String("Shift2ButtonKeyBrd").c_str());
	strcpy_s(KeyBrdShift3Char, ffb::Settings::String("Shift3ButtonKeyBrd").c_str());
	strcpy_s(KeyBrdShift4Char, ffb::Settings::String("Shift4ButtonKeyBrd").c_str());
	strcpy_s(KeyBrdShiftUpChar, ffb::Settings::String("ShiftUpButtonKeyBrd").c_str());
	strcpy_s(KeyBrdShiftDownChar, ffb::Settings::String("ShiftDownButtonKeyBrd").c_str());
	std::string exit("ExitButton");
	std::string test("TestButton");
	std::string service("ServiceButton");
	std::string coin1("Coin1Button");
	std::string coin2("Coin2Button");
	std::string vr1("VR1Button");
	std::string vr2("VR2Button");
	std::string start("StartButton");
	std::string shiftup("ShiftUpButton");
	std::string shiftdwn("ShiftDownButton");
	std::string exit2("ExitButtonDevice2");
	std::string test2("TestButtonDevice2");
	std::string service2("ServiceButtonDevice2");
	std::string coin12("Coin1ButtonDevice2");
	std::string coin22("Coin2ButtonDevice2");
	std::string vr12("VR1ButtonDevice2");
	std::string vr22("VR2ButtonDevice2");
	std::string start2("StartButtonDevice2");
	std::string shiftup2("ShiftUpButtonDevice2");
	std::string shiftdwn2("ShiftDownButtonDevice2");
	std::string dpdup(DpadUpChar);
	std::string dpddown(DpadDownChar);
	std::string dpdleft(DpadLeftChar);
	std::string dpdright(DpadRightChar);
	std::string dpdup2(DpadUpCharDevice2);
	std::string dpddown2(DpadDownCharDevice2);
	std::string dpdleft2(DpadLeftCharDevice2);
	std::string dpdright2(DpadRightCharDevice2);
	std::string KeyBrdExit(KeyBrdExitChar);
	std::string KeyBrdTest(KeyBrdTestChar);
	std::string KeyBrdService(KeyBrdServiceChar);
	std::string KeyBrdCoin1(KeyBrdCoin1Char);
	std::string KeyBrdCoin2(KeyBrdCoin2Char);
	std::string KeyBrdVR1(KeyBrdVR1Char);
	std::string KeyBrdVR2(KeyBrdVR2Char);
	std::string KeyBrdVR3(KeyBrdVR3Char);
	std::string KeyBrdVR4(KeyBrdVR4Char);
	std::string KeyBrdStart(KeyBrdStartChar);
	std::string KeyBrdShift1(KeyBrdShift1Char);
	std::string KeyBrdShift2(KeyBrdShift2Char);
	std::string KeyBrdShift3(KeyBrdShift3Char);
	std::string KeyBrdShift4(KeyBrdShift4Char);
	std::string KeyBrdShiftUp(KeyBrdShiftUpChar);
	std::string KeyBrdShiftDown(KeyBrdShiftDownChar);

	const int WHEEL_DEAD_ZONE = (SteeringDeadzone * 100);
	const int ACCL_DEAD_ZONE = (1 + PedalDeadzone * 100);
	const int BRAKE_DEAD_ZONE = (1 + PedalDeadzone * 100);
	const int SETUP_DEAD_ZONE = 20000;

	SDL_Window* w = SDL_CreateWindowFrom(hWndM2);
	SDL_MinimizeWindow(w);
	SDL_RestoreWindow(w);

	while (SDL_WaitEvent(&e) != 0)
	{
		UINT8 button1read = ffb::Memory::Read<UINT8>(Button1Address);
		UINT8 button2read = ffb::Memory::Read<UINT8>(Button2Address);

		if ((e.type == SDL_JOYAXISMOTION) && (ShowAxisForSetup == 0))
		{
			if (e.jaxis.which == joystick_index1)
			{
				if (e.jaxis.axis == InputDeviceWheelSteeringAxis)
				{
					if (e.jaxis.value < -WHEEL_DEAD_ZONE)
					{
						e.jaxis.value = e.jaxis.value - WHEEL_DEAD_ZONE;
						SteeringValue = 128 + (e.jaxis.value + WHEEL_DEAD_ZONE) / 255;
					}
					else if (e.jaxis.value > WHEEL_DEAD_ZONE)
					{
						e.jaxis.value = e.jaxis.value + WHEEL_DEAD_ZONE;
						SteeringValue = 127 + (e.jaxis.value - WHEEL_DEAD_ZONE) / 255;
					}
					else
					{
						SteeringValue = 0x80;
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
								AcclValue = (INT8)((-e.jaxis.value + ACCL_DEAD_ZONE) / 128.5);
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = e.jaxis.value - ACCL_DEAD_ZONE;
								BrakeValue = (INT8)((e.jaxis.value + ACCL_DEAD_ZONE) / 128);
							}
							else
							{
								AcclValue = 0x00;
								BrakeValue = 0x00;
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
								AcclValue = 127 - e.jaxis.value;
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								AcclValue = 128 - e.jaxis.value;
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
								AcclValue = (e.jaxis.value + ACCL_DEAD_ZONE) / 128;
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = e.jaxis.value + ACCL_DEAD_ZONE;
								BrakeValue = (-e.jaxis.value + ACCL_DEAD_ZONE) / 128;
							}
							else if ((e.jaxis.value < ACCL_DEAD_ZONE) && (e.jaxis.value > -ACCL_DEAD_ZONE))
							{
								AcclValue = 0xFF;
								BrakeValue = 0xFF;
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
								AcclValue = 128 + e.jaxis.value;
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								AcclValue = 127 + e.jaxis.value;
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
						}
					}
				}
				ffb::Memory::Write<UINT8>(SteeringAddress, SteeringValue);
				ffb::Memory::Write<UINT8>(AcclAddress, AcclValue);
				ffb::Memory::Write<UINT8>(BrakeAddress, BrakeValue);
			}
			if (e.jaxis.which == joystick_index2)
			{
				if (e.jaxis.axis == InputDeviceWheelSteeringAxisDevice2)
				{
					if (e.jaxis.value < -WHEEL_DEAD_ZONE)
					{
						e.jaxis.value = e.jaxis.value - WHEEL_DEAD_ZONE;
						SteeringValue = 128 + (e.jaxis.value + WHEEL_DEAD_ZONE) / 255;
					}
					else if (e.jaxis.value > WHEEL_DEAD_ZONE)
					{
						e.jaxis.value = e.jaxis.value + WHEEL_DEAD_ZONE;
						SteeringValue = 127 + (e.jaxis.value - WHEEL_DEAD_ZONE) / 255;
					}
					else
					{
						SteeringValue = 0x80;
					}
				}
				if (InputDeviceWheelReverseAxis == 1)
				{
					if (InputDeviceCombinedPedals == 1)
					{
						if (e.jaxis.axis == InputDeviceWheelAcclAxisDevice2)
						{
							if (e.jaxis.value < -ACCL_DEAD_ZONE)
							{
								e.jaxis.value = e.jaxis.value + ACCL_DEAD_ZONE;
								AcclValue = (INT8)((-e.jaxis.value + ACCL_DEAD_ZONE) / 128.5);
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = e.jaxis.value - ACCL_DEAD_ZONE;
								BrakeValue = (INT8)((e.jaxis.value + ACCL_DEAD_ZONE) / 128.0);
							}
							else
							{
								AcclValue = 0x00;
								BrakeValue = 0x00;
							}
						}
					}
					else
					{
						if (e.jaxis.axis == InputDeviceWheelAcclAxisDevice2)
						{
							if (e.jaxis.value < -ACCL_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								AcclValue = 127 - e.jaxis.value;
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								AcclValue = 128 - e.jaxis.value;
							}
						}
						else if (e.jaxis.axis == InputDeviceWheelBrakeAxisDevice2)
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
						}
					}
				}
				else
				{
					if (InputDeviceCombinedPedals == 1)
					{
						if (e.jaxis.axis == InputDeviceWheelAcclAxisDevice2)
						{
							if (e.jaxis.value < -ACCL_DEAD_ZONE)
							{
								e.jaxis.value = e.jaxis.value - ACCL_DEAD_ZONE;
								AcclValue = (e.jaxis.value + ACCL_DEAD_ZONE) / 128;
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = e.jaxis.value + ACCL_DEAD_ZONE;
								BrakeValue = (-e.jaxis.value + ACCL_DEAD_ZONE) / 128;
							}
							else if ((e.jaxis.value < ACCL_DEAD_ZONE) && (e.jaxis.value > -ACCL_DEAD_ZONE))
							{
								AcclValue = 0xFF;
								BrakeValue = 0xFF;
							}
						}
					}
					else
					{
						if (e.jaxis.axis == InputDeviceWheelAcclAxisDevice2)
						{
							if (e.jaxis.value < -ACCL_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								AcclValue = 128 + e.jaxis.value;
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								AcclValue = 127 + e.jaxis.value;
							}
						}
						if (e.jaxis.axis == InputDeviceWheelBrakeAxisDevice2)
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
						}
					}
				}
				ffb::Memory::Write<UINT8>(SteeringAddress, SteeringValue);
				ffb::Memory::Write<UINT8>(AcclAddress, AcclValue);
				ffb::Memory::Write<UINT8>(BrakeAddress, BrakeValue);
			}
		}

		if (ShowAxisForSetup)
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

		if (ShowKeyBoardNameForSetup)
		{
			if (e.type == SDL_KEYDOWN)
			{
				const char* KeyPressed = SDL_GetKeyName(e.key.keysym.sym);

				char buff[100];
				sprintf_s(buff, "Key %s Pressed", KeyPressed);
				MessageBoxA(NULL, buff, "", NULL);
			}
		}

		if (e.jhat.type == SDL_JOYHATMOTION)
		{
			if (e.jhat.value == SDL_HAT_CENTERED)
			{
				if (e.jhat.which == joystick_index1 || e.jhat.which == joystick_index2)
				{
					if (testA)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x04);
						testA = false;
					}
					if (serviceA)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x08);
						serviceA = false;
					}
					if (startA)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x40);
						startA = false;
					}
					if (vr1A)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read += 0x01);
						vr1A = false;
					}
					if (vr2A)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read += 0x02);
						vr2A = false;
					}
					if (coin1A)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x01);
						coin1A = false;
					}
					if (coin2A)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x02);
						coin2A = false;
					}
					if (shiftupA)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read += 0x10);
						shiftupA = false;
					}
					if (shiftdwnA)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read += 0x20);
						shiftdwnA = false;
					}
				}
			}
			if (e.jhat.value == SDL_HAT_UP)
			{
				if (e.jhat.which == joystick_index1)
				{
					if (dpdup.compare(exit) == 0)
					{
						SendMessage(hWndM2, WM_CLOSE, NULL, NULL);
					}
					if (dpdup.compare(test) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x04);
						testA = true;
					}
					if (dpdup.compare(service) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x08);
						serviceA = true;
					}
					if (dpdup.compare(coin1) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x01);
						coin1A = true;
					}
					if (dpdup.compare(coin2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x02);
						coin2A = true;
					}
					if (dpdup.compare(start) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x40);
						startA = true;
					}
					if (dpdup.compare(vr1) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x01);
						vr1A = true;
					}
					if (dpdup.compare(vr2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x02);
						vr2A = true;
					}
					if (dpdup.compare(shiftup) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x10);
						shiftupA = true;
					}
					if (dpdup.compare(shiftdwn) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x20);
						shiftdwnA = true;
					}
				}
				else if (e.jhat.which == joystick_index2)
				{
					if (dpdup2.compare(exit2) == 0)
					{
						SendMessage(hWndM2, WM_CLOSE, NULL, NULL);
					}
					if (dpdup2.compare(test2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x04);
						testA = true;
					}
					if (dpdup2.compare(service2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x08);
						serviceA = true;
					}
					if (dpdup2.compare(coin12) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x01);
						coin1A = true;
					}
					if (dpdup2.compare(coin22) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x02);
						coin2A = true;
					}
					if (dpdup2.compare(start2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x40);
						startA = true;
					}
					if (dpdup2.compare(vr12) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x01);
						vr1A = true;
					}
					if (dpdup2.compare(vr22) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x02);
						vr2A = true;
					}
					if (dpdup2.compare(shiftup2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x10);
						shiftupA = true;
					}
					if (dpdup2.compare(shiftdwn2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x20);
						shiftdwnA = true;
					}
				}
			}
			if (e.jhat.value == SDL_HAT_DOWN)
			{
				if (e.jhat.which == joystick_index1)
				{
					if (dpddown.compare(exit) == 0)
					{
						SendMessage(hWndM2, WM_CLOSE, NULL, NULL);
					}
					if (dpddown.compare(test) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x04);
						testA = true;
					}
					if (dpddown.compare(service) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x08);
						serviceA = true;
					}
					if (dpddown.compare(coin1) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x01);
						coin1A = true;
					}
					if (dpddown.compare(coin2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x02);
						coin2A = true;
					}
					if (dpddown.compare(start) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x40);
						startA = true;
					}
					if (dpddown.compare(vr1) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x01);
						vr1A = true;
					}
					if (dpddown.compare(vr2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x02);
						vr2A = true;
					}
					if (dpddown.compare(shiftup) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x10);
						shiftupA = true;
					}
					if (dpddown.compare(shiftdwn) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x20);
						shiftdwnA = true;
					}
				}
				else if (e.jhat.which == joystick_index2)
				{
					if (dpddown2.compare(exit2) == 0)
					{
						SendMessage(hWndM2, WM_CLOSE, NULL, NULL);
					}
					if (dpddown2.compare(test2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x04);
						testA = true;
					}
					if (dpddown2.compare(service2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x08);
						serviceA = true;
					}
					if (dpddown2.compare(coin12) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x01);
						coin1A = true;
					}
					if (dpddown2.compare(coin22) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x02);
						coin2A = true;
					}
					if (dpddown2.compare(start2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x40);
						startA = true;
					}
					if (dpddown2.compare(vr12) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x01);
						vr1A = true;
					}
					if (dpddown2.compare(vr22) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x02);
						vr2A = true;
					}
					if (dpddown2.compare(shiftup2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x10);
						shiftupA = true;
					}
					if (dpddown2.compare(shiftdwn2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x20);
						shiftdwnA = true;
					}
				}
			}
			if (e.jhat.value == SDL_HAT_LEFT)
			{
				if (e.jhat.which == joystick_index1)
				{
					if (dpdleft.compare(exit) == 0)
					{
						SendMessage(hWndM2, WM_CLOSE, NULL, NULL);
					}
					if (dpdleft.compare(test) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x04);
						testA = true;
					}
					if (dpdleft.compare(service) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x08);
						serviceA = true;
					}
					if (dpdleft.compare(coin1) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x01);
						coin1A = true;
					}
					if (dpdleft.compare(coin2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x02);
						coin2A = true;
					}
					if (dpdleft.compare(start) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x40);
						startA = true;
					}
					if (dpdleft.compare(vr1) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x01);
						vr1A = true;
					}
					if (dpdleft.compare(vr2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x02);
						vr2A = true;
					}
					if (dpdleft.compare(shiftup) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x10);
						shiftupA = true;
					}
					if (dpdleft.compare(shiftdwn) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x20);
						shiftdwnA = true;
					}
				}
				else if (e.jhat.which == joystick_index2)
				{
					if (dpdleft2.compare(exit2) == 0)
					{
						SendMessage(hWndM2, WM_CLOSE, NULL, NULL);
					}
					if (dpdleft2.compare(test2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x04);
						testA = true;
					}
					if (dpdleft2.compare(service2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x08);
						serviceA = true;
					}
					if (dpdleft2.compare(coin12) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x01);
						coin1A = true;
					}
					if (dpdleft2.compare(coin22) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x02);
						coin2A = true;
					}
					if (dpdleft2.compare(start2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x40);
						startA = true;
					}
					if (dpdleft2.compare(vr12) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x01);
						vr1A = true;
					}
					if (dpdleft2.compare(vr22) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x02);
						vr2A = true;
					}
					if (dpdleft2.compare(shiftup2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x10);
						shiftupA = true;
					}
					if (dpdleft2.compare(shiftdwn2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x20);
						shiftdwnA = true;
					}
				}
			}
			if (e.jhat.value == SDL_HAT_RIGHT)
			{
				if (e.jhat.which == joystick_index1)
				{
					if (dpdright.compare(exit) == 0)
					{
						SendMessage(hWndM2, WM_CLOSE, NULL, NULL);
					}
					if (dpdright.compare(test) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x04);
						testA = true;
					}
					if (dpdright.compare(service) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x08);
						serviceA = true;
					}
					if (dpdright.compare(coin1) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x01);
						coin1A = true;
					}
					if (dpdright.compare(coin2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x02);
						coin2A = true;
					}
					if (dpdright.compare(start) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x40);
						startA = true;
					}
					if (dpdright.compare(vr1) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x01);
						vr1A = true;
					}
					if (dpdright.compare(vr2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x02);
						vr2A = true;
					}
					if (dpdright.compare(shiftup) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x10);
						shiftupA = true;
					}
					if (dpdright.compare(shiftdwn) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x20);
						shiftdwnA = true;
					}
				}
				else if (e.jhat.which == joystick_index2)
				{
					if (dpdright2.compare(exit2) == 0)
					{
						SendMessage(hWndM2, WM_CLOSE, NULL, NULL);
					}
					if (dpdright2.compare(test2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x04);
						testA = true;
					}
					if (dpdright2.compare(service2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x08);
						serviceA = true;
					}
					if (dpdright2.compare(coin12) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x01);
						coin1A = true;
					}
					if (dpdright2.compare(coin22) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x02);
						coin2A = true;
					}
					if (dpdright2.compare(start2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x40);
						startA = true;
					}
					if (dpdright2.compare(vr12) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x01);
						vr1A = true;
					}
					if (dpdright2.compare(vr22) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x02);
						vr2A = true;
					}
					if (dpdright2.compare(shiftup2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x10);
						shiftupA = true;
					}
					if (dpdright2.compare(shiftdwn2) == 0)
					{
						ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x20);
						shiftdwnA = true;
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
					ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x04);
				}
				if (e.jbutton.button == ExitButton)
				{
					SendMessage(hWndM2, WM_CLOSE, NULL, NULL);
				}
				if (e.jbutton.button == ServiceButton)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x08);
				}
				if (e.jbutton.button == Coin1Button)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x01);
				}
				if (e.jbutton.button == Coin2Button)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x02);
				}
				if (e.jbutton.button == VR1Button)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x01);
				}
				if (e.jbutton.button == VR2Button)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x02);
				}
				if (e.jbutton.button == ShiftUpButton)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x10);
				}
				if (e.jbutton.button == ShiftDownButton)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x20);
				}
				if (e.jbutton.button == StartButton)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x40);
				}
			}
			else if (e.jbutton.which == joystick_index2)
			{
				if (e.jbutton.button == TestButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x04);
				}
				if (e.jbutton.button == ExitButtonDevice2)
				{
					SendMessage(hWndM2, WM_CLOSE, NULL, NULL);
				}
				if (e.jbutton.button == ServiceButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x08);
				}
				if (e.jbutton.button == Coin1ButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x01);
				}
				if (e.jbutton.button == Coin2ButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x02);
				}
				if (e.jbutton.button == VR1ButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x01);
				}
				if (e.jbutton.button == VR2ButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x02);
				}
				if (e.jbutton.button == ShiftUpButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x10);
				}
				if (e.jbutton.button == ShiftDownButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x20);
				}
				if (e.jbutton.button == StartButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x40);
				}
			}
		}
		if (e.type == SDL_JOYBUTTONUP)
		{
			if (e.jbutton.which == joystick_index1)
			{
				if (e.jbutton.button == TestButton)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x04);
				}
				if (e.jbutton.button == ServiceButton)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x08);
				}
				if (e.jbutton.button == Coin1Button)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x01);
				}
				if (e.jbutton.button == Coin2Button)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x02);
				}
				if (e.jbutton.button == VR1Button)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read += 0x01);
				}
				if (e.jbutton.button == VR2Button)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read += 0x02);
				}
				if (e.jbutton.button == ShiftUpButton)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read += 0x10);
				}
				if (e.jbutton.button == ShiftDownButton)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read += 0x20);
				}
				if (e.jbutton.button == StartButton)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x40);
				}
			}
			else if (e.jbutton.which == joystick_index2)
			{
				if (e.jbutton.button == TestButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x04);
				}
				if (e.jbutton.button == ServiceButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x08);
				}
				if (e.jbutton.button == Coin1ButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x01);
				}
				if (e.jbutton.button == Coin2ButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x02);
				}
				if (e.jbutton.button == VR1ButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read += 0x01);
				}
				if (e.jbutton.button == VR2ButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read += 0x02);
				}
				if (e.jbutton.button == ShiftUpButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read += 0x10);
				}
				if (e.jbutton.button == ShiftDownButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read += 0x20);
				}
				if (e.jbutton.button == StartButtonDevice2)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x40);
				}
			}
		}
		if (e.type == SDL_KEYDOWN)
		{
			const char* KeyPressed = SDL_GetKeyName(e.key.keysym.sym);

			if (KeyBrdExit.compare(KeyPressed) == 0)
			{
				SendMessage(hWndM2, WM_CLOSE, NULL, NULL);
			}
			if (KeyBrdTest.compare(KeyPressed) == 0)
			{
				if (!testA)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x04);
					testA = true;
				}
			}
			if (KeyBrdService.compare(KeyPressed) == 0)
			{
				if (!serviceA)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x08);
					serviceA = true;
				}
			}
			if (KeyBrdCoin1.compare(KeyPressed) == 0)
			{
				if (!coin1A)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x01);
					coin1A = true;
				}
			}
			if (KeyBrdCoin2.compare(KeyPressed) == 0)
			{
				if (!coin2A)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x02);
					coin2A = true;
				}
			}
			if (KeyBrdVR1.compare(KeyPressed) == 0)
			{
				if (!vr1A)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x01);
					vr1A = true;
				}
			}
			if (KeyBrdVR2.compare(KeyPressed) == 0)
			{
				if (!vr2A)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x02);
					vr2A = true;
				}
			}
			if (KeyBrdStart.compare(KeyPressed) == 0)
			{
				if (!startA)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read -= 0x40);
					startA = true;
				}
			}
			if (KeyBrdShiftUp.compare(KeyPressed) == 0)
			{
				if (!shiftupA)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x10);
					shiftupA = true;
				}
			}
			if (KeyBrdShiftDown.compare(KeyPressed) == 0)
			{
				if (!shiftdwnA)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read -= 0x20);
					shiftdwnA = true;
				}
			}
		}

		if (e.type == SDL_KEYUP)
		{
			const char* KeyPressed = SDL_GetKeyName(e.key.keysym.sym);

			if (KeyBrdTest.compare(KeyPressed) == 0)
			{
				if (testA)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x04);
					testA = false;
				}
			}
			if (KeyBrdService.compare(KeyPressed) == 0)
			{
				if (serviceA)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x08);
					serviceA = false;
				}
			}
			if (KeyBrdCoin1.compare(KeyPressed) == 0)
			{
				if (coin1A)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x01);
					coin1A = false;
				}
			}
			if (KeyBrdCoin2.compare(KeyPressed) == 0)
			{
				if (coin2A)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x02);
					coin2A = false;
				}
			}
			if (KeyBrdVR1.compare(KeyPressed) == 0)
			{
				if (vr1A)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read += 0x01);
					vr1A = false;
				}
			}
			if (KeyBrdVR2.compare(KeyPressed) == 0)
			{
				if (vr2A)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read += 0x02);
					vr2A = false;
				}
			}
			if (KeyBrdStart.compare(KeyPressed) == 0)
			{
				if (startA)
				{
					ffb::Memory::Write<UINT8>(Button2Address, button2read += 0x40);
					startA = false;
				}
			}
			if (KeyBrdShiftUp.compare(KeyPressed) == 0)
			{
				if (shiftupA)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read += 0x10);
					shiftupA = false;
				}

			}
			if (KeyBrdShiftDown.compare(KeyPressed) == 0)
			{
				if (shiftdwnA)
				{
					ffb::Memory::Write<UINT8>(Button1Address, button1read += 0x20);
					shiftdwnA = false;
				}
			}
		}
	}
}
