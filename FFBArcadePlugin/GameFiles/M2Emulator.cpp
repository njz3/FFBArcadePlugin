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
#include <tchar.h>
#include "M2Emulator.h"
#include "math.h"
#include "SDL.h"

extern void M2EmulatorDaytonaUSAInputsEnabled(Helpers* helpers);
extern void M2EmulatorSegaRallyInputsEnabled(Helpers* helpers);
extern void M2EmulatorIndy500InputsEnabled(Helpers* helpers);

//M2 Emulator Games
std::string SegaRallyChampionship("Sega Rally Championship");
std::string SegaRallyChampionshipRevB("Sega Rally Championship (Rev B)");
std::string SegaRallyProDrivin("Sega Rally Pro Drivin'");
std::string DaytonaUSA("Daytona USA");
std::string DaytonaUSA93Edition("Daytona USA '93 Edition");
std::string DaytonaUSASaturnAds("Daytona USA (Saturn Ads)");
std::string DaytonaUSASpecialEdition("Daytona USA Special Edition");
std::string DaytonaUSATurbo("Daytona USA Turbo");
std::string DaytonaUSATurboRevA("Daytona USA Turbo (Rev A)");
std::string DaytonaUSAGTX2004("Daytona USA: GTX 2004");
std::string DaytonaUSAToTheMaxx("Daytona USA: To The Maxx");
std::string Indianapolis500RevADeluxe("Indianapolis 500 (Rev A, Deluxe)");
std::string Indianapolis500RevATwinNewerrev("Indianapolis 500 (Rev A, Twin, Newer rev)");
std::string Indianapolis500RevATwinOlderrev("Indianapolis 500 (Rev A, Twin, Older rev)");
std::string OverRev("Over Rev");
std::string OverRevModel2B("Over Rev (Model 2B)");
std::string SuperGT24h("Super GT 24h");
std::string SegaTouringCarChampionship("Sega Touring Car Championship");
std::string SegaTouringCarChampionshipRevA("Sega Touring Car Championship (Rev A)");
std::string SegaTouringCarChampionshipRevB("Sega Touring Car Championship (Rev B)");

//Config Settings
extern int configFeedbackLength;
extern int configMinForce;
extern int configMaxForce;
extern int PowerMode;
extern int configFeedbackLength;
extern int configAlternativeMinForceLeft;
extern int configAlternativeMaxForceLeft;
extern int configAlternativeMinForceRight;
extern int configAlternativeMaxForceRight;
extern int EnableForceSpringEffect;
extern int ForceSpringStrength;
extern int EnableDamper;
extern int DamperStrength;
static INT_PTR hookAddressM2A;
static INT_PTR hookAddressM2B;
static INT_PTR hookAddressM2C;
static UINT8 OldFFB;

static int InputDeviceWheelEnable = GetSettingInt(TEXT("InputDeviceWheelEnable"), 0);
static int DaytonaAIMultiplayerHack = GetSettingInt(TEXT("DaytonaAIMultiplayerHack"), 0);
static int DaytonaForcePanoramicAttract = GetSettingInt(TEXT("DaytonaForcePanoramicAttract"), 0);
static int EnableOutputs = GetSettingInt(TEXT("EnableOutputs"), 0);

static int configMinForceDaytona = GetSettingInt(TEXT("MinForceDaytona"), 0);
static int configMaxForceDaytona = GetSettingInt(TEXT("MaxForceDaytona"), 100);
static int configAlternativeMinForceLeftDaytona = GetSettingInt(TEXT("AlternativeMinForceLeftDaytona"), 0);
static int configAlternativeMaxForceLeftDaytona = GetSettingInt(TEXT("AlternativeMaxForceLeftDaytona"), 100);
static int configAlternativeMinForceRightDaytona = GetSettingInt(TEXT("AlternativeMinForceRightDaytona"), 0);
static int configAlternativeMaxForceRightDaytona = GetSettingInt(TEXT("AlternativeMaxForceRightDaytona"), 100);
static int configFeedbackLengthDaytona = GetSettingInt(TEXT("FeedbackLengthDaytona"), 120);
static int PowerModeDaytona = GetSettingInt(TEXT("PowerModeDaytona"), 0);
static int EnableForceSpringEffectDaytona = GetSettingInt(TEXT("EnableForceSpringEffectDaytona"), 0);
static int ForceSpringStrengthDaytona = GetSettingInt(TEXT("ForceSpringStrengthDaytona"), 0);
static int EnableDamperDaytona = GetSettingInt(TEXT("EnableDamperDaytona"), 0);
static int DamperStrengthDaytona = GetSettingInt(TEXT("DamperStrengthDaytona"), 100);

static int configMinForceSRally = GetSettingInt(TEXT("MinForceSRally"), 0);
static int configMaxForceSRally = GetSettingInt(TEXT("MaxForceSRally"), 100);
static int configAlternativeMinForceLeftSRally = GetSettingInt(TEXT("AlternativeMinForceLeftSRally"), 0);
static int configAlternativeMaxForceLeftSRally = GetSettingInt(TEXT("AlternativeMaxForceLeftSRally"), 100);
static int configAlternativeMinForceRightSRally = GetSettingInt(TEXT("AlternativeMinForceRightSRally"), 0);
static int configAlternativeMaxForceRightSRally = GetSettingInt(TEXT("AlternativeMaxForceRightSRally"), 100);
static int configFeedbackLengthSRally = GetSettingInt(TEXT("FeedbackLengthSRally"), 120);
static int PowerModeSRally = GetSettingInt(TEXT("PowerModeSRally"), 0);
static int EnableForceSpringEffectSRally = GetSettingInt(TEXT("EnableForceSpringEffectSRally"), 0);
static int ForceSpringStrengthSRally = GetSettingInt(TEXT("ForceSpringStrengthSRally"), 0);
static int EnableDamperSRally = GetSettingInt(TEXT("EnableDamperSRally"), 0);
static int DamperStrengthSRally = GetSettingInt(TEXT("DamperStrengthSRally"), 100);

static int configMinForceIndy500 = GetSettingInt(TEXT("MinForceIndy500"), 0);
static int configMaxForceIndy500 = GetSettingInt(TEXT("MaxForceIndy500"), 100);
static int configAlternativeMinForceLeftIndy500 = GetSettingInt(TEXT("AlternativeMinForceLeftIndy500"), 0);
static int configAlternativeMaxForceLeftIndy500 = GetSettingInt(TEXT("AlternativeMaxForceLeftIndy500"), 100);
static int configAlternativeMinForceRightIndy500 = GetSettingInt(TEXT("AlternativeMinForceRightIndy500"), 0);
static int configAlternativeMaxForceRightIndy500 = GetSettingInt(TEXT("AlternativeMaxForceRightIndy500"), 100);
static int configFeedbackLengthIndy500 = GetSettingInt(TEXT("FeedbackLengthIndy500"), 120);
static int PowerModeIndy500 = GetSettingInt(TEXT("PowerModeIndy500"), 0);
static int EnableForceSpringEffectIndy500 = GetSettingInt(TEXT("EnableForceSpringEffectIndy500"), 0);
static int ForceSpringStrengthIndy500 = GetSettingInt(TEXT("ForceSpringStrengthIndy500"), 0);
static int EnableDamperIndy500 = GetSettingInt(TEXT("EnableDamperIndy500"), 0);
static int DamperStrengthIndy500 = GetSettingInt(TEXT("DamperStrengthIndy500"), 100);

static int configMinForceSTCC = GetSettingInt(TEXT("MinForceSTCC"), 0);
static int configMaxForceSTCC = GetSettingInt(TEXT("MaxForceSTCC"), 100);
static int configAlternativeMinForceLeftSTCC = GetSettingInt(TEXT("AlternativeMinForceLeftSTCC"), 0);
static int configAlternativeMaxForceLeftSTCC = GetSettingInt(TEXT("AlternativeMaxForceLeftSTCC"), 100);
static int configAlternativeMinForceRightSTCC = GetSettingInt(TEXT("AlternativeMinForceRightSTCC"), 0);
static int configAlternativeMaxForceRightSTCC = GetSettingInt(TEXT("AlternativeMaxForceRightSTCC"), 100);
static int configFeedbackLengthSTCC = GetSettingInt(TEXT("FeedbackLengthSTCC"), 120);
static int PowerModeSTCC = GetSettingInt(TEXT("PowerModeSTCC"), 0);
static int EnableForceSpringEffectSTCC = GetSettingInt(TEXT("EnableForceSpringEffectSTCC"), 0);
static int ForceSpringStrengthSTCC = GetSettingInt(TEXT("ForceSpringStrengthSTCC"), 0);
static int EnableDamperSTCC = GetSettingInt(TEXT("EnableDamperSTCC"), 0);
static int DamperStrengthSTCC = GetSettingInt(TEXT("DamperStrengthSTCC"), 100);

static int configMinForceOverRev = GetSettingInt(TEXT("MinForceOverRev"), 0);
static int configMaxForceOverRev = GetSettingInt(TEXT("MaxForceOverRev"), 100);
static int configAlternativeMinForceLeftOverRev = GetSettingInt(TEXT("AlternativeMinForceLeftOverRev"), 0);
static int configAlternativeMaxForceLeftOverRev = GetSettingInt(TEXT("AlternativeMaxForceLeftOverRev"), 100);
static int configAlternativeMinForceRightOverRev = GetSettingInt(TEXT("AlternativeMinForceRightOverRev"), 0);
static int configAlternativeMaxForceRightOverRev = GetSettingInt(TEXT("AlternativeMaxForceRightOverRev"), 100);
static int configFeedbackLengthOverRev = GetSettingInt(TEXT("FeedbackLengthOverRev"), 120);
static int PowerModeOverRev = GetSettingInt(TEXT("PowerModeOverRev"), 0);
static int EnableForceSpringEffectOverRev = GetSettingInt(TEXT("EnableForceSpringEffectOverRev"), 0);
static int ForceSpringStrengthOverRev = GetSettingInt(TEXT("ForceSpringStrengthOverRev"), 0);
static int EnableDamperOverRev = GetSettingInt(TEXT("EnableDamperOverRev"), 0);
static int DamperStrengthOverRev = GetSettingInt(TEXT("DamperStrengthOverRev"), 100);

static int configMinForceSuperGT = GetSettingInt(TEXT("MinForceSuperGT"), 0);
static int configMaxForceSuperGT = GetSettingInt(TEXT("MaxForceSuperGT"), 100);
static int configAlternativeMinForceLeftSuperGT = GetSettingInt(TEXT("AlternativeMinForceLeftSuperGT"), 0);
static int configAlternativeMaxForceLeftSuperGT = GetSettingInt(TEXT("AlternativeMaxForceLeftSuperGT"), 100);
static int configAlternativeMinForceRightSuperGT = GetSettingInt(TEXT("AlternativeMinForceRightSuperGT"), 0);
static int configAlternativeMaxForceRightSuperGT = GetSettingInt(TEXT("AlternativeMaxForceRightSuperGT"), 100);
static int configFeedbackLengthSuperGT = GetSettingInt(TEXT("FeedbackLengthSuperGT"), 120);
static int PowerModeSuperGT = GetSettingInt(TEXT("PowerModeSuperGT"), 0);
static int EnableForceSpringEffectSuperGT = GetSettingInt(TEXT("EnableForceSpringEffectSuperGT"), 0);
static int ForceSpringStrengthSuperGT = GetSettingInt(TEXT("ForceSpringStrengthSuperGT"), 0);
static int EnableDamperSuperGT = GetSettingInt(TEXT("EnableDamperSuperGT"), 0);
static int DamperStrengthSuperGT = GetSettingInt(TEXT("DamperStrengthSuperGT"), 100);

static HWND hWnd1;
static HWND hWnd2;
static HWND hWnd3;
static HWND hWnd4;
static HWND hWnd5;
static HWND hWnd6;
static HWND hWnd7;
static HWND hWnd8;
static HWND hWnd9;
static HWND hWnd10;
static HWND hWnd11;
static HWND hWnd12;
static HWND hWnd13;
static HWND hWnd14;
static HWND hWnd15;
static HWND hWnd16;
static HWND hWnd17;
static HWND hWnd18;
static HWND hWnd19;
static HWND hWnd20;
HWND hWndM2;

static bool init = false;
static bool inputinit = false;
static bool CustomStrengthInit = false;
static bool outputinit = false;

static UINT8 ff;

static void OldInputs(){}

static bool __stdcall ExitHook(UINT uExitCode)
{
	ExitProcess(0);
	return 0;
}

static bool Hook(void * toHook, void * ourFunct, int len) {
	if (len < 5) {
		return false;
	}

	DWORD curProtection;
	VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);

	memset(toHook, 0x90, len);

	INT_PTR relativeAddress = ((INT_PTR)ourFunct - (INT_PTR)toHook) - 5;

	*(BYTE*)toHook = 0xE9;
	*(DWORD*)((INT_PTR)toHook + 1) = (DWORD)relativeAddress;

	DWORD temp;
	VirtualProtect(toHook, len, curProtection, &temp);

	return true;
}

static INT_PTR jmpBackAddy;

char* romnameM2;

static int ThreadLoop()
{
	if (hWnd1 > NULL || hWnd14 > NULL || hWnd15 > NULL)
	{
		if (InputDeviceWheelEnable)
			M2EmulatorSegaRallyInputsEnabled(0);
	}
	
	if (hWnd2 > NULL || hWnd7 > NULL || hWnd8 > NULL || hWnd9 > NULL || hWnd10 > NULL || hWnd11 > NULL || hWnd12 > NULL || hWnd13 > NULL)
	{
		if (InputDeviceWheelEnable)
			M2EmulatorDaytonaUSAInputsEnabled(0);
	}

	if (hWnd3 > NULL || hWnd4 > NULL || hWnd5 > NULL || hWnd6 > NULL || hWnd16 > NULL || hWnd17 > NULL || hWnd18 > NULL || hWnd19 > NULL || hWnd20 > NULL)
	{
		if (InputDeviceWheelEnable)
			M2EmulatorIndy500InputsEnabled(0);
	}
	return 0;
}

static DWORD WINAPI InputLoop(LPVOID lpParam)
{
	while (true)
	{
		ThreadLoop();
		Sleep(16);
	}
}

void M2Emulator::FFBLoop(EffectConstants * constants, Helpers * helpers, EffectTriggers * triggers) {

	hWnd1 = FindWindowA(0, ("Sega Rally Championship"));
	hWnd2 = FindWindowA(0, ("Daytona USA"));
	hWnd3 = FindWindowA(0, ("Indianapolis 500 (Rev A, Deluxe)"));
	hWnd4 = FindWindowA(0, ("Sega Touring Car Championship (Rev A)"));
	hWnd5 = FindWindowA(0, ("Over Rev"));
	hWnd6 = FindWindowA(0, ("Super GT 24h"));
	hWnd7 = FindWindowA(0, ("Daytona USA '93 Edition"));
	hWnd8 = FindWindowA(0, ("Daytona USA (Saturn Ads)"));
	hWnd9 = FindWindowA(0, ("Daytona USA Special Edition"));
	hWnd10 = FindWindowA(0, ("Daytona USA Turbo"));
	hWnd11 = FindWindowA(0, ("Daytona USA Turbo (Rev A)"));
	hWnd12 = FindWindowA(0, ("Daytona USA: GTX 2004"));
	hWnd13 = FindWindowA(0, ("Daytona USA: To The Maxx"));
	hWnd14 = FindWindowA(0, ("Sega Rally Championship (Rev B)"));
	hWnd15 = FindWindowA(0, ("Sega Rally Pro Drivin'"));
	hWnd16 = FindWindowA(0, ("Indianapolis 500 (Rev A, Twin, Newer rev)"));
	hWnd17 = FindWindowA(0, ("Indianapolis 500 (Rev A, Twin, Older rev)"));
	hWnd18 = FindWindowA(0, ("Sega Touring Car Championship"));
	hWnd19 = FindWindowA(0, ("Sega Touring Car Championship (Rev B)"));
	hWnd20 = FindWindowA(0, ("Over Rev (Model 2B)"));

	// used to be a new char[256] on every call (every 16 ms), never freed, that the FFB strength
	// adjustment thread could read before anything was written in it (no game window found yet)
	static char romnameM2Buffer[256];
	romnameM2 = romnameM2Buffer;

	if (hWnd1 > NULL || hWnd2 > NULL || hWnd3 > NULL || hWnd4 > NULL || hWnd5 > NULL || hWnd6 > NULL || hWnd7 > NULL || hWnd8 > NULL || hWnd9 > NULL || hWnd10 > NULL ||
		hWnd11 > NULL || hWnd12 > NULL || hWnd13 > NULL || hWnd14 > NULL || hWnd15 > NULL || hWnd16 > NULL || hWnd17 > NULL || hWnd18 > NULL || hWnd19 > NULL || hWnd20 > NULL)
	{
		if (InputDeviceWheelEnable)
		{
			hookAddressM2A = 0x4CA450;
			hookAddressM2B = 0x4CB870;
			hookAddressM2C = 0x4C9080;
			helpers->WriteNop(0xCCA6B, 6, true);

			int hookLength = 6;

			if (hookAddressM2A)
			{
				jmpBackAddy = hookAddressM2A + (INT_PTR)hookLength;
				Hook((void*)hookAddressM2A, OldInputs, hookLength);
			}

			if (hookAddressM2B)
			{
				jmpBackAddy = hookAddressM2B + (INT_PTR)hookLength;
				Hook((void*)hookAddressM2B, OldInputs, hookLength);
			}

			if (hookAddressM2C)
			{
				jmpBackAddy = hookAddressM2C + (INT_PTR)hookLength;
				Hook((void*)hookAddressM2C, OldInputs, hookLength);
			}
		}
	}

	if (hWnd1 > NULL)
	{
		sprintf(romnameM2, "%s", "Sega Rally Championship");
		hWndM2 = hWnd1;
	}
	else if(hWnd2 > NULL)
	{
		sprintf(romnameM2, "%s", "Daytona USA");
		hWndM2 = hWnd2;
	}
	else if (hWnd3 > NULL)
	{
		sprintf(romnameM2, "%s", "Indianapolis 500 (Rev A, Deluxe)");
		hWndM2 = hWnd3;
	}
	else if (hWnd4 > NULL)
	{
		sprintf(romnameM2, "%s", "Sega Touring Car Championship (Rev A)");
		hWndM2 = hWnd4;
	}
	else if (hWnd5 > NULL)
	{
		sprintf(romnameM2, "%s", "Over Rev");
		hWndM2 = hWnd5;
	}
	else if (hWnd6 > NULL)
	{
		sprintf(romnameM2, "%s", "Super GT 24h");
		hWndM2 = hWnd6;
	}
	else if (hWnd7 > NULL)
	{
		sprintf(romnameM2, "%s", "Daytona USA '93 Edition");
		hWndM2 = hWnd7;
	}
	else if (hWnd8 > NULL)
	{
		sprintf(romnameM2, "%s", "Daytona USA (Saturn Ads)");
		hWndM2 = hWnd8;
	}
	else if (hWnd9 > NULL)
	{
		sprintf(romnameM2, "%s", "Daytona USA Special Edition");
		hWndM2 = hWnd9;
	}
	else if (hWnd10 > NULL)
	{
		sprintf(romnameM2, "%s", "Daytona USA Turbo");
		hWndM2 = hWnd10;
	}
	else if (hWnd11 > NULL)
	{
		sprintf(romnameM2, "%s", "Daytona USA Turbo (Rev A)");
		hWndM2 = hWnd11;
	}
	else if (hWnd12 > NULL)
	{
		sprintf(romnameM2, "%s", "Daytona USA: GTX 2004");
		hWndM2 = hWnd12;
	}
	else if (hWnd13 > NULL)
	{
		sprintf(romnameM2, "%s", "Daytona USA: To The Maxx");
		hWndM2 = hWnd13;
	}
	else if (hWnd14 > NULL)
	{
		sprintf(romnameM2, "%s", "Sega Rally Championship (Rev B)");
		hWndM2 = hWnd14;
	}
	else if (hWnd15 > NULL)
	{
		sprintf(romnameM2, "%s", "Sega Rally Pro Drivin'");
		hWndM2 = hWnd15;
	}
	else if (hWnd16 > NULL)
	{
		sprintf(romnameM2, "%s", "Indianapolis 500 (Rev A, Twin, Newer rev)");
		hWndM2 = hWnd16;
	}
	else if (hWnd17 > NULL)
	{
		sprintf(romnameM2, "%s", "Indianapolis 500 (Rev A, Twin, Older rev)");
		hWndM2 = hWnd17;
	}
	else if (hWnd18 > NULL)
	{
		sprintf(romnameM2, "%s", "Sega Touring Car Championship");
		hWndM2 = hWnd18;
	}
	else if (hWnd19 > NULL)
	{
		sprintf(romnameM2, "%s", "Sega Touring Car Championship (Rev B)");
		hWndM2 = hWnd19;
	}
	else if (hWnd20 > NULL)
	{
		sprintf(romnameM2, "%s", "Over Rev (Model 2B)");
		hWndM2 = hWnd20;
	}

	if (EnableForceSpringEffect == 1)
	{
		triggers->Springi(ForceSpringStrength / 100.0);
	}

	ApplyDamper(triggers);

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
			CreateThread(NULL, 0, InputLoop, NULL, 0, NULL);
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
				jmpBackAddy = hookAddress + (INT_PTR)hookLength;
				Hook((void*)hookAddress, ExitHook, hookLength);
				init = true;
			}
		}
	}

	if ((hWnd2 > NULL) || (hWnd3 > NULL) || (hWnd4 > NULL) || (hWnd5 > NULL) || (hWnd6 > NULL) || (hWnd7 > NULL) || (hWnd8 > NULL) || (hWnd9 > NULL) || (hWnd10 > NULL) || (hWnd11 > NULL) || (hWnd12 > NULL) || (hWnd13 > NULL) ||(hWnd16 > NULL) || (hWnd17 > NULL) || (hWnd18 > NULL) || (hWnd19 > NULL) || (hWnd20 > NULL))
	{
		std::string ffs = std::to_string(ff);
		helpers->log((char*)ffs.c_str());
		helpers->log("got value: ");

		UINT32 length_ms = 100;
		if ((ff > 0x09) && (ff < 0x18))
		{
			//Spring
			double percentForce = (ff - 15) / 8.0;
			triggers->Spring(percentForce);
		}

		if ((ff > 0x1F) && (ff < 0x28))
		{
			//Clutch
			double percentForce = (ff - 31) / 8.0;
			triggers->Friction(percentForce);
		}

		if ((ff > 0x2F) && (ff < 0x3D))
		{
			//Centering
			double percentForce = (ff - 47) / 13.0;
			triggers->Spring(percentForce);
		}

		if ((ff > 0x3F) && (ff < 0x48))
		{
			//Uncentering
			double percentForce = (ff - 63) / 8.0;
			triggers->Sine(40, 0, percentForce, length_ms);
			triggers->Rumble(percentForce, percentForce, length_ms);
		}

		if ((ff > 0x4F) && (ff < 0x58))
		{
			//Roll Left
			double percentForce = (ff - 79) / 8.0;
			triggers->Rumble(0, percentForce, length_ms);
			triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
		}
		else if ((ff > 0x5F) && (ff < 0x68))
		{
			//Roll Right
			double percentForce = (ff - 95) / 8.0;
			triggers->Rumble(percentForce, 0, length_ms);
			triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
		}
	}

	if ((hWnd1 > NULL) || (hWnd14 > NULL) || (hWnd15 > NULL))
	{
		if (!CustomStrengthInit)
		{
			CustomStrengthInit = true;
			configMinForce = configMinForceSRally;
			configMaxForce = configMaxForceSRally;
			configAlternativeMinForceLeft = configAlternativeMinForceLeftSRally;
			configAlternativeMaxForceLeft = configAlternativeMaxForceLeftSRally;
			configAlternativeMinForceRight = configAlternativeMinForceRightSRally;
			configAlternativeMaxForceRight = configAlternativeMaxForceRightSRally;
			configFeedbackLength = configFeedbackLengthSRally;
			PowerMode = PowerModeSRally;
			EnableForceSpringEffect = EnableForceSpringEffectSRally;
			ForceSpringStrength = ForceSpringStrengthSRally;
			EnableDamper = EnableDamperSRally;
			DamperStrength = DamperStrengthSRally;
		}

		UINT8 ff1 = helpers->ReadByte(0x174CF4, /* isRelativeOffset*/ true); //SegaRallyChampionship
		std::string ffs = std::to_string(ff1);
		helpers->log((char*)ffs.c_str());
		helpers->log("got value: ");

		if (OldFFB != ff1)
		{
			UINT32 length_ms = 100;
			if (ff1 >= 0xC0 && ff1 <= 0xDF)
			{
				helpers->log("moving wheel left");
				double percentForce = (ff1 - 191) / 32.0;

				triggers->Rumble(0, percentForce, length_ms);
				triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
			}
			
			if (ff1 >= 0x80 && ff1 <= 0x9F)
			{
				helpers->log("moving wheel right");
				double percentForce = (ff1 - 127) / 32.0;

				triggers->Rumble(percentForce, 0, length_ms);
				triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
			}
		}
		OldFFB = ff1;
	}

	if ((hWnd2 > NULL) || (hWnd7 > NULL) || (hWnd8 > NULL) || (hWnd9 > NULL) || (hWnd10 > NULL) || (hWnd11 > NULL) || (hWnd12 > NULL) || (hWnd13 > NULL))
	{
		if (!CustomStrengthInit)
		{
			CustomStrengthInit = true;
			configMinForce = configMinForceDaytona;
			configMaxForce = configMaxForceDaytona;
			configAlternativeMinForceLeft = configAlternativeMinForceLeftDaytona;
			configAlternativeMaxForceLeft = configAlternativeMaxForceLeftDaytona;
			configAlternativeMinForceRight = configAlternativeMinForceRightDaytona;
			configAlternativeMaxForceRight = configAlternativeMaxForceRightDaytona;
			configFeedbackLength = configFeedbackLengthDaytona;
			PowerMode = PowerModeDaytona;
			EnableForceSpringEffect = EnableForceSpringEffectDaytona;
			ForceSpringStrength = ForceSpringStrengthDaytona;
			EnableDamper = EnableDamperDaytona;
			DamperStrength = DamperStrengthDaytona;
		}

		if (DaytonaForcePanoramicAttract == 1)
		{
			INT_PTR Rambase = helpers->ReadIntPtr(0x1AA888, /* isRelativeOffset*/ true);
			INT_PTR Rambase1 = helpers->ReadIntPtr(Rambase + 0x100, /* isRelativeOffset */ false);
			helpers->WriteByte(Rambase1 + 0x1070, 0x00, /* isRelativeOffset*/ false);
		}

		if (DaytonaAIMultiplayerHack == 1)
		{
			INT_PTR Rambase = helpers->ReadIntPtr(0x1AA888, /* isRelativeOffset*/ true);
			INT_PTR Rambase1 = helpers->ReadIntPtr(Rambase + 0x100, /* isRelativeOffset */ false);
			UINT8 gamestate = helpers->ReadByte(Rambase1 + 0x10A4, /* isRelativeOffset*/ false);
			UINT8 track = helpers->ReadByte(Rambase1 + 0x1460, /* isRelativeOffset*/ false);
			UINT8 trackselect = helpers->ReadByte(Rambase1 + 0x1468, /* isRelativeOffset*/ false);
			UINT8 countdown = helpers->ReadByte(Rambase1 + 0x1850, /* isRelativeOffset*/ false);
			UINT8 linksize = helpers->ReadByte(Rambase1 + 0x40001, /* isRelativeOffset*/ false);
			UINT8 linkID = helpers->ReadByte(Rambase1 + 0x40002, /* isRelativeOffset*/ false);
			INT_PTR gamestatetimer = helpers->ReadIntPtr(Rambase1 + 0x10A8, /* isRelativeOffset*/ false);
			UINT8 camhack = helpers->ReadByte(Rambase1 + 0x14A4, /* isRelativeOffset*/ false);
			UINT8 carsinfront = helpers->ReadByte(Rambase1 + 0x51EC, /* isRelativeOffset*/ false);
			UINT8 mpposition = helpers->ReadByte(Rambase1 + 0x51ED, /* isRelativeOffset*/ false);
			float cary = helpers->ReadFloat32(Rambase1 + 0x519C, /* isRelativeOffset */ false);
			float carx = helpers->ReadFloat32(Rambase1 + 0x51A4, /* isRelativeOffset */ false);
			UINT8 nop1 = helpers->ReadByte(0xC2130, /* isRelativeOffset*/ true);
			UINT8 nop2 = helpers->ReadByte(0xC2131, /* isRelativeOffset*/ true);
			UINT8 nop3 = helpers->ReadByte(0xC2132, /* isRelativeOffset*/ true);


			if (gamestate == 0x1A)
			{
				helpers->WriteByte(Rambase1 + 0x51ED, carsinfront, /* isRelativeOffset*/ false);
			}

			if (trackselect == 0x01)
			{
				helpers->WriteByte(Rambase1 + 0x1460, 0x02, /* isRelativeOffset*/ false);
			}
			else if (trackselect == 0x02)
			{
				helpers->WriteByte(Rambase1 + 0x1460, 0x01, /* isRelativeOffset*/ false);
			}

			if (linksize == 0x02)
			{
				if (linkID == 0x02)
				{
					if (gamestate > 0x07)
					{
						helpers->WriteIntPtr(Rambase1 + 0x40004, 0x00540C80, /* isRelativeOffset*/ false);
						helpers->WriteIntPtr(Rambase1 + 0x40008, 0x00540C80, /* isRelativeOffset*/ false);
						helpers->WriteIntPtr(Rambase1 + 0x4000C, 0x00540C40, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5484, 0x01, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5784, 0x02, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5A84, 0x03, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5D84, 0x04, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6084, 0x05, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6384, 0x06, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6684, 0x07, /* isRelativeOffset*/ false);
					}
					if ((gamestate == 0x16) && (countdown == 0x00) && (track == 0x00))
					{
						Sleep(150);
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						helpers->WriteByte(Rambase1 + 0x1850, 0x01, /* isRelativeOffset*/ false);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x02 || track == 0x01))
					{
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						helpers->WriteByte(Rambase1 + 0x1468, 0x01, /* isRelativeOffset*/ false);
					}
				}
				else if (linkID == 0x01)
				{
					if ((gamestate == 0x1A) && (gamestatetimer < 100))
					{
						helpers->WriteByte(0xC2130, 0x88, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2131, 0x14, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2132, 0x01, /* isRelativeOffset*/ true);
					}
					else if ((gamestate == 0x1A) && (gamestatetimer > 100))
					{
						Sleep(100);
						helpers->WriteNop(0xC2130, 3, true);
					}

					if ((gamestate > 0x1A) && (gamestate < 0x1A))
					{
						helpers->WriteByte(0xC2130, 0x88, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2131, 0x14, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2132, 0x01, /* isRelativeOffset*/ true);
					}

					if (gamestate > 0x07)
					{
						helpers->WriteIntPtr(Rambase1 + 0x40004, 0x00540C80, /* isRelativeOffset*/ false);
						helpers->WriteIntPtr(Rambase1 + 0x40008, 0x00540C40, /* isRelativeOffset*/ false);
						helpers->WriteIntPtr(Rambase1 + 0x4000C, 0x00540C40, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5484, 0x00, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5784, 0x02, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5A84, 0x03, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5D84, 0x04, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6084, 0x05, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6384, 0x06, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6684, 0x07, /* isRelativeOffset*/ false);
					}
					if ((gamestatetimer > 4294964133) && (gamestate == 0x12))
					{
						helpers->WriteIntPtr(Rambase1 + 0x10A8, 0x3B6, /* isRelativeOffset*/ false);
					}
					if ((gamestate == 0x16) && (countdown == 0x00) && (track == 0x00))
					{
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						helpers->WriteByte(Rambase1 + 0x1850, 0x01, /* isRelativeOffset*/ false);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x02 || track == 0x01))
					{
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						Sleep(50);
						helpers->WriteFloat32(Rambase1 + 0x519C, 0.0, /* isRelativeOffset */ false);
						helpers->WriteByte(Rambase1 + 0x1468, 0x01, /* isRelativeOffset*/ false);
					}
				}
			}
			else if (linksize == 0x03)
			{
				if (linkID == 0x03)
				{
					if (gamestate > 0x07)
					{
						helpers->WriteIntPtr(Rambase1 + 0x40004, 0x00540CC0, /* isRelativeOffset*/ false);
						helpers->WriteIntPtr(Rambase1 + 0x40008, 0x00540CC0, /* isRelativeOffset*/ false);
						helpers->WriteIntPtr(Rambase1 + 0x4000C, 0x00540CC0, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5484, 0x01, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5784, 0x02, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5A84, 0x03, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5D84, 0x04, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6084, 0x05, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6384, 0x06, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6684, 0x07, /* isRelativeOffset*/ false);
					}
					if ((gamestate == 0x16) && (countdown == 0x00) && (track == 0x00))
					{
						Sleep(250);
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						helpers->WriteByte(Rambase1 + 0x1850, 0x01, /* isRelativeOffset*/ false);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x02 || track == 0x01))
					{
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						helpers->WriteByte(Rambase1 + 0x1468, 0x01, /* isRelativeOffset*/ false);
						helpers->WriteFloat32(Rambase1 + 0x519C, 450.0, /* isRelativeOffset */ false);
					}
					if ((gamestatetimer > 4294964133) && (gamestate == 0x12))
					{
						helpers->WriteIntPtr(Rambase1 + 0x10A8, 0x3B6, /* isRelativeOffset*/ false);
					}
				}
				else if (linkID == 0x02)
				{
					if ((gamestate == 0x1A) && (gamestatetimer < 100))
					{
						helpers->WriteByte(0xC2130, 0x88, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2131, 0x14, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2132, 0x01, /* isRelativeOffset*/ true);
					}
					else if ((gamestate == 0x1A) && (gamestatetimer > 100))
					{
						Sleep(100);
						helpers->WriteNop(0xC2130, 3, true);
					}

					if ((gamestate > 0x1A) && (gamestate < 0x1A))
					{
						helpers->WriteByte(0xC2130, 0x88, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2131, 0x14, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2132, 0x01, /* isRelativeOffset*/ true);
					}
					if (gamestate > 0x07)
					{
						helpers->WriteIntPtr(Rambase1 + 0x40004, 0x00540CC0, /* isRelativeOffset*/ false);
						helpers->WriteIntPtr(Rambase1 + 0x40008, 0x00540C40, /* isRelativeOffset*/ false);
						helpers->WriteIntPtr(Rambase1 + 0x4000C, 0x00540C40, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5484, 0x01, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5784, 0x00, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5A84, 0x03, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5D84, 0x04, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6084, 0x05, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6384, 0x06, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6684, 0x07, /* isRelativeOffset*/ false);
					}
					if ((gamestate == 0x16) && (countdown == 0x00) && (track == 0x00))
					{
						Sleep(150);
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						helpers->WriteByte(Rambase1 + 0x1850, 0x01, /* isRelativeOffset*/ false);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x02 || track == 0x01))
					{
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						helpers->WriteByte(Rambase1 + 0x1468, 0x01, /* isRelativeOffset*/ false);
					}
					if ((gamestatetimer > 4294964133) && (gamestate == 0x12))
					{
						helpers->WriteIntPtr(Rambase1 + 0x10A8, 0x3B6, /* isRelativeOffset*/ false);
					}
				}
				else if (linkID == 0x01)
				{
					if ((gamestate == 0x1A) && (gamestatetimer < 100))
					{
						helpers->WriteByte(0xC2130, 0x88, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2131, 0x14, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2132, 0x01, /* isRelativeOffset*/ true);
					}
					else if ((gamestate == 0x1A) && (gamestatetimer > 100))
					{
						Sleep(100);
						helpers->WriteNop(0xC2130, 3, true);
					}

					if ((gamestate > 0x1A) && (gamestate < 0x1A))
					{
						helpers->WriteByte(0xC2130, 0x88, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2131, 0x14, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2132, 0x01, /* isRelativeOffset*/ true);
					}

					if (gamestate > 0x07)
					{
						helpers->WriteIntPtr(Rambase1 + 0x40004, 0x00540CC0, /* isRelativeOffset*/ false);
						helpers->WriteIntPtr(Rambase1 + 0x40008, 0x00540C80, /* isRelativeOffset*/ false);
						helpers->WriteIntPtr(Rambase1 + 0x4000C, 0x00540C80, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5484, 0x00, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5784, 0x02, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5A84, 0x03, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5D84, 0x04, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6084, 0x05, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6384, 0x06, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6684, 0x07, /* isRelativeOffset*/ false);
					}
					if ((gamestatetimer > 4294964133) && (gamestate == 0x12))
					{
						helpers->WriteIntPtr(Rambase1 + 0x10A8, 0x3B6, /* isRelativeOffset*/ false);
					}
					if ((gamestate == 0x16) && (countdown == 0x00) && (track == 0x00))
					{
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						helpers->WriteByte(Rambase1 + 0x1850, 0x01, /* isRelativeOffset*/ false);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x01))
					{
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						Sleep(50);
						helpers->WriteIntPtr(Rambase1 + 0x51A4, 0xC2F195E8, /* isRelativeOffset */ false);
						helpers->WriteByte(Rambase1 + 0x1468, 0x01, /* isRelativeOffset*/ false);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x02))
					{
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						Sleep(50);
						helpers->WriteIntPtr(Rambase1 + 0x51A4, 0xC37E0001, /* isRelativeOffset */ false);
						helpers->WriteByte(Rambase1 + 0x1468, 0x01, /* isRelativeOffset*/ false);
					}
				}
			}
			else if (linksize == 0x04)
			{
				if (linkID == 0x04)
				{
					if (gamestate > 0x07)
					{
						helpers->WriteIntPtr(Rambase1 + 0x40004, 0x00540D00, /* isRelativeOffset*/ false);
						helpers->WriteIntPtr(Rambase1 + 0x40008, 0x00540D00, /* isRelativeOffset*/ false);
						helpers->WriteIntPtr(Rambase1 + 0x4000C, 0x00540D00, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5484, 0x01, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5784, 0x02, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5A84, 0x03, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5D84, 0x04, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6084, 0x05, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6384, 0x06, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6684, 0x07, /* isRelativeOffset*/ false);
					}
					if ((gamestate == 0x16) && (countdown == 0x00) && (track == 0x00))
					{
						Sleep(250);
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						helpers->WriteByte(Rambase1 + 0x1850, 0x01, /* isRelativeOffset*/ false);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x01))
					{
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						Sleep(50);
						helpers->WriteIntPtr(Rambase1 + 0x51A4, 0xC302CAF4, /* isRelativeOffset */ false);
						helpers->WriteByte(Rambase1 + 0x1468, 0x01, /* isRelativeOffset*/ false);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x02))
					{
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						Sleep(50);
						helpers->WriteFloat32(Rambase1 + 0x519C, 450.0, /* isRelativeOffset */ false);
						helpers->WriteIntPtr(Rambase1 + 0x51A4, 0xC37C0001, /* isRelativeOffset */ false);
						helpers->WriteByte(Rambase1 + 0x1468, 0x01, /* isRelativeOffset*/ false);
					}
					if ((gamestatetimer > 4294964133) && (gamestate == 0x12))
					{
						helpers->WriteIntPtr(Rambase1 + 0x10A8, 0x3B6, /* isRelativeOffset*/ false);
					}
				}
				if (linkID == 0x03)
				{
					if ((gamestate == 0x1A) && (gamestatetimer < 100))
					{
						helpers->WriteByte(0xC2130, 0x88, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2131, 0x14, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2132, 0x01, /* isRelativeOffset*/ true);
					}
					else if ((gamestate == 0x1A) && (gamestatetimer > 100))
					{
						Sleep(100);
						helpers->WriteNop(0xC2130, 3, true);
					}

					if ((gamestate > 0x1A) && (gamestate < 0x1A))
					{
						helpers->WriteByte(0xC2130, 0x88, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2131, 0x14, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2132, 0x01, /* isRelativeOffset*/ true);
					}

					if (gamestate > 0x07)
					{
						helpers->WriteIntPtr(Rambase1 + 0x40004, 0x00540D00, /* isRelativeOffset*/ false);
						helpers->WriteIntPtr(Rambase1 + 0x40008, 0x00540C40, /* isRelativeOffset*/ false);
						helpers->WriteIntPtr(Rambase1 + 0x4000C, 0x00540C40, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5484, 0x01, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5784, 0x02, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5A84, 0x00, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5D84, 0x04, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6084, 0x05, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6384, 0x06, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6684, 0x07, /* isRelativeOffset*/ false);
					}
					if ((gamestate == 0x16) && (countdown == 0x00) && (track == 0x00))
					{
						Sleep(350);
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						helpers->WriteByte(Rambase1 + 0x1850, 0x01, /* isRelativeOffset*/ false);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x02))
					{
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						helpers->WriteByte(Rambase1 + 0x1468, 0x01, /* isRelativeOffset*/ false);
						helpers->WriteFloat32(Rambase1 + 0x519C, 450.0, /* isRelativeOffset */ false);
						Sleep(50);
						helpers->WriteIntPtr(Rambase1 + 0x51A4, 0xC37CF3D0, /* isRelativeOffset */ false);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x01))
					{
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						helpers->WriteByte(Rambase1 + 0x1468, 0x01, /* isRelativeOffset*/ false);
						helpers->WriteFloat32(Rambase1 + 0x519C, 450.0, /* isRelativeOffset */ false);
					}
					if ((gamestatetimer > 4294964133) && (gamestate == 0x12))
					{
						helpers->WriteIntPtr(Rambase1 + 0x10A8, 0x3B6, /* isRelativeOffset*/ false);
					}
				}
				else if (linkID == 0x02)
				{
					if ((gamestate == 0x1A) && (gamestatetimer < 100))
					{
						helpers->WriteByte(0xC2130, 0x88, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2131, 0x14, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2132, 0x01, /* isRelativeOffset*/ true);
					}
					else if ((gamestate == 0x1A) && (gamestatetimer > 100))
					{
						Sleep(100);
						helpers->WriteNop(0xC2130, 3, true);
					}

					if ((gamestate > 0x1A) && (gamestate < 0x1A))
					{
						helpers->WriteByte(0xC2130, 0x88, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2131, 0x14, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2132, 0x01, /* isRelativeOffset*/ true);
					}
					if (gamestate > 0x07)
					{
						helpers->WriteIntPtr(Rambase1 + 0x40004, 0x00540D00, /* isRelativeOffset*/ false);
						helpers->WriteIntPtr(Rambase1 + 0x40008, 0x00540C80, /* isRelativeOffset*/ false);
						helpers->WriteIntPtr(Rambase1 + 0x4000C, 0x00540C80, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5484, 0x01, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5784, 0x00, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5A84, 0x03, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5D84, 0x04, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6084, 0x05, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6384, 0x06, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6684, 0x07, /* isRelativeOffset*/ false);
					}
					if ((gamestate == 0x16) && (countdown == 0x00) && (track == 0x00))
					{
						Sleep(150);
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						helpers->WriteByte(Rambase1 + 0x1850, 0x01, /* isRelativeOffset*/ false);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x02 || track == 0x01))
					{
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						helpers->WriteByte(Rambase1 + 0x1468, 0x01, /* isRelativeOffset*/ false);
					}
					if ((gamestatetimer > 4294964133) && (gamestate == 0x12))
					{
						helpers->WriteIntPtr(Rambase1 + 0x10A8, 0x3B6, /* isRelativeOffset*/ false);
					}
				}
				else if (linkID == 0x01)
				{
					if ((gamestate == 0x1A) && (gamestatetimer < 100))
					{
						helpers->WriteByte(0xC2130, 0x88, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2131, 0x14, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2132, 0x01, /* isRelativeOffset*/ true);
					}
					else if ((gamestate == 0x1A) && (gamestatetimer > 100))
					{
						Sleep(100);
						helpers->WriteNop(0xC2130, 3, true);
					}

					if ((gamestate > 0x1A) && (gamestate < 0x1A))
					{
						helpers->WriteByte(0xC2130, 0x88, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2131, 0x14, /* isRelativeOffset*/ true);
						helpers->WriteByte(0xC2132, 0x01, /* isRelativeOffset*/ true);
					}

					if (gamestate > 0x07)
					{
						helpers->WriteIntPtr(Rambase1 + 0x40004, 0x00540D00, /* isRelativeOffset*/ false);
						helpers->WriteIntPtr(Rambase1 + 0x40008, 0x00540CC0, /* isRelativeOffset*/ false);
						helpers->WriteIntPtr(Rambase1 + 0x4000C, 0x00540CC0, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5484, 0x00, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5784, 0x02, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5A84, 0x03, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x5D84, 0x04, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6084, 0x05, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6384, 0x06, /* isRelativeOffset*/ false);
						helpers->WriteByte(Rambase1 + 0x6684, 0x07, /* isRelativeOffset*/ false);
					}
					if ((gamestatetimer > 4294964133) && (gamestate == 0x12))
					{
						helpers->WriteIntPtr(Rambase1 + 0x10A8, 0x3B6, /* isRelativeOffset*/ false);
					}
					if ((gamestate == 0x16) && (countdown == 0x00) && (track == 0x00))
					{
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						helpers->WriteByte(Rambase1 + 0x1850, 0x01, /* isRelativeOffset*/ false);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x01))
					{
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						Sleep(50);
						helpers->WriteIntPtr(Rambase1 + 0x51A4, 0xC2F195E8, /* isRelativeOffset */ false);
						helpers->WriteByte(Rambase1 + 0x1468, 0x01, /* isRelativeOffset*/ false);
					}
					else if ((gamestate == 0x14) && (trackselect == 0x00) && (track == 0x02))
					{
						helpers->WriteByte(Rambase1 + 0x40000, 0x99, /* isRelativeOffset*/ false); // change to 99
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x10A4, 0x13, /* isRelativeOffset*/ false); // restart race
						Sleep(50);
						helpers->WriteByte(Rambase1 + 0x40000, 0x01, /* isRelativeOffset*/ false); // change back to 1
						Sleep(50);
						helpers->WriteIntPtr(Rambase1 + 0x51A4, 0xC37E0001, /* isRelativeOffset */ false);
						helpers->WriteByte(Rambase1 + 0x1468, 0x01, /* isRelativeOffset*/ false);
					}
				}
			}
		}

		ff = helpers->ReadByte(0x0057285B, /* isRelativeOffset*/ false); //DaytonaUSA		
	}

	if ((hWnd3 > NULL) || (hWnd16 > NULL) || (hWnd17 > NULL))
	{
		if (!CustomStrengthInit)
		{
			CustomStrengthInit = true;
			configMinForce = configMinForceIndy500;
			configMaxForce = configMaxForceIndy500;
			configAlternativeMinForceLeft = configAlternativeMinForceLeftIndy500;
			configAlternativeMaxForceLeft = configAlternativeMaxForceLeftIndy500;
			configAlternativeMinForceRight = configAlternativeMinForceRightIndy500;
			configAlternativeMaxForceRight = configAlternativeMaxForceRightIndy500;
			configFeedbackLength = configFeedbackLengthIndy500;
			PowerMode = PowerModeIndy500;
			EnableForceSpringEffect = EnableForceSpringEffectIndy500;
			ForceSpringStrength = ForceSpringStrengthIndy500;
			EnableDamper = EnableDamperIndy500;
			DamperStrength = DamperStrengthIndy500;
		}

		ff = helpers->ReadByte(0x17285B, /* isRelativeOffset*/ true); //Indy500
	}	

	if ((hWnd4 > NULL) || (hWnd18 > NULL) || (hWnd19 > NULL))
	{
		if (!CustomStrengthInit)
		{
			CustomStrengthInit = true;
			configMinForce = configMinForceSTCC;
			configMaxForce = configMaxForceSTCC;
			configAlternativeMinForceLeft = configAlternativeMinForceLeftSTCC;
			configAlternativeMaxForceLeft = configAlternativeMaxForceLeftSTCC;
			configAlternativeMinForceRight = configAlternativeMinForceRightSTCC;
			configAlternativeMaxForceRight = configAlternativeMaxForceRightSTCC;
			configFeedbackLength = configFeedbackLengthSTCC;
			PowerMode = PowerModeSTCC;
			EnableForceSpringEffect = EnableForceSpringEffectSTCC;
			ForceSpringStrength = ForceSpringStrengthSTCC;
			EnableDamper = EnableDamperSTCC;
			DamperStrength = DamperStrengthSTCC;
		}

		ff = helpers->ReadByte(0x17285B, /* isRelativeOffset*/ true); //Sega Touring Car Championship
	}		

	if ((hWnd5 > NULL) || (hWnd20 > NULL))
	{
		if (!CustomStrengthInit)
		{
			CustomStrengthInit = true;
			configMinForce = configMinForceOverRev;
			configMaxForce = configMaxForceOverRev;
			configAlternativeMinForceLeft = configAlternativeMinForceLeftOverRev;
			configAlternativeMaxForceLeft = configAlternativeMaxForceLeftOverRev;
			configAlternativeMinForceRight = configAlternativeMinForceRightOverRev;
			configAlternativeMaxForceRight = configAlternativeMaxForceRightOverRev;
			configFeedbackLength = configFeedbackLengthOverRev;
			PowerMode = PowerModeOverRev;
			EnableForceSpringEffect = EnableForceSpringEffectOverRev;
			ForceSpringStrength = ForceSpringStrengthOverRev;
			EnableDamper = EnableDamperOverRev;
			DamperStrength = DamperStrengthOverRev;
		}

		ff = helpers->ReadByte(0x17285B, /* isRelativeOffset*/ true); //OverRev
	}	

	if (hWnd6 > NULL)
	{
		if (!CustomStrengthInit)
		{
			CustomStrengthInit = true;
			configMinForce = configMinForceSuperGT;
			configMaxForce = configMaxForceSuperGT;
			configAlternativeMinForceLeft = configAlternativeMinForceLeftSuperGT;
			configAlternativeMaxForceLeft = configAlternativeMaxForceLeftSuperGT;
			configAlternativeMinForceRight = configAlternativeMinForceRightSuperGT;
			configAlternativeMaxForceRight = configAlternativeMaxForceRightSuperGT;
			configFeedbackLength = configFeedbackLengthSuperGT;
			PowerMode = PowerModeSuperGT;
			EnableForceSpringEffect = EnableForceSpringEffectSuperGT;
			ForceSpringStrength = ForceSpringStrengthSuperGT;
			EnableDamper = EnableDamperSuperGT;
			DamperStrength = DamperStrengthSuperGT;
		}

		ff = helpers->ReadByte(0x17285B, /* isRelativeOffset*/ true); //Super GT 24h
	}		
}