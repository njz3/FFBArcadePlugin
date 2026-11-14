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

#include <string.h>
#include <tchar.h>
#include "Demul.h"
#include "../Common Files/CRCCheck.h"
#include "../Core/ForceProfile.h"

using namespace ffb;

extern bool WaitForGame;
extern int AutoCloseWindowError;
extern int EnableForceSpringEffect;
extern int ForceSpringStrength;

// Name of the game, for the keys of the persistent max force (Config/PersistentValues.cpp)
char* romnameDemul;

// force value of NASCAR to force: 1 to 16 one way, 17 to 32 the other, 0 if not a force
static int nascar(int ffnas) {
	switch (ffnas) {

	case 0x04:
		return 32;
	case 0x84:
		return 31;
	case 0x44:
		return 30;
	case 0xC4:
		return 29;
	case 0x24:
		return 28;
	case 0xA4:
		return 27;
	case 0x64:
		return 26;
	case 0xE4:
		return 25;
	case 0x14:
		return 24;
	case 0x94:
		return 23;
	case 0x54:
		return 22;
	case 0xD4:
		return 21;
	case 0x34:
		return 20;
	case 0xB4:
		return 19;
	case 0x74:
		return 18;
	case 0xF4:
		return 17;

	case 0xFC:
		return 16;
	case 0x7C:
		return 15;
	case 0xBC:
		return 14;
	case 0x3C:
		return 13;
	case 0xDC:
		return 12;
	case 0x5C:
		return 11;
	case 0x9C:
		return 10;
	case 0x1C:
		return 9;
	case 0xEC:
		return 8;
	case 0x6C:
		return 7;
	case 0xAC:
		return 6;
	case 0x2C:
		return 5;
	case 0xCC:
		return 4;
	case 0x4C:
		return 3;
	case 0x8C:
		return 2;
	case 0x0C:
		return 1;
	default:
		return 0;
	}
}

static int fasterspeed(int ffspeed) {
	switch (ffspeed) {
	case 0x1F:
		return 22;
	case 0x1D:
		return 21;
	case 0x1B:
		return 20;
	case 0x19:
		return 19;
	case 0x17:
		return 18;
	case 0x15:
		return 17;
	case 0x13:
		return 16;
	case 0x11:
		return 15;
	case 0x0F:
		return 14;
	case 0x0D:
		return 13;
	case 0x0B:
		return 12;

	case 0x1E:
		return 11;
	case 0x1C:
		return 10;
	case 0x1A:
		return 9;
	case 0x18:
		return 8;
	case 0x16:
		return 7;
	case 0x14:
		return 6;
	case 0x12:
		return 5;
	case 0x10:
		return 4;
	case 0x0E:
		return 3;
	case 0x0C:
		return 2;
	case 0x0A:
		return 1;
	default:
		return 0;
	}
}

static BOOL CALLBACK FindWindowBySubstr(HWND hwnd, LPARAM substring)
{
	const DWORD TITLE_SIZE = 1024;
	TCHAR windowTitle[TITLE_SIZE];

	if (GetWindowText(hwnd, windowTitle, TITLE_SIZE))
	{
		if (_tcsstr(windowTitle, LPCTSTR(substring)) != NULL)
		{
			return false;
		}
	}
	return true;
}

const TCHAR substring[] = TEXT("FPS");
const TCHAR substring0[] = TEXT("spg");
const TCHAR substring1[] = TEXT("NASCAR");
const TCHAR substring2[] = TEXT("Initial D Arcade Stage");
const TCHAR substring3[] = TEXT("Smashing Drive");
const TCHAR substring4[] = TEXT("Maximum Speed");
const TCHAR substring5[] = TEXT("Faster Than Speed");
const TCHAR substring6[] = TEXT("ATV Track");

int Demul::InitialDFFBLoop()
{
	DWORD FFB = Memory::Read<int>(FFBAddress);

	BYTE* ffb = reinterpret_cast<BYTE*>(&FFB);

	if (KickStartWait)
	{
		driveBoard.Play(ffb[0], ffb[1], ffb[2]);
	}
	return 0;
}

int Demul::SmashingDriveFFBLoop()
{
	INT_PTR FFBSmashingDrive = Memory::Read<INT_PTR>(FFBAddress);
	UINT32 length_ms = 100;
	if ((FFBSmashingDrive > 0x01) && (FFBSmashingDrive < 0x100))
	{
		double percentForce = FFBSmashingDrive / 255.0;
		wheel.Rumble(percentForce, 0, length_ms);
		wheel.Constant(Direction::FromLeft, percentForce);
	}
	else if ((FFBSmashingDrive > 0x1FF) && (FFBSmashingDrive < 0xFF01))
	{
		double percentForce = (FFBSmashingDrive / 65280.0);
		wheel.Rumble(0, percentForce, length_ms);
		wheel.Constant(Direction::FromRight, percentForce);
	}
	return 0;
}

int Demul::ATVTrackFFBLoop()
{
	float FFBATVTrack = Memory::Read<float>(FFBAddress);
	UINT32 length_ms = 100;
	if (FFBATVTrack > 0)
	{
		double percentForce = FFBATVTrack;
		wheel.Rumble(percentForce, 0, length_ms);
		wheel.Constant(Direction::FromLeft, percentForce);
	}
	else if (FFBATVTrack < 0)
	{
		double percentForce = -FFBATVTrack;
		wheel.Rumble(0, percentForce, length_ms);
		wheel.Constant(Direction::FromRight, percentForce);
	}
	return 0;
}

int Demul::FasterThanSpeedFFBLoop()
{
	UINT8 ffbfaster = Memory::Read<UINT8>(FFBAddress);
	fffaster = fasterspeed(ffbfaster);
	UINT32 length_ms = 100;
	if ((fffaster > 0x0B) && (fffaster < 0x17))
	{
		double percentForce = (fffaster - 11) / 11.0;
		wheel.Rumble(percentForce, 0, length_ms);
		wheel.Constant(Direction::FromLeft, percentForce);
	}
	else if ((fffaster > 0x00) && (fffaster < 0x0C))
	{
		double percentForce = fffaster / 11.0;
		wheel.Rumble(0, percentForce, length_ms);
		wheel.Constant(Direction::FromRight, percentForce);
	}
	return 0;
}

int Demul::MaximumSpeedFFBLoop()
{
	UINT8 FFBMaximumSpeed = Memory::Read<UINT8>(FFBAddress);
	UINT8 FFBMaximumSpeed2 = Memory::Read<UINT8>(FFBAddress + 0x01);
	UINT32 length_ms = 100;
	if ((FFBMaximumSpeed > 0x7F) && (FFBMaximumSpeed < 0x100) && (FFBMaximumSpeed2 == 0xFF))
	{
		double percentForce = (256 - FFBMaximumSpeed) / 128.0;
		wheel.Rumble(percentForce, 0, length_ms);
		wheel.Constant(Direction::FromLeft, percentForce);
	}
	else if ((FFBMaximumSpeed > 0x00) && (FFBMaximumSpeed < 0x81) && (FFBMaximumSpeed2 == 0x00))
	{
		double percentForce = FFBMaximumSpeed / 128.0;
		wheel.Rumble(0, percentForce, length_ms);
		wheel.Constant(Direction::FromRight, percentForce);
	}
	return 0;
}

int Demul::NascarFFBLoop()
{
	UINT8 ffnas = Memory::Read<UINT8>(FFBAddress);
	ffnascar = nascar(ffnas);
	UINT32 length_ms = 100;
	if ((ffnascar > 0x10) && (ffnascar < 0x21))
	{
		double percentForce = (ffnascar - 16) / 16.0;
		wheel.Rumble(percentForce, 0, length_ms);
		wheel.Constant(Direction::FromLeft, percentForce);
	}
	else if ((ffnascar > 0x00) && (ffnascar < 0x11))
	{
		double percentForce = (17 - ffnascar) / 16.0;
		wheel.Rumble(0, percentForce, length_ms);
		wheel.Constant(Direction::FromRight, percentForce);
	}
	return 0;
}

DWORD WINAPI Demul::NascarRunningLoop(LPVOID game)
{
	while (true)
	{
		((Demul*)game)->NascarFFBLoop();
		Sleep(16);
	}
}

DWORD WINAPI Demul::InitialDRunningLoop(LPVOID game)
{
	while (true)
	{
		((Demul*)game)->InitialDFFBLoop();
		Sleep(16);
	}
}

DWORD WINAPI Demul::SmashingDriveRunningLoop(LPVOID game)
{
	while (true)
	{
		((Demul*)game)->SmashingDriveFFBLoop();
		Sleep(16);
	}
}

DWORD WINAPI Demul::ATVTrackRunningLoop(LPVOID game)
{
	while (true)
	{
		((Demul*)game)->ATVTrackFFBLoop();
		Sleep(16);
	}
}

DWORD WINAPI Demul::FasterThanSpeedRunningLoop(LPVOID game)
{
	while (true)
	{
		((Demul*)game)->FasterThanSpeedFFBLoop();
		Sleep(16);
	}
}

DWORD WINAPI Demul::MaximumSpeedRunningLoop(LPVOID game)
{
	while (true)
	{
		((Demul*)game)->MaximumSpeedFFBLoop();
		Sleep(16);
	}
}

void Demul::Update()
{


	if (AutoCloseWindowError == 1)
	{
		if (!VolumeMute)
		{
			//Remove window error popup
			hWnd = FindWindowA(0, ("padDemul"));
			if (hWnd > NULL)
			{
				// posted, rather than sent by a new thread at each loop until the popup was gone
				PostMessage(hWnd, WM_CLOSE, NULL, NULL);
				VolumeMute = true;
			}
		}
	}

	if (!CRCinit)
	{
		if (InputDeviceWheelEnable == 1)
		{
			static char newCrc[0x400];
			// Craft CRC detection without virtual address
			memcpy(newCrc, GetModuleHandle(nullptr), 0x400);
			DWORD pePTR = *(DWORD*)(newCrc + 0x3C);

			// Overwrite ImageBase with 8 bytes of 0
			*(DWORD*)(newCrc + pePTR + 0x18) = 0x00000000;
			*(DWORD*)(newCrc + pePTR + 0x18 + 4) = 0x00000000;
#ifdef _AMD64_
			* (DWORD*)(newCrc + pePTR + 50) = 0x00000000;
#endif
			* (DWORD*)(newCrc + pePTR + 54) = 0x00000000;
			uint32_t newCrcResult = GetCRC32(newCrc, 0x400);
			switch (newCrcResult)
			{
			case 0x875a800d:
				Demul180428 = true;
				break;
			case 0x04f85ceb:
				Demul111117 = true;
				break;
			}

			if ((Demul180428) || (Demul111117))
			{
				CRCinit = true;
			}
		}
	}

	if (NOPinit)
	{
		gl_padDemul = LoadLibraryA("padDemul.dll");

		if (Demul180428)
		{
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x27013, 3);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x26F31, 3);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x271F7, 3);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x276BD, 4);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x272FF, 3);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x26FBD, 3);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x27685, 4);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x276A2, 4);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x26FEC, 3);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x270A3, 3);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x26FE1, 3);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x27048, 4);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x2703A, 4);
		}

		if (Demul111117)
		{
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x26B31, 3);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x26DD0, 3);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x272AD, 4);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x26B9C, 3);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x27275, 4);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x27292, 4);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x26BC2, 3);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x26BBA, 3);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x26BE3, 3);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x26C1E, 4);
			Memory::WriteNop((INT_PTR)gl_padDemul + 0x26C10, 4);
		}

		if (NascarRunning)
		{
			Memory::Write<UINT8>(inputAddresses.steering, 0x80);
			Memory::Write<UINT8>(inputAddresses.accelerator, 0x00);
			Memory::Write<UINT8>(inputAddresses.brake, 0x00);
			Memory::Write<UINT8>(inputAddresses.shiftUpDown, 0x28);
			nascarInputs.Update();
		}

		if (InitialDRunning)
		{
			Memory::Write<UINT8>(inputAddresses.steering, 0x80);
			Memory::Write<UINT8>(inputAddresses.accelerator, 0x00);
			Memory::Write<UINT8>(inputAddresses.brake, 0x00);
			initialDInputs.Update();
		}

		if (SmashingDriveRunning)
		{
			Memory::Write<UINT8>(inputAddresses.steering, 0x80);
			smashingDriveInputs.Update();
		}

		if (ATVTrackRunning)
		{
			Memory::Write<UINT8>(inputAddresses.steering, 0x80);
			Memory::Write<UINT8>(inputAddresses.accelerator, 0x00);
			atvTrackInputs.Update();
		}

		if (FasterThanSpeedRunning)
		{
			Memory::Write<UINT8>(inputAddresses.steering, 0x80);
			Memory::Write<UINT8>(inputAddresses.accelerator, 0x00);
			Memory::Write<UINT8>(inputAddresses.brake, 0x00);
			fasterThanSpeedInputs.Update();
		}

		if (MaximumSpeedRunning)
		{
			Memory::Write<UINT8>(inputAddresses.steering, 0x80);
			Memory::Write<UINT8>(inputAddresses.accelerator, 0x00);
			Memory::Write<UINT8>(inputAddresses.brake, 0x00);
			maximumSpeedInputs.Update();
		}
	}
		
	if (EnableForceSpringEffect == 1)
	{
		wheel.KeptSpring(ForceSpringStrength / 100.0);
	}

	if (!WindowSearch)
	{
		if (!EnumWindows(FindWindowBySubstr, (LPARAM)substring) || !EnumWindows(FindWindowBySubstr, (LPARAM)substring0))
		{
			if (!EnumWindows(FindWindowBySubstr, (LPARAM)substring1))
			{
				ForceProfile::Apply(L"NascarRacing", ForceProfile::Racing);

				romnameDemul = "Nascar";

				WaitForGame = true;
				NascarRunning = true;
				WindowSearch = true;	
			}

			if (!EnumWindows(FindWindowBySubstr, (LPARAM)substring2))
			{
				ForceProfile::Apply(L"InitialDDemul", ForceProfile::Racing | ForceProfile::Doubles);

				romnameDemul = "Initial D Arcade Stage";

				WaitForGame = true;
				InitialDRunning = true;
				WindowSearch = true;	
			}

			if (!EnumWindows(FindWindowBySubstr, (LPARAM)substring3))
			{
				ForceProfile::Apply(L"SmashingDrive", ForceProfile::Racing);

				romnameDemul = "Smashing Drive";

				WaitForGame = true;
				SmashingDriveRunning = true;
				WindowSearch = true;	
			}

			if (!EnumWindows(FindWindowBySubstr, (LPARAM)substring4))
			{
				ForceProfile::Apply(L"MaximumSpeed", ForceProfile::Racing);

				romnameDemul = "Maximum Speed";

				WaitForGame = true;
				MaximumSpeedRunning = true;
				WindowSearch = true;
			}

			if (!EnumWindows(FindWindowBySubstr, (LPARAM)substring5))
			{
				ForceProfile::Apply(L"FasterSpeed", ForceProfile::Racing);

				romnameDemul = "Faster Than Speed";

				WaitForGame = true;
				FasterThanSpeedRunning = true;
				WindowSearch = true;
			}

			if (!EnumWindows(FindWindowBySubstr, (LPARAM)substring6))
			{
				ForceProfile::Apply(L"ATVTrack", ForceProfile::Racing);

				romnameDemul = "ATV Track";

				WaitForGame = true;
				ATVTrackRunning = true;
				WindowSearch = true;
			}
		}
	}

	if (NascarRunning)
	{
		if (!FFBGameInit)
		{
			Sleep(5000);
			aAddy2 = Memory::FindPattern("\x13\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x50\x72\x6F\x64\x75\x63\x65\x64\x20\x42\x79\x20", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", aAddy2);
			FFBAddress = (INT_PTR)aAddy2 - 0x2F0;
			CreateThread(NULL, 0, NascarRunningLoop, this, 0, NULL);
			FFBGameInit = true;
		}
		else
		{
			if (InputDeviceWheelEnable == 1)
			{
				if (!InputFind)
				{
					aAddy = Memory::FindPattern("\x11\x00\x16\x1F\x05\x04\x1D\x0D\x07\x12\x18\x14\x02\x13\x09\x15\x0E\x19\x01\x1C\x08\x1A\x17\x03\x0F\x1B\x00\x10\x0A\x0B\x0C\x06\x01", "x?xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", aAddy2);
					UINT8 CheckaAddy = Memory::Read<UINT8>((INT_PTR)aAddy + 0x01);
					if (CheckaAddy == 0x1E)
					{
						inputAddresses.steering = (INT_PTR)aAddy - 0xC45;
						inputAddresses.accelerator = (INT_PTR)aAddy - 0xC0D;
						inputAddresses.brake = (INT_PTR)aAddy - 0xBD5;
						inputAddresses.shiftUpDown = (INT_PTR)aAddy - 0xDEA;
						inputAddresses.serviceTest = (INT_PTR)aAddy - 0x108A;
						inputAddresses.startView = (INT_PTR)aAddy - 0xF8E;
						inputAddresses.coin = (INT_PTR)aAddy - 0xFE2;
						NOPinit = true;
						InputFind = true;
					}
				}
			}
		}	
	}

	if (InitialDRunning)
	{
		if (!FFBGameInit)
		{
			Sleep(6000);
			aAddy2 = Memory::FindPattern("\x88\xA9\x00\x09", "xxxx", aAddy2);
			FFBAddress = (INT_PTR)aAddy2 + 0x01;
			FFBGameInit = true;
		}

		if (!KickStartWait)
		{
			if (FFBAddress > 0)
			{
				Sleep(6000);
				CreateThread(NULL, 0, InitialDRunningLoop, this, 0, NULL);
				KickStartWait = true;
			}
		}
		else
		{ 
			if (InputDeviceWheelEnable == 1)
			{
				if (!InputFind)
				{
					aAddy = Memory::FindPattern("\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\xFF\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x20", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", aAddy2);
					UINT8 CheckaAddy = Memory::Read<UINT8>((INT_PTR)aAddy - 0x2A0);
					if (CheckaAddy == 0x01)
					{
						inputAddresses.steering = (INT_PTR)aAddy - 0x85;
						inputAddresses.accelerator = (INT_PTR)aAddy - 0x4D;
						inputAddresses.brake = (INT_PTR)aAddy - 0x15;
						inputAddresses.startView = (INT_PTR)aAddy - 0x14A;
						inputAddresses.shiftUpDown = (INT_PTR)aAddy - 0xDA;
						inputAddresses.serviceTest = (INT_PTR)aAddy - 0x246;
						inputAddresses.coin = (INT_PTR)aAddy - 0x19E;
						inputAddresses.card = (INT_PTR)aAddy + 0x3E;
						inputAddresses.card2 = (INT_PTR)aAddy + 0x22;
						NOPinit = true;
						InputFind = true;
					}
				}
			}
		}
	}
		
	if (SmashingDriveRunning)
	{
		if (!FFBGameInit)
		{	
			Sleep(5000);
			aAddy2 = Memory::FindPattern("\xC0\xF3\x51\x00\x60\x9A\x2C\x01", "xxxxxxxx", aAddy2);
			
			UINT8 CheckAddy = Memory::Read<UINT8>((INT_PTR)aAddy2 - 0x0E);
			if (CheckAddy == 0x07)
			{
				FFBAddress = (INT_PTR)aAddy2 - 0xD0;
				CreateThread(NULL, 0, SmashingDriveRunningLoop, this, 0, NULL);
				FFBGameInit = true;
			}
		}
		else
		{
			if (InputDeviceWheelEnable == 1)
			{
				if (!InputFind)
				{
					aAddy = Memory::FindPattern("\x0C\x00\x0E\x0F\x08\x09\x0A\x0B\x1C\x1D\x1E\x1F\x18\x19\x1A\x1B\x04\x05\x06\x07\x00\x01\x02\x03\x14\x15\x16\x17\x10\x11", "x?xxxxxxxxxxxxxxxxxxxxxxxxxxxx", aAddy2);
					UINT8 CheckaAddy = Memory::Read<UINT8>((INT_PTR)aAddy + 0x01);
					if (CheckaAddy == 0x0D)
					{
						inputAddresses.steering = (INT_PTR)aAddy - 0x80A;
						inputAddresses.serviceTest = (INT_PTR)aAddy - 0x95A;
						inputAddresses.accelerator = (INT_PTR)aAddy - 0x896;
						NOPinit = true;
						InputFind = true;
					}
				}
			}
		}
	}

	if (MaximumSpeedRunning)
	{
		if (!FFBGameInit)
		{
			Sleep(10000);
			aAddy2 = Memory::FindPattern("\x48\x03\x00\x00\xE0\x01\x00\x00\x2C\x01\x00\x00\xB4\x00\x00\x00\x00\x00\x00\x00\x20\xFE\xFF\xFF", "xxxxxxxxxxxxxxxxxxxxxxxx", aAddy2);

			UINT8 CheckAddy = Memory::Read<UINT8>((INT_PTR)aAddy2 + 0x21);
			if (CheckAddy == 0xFC)
			{
				FFBAddress = (INT_PTR)aAddy2 + 0x18;
				CreateThread(NULL, 0, MaximumSpeedRunningLoop, this, 0, NULL);
				FFBGameInit = true;
			}
		}
		else
		{
			if (InputDeviceWheelEnable == 1)
			{
				if (!InputFind)
				{
					aAddy = Memory::FindPattern("\x14\x00\x05\x10\x0C\x18\x19\x04\x0B\x11\x0D\x1C\x1D\x02\x1F\x08\x06\x17\x0A\x0E\x16\x1A\x07\x15\x0F\x13\x01\x12\x03\x1E\x09", "x?xxxxxxxxxxxxxxxxxxxxxxxxxxxxx", aAddy2);
					UINT8 CheckaAddy = Memory::Read<UINT8>((INT_PTR)aAddy + 0x01);
					if (CheckaAddy == 0x1B)
					{
						inputAddresses.steering = (INT_PTR)aAddy - 0x1316;
						inputAddresses.accelerator = (INT_PTR)aAddy - 0x12DE;
						inputAddresses.brake = (INT_PTR)aAddy - 0x12A6;
						inputAddresses.shiftUpDown = (INT_PTR)aAddy - 0x14F2;
						inputAddresses.serviceTest = (INT_PTR)aAddy - 0x14F1;
						inputAddresses.coin = (INT_PTR)aAddy - 0x136A;
						NOPinit = true;
						InputFind = true;
					}
				}
			}
		}
	}

	if (FasterThanSpeedRunning)
	{
		if (!FFBGameInit)
		{
			Sleep(10000);
			aAddy2 = Memory::FindPattern("\x01\x00\x10\x00\x00\x00\x00\x00\x00\x2F\x76", "xxxxxxxxxxx", aAddy2);

			UINT8 CheckAddy = Memory::Read<UINT8>((INT_PTR)aAddy2 + 0x0E);
			if (CheckAddy == 0x0C)
			{
				FFBAddress = (INT_PTR)aAddy2 + 0x0F;
				CreateThread(NULL, 0, FasterThanSpeedRunningLoop, this, 0, NULL);
				FFBGameInit = true;
			}
		}
		else
		{
			if (InputDeviceWheelEnable == 1)
			{
				if (!InputFind)
				{
					aAddy = Memory::FindPattern("\x14\x00\x05\x10\x0C\x18\x19\x04\x0B\x11\x0D\x1C\x1D\x02\x1F\x08\x06\x17\x0A\x0E\x16\x1A\x07\x15\x0F\x13\x01\x12\x03\x1E\x09", "x?xxxxxxxxxxxxxxxxxxxxxxxxxxxxx", aAddy2);
					UINT8 CheckaAddy = Memory::Read<UINT8>((INT_PTR)aAddy + 0x01);
					if (CheckaAddy == 0x1B)
					{
						inputAddresses.steering = (INT_PTR)aAddy - 0x1316;
						inputAddresses.accelerator = (INT_PTR)aAddy - 0x12DE;
						inputAddresses.brake = (INT_PTR)aAddy - 0x12A6;
						inputAddresses.shiftUpDown = (INT_PTR)aAddy - 0x14F2;
						inputAddresses.serviceTest = (INT_PTR)aAddy - 0x14F1;
						inputAddresses.coin = (INT_PTR)aAddy - 0x136A;
						NOPinit = true;
						InputFind = true;
					}
				}
			}
		}	
	}

	if (ATVTrackRunning)
	{
		if (!FFBGameInit)
		{
			Sleep(15000);
			aAddy2 = Memory::FindPattern("\x49\x55\x4C\x00\x00\x00\x05", "xxxxxxx", aAddy2);
			UINT8 CheckAddy = Memory::Read<UINT8>((INT_PTR)aAddy2 - 0x04);
			if (CheckAddy == 0x1E)
			{
				FFBAddress = (INT_PTR)aAddy2 - 0x1A;
				CreateThread(NULL, 0, ATVTrackRunningLoop, this, 0, NULL);
				FFBGameInit = true;
			}
		}
		else
		{ 
			if (InputDeviceWheelEnable == 1)
			{
				if (!InputFind)
				{
					aAddy = Memory::FindPattern("\x11\x00\x16\x1F\x05\x04\x1D\x0D\x07\x12\x18\x14\x02\x13\x09\x15\x0E\x19\x01\x1C\x08\x1A\x17\x03\x0F\x1B\x00\x10\x0A\x0B\x0C\x06\x01", "x?xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", aAddy2);
					UINT8 CheckaAddy = Memory::Read<UINT8>((INT_PTR)aAddy + 0x01);
					if (CheckaAddy == 0x1E)
					{
						inputAddresses.steering = (INT_PTR)aAddy - 0xA16;
						inputAddresses.accelerator = (INT_PTR)aAddy - 0x9DE;
						inputAddresses.brake = (INT_PTR)aAddy - 0xABE;
						inputAddresses.serviceTest = (INT_PTR)aAddy - 0xB82;
						NOPinit = true;
						InputFind = true;
					}
				}
			}
		}
	}
}
