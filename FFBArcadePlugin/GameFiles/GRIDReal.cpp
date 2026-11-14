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

#include "GRIDReal.h"
#include "../Core/Hooks.h"

using namespace ffb;

extern int FFBOrRumble;

GRIDReal* GRIDReal::instance = nullptr;
int(__stdcall* GRIDReal::out32Original)(DWORD device, DWORD data) = nullptr;
int(__fastcall* GRIDReal::enableFFBOriginal)(int object, double unknown) = nullptr;

GRIDReal::GRIDReal()
{
	instance = this;
}

int __stdcall GRIDReal::Out32Hook(DWORD device, DWORD data)
{
	if (FFBOrRumble)
		return 0;

	GRIDReal& game = *instance;
	if (device == 0x1020)
	{
		++game.ffbCounter;

		if (game.ffbCounter == 5)
		{
			game.ffbCounter = 0;

			if (data > 15)
			{
				double percentForce = (31 - data) / 15.0;
				game.wheel.ConstantWithRumble(Direction::FromLeft, percentForce);
			}
			else if (data > 0)
			{
				double percentForce = (16 - data) / 15.0;
				game.wheel.ConstantWithRumble(Direction::FromRight, percentForce);
			}
		}
	}

	return 0;
}

int __fastcall GRIDReal::EnableFFBHook(int object, double unknown)
{
	enableFFBOriginal(object, unknown);

	*(BYTE*)(object + 92) = (BYTE)1; // FFB

	if (FFBOrRumble)
		*(BYTE*)(object + 12) = (BYTE)1; // Rumble

	return 0;
}

DWORD WINAPI GRIDReal::XInputSetStateHook(DWORD userIndex, XINPUT_VIBRATION* vibration)
{
	instance->wheel.Rumble(vibration->wLeftMotorSpeed / 65535.0, vibration->wRightMotorSpeed / 65535.0, 100);

	return ERROR_SUCCESS;
}

void GRIDReal::Update()
{
	if (!hooked)
	{
		hooked = true;

		Hooks::Initialize();
		Hooks::Create(Memory::Address(Base::Module, EnableFFB), EnableFFBHook, &enableFFBOriginal);
		Hooks::CreateApi(L"inpout32.dll", "Out32", Out32Hook, &out32Original);
		if (FFBOrRumble)
			Hooks::CreateApi(L"xinput1_3.dll", "XInputSetState", &XInputSetStateHook, (LPVOID*)nullptr);
		Hooks::Enable();
	}
}
