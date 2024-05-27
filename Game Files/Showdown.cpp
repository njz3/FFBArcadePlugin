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
#include "Showdown.h"
#include "SDL.h"
#include <Xinput.h>

static EffectTriggers* myTriggers;
static EffectConstants* myConstants;
static Helpers* myHelpers;

extern int EnableDamper;
extern int DamperStrength;
extern int FFBOrRumble;

static bool init;

static int(__stdcall* Out32Ori)(DWORD device, DWORD data);
static int __stdcall Out32Hook(DWORD device, DWORD data)
{
	if (device == 0x378)
	{
		//static char test[256];
		//memset(test, 0, 256);
		//sprintf(test, "data: %d", data);
		//OutputDebugStringA(test);

		UINT32 length_ms = 100;
		if (data > 15)
		{
			double percentForce = (31 - data) / 15.0;
			myTriggers->Rumble(percentForce, 0, length_ms);
			myTriggers->Constant(myConstants->DIRECTION_FROM_LEFT, percentForce);
		}
		else if (data > 0)
		{
			double percentForce = (16 - data) / 15.0;
			myTriggers->Rumble(0, percentForce, length_ms);
			myTriggers->Constant(myConstants->DIRECTION_FROM_RIGHT, percentForce);
		}
	}

	return 0;
}

//static int(__fastcall* EnableFFBOri)(int a1, double a2);
//static int __fastcall EnableFFBHook(int a1, double a2)
//{
//	EnableFFBOri(a1, a2);
//
//	*(BYTE*)(a1 + 92) = 1; // FFB
//
//	if (FFBOrRumble)
//		*(BYTE*)(a1 + 12) = 1; // Rumble
//
//	return 0;
//}

static int(__fastcall* EnableFFBOri)(float* a1, XINPUT_VIBRATION pVibration);
static int __fastcall EnableFFBHook(float* a1, XINPUT_VIBRATION pVibration)
{
	static char test[256];
	memset(test, 0, 256);
	sprintf(test, "pVibration: %08X", a1[43]);
	OutputDebugStringA(test);

	return EnableFFBOri(a1, pVibration);
}
//
//
//static DWORD WINAPI XInputSetStateGRID(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
//{
//	static char test[256];
//	memset(test, 0, 256);
//	sprintf(test, "pVibration: %08X %08X", pVibration->wLeftMotorSpeed, pVibration->wRightMotorSpeed);
//	OutputDebugStringA(test);
//	myTriggers->Rumble(pVibration->wLeftMotorSpeed / 65535.0, pVibration->wRightMotorSpeed / 65535.0, 100.0);
//
//	return ERROR_SUCCESS;
//}

void Showdown::FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers) {
	if (!init)
	{
		init = true;

		INT_PTR ImageBase = (INT_PTR)GetModuleHandleA(0);

		MH_Initialize();
		//MH_CreateHook((void*)(ImageBase + 0x92E000), EnableFFBHook, (void**)&EnableFFBOri);
		MH_CreateHookApi(L"inpout32.dll", "Out32", Out32Hook, (void**)&Out32Ori);
		//if (FFBOrRumble)
		//	MH_CreateHookApi(L"xinput1_3.dll", "XInputSetState", &XInputSetStateGRID, NULL);
		MH_EnableHook(MH_ALL_HOOKS);
	}

	myTriggers = triggers;
	myConstants = constants;
	myHelpers = helpers;
}