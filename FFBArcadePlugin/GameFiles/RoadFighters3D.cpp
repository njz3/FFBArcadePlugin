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
#include "SDL.h"
#include "../Core/Devices.h"
#include "RoadFighters3D.h"

using namespace ffb;

extern HINSTANCE gl_hjgtDll;
extern HINSTANCE gl_hlibavs;
extern int joystick_index1;
extern int joystick_index2;
extern SDL_Joystick* GameController2;
extern SDL_Event e;

int RoadFighters3D::ThreadLoop()
{
	INT_PTR menuvalue = Memory::Read<INT_PTR>((INT_PTR)gl_hjgtDll + (INT_PTR)0x0094BFFC);
	INT_PTR menuvalue1 = Memory::Read<INT_PTR>((INT_PTR)menuvalue + (INT_PTR)0x46C);
	INT_PTR serviceread3 = Memory::Read<INT_PTR>((INT_PTR)gl_hjgtDll + (INT_PTR)0x7D2B24);
	INT_PTR timer = Memory::Read<INT_PTR>((INT_PTR)gl_hjgtDll + (INT_PTR)0x954394);
	int cabid = Memory::Read<UINT8>((INT_PTR)gl_hjgtDll + (INT_PTR)0x951034);
	int cabid2 = Memory::Read<UINT8>((INT_PTR)gl_hjgtDll + (INT_PTR)0x952B9C);
	float timeroutofmenu = Memory::Read<UINT8>((INT_PTR)gl_hjgtDll + (INT_PTR)0x94BEE8);
	uintptr_t jgtBase = (uintptr_t)GetModuleHandleA("jgt.dll");

	if (CabinetID == 2)
	{
		Memory::Patch(jgtBase + 0x951034, "\x01", 1);
		Memory::Patch(jgtBase + 0x42EBB9, "\x75", 1);
	}
	else if (CabinetID == 3)
	{
		Memory::Patch(jgtBase + 0x951034, "\x02", 1);
		Memory::Patch(jgtBase + 0x42EBB9, "\x75", 1);
	}
	else if (CabinetID == 4)
	{
		Memory::Patch(jgtBase + 0x951034, "\x03", 1);
		Memory::Patch(jgtBase + 0x42EBB9, "\x75", 1);
	}
	else
	{
		Memory::Patch(jgtBase + 0x951034, "\x00", 1);
		Memory::Patch(jgtBase + 0x42EBB9, "\x74", 1);
	}

	if (HackToCloseLibmovieErrorAuto == 1)
	{
		//Remove fucken window error popup
		HWND hWnd = FindWindowA(0, ("Libmovie Error Report"));
		if (hWnd > NULL)
		{
			SendMessage(hWnd, WM_CLOSE, NULL, NULL);
		}
	}
	if (HackToSkipMenuError == 1)
	{
		// Hack to quickly bypass error at start
		if (avoiderror)
		{
			if (cabid > 0)
			{
				if (menuvalue1 > 700000)
				{
					Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, 0x02);
				}
				if (menuvalue1 == 0)
				{
					SDL_Delay(50);
					Memory::Write<INT_PTR>((INT_PTR)menuvalue + 0x46C, 0x0F);
				}
				if (menuvalue1 == 15)
				{
					Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, 0x01);
				}
				if (timeroutofmenu != 0)
				{
					Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, 0x00);
					avoiderror = false;
				}
			}
			else
			{
				if (menuvalue1 > 700000)
				{
					Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, 0x01);
				}
				if (timeroutofmenu != 0)
				{
					Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, 0x00);
					Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B39, 0x7F);
					avoiderror = false;
				}
			}
		}
	}

	if (Only2D == 1)
	{
		//2D Only
		Memory::WriteNop((INT_PTR)gl_hjgtDll + 0x24C9F, 3);
		Memory::WriteNop((INT_PTR)gl_hjgtDll + 0x24CAA, 3);
		Memory::WriteNop((INT_PTR)gl_hjgtDll + 0x24CBA, 3);
		Memory::WriteNop((INT_PTR)gl_hjgtDll + 0x478F, 3);
		Memory::WriteNop((INT_PTR)gl_hjgtDll + 0x3E6DB, 3);
		Memory::WriteNop((INT_PTR)gl_hjgtDll + 0x1391D8, 3);
		Memory::WriteNop((INT_PTR)gl_hjgtDll + 0x1391EF, 3);
		Memory::WriteNop((INT_PTR)gl_hjgtDll + 0x5962F, 3);
		INT_PTR TwoDee1 = Memory::Read<INT_PTR>((INT_PTR)gl_hjgtDll + 0x00946DA0);
		INT_PTR TwoDee2 = Memory::Read<INT_PTR>((INT_PTR)TwoDee1 + 0x38);
		INT_PTR TwoDee3 = Memory::Read<INT_PTR>((INT_PTR)TwoDee2 + 0x94);
		Memory::Write<INT_PTR>((INT_PTR)TwoDee3 + 0x74, 0x00);
	}
	if (InputDeviceWheelEnable == 1)
	{
		//Write New Calibration Values	
		Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B00, 0x00);
		Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B01, 0x7F);
		Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B0C, 0x00);
		Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B0D, 0xFF);
		Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B14, 0x00);
		Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B15, 0xFF);
		Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B10, 0x00);
		Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B11, 0xFF);
		Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B21, 0x00);

		// Remove error flashing
		Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x951154, 0x01);
		Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x951155, 0x01);
		Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x951156, 0x01);
	}

	UINT8 ff1 = Memory::Read<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2BB9);
	UINT8 ff2 = Memory::Read<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2BBA);
	UINT8 ff3 = Memory::Read<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2BBB);
	UINT8 ff4 = Memory::Read<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2BBC);
	//Change timer back to 0 when test menu & FFB Only while timer above 0
	if ((timer != 0) || (menuvalue1 == 0x00))
	{
		if (menuvalue1 == 0x00)
		{
			Memory::Write<float>((INT_PTR)gl_hjgtDll + 0x954394, 0);
		}

		ApplyDamper();

		if ((ff3 != 0x00) && (ff4 != 0x00))
		{
			UINT32 length_ms = 100;
			if (ff2 > 0x00 && ff2 < 0x40)
			{
				double percentForce = (ff2) / 63.0;
				wheel.Rumble(percentForce, percentForce, length_ms);
				wheel.Sine(120, 120, percentForce, length_ms);
			}

			if (ff1 > 0x00 && ff1 < 0x08)
			{
				//Log::Write("moving wheel left");
				double percentForce = (ff1) / 7.0;
				wheel.Rumble(0, percentForce, length_ms);
				wheel.Constant(Direction::FromLeft, percentForce);
			}
			else if (ff1 > 0x07 && ff1 < 0x10)
			{
				//Log::Write("moving wheel right");
				double percentForce = (16 - ff1) / 8.0;
				wheel.Rumble(percentForce, 0, length_ms);
				wheel.Constant(Direction::FromRight, percentForce);
			}
		}
	}
	return 0;
}

DWORD WINAPI RoadFighters3D::RunningLoop(LPVOID game)
{
	while (true)
	{
		((RoadFighters3D*)game)->ThreadLoop();
		Sleep(16);
	}
}

void RoadFighters3D::Update()
{

	if (InputDeviceWheelEnable == 1)
	{
		if (!init)
		{

			CreateThread(NULL, 0, RunningLoop, this, 0, NULL);

			OpenInputDevice2();

			// Without a joystick matching Device2GUID, the last joystick is used as the second device
			if (GameController2 == NULL && SDL_NumJoysticks() > 0)
			{
				SDL_Joystick* lastJoystick = SDL_JoystickOpen(SDL_NumJoysticks() - 1);
				joystick_index2 = SDL_JoystickInstanceID(lastJoystick);
				SDL_JoystickClose(lastJoystick);
			}
			init = true;
		}

			Memory::WriteNop((INT_PTR)gl_hjgtDll + 0x18D84B, 3);
			Memory::WriteNop((INT_PTR)gl_hjgtDll + 0x18D852, 3);
			Memory::WriteNop((INT_PTR)gl_hjgtDll + 0x18D85C, 3);
			// Spicetools shit below. STOPS test buttons showing in menu etc if not nop
		    Memory::WriteNop((INT_PTR)gl_hjgtDll + 0x18DA89, 6);
			Memory::WriteNop((INT_PTR)gl_hjgtDll + 0x18DAA3, 3);
			Memory::WriteNop((INT_PTR)gl_hjgtDll + 0x18DA97, 6);

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
		std::string three("ThreeDimensionalButton");
		std::string lvup("leverUp");
		std::string lvdown("leverDown");
		std::string lvleft("leverLeft");
		std::string lvright("leverRight");
		std::string exit2("ExitButtonDevice2");
		std::string test2("TestButtonDevice2");
		std::string service2("ServiceButtonDevice2");
		std::string coin2("CoinButtonDevice2");
		std::string view2("ViewButtonDevice2");
		std::string three2("ThreeDimensionalButtonDevice2");
		std::string lvup2("leverUpDevice2");
		std::string lvdown2("leverDownDevice2");
		std::string lvleft2("leverLeftDevice2");
		std::string lvright2("leverRightDevice2");
		std::string dpdup(DpadUpChar);
		std::string dpddown(DpadDownChar);
		std::string dpdleft(DpadLeftChar);
		std::string dpdright(DpadRightChar);
		std::string dpdup2(DpadUpCharDevice2);
		std::string dpddown2(DpadDownCharDevice2);
		std::string dpdleft2(DpadLeftCharDevice2);
		std::string dpdright2(DpadRightCharDevice2);

		INT_PTR serviceread = Memory::Read<INT_PTR>(Memory::Address(Base::Module, (INT_PTR)gl_hjgtDll + 0x7D2B24));
		INT_PTR creditnumber = Memory::Read<INT_PTR>((INT_PTR)gl_hlibavs + 0x00042C10);
		INT_PTR creditnumber1 = Memory::Read<INT_PTR>((INT_PTR)creditnumber + 0x20);
		INT_PTR gearnumber = Memory::Read<INT_PTR>((INT_PTR)gl_hjgtDll + 0x00953F70);
		INT_PTR gearnumber1 = Memory::Read<INT_PTR>((INT_PTR)gearnumber + 0x5C);
		INT_PTR gearnumber2 = Memory::Read<INT_PTR>((INT_PTR)gearnumber1 + 0x390);
		INT_PTR gearnumber3 = Memory::Read<INT_PTR>((INT_PTR)gearnumber2 + 0x18);

		if (SequentialGears == 1)
		{
			Memory::WriteNop((INT_PTR)gl_hjgtDll + 0x95DDA, 3);
		}

		const int WHEEL_DEAD_ZONE = (SteeringDeadzone * 100);
		const int ACCL_DEAD_ZONE = (1 + PedalDeadzone * 100);
		const int BRAKE_DEAD_ZONE = (1 + PedalDeadzone * 100);
		const int SETUP_DEAD_ZONE = 20000;

		while (SDL_WaitEvent(&e) != 0)
		{
			INT_PTR menuvalue = Memory::Read<INT_PTR>((INT_PTR)gl_hjgtDll + 0x0094BFFC);
			INT_PTR menuvalue1 = Memory::Read<INT_PTR>((INT_PTR)menuvalue + 0x46C);

			if (e.type == SDL_JOYAXISMOTION && ShowAxisForSetup == 0)
			{
				if (e.jaxis.which == joystick_index1)
				{
					if (e.jaxis.axis == InputDeviceWheelSteeringAxis)
					{
						//Left of dead zone
						if (e.jaxis.value < -WHEEL_DEAD_ZONE)
						{
							e.jaxis.value = e.jaxis.value - WHEEL_DEAD_ZONE;
							Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B39, (128 + (e.jaxis.value + WHEEL_DEAD_ZONE) / 255));
						}
						//Right of dead zone
						else if (e.jaxis.value > WHEEL_DEAD_ZONE)
						{
							e.jaxis.value = e.jaxis.value + WHEEL_DEAD_ZONE;
							Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B39, (127 + (e.jaxis.value - WHEEL_DEAD_ZONE) / 255));
						}
						else if (e.jaxis.value < WHEEL_DEAD_ZONE && e.jaxis.value > -WHEEL_DEAD_ZONE)
						{
							Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B39, 0x7F);
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
									Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B3D, (UINT8)((-e.jaxis.value + ACCL_DEAD_ZONE) / 128.5));
								}
								else if (e.jaxis.value > ACCL_DEAD_ZONE)
								{
									e.jaxis.value = e.jaxis.value - ACCL_DEAD_ZONE;
									Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B41, (UINT8)((e.jaxis.value + ACCL_DEAD_ZONE) / 128));
								}
								else if (e.jaxis.value < ACCL_DEAD_ZONE && e.jaxis.value > -ACCL_DEAD_ZONE)
								{
									Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B41, 0x00);
									Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B3D, 0x00);
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
									Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B3D, 127 - e.jaxis.value);
								}
								else if (e.jaxis.value > ACCL_DEAD_ZONE)
								{
									e.jaxis.value = (e.jaxis.value / 255);
									Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B3D, 128 - e.jaxis.value);
								}
							}
							else if (e.jaxis.axis == InputDeviceWheelBrakeAxis)
							{
								if (e.jaxis.value < -BRAKE_DEAD_ZONE)
								{
									e.jaxis.value = (e.jaxis.value / 255);
									Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B41, 127 - e.jaxis.value);
								}
								else if (e.jaxis.value > BRAKE_DEAD_ZONE)
								{
									e.jaxis.value = (e.jaxis.value / 255);
									Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B41, 128 - e.jaxis.value);
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
									Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B3D, ((e.jaxis.value + ACCL_DEAD_ZONE) / 128));
								}
								else if (e.jaxis.value > ACCL_DEAD_ZONE)
								{
									e.jaxis.value = e.jaxis.value + ACCL_DEAD_ZONE;
									Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B41, ((-e.jaxis.value + ACCL_DEAD_ZONE) / 128));
								}
								else if (e.jaxis.value < ACCL_DEAD_ZONE && e.jaxis.value > -ACCL_DEAD_ZONE)
								{
									Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B41, 0xFF);
									Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B3D, 0xFF);
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
									Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B3D, 128 + e.jaxis.value);
								}
								else if (e.jaxis.value > ACCL_DEAD_ZONE)
								{
									e.jaxis.value = (e.jaxis.value / 255);
									Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B3D, 127 + e.jaxis.value);
								}
							}
							if (e.jaxis.axis == InputDeviceWheelBrakeAxis)
							{
								if (e.jaxis.value < -BRAKE_DEAD_ZONE)
								{
									e.jaxis.value = (e.jaxis.value / 255);
									Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B41, 128 + e.jaxis.value);
								}
								else if (e.jaxis.value > BRAKE_DEAD_ZONE)
								{
									e.jaxis.value = (e.jaxis.value / 255);
									Memory::Write<UINT8>((INT_PTR)gl_hjgtDll + 0x7D2B41, 127 + e.jaxis.value);
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
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x02);
							testbuttonA = false;
						}
						if (servicebuttonA)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x01);
							servicebuttonA = false;
						}
						if (viewbuttonA)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x400);
							viewbuttonA = false;
						}
						if (threedeebuttonA)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x800);
							threedeebuttonA = false;
						}						
						if (leverupA)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x1040);
							leverupA = false;
						}
						if (leverdownA)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x2080);
							leverdownA = false;
						}
						if (leverleftA)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x4000);
							leverleftA = false;
						}
						if (leverrightA)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x8000);
							leverrightA = false;
						}
					}
					else if (e.jhat.which == joystick_index2)
					{
						if (testbuttonA)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x02);
							testbuttonA = false;
						}
						if (servicebuttonA)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x01);
							servicebuttonA = false;
						}
						if (viewbuttonA)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x400);
							viewbuttonA = false;
						}
						if (threedeebuttonA)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x800);
							threedeebuttonA = false;
						}
						if (leverupA)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x1040);
							leverupA = false;
						}
						if (leverdownA)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x2080);
							leverdownA = false;
						}
						if (leverleftA)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x4000);
							leverleftA = false;
						}
						if (leverrightA)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x8000);
							leverrightA = false;
						}
					}
				}
				if (e.jhat.value == SDL_HAT_UP)
				{
					if (e.jhat.which == joystick_index1)
					{
						if (dpdup.compare(exit) == 0)
						{
							system("taskkill /f /im spice.exe");
						}
						if (dpdup.compare(test) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x02);
							testbuttonA = true;
						}
						if (dpdup.compare(service) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x01);
							servicebuttonA = true;
						}
						if (dpdup.compare(coin) == 0)
						{
							INT_PTR credit = Memory::Read<INT_PTR>((INT_PTR)gl_hlibavs + 0x00042C10);
							Memory::Write<INT_PTR>((INT_PTR)credit + 0x20, ++creditnumber1);
						}
						if (dpdup.compare(view) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x400);
							viewbuttonA = true;
						}
						if (dpdup.compare(three) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x800);
							threedeebuttonA = true;
						}
						if (dpdup.compare(lvup) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x1040);
							leverupA = true;
						}
						if (dpdup.compare(lvdown) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x2080);
							leverdownA = true;
						}
						if (dpdup.compare(lvleft) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x4000);
							leverleftA = true;
						}
						if (dpdup.compare(lvright) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x8000);
							leverrightA = true;
						}
					}
					else if (e.jhat.which == joystick_index2)
					{
						if (dpdup2.compare(exit2) == 0)
						{
							system("taskkill /f /im spice.exe");
						}
						if (dpdup2.compare(test2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x02);
							testbuttonA = true;
						}
						if (dpdup2.compare(service2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x01);
							servicebuttonA = true;
						}
						if (dpdup2.compare(coin2) == 0)
						{
							INT_PTR credit = Memory::Read<INT_PTR>((INT_PTR)gl_hlibavs + 0x00042C10);
							Memory::Write<INT_PTR>((INT_PTR)credit + 0x20, ++creditnumber1);
						}
						if (dpdup2.compare(view2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x400);
							viewbuttonA = true;
						}
						if (dpdup2.compare(three2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x800);
							threedeebuttonA = true;
						}
						if (dpdup2.compare(lvup2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x1040);
							leverupA = true;
						}
						if (dpdup2.compare(lvdown2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x2080);
							leverdownA = true;
						}
						if (dpdup2.compare(lvleft2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x4000);
							leverleftA = true;
						}
						if (dpdup2.compare(lvright2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x8000);
							leverrightA = true;
						}
					}
				}
				if (e.jhat.value == SDL_HAT_DOWN)
				{
					if (e.jhat.which == joystick_index1)
					{
						if (dpddown.compare(exit) == 0)
						{
							system("taskkill /f /im spice.exe");
						}
						if (dpddown.compare(test) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x02);
							testbuttonA = true;
						}
						if (dpddown.compare(service) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x01);
							servicebuttonA = true;
						}
						if (dpddown.compare(coin) == 0)
						{
							INT_PTR credit = Memory::Read<INT_PTR>((INT_PTR)gl_hlibavs + 0x00042C10);
							Memory::Write<INT_PTR>((INT_PTR)credit + 0x20, ++creditnumber1);
						}
						if (dpddown.compare(view) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x400);
							viewbuttonA = true;
						}
						if (dpddown.compare(three) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x800);
							threedeebuttonA = true;
						}
						if (dpddown.compare(lvup) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x1040);
							leverupA = true;
						}
						if (dpddown.compare(lvdown) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x2080);
							leverdownA = true;
						}
						if (dpddown.compare(lvleft) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x4000);
							leverleftA = true;
						}
						if (dpddown.compare(lvright) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x8000);
							leverrightA = true;
						}
					}
					else if (e.jhat.which == joystick_index2)
					{
						if (dpddown2.compare(exit2) == 0)
						{
							system("taskkill /f /im spice.exe");
						}
						if (dpdup2.compare(test2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x02);
							testbuttonA = true;
						}
						if (dpddown2.compare(service2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x01);
							servicebuttonA = true;
						}
						if (dpddown2.compare(coin2) == 0)
						{
							INT_PTR credit = Memory::Read<INT_PTR>((INT_PTR)gl_hlibavs + 0x00042C10);
							Memory::Write<INT_PTR>((INT_PTR)credit + 0x20, ++creditnumber1);
						}
						if (dpddown2.compare(view2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x400);
							viewbuttonA = true;
						}
						if (dpddown2.compare(three2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x800);
							threedeebuttonA = true;
						}
						if (dpddown2.compare(lvup2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x1040);
							leverupA = true;
						}
						if (dpddown2.compare(lvdown2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x2080);
							leverdownA = true;
						}
						if (dpddown2.compare(lvleft2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x4000);
							leverleftA = true;
						}
						if (dpddown2.compare(lvright2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x8000);
							leverrightA = true;
						}
					}
				}
				if (e.jhat.value == SDL_HAT_LEFT)
					{
					if (e.jhat.which == joystick_index1)
					{
						if (dpdleft.compare(exit) == 0)
						{
							system("taskkill /f /im spice.exe");
						}
						if (dpdleft.compare(test) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x02);
							testbuttonA = true;
						}
						if (dpdleft.compare(service) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x01);
							servicebuttonA = true;
						}
						if (dpdleft.compare(coin) == 0)
						{
							INT_PTR credit = Memory::Read<INT_PTR>((INT_PTR)gl_hlibavs + 0x00042C10);
							Memory::Write<INT_PTR>((INT_PTR)credit + 0x20, ++creditnumber1);
						}
						if (dpdleft.compare(view) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x400);
							viewbuttonA = true;
						}
						if (dpdleft.compare(three) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x800);
							threedeebuttonA = true;
						}
						if (dpdleft.compare(lvup) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x1040);
							leverupA = true;
						}
						if (dpdleft.compare(lvdown) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x2080);
							leverdownA = true;
						}
						if (dpdleft.compare(lvleft) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x4000);
							leverleftA = true;
						}
						if (dpdleft.compare(lvright) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x8000);
							leverrightA = true;
						}
					}
				else if (e.jhat.which == joystick_index2)
				{
						if (dpdleft2.compare(exit2) == 0)
						{
							system("taskkill /f /im spice.exe");
						}
						if (dpdleft2.compare(test2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x02);
							testbuttonA = true;
						}
						if (dpdleft2.compare(service2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x01);
							servicebuttonA = true;
						}
						if (dpdleft2.compare(coin2) == 0)
						{
							INT_PTR credit = Memory::Read<INT_PTR>((INT_PTR)gl_hlibavs + 0x00042C10);
							Memory::Write<INT_PTR>((INT_PTR)credit + 0x20, ++creditnumber1);
						}
						if (dpdleft2.compare(view2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x400);
							viewbuttonA = true;
						}
						if (dpdleft2.compare(three2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x800);
							threedeebuttonA = true;
						}
						if (dpdleft2.compare(lvup2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x1040);
							leverupA = true;
						}
						if (dpdleft2.compare(lvdown2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x2080);
							leverdownA = true;
						}
						if (dpdleft2.compare(lvleft2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x4000);
							leverleftA = true;
						}
						if (dpdleft2.compare(lvright2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x8000);
							leverrightA = true;
						}
					}
				}
				if (e.jhat.value == SDL_HAT_RIGHT)
				{
					if (e.jhat.which == joystick_index1)
					{
						if (dpdright.compare(exit) == 0)
						{
							system("taskkill /f /im spice.exe");
						}
						if (dpdright.compare(test) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x02);
							testbuttonA = true;
						}
						if (dpdright.compare(service) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x01);
							servicebuttonA = true;
						}
						if (dpdright.compare(coin) == 0)
						{
							INT_PTR credit = Memory::Read<INT_PTR>((INT_PTR)gl_hlibavs + 0x00042C10);
							Memory::Write<INT_PTR>((INT_PTR)credit + 0x20, ++creditnumber1);
						}
						if (dpdright.compare(view) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x400);
							viewbuttonA = true;
						}
						if (dpdright.compare(three) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x800);
							threedeebuttonA = true;
						}
						if (dpdright.compare(lvup) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x1040);
							leverupA = true;
						}
						if (dpdright.compare(lvdown) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x2080);
							leverdownA = true;
						}
						if (dpdright.compare(lvleft) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x4000);
							leverleftA = true;
						}
						if (dpdright.compare(lvright) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x8000);
							leverrightA = true;
						}
					}
					else if (e.jhat.which == joystick_index2)
					{
						if (dpdright2.compare(exit2) == 0)
						{
							system("taskkill /f /im spice.exe");
						}
						if (dpdright2.compare(test2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x02);
							testbuttonA = true;
						}
						if (dpdright2.compare(service2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x01);
							testbuttonA = true;
						}
						if (dpdright2.compare(coin2) == 0)
						{
							INT_PTR credit = Memory::Read<INT_PTR>((INT_PTR)gl_hlibavs + 0x00042C10);
							Memory::Write<INT_PTR>((INT_PTR)credit + 0x20, ++creditnumber1);
						}
						if (dpdright2.compare(view2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x400);
							viewbuttonA = true;
						}
						if (dpdright2.compare(three2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x800);
							threedeebuttonA = true;
						}
						if (dpdright2.compare(lvup2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x1040);
							leverupA = true;
						}
						if (dpdright2.compare(lvdown2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x2080);
							leverdownA = true;
						}
						if (dpdright2.compare(lvleft2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x4000);
							leverleftA = true;
						}
						if (dpdright2.compare(lvright2) == 0)
						{
							Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x8000);
							leverrightA = true;
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
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x02);
					}
					if (e.jbutton.button == ExitButton)
					{
						system("taskkill /f /im spice.exe");
					}
					if (e.jbutton.button == ServiceButton)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x01);
					}
					if (e.jbutton.button == CreditButton)
					{
						INT_PTR credit = Memory::Read<INT_PTR>((INT_PTR)gl_hlibavs + 0x00042C10);
						Memory::Write<INT_PTR>((INT_PTR)credit + 0x20, ++creditnumber1);
					}
					if (e.jbutton.button == ViewButton)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x400);
					}
					if (e.jbutton.button == ThreeDimensionalButton)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x800);
					}
					if ((e.jbutton.button == leverUp) && (SequentialGears == 0))
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x1040);
					}
					if ((e.jbutton.button == leverUp) && (SequentialGears == 1) && (menuvalue1 == 0x00))
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x1040);
					}
					if ((e.jbutton.button == leverUp) && (SequentialGears == 1) && (gearnumber3 < 0x06))
					{
						INT_PTR Writegearnumber = Memory::Read<INT_PTR>((INT_PTR)gl_hjgtDll + 0x00953F70);
						INT_PTR Writegearnumber1 = Memory::Read<INT_PTR>((INT_PTR)Writegearnumber + 0x5C);
						INT_PTR Writegearnumber2 = Memory::Read<INT_PTR>((INT_PTR)Writegearnumber1 + 0x390);
						Memory::Write<INT_PTR>((INT_PTR)Writegearnumber2 + 0x18, ++gearnumber3);
					}
					if ((e.jbutton.button == leverDown) && (SequentialGears == 0))
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x2080);
					}
					if ((e.jbutton.button == leverDown) && (SequentialGears == 1) && (menuvalue1 == 0x00))
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x2080);
					}
					if ((e.jbutton.button == leverDown) && (SequentialGears == 1) && (gearnumber3 > 0x01))
					{
						INT_PTR Writegearnumber = Memory::Read<INT_PTR>((INT_PTR)gl_hjgtDll + 0x00953F70);
						INT_PTR Writegearnumber1 = Memory::Read<INT_PTR>((INT_PTR)Writegearnumber + 0x5C);
						INT_PTR Writegearnumber2 = Memory::Read<INT_PTR>((INT_PTR)Writegearnumber1 + 0x390);
						Memory::Write<INT_PTR>((INT_PTR)Writegearnumber2 + 0x18, --gearnumber3);
					}
					if (e.jbutton.button == leverLeft)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x4000);
					}
					if (e.jbutton.button == leverRight)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x8000);
					}
					if (e.jbutton.button == Gear1Button)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x1040);
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x4000);
					}
					if (e.jbutton.button == Gear2Button)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x2080);
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x4000);
					}
					if (e.jbutton.button == Gear3Button)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x1040);
					}
					if (e.jbutton.button == Gear4Button)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x2080);
					}
					if (e.jbutton.button == Gear5Button)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x1040);
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x8000);
					}
					if (e.jbutton.button == Gear6Button)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x2080);
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x8000);
					}
				}
				else if (e.jbutton.which == joystick_index2)
				{
					if (e.jbutton.button == TestButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x02);
					}
					if (e.jbutton.button == ExitButtonDevice2)
					{
						system("taskkill /f /im spice.exe");
					}
					if (e.jbutton.button == ServiceButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x01);
					}
					if (e.jbutton.button == CreditButtonDevice2)
					{
						INT_PTR credit = Memory::Read<INT_PTR>((INT_PTR)gl_hlibavs + 0x00042C10);
						Memory::Write<INT_PTR>((INT_PTR)credit + 0x20, ++creditnumber1);
					}
					if (e.jbutton.button == ViewButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x400);
					}
					if (e.jbutton.button == ThreeDimensionalButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x800);
					}
					if ((e.jbutton.button == leverUpDevice2) && (SequentialGears == 0))
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x1040);
					}
					if ((e.jbutton.button == leverUpDevice2) && (SequentialGears == 1) && (menuvalue1 == 0x00))
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x1040);
					}
					if ((e.jbutton.button == leverUpDevice2) && (SequentialGears == 1) && (gearnumber3 < 0x06))
					{
						INT_PTR Writegearnumber = Memory::Read<INT_PTR>((INT_PTR)gl_hjgtDll + 0x00953F70);
						INT_PTR Writegearnumber1 = Memory::Read<INT_PTR>((INT_PTR)Writegearnumber + 0x5C);
						INT_PTR Writegearnumber2 = Memory::Read<INT_PTR>((INT_PTR)Writegearnumber1 + 0x390);
						Memory::Write<INT_PTR>((INT_PTR)Writegearnumber2 + 0x18, ++gearnumber3);
					}
					if (e.jbutton.button == leverDownDevice2 && SequentialGears == 0)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x2080);
					}
					if (e.jbutton.button == leverDownDevice2 && SequentialGears == 1 && menuvalue1 == 0x00)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x2080);
					}
					if (e.jbutton.button == leverDownDevice2 && SequentialGears == 1 && gearnumber3 > 0x01)
					{
						INT_PTR Writegearnumber = Memory::Read<INT_PTR>((INT_PTR)gl_hjgtDll + 0x00953F70);
						INT_PTR Writegearnumber1 = Memory::Read<INT_PTR>((INT_PTR)Writegearnumber + 0x5C);
						INT_PTR Writegearnumber2 = Memory::Read<INT_PTR>((INT_PTR)Writegearnumber1 + 0x390);
						Memory::Write<INT_PTR>((INT_PTR)Writegearnumber2 + 0x18, --gearnumber3);
					}
					if (e.jbutton.button == leverLeftDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x4000);
					}
					if (e.jbutton.button == leverRightDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x8000);
					}
					if (e.jbutton.button == Gear1ButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x1040);
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x4000);
					}
					if (e.jbutton.button == Gear2ButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x2080);
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x4000);
					}
					if (e.jbutton.button == Gear3ButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x1040);
					}
					if (e.jbutton.button == Gear4ButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x2080);
					}
					if (e.jbutton.button == Gear5ButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x1040);
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x8000);
					}
					if (e.jbutton.button == Gear6ButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x2080);
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread += 0x8000);
					}
				}
			}
			if (e.type == SDL_JOYBUTTONUP)
			{
				if (e.jbutton.which == joystick_index1)
				{
					if (e.jbutton.button == TestButton)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x02);
					}
					if (e.jbutton.button == ServiceButton)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x01);
					}
					if (e.jbutton.button == ViewButton)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x400);
					}
					if (e.jbutton.button == ThreeDimensionalButton)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x800);
					}
					if (e.jbutton.button == leverUp && SequentialGears == 0)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x1040);
					}
					if (e.jbutton.button == leverUp && SequentialGears == 1 && menuvalue1 == 0x00)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x1040);
					}
					if (e.jbutton.button == leverDown && SequentialGears == 0)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x2080);
					}
					if ((e.jbutton.button == leverDown) && (SequentialGears == 1) && (menuvalue1 == 0x00))
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x2080);
					}
					if (e.jbutton.button == leverLeft)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x4000);
					}
					if (e.jbutton.button == leverRight)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x8000);
					}
					if (e.jbutton.button == Gear1Button)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x1040);
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x4000);
					}
					if (e.jbutton.button == Gear2Button)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x2080);
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x4000);
					}
					if (e.jbutton.button == Gear3Button)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x1040);
					}
					if (e.jbutton.button == Gear4Button)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x2080);
					}
					if (e.jbutton.button == Gear5Button)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x1040);
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x8000);
					}
					if (e.jbutton.button == Gear6Button)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x2080);
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x8000);
					}
				}
				else if (e.jbutton.which == joystick_index2)
				{
					if (e.jbutton.button == TestButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x02);
					}
					if (e.jbutton.button == ServiceButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x01);
					}
					if (e.jbutton.button == ViewButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x400);
					}
					if (e.jbutton.button == ThreeDimensionalButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x800);
					}
					if ((e.jbutton.button == leverUpDevice2) && (SequentialGears == 0))
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x1040);
					}
					if ((e.jbutton.button == leverUpDevice2) && (SequentialGears == 1) && (menuvalue1 == 0x00))
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x1040);
					}
					if ((e.jbutton.button == leverDownDevice2) && (SequentialGears == 0))
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x2080);
					}
					if ((e.jbutton.button == leverDownDevice2) && (SequentialGears == 1) && (menuvalue1 == 0x00))
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x2080);
					}
					if (e.jbutton.button == leverLeftDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x4000);
					}
					if (e.jbutton.button == leverRightDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x8000);
					}
					if (e.jbutton.button == Gear1ButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x1040);
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x4000);
					}
					if (e.jbutton.button == Gear2ButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x2080);
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x4000);
					}
					if (e.jbutton.button == Gear3ButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x1040);
					}
					if (e.jbutton.button == Gear4ButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x2080);
					}
					if (e.jbutton.button == Gear5ButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x1040);
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x8000);
					}
					if (e.jbutton.button == Gear6ButtonDevice2)
					{
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x2080);
						Memory::Write<INT_PTR>((INT_PTR)gl_hjgtDll + 0x7D2B24, serviceread -= 0x8000);
					}
				}
			}
		}
	}
	else
	{
		if (!init)
		{
			CreateThread(NULL, 0, RunningLoop, this, 0, NULL);
			init = true;
		}

		while (SDL_WaitEvent(&e) != 0)
		{
		}
	}
}
