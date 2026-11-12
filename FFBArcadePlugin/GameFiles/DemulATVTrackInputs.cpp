#include "../Common Files/Game.h"
#include <string>
#include <windows.h>
#include "SDL.h"
#include "../Common Files/Devices.h"

extern int joystick_index1;
extern int joystick_index2;
static bool testbuttonA;
static bool servicebuttonA;
static bool startbuttonA;
static bool volumeupA;
static bool volumedownA;
static bool coinA;
static bool brakepressed;
static bool emergencystopA;
static bool Inputsinit = false;
extern SDL_Event e;

static int SteeringValue;
static int BrakeValue;

extern INT_PTR SteeringAddress;
extern INT_PTR AcclAddress;
extern INT_PTR BrakeAddress;
extern INT_PTR ServiceTestAddress;

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
static int StartButton = GetSettingInt(TEXT("StartButton"), 0);
static int VolumeUp = GetSettingInt(TEXT("VolumeUp"), 0);
static int VolumeDown = GetSettingInt(TEXT("VolumeDown"), 0);
static int EmergencyStop = GetSettingInt(TEXT("EmergencyStop"), 0);
static int ExitButtonDevice2 = GetSettingInt(TEXT("ExitButtonDevice2"), 0);
static int TestButtonDevice2 = GetSettingInt(TEXT("TestButtonDevice2"), 0);
static int ServiceButtonDevice2 = GetSettingInt(TEXT("ServiceButtonDevice2"), 0);
static int CreditButtonDevice2 = GetSettingInt(TEXT("CreditButtonDevice2"), 0);
static int StartButtonDevice2 = GetSettingInt(TEXT("StartButtonDevice2"), 0);
static int VolumeUpDevice2 = GetSettingInt(TEXT("VolumeUpDevice2"), 0);
static int VolumeDownDevice2 = GetSettingInt(TEXT("VolumeDownDevice2"), 0);
static int EmergencyStopDevice2 = GetSettingInt(TEXT("EmergencyStopDevice2"), 0);

void ATVTrackInputsEnabled(Helpers* helpers)
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

	UINT8 brakeread = helpers->ReadByte(BrakeAddress, false);
	UINT8 servicetestread = helpers->ReadByte(ServiceTestAddress, false);

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

					helpers->WriteByte(SteeringAddress, -SteeringValue - 1, false);
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
								helpers->WriteByte(AcclAddress, (UINT8)((-e.jaxis.value + ACCL_DEAD_ZONE) / 128.5), false);
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = e.jaxis.value - ACCL_DEAD_ZONE;
								BrakeValue = ((e.jaxis.value + ACCL_DEAD_ZONE) / 128);
							}
							else if ((e.jaxis.value < ACCL_DEAD_ZONE) && (e.jaxis.value > -ACCL_DEAD_ZONE))
							{
								helpers->WriteByte(AcclAddress, 0x00, false);
								BrakeValue = 0x00;
							}

							if (BrakeValue > 0)
							{
								if (!brakepressed)
								{
									helpers->WriteByte(BrakeAddress, brakeread -= 0x10, false);
									brakepressed = true;
								}
							}
							else
							{
								if (brakepressed)
								{
									helpers->WriteByte(BrakeAddress, brakeread += 0x10, false);
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
								helpers->WriteByte(AcclAddress, 127 - e.jaxis.value, false);
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								helpers->WriteByte(AcclAddress, 128 - e.jaxis.value, false);
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
									helpers->WriteByte(BrakeAddress, brakeread -= 0x10, false);
									brakepressed = true;
								}
							}
							else
							{
								if (brakepressed)
								{
									helpers->WriteByte(BrakeAddress, brakeread += 0x10, false);
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
								helpers->WriteByte(AcclAddress, ((e.jaxis.value + ACCL_DEAD_ZONE) / 128), false);
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = e.jaxis.value + ACCL_DEAD_ZONE;
								BrakeValue = ((-e.jaxis.value + ACCL_DEAD_ZONE) / 128);
							}
							else if ((e.jaxis.value < ACCL_DEAD_ZONE) && (e.jaxis.value > -ACCL_DEAD_ZONE))
							{
								helpers->WriteByte(AcclAddress, 0xFF, false);
								BrakeValue = 0x00;
							}

							if (BrakeValue > 0)
							{
								if (!brakepressed)
								{
									helpers->WriteByte(BrakeAddress, brakeread -= 0x10, false);
									brakepressed = true;
								}
							}
							else
							{
								if (brakepressed)
								{
									helpers->WriteByte(BrakeAddress, brakeread += 0x10, false);
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
								helpers->WriteByte(AcclAddress, 128 + e.jaxis.value, false);
							}
							else if (e.jaxis.value > ACCL_DEAD_ZONE)
							{
								e.jaxis.value = (e.jaxis.value / 255);
								helpers->WriteByte(AcclAddress, 127 + e.jaxis.value, false);
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
									helpers->WriteByte(BrakeAddress, brakeread -= 0x10, false);
									brakepressed = true;
								}
							}
							else
							{
								if (brakepressed)
								{
									helpers->WriteByte(BrakeAddress, brakeread += 0x10, false);
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
					if (startbuttonA)
					{
						helpers->WriteByte(BrakeAddress, brakeread += 0x80, false);
						startbuttonA = false;
					}
					if (volumeupA)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread += 0x01, false);
						volumeupA = false;
					}
					if (volumedownA)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread += 0x04, false);
						volumedownA = false;
					}
					if (emergencystopA)
					{
						helpers->WriteByte(BrakeAddress, brakeread -= 0x40, false);
						emergencystopA = false;
					}
					if (coinA)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread += 0x20, false);
						coinA = false;
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
					if (startbuttonA)
					{
						helpers->WriteByte(BrakeAddress, brakeread += 0x80, false);
						startbuttonA = false;
					}
					if (volumeupA)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread += 0x01, false);
						volumeupA = false;
					}
					if (volumedownA)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread += 0x04, false);
						volumedownA = false;
					}
					if (emergencystopA)
					{
						helpers->WriteByte(BrakeAddress, brakeread -= 0x40, false);
						emergencystopA = false;
					}
					if (coinA)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread += 0x20, false);
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
						coinA = true;
					}
					if (dpdup.compare(emergency) == 0)
					{
						helpers->WriteByte(BrakeAddress, brakeread += 0x40, false);
						emergencystopA = true;
					}
					if (dpdup.compare(start) == 0)
					{
						helpers->WriteByte(BrakeAddress, brakeread -= 0x80, false);
						startbuttonA = true;
					}
					if (dpdup.compare(volup) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x01, false);
						volumeupA = true;
					}
					if (dpdup.compare(voldown) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x04, false);
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
						coinA = true;
					}
					if (dpdup2.compare(emergency2) == 0)
					{
						helpers->WriteByte(BrakeAddress, brakeread += 0x40, false);
						emergencystopA = true;
					}
					if (dpdup2.compare(start2) == 0)
					{
						helpers->WriteByte(BrakeAddress, brakeread -= 0x80, false);
						startbuttonA = true;
					}
					if (dpdup2.compare(volup2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x01, false);
						volumeupA = true;
					}
					if (dpdup2.compare(voldown2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x04, false);
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
						coinA = true;
					}
					if (dpddown.compare(emergency) == 0)
					{
						helpers->WriteByte(BrakeAddress, brakeread += 0x40, false);
						emergencystopA = true;
					}
					if (dpddown.compare(start) == 0)
					{
						helpers->WriteByte(BrakeAddress, brakeread -= 0x80, false);
						startbuttonA = true;
					}
					if (dpddown.compare(volup) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x01, false);
						volumeupA = true;
					}
					if (dpddown.compare(voldown) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x04, false);
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
						coinA = true;
					}
					if (dpddown2.compare(emergency2) == 0)
					{
						helpers->WriteByte(BrakeAddress, brakeread += 0x40, false);
						emergencystopA = true;
					}
					if (dpddown2.compare(start2) == 0)
					{
						helpers->WriteByte(BrakeAddress, brakeread -= 0x80, false);
						startbuttonA = true;
					}
					if (dpddown2.compare(volup2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x01, false);
						volumeupA = true;
					}
					if (dpddown2.compare(voldown2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x04, false);
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
						coinA = true;
					}
					if (dpdleft.compare(emergency) == 0)
					{
						helpers->WriteByte(BrakeAddress, brakeread += 0x40, false);
						emergencystopA = true;
					}
					if (dpdleft.compare(start) == 0)
					{
						helpers->WriteByte(BrakeAddress, brakeread -= 0x80, false);
						startbuttonA = true;
					}
					if (dpdleft.compare(volup) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x01, false);
						volumeupA = true;
					}
					if (dpdleft.compare(voldown) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x04, false);
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
						coinA = true;
					}
					if (dpdleft2.compare(emergency2) == 0)
					{
						helpers->WriteByte(BrakeAddress, brakeread += 0x40, false);
						emergencystopA = true;
					}
					if (dpdleft2.compare(start2) == 0)
					{
						helpers->WriteByte(BrakeAddress, brakeread -= 0x80, false);
						startbuttonA = true;
					}
					if (dpdleft2.compare(volup2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x01, false);
						volumeupA = true;
					}
					if (dpdleft2.compare(voldown2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x04, false);
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
						coinA = true;
					}
					if (dpdright.compare(emergency) == 0)
					{
						helpers->WriteByte(BrakeAddress, brakeread += 0x40, false);
						emergencystopA = true;
					}
					if (dpdright.compare(start) == 0)
					{
						helpers->WriteByte(BrakeAddress, brakeread -= 0x80, false);
						startbuttonA = true;
					}
					if (dpdright.compare(volup) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x01, false);
						volumeupA = true;
					}
					if (dpdright.compare(voldown) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x04, false);
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
						coinA = true;
					}
					if (dpdright2.compare(emergency2) == 0)
					{
						helpers->WriteByte(BrakeAddress, brakeread += 0x40, false);
						emergencystopA = true;
					}
					if (dpdright2.compare(start2) == 0)
					{
						helpers->WriteByte(BrakeAddress, brakeread -= 0x80, false);
						startbuttonA = true;
					}
					if (dpdright2.compare(volup2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x01, false);
						volumeupA = true;
					}
					if (dpdright2.compare(voldown2) == 0)
					{
						helpers->WriteByte(ServiceTestAddress, servicetestread -= 0x04, false);
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
				if (e.jbutton.button == EmergencyStop)
				{
					helpers->WriteByte(BrakeAddress, brakeread += 0x40, false);
				}
				if (e.jbutton.button == StartButton)
				{
					helpers->WriteByte(BrakeAddress, brakeread -= 0x80, false);
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
				if (e.jbutton.button == EmergencyStopDevice2)
				{
					helpers->WriteByte(BrakeAddress, brakeread += 0x40, false);
				}
				if (e.jbutton.button == StartButtonDevice2)
				{
					helpers->WriteByte(BrakeAddress, brakeread -= 0x80, false);
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
				if (e.jbutton.button == EmergencyStop)
				{
					helpers->WriteByte(BrakeAddress, brakeread -= 0x40, false);
				}
				if (e.jbutton.button == StartButton)
				{
					helpers->WriteByte(BrakeAddress, brakeread += 0x80, false);
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
				if (e.jbutton.button == EmergencyStopDevice2)
				{
					helpers->WriteByte(BrakeAddress, brakeread -= 0x40, false);
				}
				if (e.jbutton.button == StartButtonDevice2)
				{
					helpers->WriteByte(BrakeAddress, brakeread += 0x80, false);
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