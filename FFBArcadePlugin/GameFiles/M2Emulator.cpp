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
#include <string.h>
#include "M2Emulator.h"
#include "../Core/ForceProfile.h"
#include "../Core/Config.h"

static ffb::Config& config = ffb::Config::Get();

using namespace ffb;


// Rom name of the game, for the keys of the persistent max force (Config/PersistentValues.cpp)
char* romnameM2;

const char* const M2Emulator::Titles[21] = {
	nullptr,
	"Sega Rally Championship",
	"Daytona USA",
	"Indianapolis 500 (Rev A, Deluxe)",
	"Sega Touring Car Championship (Rev A)",
	"Over Rev",
	"Super GT 24h",
	"Daytona USA '93 Edition",
	"Daytona USA (Saturn Ads)",
	"Daytona USA Special Edition",
	"Daytona USA Turbo",
	"Daytona USA Turbo (Rev A)",
	"Daytona USA: GTX 2004",
	"Daytona USA: To The Maxx",
	"Sega Rally Championship (Rev B)",
	"Sega Rally Pro Drivin'",
	"Indianapolis 500 (Rev A, Twin, Newer rev)",
	"Indianapolis 500 (Rev A, Twin, Older rev)",
	"Sega Touring Car Championship",
	"Sega Touring Car Championship (Rev B)",
	"Over Rev (Model 2B)",
};

void M2Emulator::OldInputs()
{
}

bool __stdcall M2Emulator::ExitHook(UINT exitCode)
{
	ExitProcess(0);
	return 0;
}

void M2Emulator::HookWithJump(INT_PTR at, void* to, int length)
{
	if (length < 5)
		return;

	DWORD curProtection;
	VirtualProtect((LPVOID)at, length, PAGE_EXECUTE_READWRITE, &curProtection);

	memset((void*)at, 0x90, length);

	INT_PTR relativeAddress = ((INT_PTR)to - at) - 5;

	*(BYTE*)at = 0xE9;
	*(DWORD*)(at + 1) = (DWORD)relativeAddress;

	DWORD temp;
	VirtualProtect((LPVOID)at, length, curProtection, &temp);
}

void M2Emulator::UpdateInputs()
{
	if (hWnd[1] > NULL || hWnd[14] > NULL || hWnd[15] > NULL)
	{
		if (InputDeviceWheelEnable)
			segaRallyInputs.Update();
	}

	if (hWnd[2] > NULL || hWnd[7] > NULL || hWnd[8] > NULL || hWnd[9] > NULL || hWnd[10] > NULL || hWnd[11] > NULL || hWnd[12] > NULL || hWnd[13] > NULL)
	{
		if (InputDeviceWheelEnable)
			daytonaInputs.Update();
	}

	if (hWnd[3] > NULL || hWnd[4] > NULL || hWnd[5] > NULL || hWnd[6] > NULL || hWnd[16] > NULL || hWnd[17] > NULL || hWnd[18] > NULL || hWnd[19] > NULL || hWnd[20] > NULL)
	{
		if (InputDeviceWheelEnable)
			indy500Inputs.Update();
	}
}

DWORD WINAPI M2Emulator::InputLoop(LPVOID game)
{
	while (true)
	{
		((M2Emulator*)game)->UpdateInputs();
		Sleep(16);
	}
}

void M2Emulator::Update()
{

	for (int i = 1; i <= 20; i++)
		hWnd[i] = FindWindowA(0, Titles[i]);

	romnameM2 = romnameM2Buffer;

	if (hWnd[1] > NULL || hWnd[2] > NULL || hWnd[3] > NULL || hWnd[4] > NULL || hWnd[5] > NULL || hWnd[6] > NULL || hWnd[7] > NULL || hWnd[8] > NULL || hWnd[9] > NULL || hWnd[10] > NULL ||
		hWnd[11] > NULL || hWnd[12] > NULL || hWnd[13] > NULL || hWnd[14] > NULL || hWnd[15] > NULL || hWnd[16] > NULL || hWnd[17] > NULL || hWnd[18] > NULL || hWnd[19] > NULL || hWnd[20] > NULL)
	{
		if (InputDeviceWheelEnable)
		{
			hookAddressM2A = 0x4CA450;
			hookAddressM2B = 0x4CB870;
			hookAddressM2C = 0x4C9080;
			Memory::WriteNop(Memory::Address(Base::Module, 0xCCA6B), 6);

			int hookLength = 6;

			if (hookAddressM2A)
			{
				HookWithJump(hookAddressM2A, OldInputs, hookLength);
			}

			if (hookAddressM2B)
			{
				HookWithJump(hookAddressM2B, OldInputs, hookLength);
			}

			if (hookAddressM2C)
			{
				HookWithJump(hookAddressM2C, OldInputs, hookLength);
			}
		}
	}

	for (int i = 1; i <= 20; i++)
	{
		if (hWnd[i] > NULL)
		{
			sprintf(romnameM2, "%s", Titles[i]);
			hWndM2 = hWnd[i];
			break;
		}
	}

	if (config.enableForceSpringEffect == 1)
	{
		wheel.KeptSpring(config.forceSpringStrength / 100.0);
	}

	ApplyDamper();

	if (!outputinit)
	{
		if (EnableOutputs)
		{
			HMODULE lib = LoadLibraryA("OutputBlaster.dll");
			outputinit = true;
		}		
	}

	if (!inputinit)
	{
		if (InputDeviceWheelEnable)
			CreateThread(NULL, 0, InputLoop, this, 0, NULL);
		inputinit = true;
	}

	HMODULE hMod = GetModuleHandleA("KERNEL32.dll");
	if (hMod)
	{
		if (!init)
		{
			int hookLength = 6;
			INT_PTR hookAddress = (INT_PTR)GetProcAddress(GetModuleHandle(L"KERNEL32.dll"), "ExitProcess");
			if (hookAddress)
			{
				HookWithJump(hookAddress, ExitHook, hookLength);
				init = true;
			}
		}
	}

	if ((hWnd[2] > NULL) || (hWnd[3] > NULL) || (hWnd[4] > NULL) || (hWnd[5] > NULL) || (hWnd[6] > NULL) || (hWnd[7] > NULL) || (hWnd[8] > NULL) || (hWnd[9] > NULL) || (hWnd[10] > NULL) || (hWnd[11] > NULL) || (hWnd[12] > NULL) || (hWnd[13] > NULL) ||(hWnd[16] > NULL) || (hWnd[17] > NULL) || (hWnd[18] > NULL) || (hWnd[19] > NULL) || (hWnd[20] > NULL))
	{
		std::string ffs = std::to_string(ff);
		Log::Write((char*)ffs.c_str());
		Log::Write("got value: ");

		UINT32 length_ms = 100;
		if ((ff > 0x09) && (ff < 0x18))
		{
			//Spring
			double percentForce = (ff - 15) / 8.0;
			wheel.Spring(percentForce);
		}

		if ((ff > 0x1F) && (ff < 0x28))
		{
			//Clutch
			double percentForce = (ff - 31) / 8.0;
			wheel.Friction(percentForce);
		}

		if ((ff > 0x2F) && (ff < 0x3D))
		{
			//Centering
			double percentForce = (ff - 47) / 13.0;
			wheel.Spring(percentForce);
		}

		if ((ff > 0x3F) && (ff < 0x48))
		{
			//Uncentering
			double percentForce = (ff - 63) / 8.0;
			wheel.Sine(40, 0, percentForce, length_ms);
			wheel.Rumble(percentForce, percentForce, length_ms);
		}

		if ((ff > 0x4F) && (ff < 0x58))
		{
			//Roll Left
			double percentForce = (ff - 79) / 8.0;
			wheel.Rumble(0, percentForce, length_ms);
			wheel.Constant(Direction::FromRight, percentForce);
		}
		else if ((ff > 0x5F) && (ff < 0x68))
		{
			//Roll Right
			double percentForce = (ff - 95) / 8.0;
			wheel.Rumble(percentForce, 0, length_ms);
			wheel.Constant(Direction::FromLeft, percentForce);
		}
	}

	if ((hWnd[1] > NULL) || (hWnd[14] > NULL) || (hWnd[15] > NULL))
	{
		if (!CustomStrengthInit)
		{
			CustomStrengthInit = true;
			ForceProfile::Apply(L"SRally", ForceProfile::Racing);
		}

		UINT8 ff1 = Memory::Read<UINT8>(Memory::Address(Base::Module, 0x174CF4)); //SegaRallyChampionship
		std::string ffs = std::to_string(ff1);
		Log::Write((char*)ffs.c_str());
		Log::Write("got value: ");

		if (OldFFB != ff1)
		{
			UINT32 length_ms = 100;
			if (ff1 >= 0xC0 && ff1 <= 0xDF)
			{
				Log::Write("moving wheel left");
				double percentForce = (ff1 - 191) / 32.0;

				wheel.Rumble(0, percentForce, length_ms);
				wheel.Constant(Direction::FromRight, percentForce);
			}
			
			if (ff1 >= 0x80 && ff1 <= 0x9F)
			{
				Log::Write("moving wheel right");
				double percentForce = (ff1 - 127) / 32.0;

				wheel.Rumble(percentForce, 0, length_ms);
				wheel.Constant(Direction::FromLeft, percentForce);
			}
		}
		OldFFB = ff1;
	}

	if ((hWnd[2] > NULL) || (hWnd[7] > NULL) || (hWnd[8] > NULL) || (hWnd[9] > NULL) || (hWnd[10] > NULL) || (hWnd[11] > NULL) || (hWnd[12] > NULL) || (hWnd[13] > NULL))
	{
		if (!CustomStrengthInit)
		{
			CustomStrengthInit = true;
			ForceProfile::Apply(L"Daytona", ForceProfile::Racing);
		}

		if (DaytonaForcePanoramicAttract == 1)
		{
			INT_PTR Rambase = Memory::Read<INT_PTR>(Memory::Address(Base::Module, 0x1AA888));
			INT_PTR Rambase1 = Memory::Read<INT_PTR>(Rambase + 0x100);
			Memory::Write<UINT8>(Rambase1 + 0x1070, 0x00);
		}

		if (DaytonaAIMultiplayerHack == 1)
		{
			INT_PTR Rambase = Memory::Read<INT_PTR>(Memory::Address(Base::Module, 0x1AA888));
			INT_PTR Rambase1 = Memory::Read<INT_PTR>(Rambase + 0x100);
			UINT8 gamestate = Memory::Read<UINT8>(Rambase1 + 0x10A4);
			UINT8 track = Memory::Read<UINT8>(Rambase1 + 0x1460);
			UINT8 trackselect = Memory::Read<UINT8>(Rambase1 + 0x1468);
			UINT8 countdown = Memory::Read<UINT8>(Rambase1 + 0x1850);
			UINT8 linksize = Memory::Read<UINT8>(Rambase1 + 0x40001);
			UINT8 linkID = Memory::Read<UINT8>(Rambase1 + 0x40002);
			INT_PTR gamestatetimer = Memory::Read<INT_PTR>(Rambase1 + 0x10A8);
			UINT8 camhack = Memory::Read<UINT8>(Rambase1 + 0x14A4);
			UINT8 carsinfront = Memory::Read<UINT8>(Rambase1 + 0x51EC);
			UINT8 mpposition = Memory::Read<UINT8>(Rambase1 + 0x51ED);
			float cary = Memory::Read<float>(Rambase1 + 0x519C);
			float carx = Memory::Read<float>(Rambase1 + 0x51A4);
			UINT8 nop1 = Memory::Read<UINT8>(Memory::Address(Base::Module, 0xC2130));
			UINT8 nop2 = Memory::Read<UINT8>(Memory::Address(Base::Module, 0xC2131));
			UINT8 nop3 = Memory::Read<UINT8>(Memory::Address(Base::Module, 0xC2132));


			if (gamestate == 0x1A)
			{
				Memory::Write<UINT8>(Rambase1 + 0x51ED, carsinfront);
			}

			if (trackselect == 0x01)
			{
				Memory::Write<UINT8>(Rambase1 + 0x1460, 0x02);
			}
			else if (trackselect == 0x02)
			{
				Memory::Write<UINT8>(Rambase1 + 0x1460, 0x01);
			}

			if (linksize == 0x02)
			{
				if (linkID == 0x02)
				{
					if (gamestate > 0x07)
					{
						Memory::Write<INT_PTR>(Rambase1 + 0x40004, 0x00540C80);
						Memory::Write<INT_PTR>(Rambase1 + 0x40008, 0x00540C80);
						Memory::Write<INT_PTR>(Rambase1 + 0x4000C, 0x00540C40);
						Memory::Write<UINT8>(Rambase1 + 0x5484, 0x01);
						Memory::Write<UINT8>(Rambase1 + 0x5784, 0x02);
						Memory::Write<UINT8>(Rambase1 + 0x5A84, 0x03);
						Memory::Write<UINT8>(Rambase1 + 0x5D84, 0x04);
						Memory::Write<UINT8>(Rambase1 + 0x6084, 0x05);
						Memory::Write<UINT8>(Rambase1 + 0x6384, 0x06);
						Memory::Write<UINT8>(Rambase1 + 0x6684, 0x07);
					}
					if ((gamestate == 0x16) && (countdown == 0x00) && (track == 0x00))
					{
						Sleep(150);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Memory::Write<UINT8>(Rambase1 + 0x1850, 0x01);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x02 || track == 0x01))
					{
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Memory::Write<UINT8>(Rambase1 + 0x1468, 0x01);
					}
				}
				else if (linkID == 0x01)
				{
					if ((gamestate == 0x1A) && (gamestatetimer < 100))
					{
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2130), 0x88);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2131), 0x14);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2132), 0x01);
					}
					else if ((gamestate == 0x1A) && (gamestatetimer > 100))
					{
						Sleep(100);
						Memory::WriteNop(Memory::Address(Base::Module, 0xC2130), 3);
					}

					if ((gamestate > 0x1A) && (gamestate < 0x1A))
					{
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2130), 0x88);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2131), 0x14);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2132), 0x01);
					}

					if (gamestate > 0x07)
					{
						Memory::Write<INT_PTR>(Rambase1 + 0x40004, 0x00540C80);
						Memory::Write<INT_PTR>(Rambase1 + 0x40008, 0x00540C40);
						Memory::Write<INT_PTR>(Rambase1 + 0x4000C, 0x00540C40);
						Memory::Write<UINT8>(Rambase1 + 0x5484, 0x00);
						Memory::Write<UINT8>(Rambase1 + 0x5784, 0x02);
						Memory::Write<UINT8>(Rambase1 + 0x5A84, 0x03);
						Memory::Write<UINT8>(Rambase1 + 0x5D84, 0x04);
						Memory::Write<UINT8>(Rambase1 + 0x6084, 0x05);
						Memory::Write<UINT8>(Rambase1 + 0x6384, 0x06);
						Memory::Write<UINT8>(Rambase1 + 0x6684, 0x07);
					}
					if ((gamestatetimer > 4294964133) && (gamestate == 0x12))
					{
						Memory::Write<INT_PTR>(Rambase1 + 0x10A8, 0x3B6);
					}
					if ((gamestate == 0x16) && (countdown == 0x00) && (track == 0x00))
					{
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Memory::Write<UINT8>(Rambase1 + 0x1850, 0x01);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x02 || track == 0x01))
					{
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Sleep(50);
						Memory::Write<float>(Rambase1 + 0x519C, 0.0);
						Memory::Write<UINT8>(Rambase1 + 0x1468, 0x01);
					}
				}
			}
			else if (linksize == 0x03)
			{
				if (linkID == 0x03)
				{
					if (gamestate > 0x07)
					{
						Memory::Write<INT_PTR>(Rambase1 + 0x40004, 0x00540CC0);
						Memory::Write<INT_PTR>(Rambase1 + 0x40008, 0x00540CC0);
						Memory::Write<INT_PTR>(Rambase1 + 0x4000C, 0x00540CC0);
						Memory::Write<UINT8>(Rambase1 + 0x5484, 0x01);
						Memory::Write<UINT8>(Rambase1 + 0x5784, 0x02);
						Memory::Write<UINT8>(Rambase1 + 0x5A84, 0x03);
						Memory::Write<UINT8>(Rambase1 + 0x5D84, 0x04);
						Memory::Write<UINT8>(Rambase1 + 0x6084, 0x05);
						Memory::Write<UINT8>(Rambase1 + 0x6384, 0x06);
						Memory::Write<UINT8>(Rambase1 + 0x6684, 0x07);
					}
					if ((gamestate == 0x16) && (countdown == 0x00) && (track == 0x00))
					{
						Sleep(250);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Memory::Write<UINT8>(Rambase1 + 0x1850, 0x01);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x02 || track == 0x01))
					{
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Memory::Write<UINT8>(Rambase1 + 0x1468, 0x01);
						Memory::Write<float>(Rambase1 + 0x519C, 450.0);
					}
					if ((gamestatetimer > 4294964133) && (gamestate == 0x12))
					{
						Memory::Write<INT_PTR>(Rambase1 + 0x10A8, 0x3B6);
					}
				}
				else if (linkID == 0x02)
				{
					if ((gamestate == 0x1A) && (gamestatetimer < 100))
					{
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2130), 0x88);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2131), 0x14);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2132), 0x01);
					}
					else if ((gamestate == 0x1A) && (gamestatetimer > 100))
					{
						Sleep(100);
						Memory::WriteNop(Memory::Address(Base::Module, 0xC2130), 3);
					}

					if ((gamestate > 0x1A) && (gamestate < 0x1A))
					{
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2130), 0x88);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2131), 0x14);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2132), 0x01);
					}
					if (gamestate > 0x07)
					{
						Memory::Write<INT_PTR>(Rambase1 + 0x40004, 0x00540CC0);
						Memory::Write<INT_PTR>(Rambase1 + 0x40008, 0x00540C40);
						Memory::Write<INT_PTR>(Rambase1 + 0x4000C, 0x00540C40);
						Memory::Write<UINT8>(Rambase1 + 0x5484, 0x01);
						Memory::Write<UINT8>(Rambase1 + 0x5784, 0x00);
						Memory::Write<UINT8>(Rambase1 + 0x5A84, 0x03);
						Memory::Write<UINT8>(Rambase1 + 0x5D84, 0x04);
						Memory::Write<UINT8>(Rambase1 + 0x6084, 0x05);
						Memory::Write<UINT8>(Rambase1 + 0x6384, 0x06);
						Memory::Write<UINT8>(Rambase1 + 0x6684, 0x07);
					}
					if ((gamestate == 0x16) && (countdown == 0x00) && (track == 0x00))
					{
						Sleep(150);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Memory::Write<UINT8>(Rambase1 + 0x1850, 0x01);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x02 || track == 0x01))
					{
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Memory::Write<UINT8>(Rambase1 + 0x1468, 0x01);
					}
					if ((gamestatetimer > 4294964133) && (gamestate == 0x12))
					{
						Memory::Write<INT_PTR>(Rambase1 + 0x10A8, 0x3B6);
					}
				}
				else if (linkID == 0x01)
				{
					if ((gamestate == 0x1A) && (gamestatetimer < 100))
					{
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2130), 0x88);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2131), 0x14);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2132), 0x01);
					}
					else if ((gamestate == 0x1A) && (gamestatetimer > 100))
					{
						Sleep(100);
						Memory::WriteNop(Memory::Address(Base::Module, 0xC2130), 3);
					}

					if ((gamestate > 0x1A) && (gamestate < 0x1A))
					{
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2130), 0x88);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2131), 0x14);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2132), 0x01);
					}

					if (gamestate > 0x07)
					{
						Memory::Write<INT_PTR>(Rambase1 + 0x40004, 0x00540CC0);
						Memory::Write<INT_PTR>(Rambase1 + 0x40008, 0x00540C80);
						Memory::Write<INT_PTR>(Rambase1 + 0x4000C, 0x00540C80);
						Memory::Write<UINT8>(Rambase1 + 0x5484, 0x00);
						Memory::Write<UINT8>(Rambase1 + 0x5784, 0x02);
						Memory::Write<UINT8>(Rambase1 + 0x5A84, 0x03);
						Memory::Write<UINT8>(Rambase1 + 0x5D84, 0x04);
						Memory::Write<UINT8>(Rambase1 + 0x6084, 0x05);
						Memory::Write<UINT8>(Rambase1 + 0x6384, 0x06);
						Memory::Write<UINT8>(Rambase1 + 0x6684, 0x07);
					}
					if ((gamestatetimer > 4294964133) && (gamestate == 0x12))
					{
						Memory::Write<INT_PTR>(Rambase1 + 0x10A8, 0x3B6);
					}
					if ((gamestate == 0x16) && (countdown == 0x00) && (track == 0x00))
					{
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Memory::Write<UINT8>(Rambase1 + 0x1850, 0x01);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x01))
					{
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Sleep(50);
						Memory::Write<INT_PTR>(Rambase1 + 0x51A4, 0xC2F195E8);
						Memory::Write<UINT8>(Rambase1 + 0x1468, 0x01);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x02))
					{
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Sleep(50);
						Memory::Write<INT_PTR>(Rambase1 + 0x51A4, 0xC37E0001);
						Memory::Write<UINT8>(Rambase1 + 0x1468, 0x01);
					}
				}
			}
			else if (linksize == 0x04)
			{
				if (linkID == 0x04)
				{
					if (gamestate > 0x07)
					{
						Memory::Write<INT_PTR>(Rambase1 + 0x40004, 0x00540D00);
						Memory::Write<INT_PTR>(Rambase1 + 0x40008, 0x00540D00);
						Memory::Write<INT_PTR>(Rambase1 + 0x4000C, 0x00540D00);
						Memory::Write<UINT8>(Rambase1 + 0x5484, 0x01);
						Memory::Write<UINT8>(Rambase1 + 0x5784, 0x02);
						Memory::Write<UINT8>(Rambase1 + 0x5A84, 0x03);
						Memory::Write<UINT8>(Rambase1 + 0x5D84, 0x04);
						Memory::Write<UINT8>(Rambase1 + 0x6084, 0x05);
						Memory::Write<UINT8>(Rambase1 + 0x6384, 0x06);
						Memory::Write<UINT8>(Rambase1 + 0x6684, 0x07);
					}
					if ((gamestate == 0x16) && (countdown == 0x00) && (track == 0x00))
					{
						Sleep(250);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Memory::Write<UINT8>(Rambase1 + 0x1850, 0x01);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x01))
					{
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Sleep(50);
						Memory::Write<INT_PTR>(Rambase1 + 0x51A4, 0xC302CAF4);
						Memory::Write<UINT8>(Rambase1 + 0x1468, 0x01);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x02))
					{
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Sleep(50);
						Memory::Write<float>(Rambase1 + 0x519C, 450.0);
						Memory::Write<INT_PTR>(Rambase1 + 0x51A4, 0xC37C0001);
						Memory::Write<UINT8>(Rambase1 + 0x1468, 0x01);
					}
					if ((gamestatetimer > 4294964133) && (gamestate == 0x12))
					{
						Memory::Write<INT_PTR>(Rambase1 + 0x10A8, 0x3B6);
					}
				}
				if (linkID == 0x03)
				{
					if ((gamestate == 0x1A) && (gamestatetimer < 100))
					{
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2130), 0x88);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2131), 0x14);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2132), 0x01);
					}
					else if ((gamestate == 0x1A) && (gamestatetimer > 100))
					{
						Sleep(100);
						Memory::WriteNop(Memory::Address(Base::Module, 0xC2130), 3);
					}

					if ((gamestate > 0x1A) && (gamestate < 0x1A))
					{
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2130), 0x88);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2131), 0x14);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2132), 0x01);
					}

					if (gamestate > 0x07)
					{
						Memory::Write<INT_PTR>(Rambase1 + 0x40004, 0x00540D00);
						Memory::Write<INT_PTR>(Rambase1 + 0x40008, 0x00540C40);
						Memory::Write<INT_PTR>(Rambase1 + 0x4000C, 0x00540C40);
						Memory::Write<UINT8>(Rambase1 + 0x5484, 0x01);
						Memory::Write<UINT8>(Rambase1 + 0x5784, 0x02);
						Memory::Write<UINT8>(Rambase1 + 0x5A84, 0x00);
						Memory::Write<UINT8>(Rambase1 + 0x5D84, 0x04);
						Memory::Write<UINT8>(Rambase1 + 0x6084, 0x05);
						Memory::Write<UINT8>(Rambase1 + 0x6384, 0x06);
						Memory::Write<UINT8>(Rambase1 + 0x6684, 0x07);
					}
					if ((gamestate == 0x16) && (countdown == 0x00) && (track == 0x00))
					{
						Sleep(350);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Memory::Write<UINT8>(Rambase1 + 0x1850, 0x01);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x02))
					{
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Memory::Write<UINT8>(Rambase1 + 0x1468, 0x01);
						Memory::Write<float>(Rambase1 + 0x519C, 450.0);
						Sleep(50);
						Memory::Write<INT_PTR>(Rambase1 + 0x51A4, 0xC37CF3D0);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x01))
					{
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Memory::Write<UINT8>(Rambase1 + 0x1468, 0x01);
						Memory::Write<float>(Rambase1 + 0x519C, 450.0);
					}
					if ((gamestatetimer > 4294964133) && (gamestate == 0x12))
					{
						Memory::Write<INT_PTR>(Rambase1 + 0x10A8, 0x3B6);
					}
				}
				else if (linkID == 0x02)
				{
					if ((gamestate == 0x1A) && (gamestatetimer < 100))
					{
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2130), 0x88);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2131), 0x14);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2132), 0x01);
					}
					else if ((gamestate == 0x1A) && (gamestatetimer > 100))
					{
						Sleep(100);
						Memory::WriteNop(Memory::Address(Base::Module, 0xC2130), 3);
					}

					if ((gamestate > 0x1A) && (gamestate < 0x1A))
					{
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2130), 0x88);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2131), 0x14);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2132), 0x01);
					}
					if (gamestate > 0x07)
					{
						Memory::Write<INT_PTR>(Rambase1 + 0x40004, 0x00540D00);
						Memory::Write<INT_PTR>(Rambase1 + 0x40008, 0x00540C80);
						Memory::Write<INT_PTR>(Rambase1 + 0x4000C, 0x00540C80);
						Memory::Write<UINT8>(Rambase1 + 0x5484, 0x01);
						Memory::Write<UINT8>(Rambase1 + 0x5784, 0x00);
						Memory::Write<UINT8>(Rambase1 + 0x5A84, 0x03);
						Memory::Write<UINT8>(Rambase1 + 0x5D84, 0x04);
						Memory::Write<UINT8>(Rambase1 + 0x6084, 0x05);
						Memory::Write<UINT8>(Rambase1 + 0x6384, 0x06);
						Memory::Write<UINT8>(Rambase1 + 0x6684, 0x07);
					}
					if ((gamestate == 0x16) && (countdown == 0x00) && (track == 0x00))
					{
						Sleep(150);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Memory::Write<UINT8>(Rambase1 + 0x1850, 0x01);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x02 || track == 0x01))
					{
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Memory::Write<UINT8>(Rambase1 + 0x1468, 0x01);
					}
					if ((gamestatetimer > 4294964133) && (gamestate == 0x12))
					{
						Memory::Write<INT_PTR>(Rambase1 + 0x10A8, 0x3B6);
					}
				}
				else if (linkID == 0x01)
				{
					if ((gamestate == 0x1A) && (gamestatetimer < 100))
					{
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2130), 0x88);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2131), 0x14);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2132), 0x01);
					}
					else if ((gamestate == 0x1A) && (gamestatetimer > 100))
					{
						Sleep(100);
						Memory::WriteNop(Memory::Address(Base::Module, 0xC2130), 3);
					}

					if ((gamestate > 0x1A) && (gamestate < 0x1A))
					{
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2130), 0x88);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2131), 0x14);
						Memory::Write<UINT8>(Memory::Address(Base::Module, 0xC2132), 0x01);
					}

					if (gamestate > 0x07)
					{
						Memory::Write<INT_PTR>(Rambase1 + 0x40004, 0x00540D00);
						Memory::Write<INT_PTR>(Rambase1 + 0x40008, 0x00540CC0);
						Memory::Write<INT_PTR>(Rambase1 + 0x4000C, 0x00540CC0);
						Memory::Write<UINT8>(Rambase1 + 0x5484, 0x00);
						Memory::Write<UINT8>(Rambase1 + 0x5784, 0x02);
						Memory::Write<UINT8>(Rambase1 + 0x5A84, 0x03);
						Memory::Write<UINT8>(Rambase1 + 0x5D84, 0x04);
						Memory::Write<UINT8>(Rambase1 + 0x6084, 0x05);
						Memory::Write<UINT8>(Rambase1 + 0x6384, 0x06);
						Memory::Write<UINT8>(Rambase1 + 0x6684, 0x07);
					}
					if ((gamestatetimer > 4294964133) && (gamestate == 0x12))
					{
						Memory::Write<INT_PTR>(Rambase1 + 0x10A8, 0x3B6);
					}
					if ((gamestate == 0x16) && (countdown == 0x00) && (track == 0x00))
					{
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Memory::Write<UINT8>(Rambase1 + 0x1850, 0x01);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x01))
					{
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Sleep(50);
						Memory::Write<INT_PTR>(Rambase1 + 0x51A4, 0xC2F195E8);
						Memory::Write<UINT8>(Rambase1 + 0x1468, 0x01);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x02))
					{
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x99); // change to 99
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x10A4, 0x13); // restart race
						Sleep(50);
						Memory::Write<UINT8>(Rambase1 + 0x40000, 0x01); // change back to 1
						Sleep(50);
						Memory::Write<INT_PTR>(Rambase1 + 0x51A4, 0xC37E0001);
						Memory::Write<UINT8>(Rambase1 + 0x1468, 0x01);
					}
				}
			}
		}

		ff = Memory::Read<UINT8>(0x0057285B); //DaytonaUSA		
	}

	if ((hWnd[3] > NULL) || (hWnd[16] > NULL) || (hWnd[17] > NULL))
	{
		if (!CustomStrengthInit)
		{
			CustomStrengthInit = true;
			ForceProfile::Apply(L"Indy500", ForceProfile::Racing);
		}

		ff = Memory::Read<UINT8>(Memory::Address(Base::Module, 0x17285B)); //Indy500
	}	

	if ((hWnd[4] > NULL) || (hWnd[18] > NULL) || (hWnd[19] > NULL))
	{
		if (!CustomStrengthInit)
		{
			CustomStrengthInit = true;
			ForceProfile::Apply(L"STCC", ForceProfile::Racing);
		}

		ff = Memory::Read<UINT8>(Memory::Address(Base::Module, 0x17285B)); //Sega Touring Car Championship
	}		

	if ((hWnd[5] > NULL) || (hWnd[20] > NULL))
	{
		if (!CustomStrengthInit)
		{
			CustomStrengthInit = true;
			ForceProfile::Apply(L"OverRev", ForceProfile::Racing);
		}

		ff = Memory::Read<UINT8>(Memory::Address(Base::Module, 0x17285B)); //OverRev
	}	

	if (hWnd[6] > NULL)
	{
		if (!CustomStrengthInit)
		{
			CustomStrengthInit = true;
			ForceProfile::Apply(L"SuperGT", ForceProfile::Racing);
		}

		ff = Memory::Read<UINT8>(Memory::Address(Base::Module, 0x17285B)); //Super GT 24h
	}		
}
