#include "../Common Files/Game.h"
#include <string>
#include <windows.h>
#include "SDL.h"
#include "../Common Files/Devices.h"

extern int joystick_index1;
extern int joystick_index2;
static bool testbuttonA;
static bool servicebuttonA;
static bool coinbuttonA;
static bool hornbuttonA;
static bool volupA;
static bool voldownA;
static bool acclpressed;
static bool brakepressed;
static bool Inputsinit = false;
extern SDL_Event e;

extern INT_PTR SteeringAddress;
extern INT_PTR AcclAddress;
extern INT_PTR ServiceTestAddress;

static int AcclValue;
static int BrakeValue;

static int InputDeviceWheelSteeringAxis = GetSettingInt(TEXT("InputDeviceWheelSteeringAxis"), 0);
static int InputDeviceWheelAcclAxis = GetSettingInt(TEXT("InputDeviceWheelAcclAxis"), 0);
static int InputDeviceWheelBrakeAxis = GetSettingInt(TEXT("InputDeviceWheelBrakeAxis"), 0);
static int InputDeviceWheelReverseAxis = GetSettingInt(TEXT("InputDeviceWheelReverseAxis"), 0);
static int InputDeviceCombinedPedals = GetSettingInt(TEXT("InputDeviceCombinedPedals"), 0);
static int SteeringDeadzone = GetSettingInt(TEXT("SteeringDeadzone"), 0);
static int PedalDeadzone = GetSettingInt(TEXT("PedalDeadzone"), 0);
static int ShowButtonNumbersForSetup = GetSettingInt(TEXT("ShowButtonNumbersForSetup"), 0);
static int ShowAxisForSetup = GetSettingInt(TEXT("ShowAxisForSetup"), 0);
static int ExitButton = GetSettingInt(TEXT("ExitButton"), 0);
static int TestButton = GetSettingInt(TEXT("TestButton"), 0);
static int ServiceButton = GetSettingInt(TEXT("ServiceButton"), 0);
static int CreditButton = GetSettingInt(TEXT("CreditButton"), 0);
static int HornButton = GetSettingInt(TEXT("HornButton"), 0);
static int VolumeUp = GetSettingInt(TEXT("VolumeUp"), 0);
static int VolumeDown = GetSettingInt(TEXT("VolumeDown"), 0);
static int ExitButtonDevice2 = GetSettingInt(TEXT("ExitButtonDevice2"), 0);
static int TestButtonDevice2 = GetSettingInt(TEXT("TestButtonDevice2"), 0);
static int ServiceButtonDevice2 = GetSettingInt(TEXT("ServiceButtonDevice2"), 0);
static int CreditButtonDevice2 = GetSettingInt(TEXT("CreditButtonDevice2"), 0);
static int HornButtonDevice2 = GetSettingInt(TEXT("HornButtonDevice2"), 0);
static int VolumeUpDevice2 = GetSettingInt(TEXT("VolumeUpDevice2"), 0);
static int VolumeDownDevice2 = GetSettingInt(TEXT("VolumeDownDevice2"), 0);

void SmashingDriveInputsEnabled(Helpers* helpers)
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
	GetPrivateProfileStringA("Settings", "DpadUp", "", DpadUpChar, 256, ".\\FFBplugin.ini");
	GetPrivateProfileStringA("Settings", "DpadDown", "", DpadDownChar, 256, ".\\FFBplugin.ini");
	GetPrivateProfileStringA("Settings", "DpadLeft", "", DpadLeftChar, 256, ".\\FFBplugin.ini");
	GetPrivateProfileStringA("Settings", "DpadRight", "", DpadRightChar, 256, ".\\FFBplugin.ini");
	GetPrivateProfileStringA("Settings", "DpadUpDevice2", "", DpadUpCharDevice2, 256, ".\\FFBplugin.ini");
	GetPrivateProfileStringA("Settings", "DpadDownDevice2", "", DpadDownCharDevice2, 256, ".\\FFBplugin.ini");
	GetPrivateProfileStringA("Settings", "DpadLeftDevice2", "", DpadLeftCharDevice2, 256, ".\\FFBplugin.ini");
	GetPrivateProfileStringA("Settings", "DpadRightDevice2", "", DpadRightCharDevice2, 256, ".\\FFBplugin.ini");
	std::string exit("ExitButton");
	std::string test("TestButton");
	std::string service("ServiceButton");
	std::string coin("CoinButton");
	std::string horn("HornButton");
	std::string volup("VolumeUp");
	std::string voldown("VolumeDown");
	std::string exit2("ExitButtonDevice2");
	std::string test2("TestButtonDevice2");
	std::string service2("ServiceButtonDevice2");
	std::string coin2("CoinButtonDevice2");
	std::string horn2("HornButtonDevice2");
	std::string volup2("VolumeUpDevice2");
	std::string voldown2("VolumeDownDevice2");
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

	UINT8 servicetestread = helpers->ReadByte(ServiceTestAddress, false);
	UINT8 acclbrakeread = helpers->ReadByte(AcclAddress, false);

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
						helpers->WriteByte(SteeringAddress, (128 + (e.jaxis.value + WHEEL_DEAD_ZONE) / 255), false);
					}
					//Right of dead zone
					else if (e.jaxis.value > WHEEL_DEAD_ZONE)
					{
						e.jaxis.value = e.jaxis.value + WHEEL_DEAD_ZONE;
						helpers->WriteByte(SteeringAddress, (127 + (e.jaxis.value - WHEEL_DEAD_ZONE) / 255), false);
					}
					else if ((e.jaxis.value < WHEEL_DEAD_ZONE) && (e.jaxis.value > -WHEEL_DEAD_ZONE))
					{
						helpers->WriteByte(SteeringAddress, 0x7F, false);
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
								AcclValue = (INT)((-e.jaxis.value + ACCL_DEAD_ZONE) / 128.5);
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = e.jaxis.value - ACCL_DEAD_ZONE;
								BrakeValue = ((e.jaxis.value + ACCL_DEAD_ZONE) / 128);
							}
							else if ((e.jaxis.value < ACCL_DEAD_ZONE) && (e.jaxis.value > -ACCL_DEAD_ZONE))
							{
								AcclValue = 0x00;
								BrakeValue = 0x00;
							}

							if (AcclValue > 0)
							{
								if (!acclpressed)
								{
									helpers->WriteByte(AcclAddress, acclbrakeread -= 0x10, false);
									acclpressed = true;
								}
							}
							else
							{
								if (acclpressed)
								{
									helpers->WriteByte(AcclAddress, acclbrakeread += 0x10, false);
									acclpressed = false;
								}
							}

							if (BrakeValue > 0)
							{
								if (!brakepressed)
								{
									helpers->WriteByte(AcclAddress, acclbrakeread -= 0x20, false);
									brakepressed = true;
								}
							}
							else
							{
								if (brakepressed)
								{
									helpers->WriteByte(AcclAddress, acclbrakeread += 0x20, false);
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
								AcclValue = 127 - e.jaxis.value;
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								AcclValue = 128 - e.jaxis.value;
							}

							if (AcclValue > 0)
							{
								if (!acclpressed)
								{
									helpers->WriteByte(AcclAddress, acclbrakeread -= 0x10, false);
									acclpressed = true;
								}
							}
							else
							{
								if (acclpressed)
								{
									helpers->WriteByte(AcclAddress, acclbrakeread += 0x10, false);
									acclpressed = false;
								}
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
									helpers->WriteByte(AcclAddress, acclbrakeread -= 0x20, false);
									brakepressed = true;
								}
							}
							else
							{
								if (brakepressed)
								{
									helpers->WriteByte(AcclAddress, acclbrakeread += 0x20, false);
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
								AcclValue = ((e.jaxis.value + ACCL_DEAD_ZONE) / 128);
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = e.jaxis.value + ACCL_DEAD_ZONE;
								BrakeValue = ((-e.jaxis.value + ACCL_DEAD_ZONE) / 128);
							}
							else if ((e.jaxis.value < ACCL_DEAD_ZONE) && (e.jaxis.value > -ACCL_DEAD_ZONE))
							{
								AcclValue = 0xFF;
								BrakeValue = 0xFF;
							}

							if (AcclValue > 0)
							{
								if (!acclpressed)
								{
									helpers->WriteByte(AcclAddress, acclbrakeread -= 0x10, false);
									acclpressed = true;
								}
							}
							else
							{
								if (acclpressed)
								{
									helpers->WriteByte(AcclAddress, acclbrakeread += 0x10, false);
									acclpressed = false;
								}
							}

							if (BrakeValue > 0)
							{
								if (!brakepressed)
								{
									helpers->WriteByte(AcclAddress, acclbrakeread -= 0x20, false);
									brakepressed = true;
								}
							}
							else
							{
								if (brakepressed)
								{
									helpers->WriteByte(AcclAddress, acclbrakeread += 0x20, false);
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
								AcclValue =  128 + e.jaxis.value;
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								AcclValue = 127 + e.jaxis.value;
							}

							if (AcclValue > 0)
							{
								if (!acclpressed)
								{
									helpers->WriteByte(AcclAddress, acclbrakeread -= 0x10, false);
									acclpressed = true;
								}
							}
							else
							{
								if (acclpressed)
								{
									helpers->WriteByte(AcclAddress, acclbrakeread += 0x10, false);
									acclpressed = false;
								}
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
									helpers->WriteByte(AcclAddress, acclbrakeread -= 0x20, false);
									brakepressed = true;
								}
							}
							else
							{
								if (brakepressed)
								{
									helpers->WriteByte(AcclAddress, acclbrakeread += 0x20, false);
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
						helpers->WriteByte(ServiceTestAddress, servicetestread += 0x08, false);
						testbuttonA = false;
					}
					if (servicebuttonA)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread += 0x02, false);
						servicebuttonA = false;
					}
					if (hornbuttonA)
					{
						helpers->WriteByte(AcclAddress, acclbrakeread += 0x40, false);
						hornbuttonA = false;
					}
					if (coinbuttonA)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread += 0x20, false);
						coinbuttonA = false;
					}
					if (volupA)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread += 0x01, false);
						volupA = false;
					}
					if (voldownA)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread += 0x04, false);
						voldownA = false;
					}
				}
				else if (e.jhat.which == joystick_index2)
				{
					if (testbuttonA)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread += 0x08, false);
						testbuttonA = false;
					}
					if (servicebuttonA)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread += 0x02, false);
						servicebuttonA = false;
					}
					if (hornbuttonA)
					{
						helpers->WriteByte(AcclAddress, acclbrakeread += 0x40, false);
						hornbuttonA = false;
					}
					if (coinbuttonA)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread += 0x20, false);
						coinbuttonA = false;
					}
					if (volupA)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread += 0x01, false);
						volupA = false;
					}
					if (voldownA)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread += 0x04, false);
						voldownA = false;
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
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x08, false);
						testbuttonA = true;
					}
					if (dpdup.compare(service) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x02, false);
						servicebuttonA = true;
					}
					if (dpdup.compare(coin) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x20, false);
						coinbuttonA = true;
					}
					if (dpdup.compare(horn) == 0)
					{
						helpers->WriteByte(AcclAddress, acclbrakeread -= 0x40, false);
						hornbuttonA = true;
					}
					if (dpdup.compare(volup) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x01, false);
						volupA = true;
					}
					if (dpdup.compare(voldown) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x04, false);
						voldownA = true;
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
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x08, false);
						testbuttonA = true;
					}
					if (dpdup2.compare(service2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x02, false);
						servicebuttonA = true;
					}
					if (dpdup2.compare(coin2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x20, false);
						coinbuttonA = true;
					}
					if (dpdup2.compare(horn2) == 0)
					{
						helpers->WriteByte(AcclAddress, acclbrakeread -= 0x40, false);
						hornbuttonA = true;
					}
					if (dpdup2.compare(volup2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x01, false);
						volupA = true;
					}
					if (dpdup2.compare(voldown2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x04, false);
						voldownA = true;
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
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x08, false);
						testbuttonA = true;
					}
					if (dpddown.compare(service) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x02, false);
						servicebuttonA = true;
					}
					if (dpddown.compare(coin) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x20, false);
						coinbuttonA = true;
					}
					if (dpddown.compare(horn) == 0)
					{
						helpers->WriteByte(AcclAddress, acclbrakeread -= 0x40, false);
						hornbuttonA = true;
					}
					if (dpddown.compare(volup) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x01, false);
						volupA = true;
					}
					if (dpddown.compare(voldown) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x04, false);
						voldownA = true;
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
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x08, false);
						testbuttonA = true;
					}
					if (dpddown2.compare(service2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x02, false);
						servicebuttonA = true;
					}
					if (dpddown2.compare(coin2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x20, false);
						coinbuttonA = true;
					}
					if (dpddown2.compare(horn2) == 0)
					{
						helpers->WriteByte(AcclAddress, acclbrakeread -= 0x40, false);
						hornbuttonA = true;
					}
					if (dpddown2.compare(volup2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x01, false);
						volupA = true;
					}
					if (dpddown2.compare(voldown2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x04, false);
						voldownA = true;
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
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x08, false);
						testbuttonA = true;
					}
					if (dpdleft.compare(service) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x02, false);
						servicebuttonA = true;
					}
					if (dpdleft.compare(coin) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x20, false);
						coinbuttonA = true;
					}
					if (dpdleft.compare(horn) == 0)
					{
						helpers->WriteByte(AcclAddress, acclbrakeread -= 0x40, false);
						hornbuttonA = true;
					}
					if (dpdleft.compare(volup) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x01, false);
						volupA = true;
					}
					if (dpdleft.compare(voldown) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x04, false);
						voldownA = true;
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
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x08, false);
						testbuttonA = true;
					}
					if (dpdleft2.compare(service2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x02, false);
						servicebuttonA = true;
					}
					if (dpdleft2.compare(coin2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x20, false);
						coinbuttonA = true;
					}
					if (dpdleft2.compare(horn2) == 0)
					{
						helpers->WriteByte(AcclAddress, acclbrakeread -= 0x40, false);
						hornbuttonA = true;
					}
					if (dpdleft2.compare(volup2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x01, false);
						volupA = true;
					}
					if (dpdleft2.compare(voldown2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x04, false);
						voldownA = true;
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
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x08, false);
						testbuttonA = true;
					}
					if (dpdright.compare(service) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x02, false);
						servicebuttonA = true;
					}
					if (dpdright.compare(coin) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x20, false);
						coinbuttonA = true;
					}
					if (dpdright.compare(horn) == 0)
					{
						helpers->WriteByte(AcclAddress, acclbrakeread -= 0x40, false);
						hornbuttonA = true;
					}
					if (dpdright.compare(volup) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x01, false);
						volupA = true;
					}
					if (dpdright.compare(voldown) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x04, false);
						voldownA = true;
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
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x08, false);
						testbuttonA = true;
					}
					if (dpdright2.compare(service2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x02, false);
						servicebuttonA = true;
					}
					if (dpdright2.compare(coin2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x20, false);
						coinbuttonA = true;
					}
					if (dpdright2.compare(horn2) == 0)
					{
						helpers->WriteByte(AcclAddress, acclbrakeread -= 0x40, false);
						hornbuttonA = true;
					}
					if (dpdright2.compare(volup2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x01, false);
						volupA = true;
					}
					if (dpdright2.compare(voldown2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x04, false);
						voldownA = true;
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
					helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x08, false);
				}
				if (e.jbutton.button == ExitButton)
				{
					system("taskkill /f /im demul.exe");
				}
				if (e.jbutton.button == ServiceButton)
				{
					helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x02, false);
				}
				if (e.jbutton.button == CreditButton)
				{
					helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x20, false);
				}
				if (e.jbutton.button == HornButton)
				{
					helpers->WriteByte(AcclAddress, acclbrakeread -= 0x40, false);
				}
				if (e.jbutton.button == VolumeUp)
				{
					helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x01, false);
				}
				if (e.jbutton.button == VolumeDown)
				{
					helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x04, false);
				}
			}
			else if (e.jbutton.which == joystick_index2)
			{
				if (e.jbutton.button == TestButtonDevice2)
				{
					helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x08, false);
				}
				if (e.jbutton.button == ExitButtonDevice2)
				{
					system("taskkill /f /im demul.exe");
				}
				if (e.jbutton.button == ServiceButtonDevice2)
				{
					helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x02, false);
				}
				if (e.jbutton.button == CreditButtonDevice2)
				{
					helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x20, false);
				}
				if (e.jbutton.button == HornButtonDevice2)
				{
					helpers->WriteByte(AcclAddress, acclbrakeread -= 0x40, false);
				}
				if (e.jbutton.button == VolumeUpDevice2)
				{
					helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x01, false);
				}
				if (e.jbutton.button == VolumeDownDevice2)
				{
					helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x04, false);
				}
			}
		}
		if (e.type == SDL_JOYBUTTONUP)
		{
			if (e.jbutton.which == joystick_index1)
			{
				if (e.jbutton.button == TestButton)
				{
					helpers->WriteByte(ServiceTestAddress, servicetestread += 0x08, false);
				}
				if (e.jbutton.button == ServiceButton)
				{
					helpers->WriteByte(ServiceTestAddress, servicetestread += 0x02, false);
				}
				if (e.jbutton.button == CreditButton)
				{
					helpers->WriteByte(ServiceTestAddress, servicetestread += 0x20, false);
				}
				if (e.jbutton.button == HornButton)
				{
					helpers->WriteByte(AcclAddress, acclbrakeread += 0x40, false);
				}
				if (e.jbutton.button == VolumeUp)
				{
					helpers->WriteByte(ServiceTestAddress, servicetestread += 0x01, false);
				}
				if (e.jbutton.button == VolumeDown)
				{
					helpers->WriteByte(ServiceTestAddress, servicetestread += 0x04, false);
				}
			}
			else if (e.jbutton.which == joystick_index2)
			{
				if (e.jbutton.button == TestButtonDevice2)
				{
					helpers->WriteByte(ServiceTestAddress, servicetestread += 0x08, false);
				}
				if (e.jbutton.button == ServiceButtonDevice2)
				{
					helpers->WriteByte(ServiceTestAddress, servicetestread += 0x02, false);
				}
				if (e.jbutton.button == CreditButtonDevice2)
				{
					helpers->WriteByte(ServiceTestAddress, servicetestread += 0x20, false);
				}
				if (e.jbutton.button == HornButtonDevice2)
				{
					helpers->WriteByte(AcclAddress, acclbrakeread += 0x40, false);
				}
				if (e.jbutton.button == VolumeUpDevice2)
				{
					helpers->WriteByte(ServiceTestAddress, servicetestread += 0x01, false);
				}
				if (e.jbutton.button == VolumeDownDevice2)
				{
					helpers->WriteByte(ServiceTestAddress, servicetestread += 0x04, false);
				}
			}
		}
	}
}