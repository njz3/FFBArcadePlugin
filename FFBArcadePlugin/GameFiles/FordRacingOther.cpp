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
#include "FordRacingOther.h"

using namespace ffb;

FordRacingOther* FordRacingOther::instance = nullptr;

FordRacingOther::FordRacingOther()
{
	instance = this;
}

int __stdcall FordRacingOther::Out32Hook(DWORD device, DWORD data)
{
	if (device == 0x378)
	{
		FordRacingOther& game = *instance;
		if (data > 0xFFFF0013 && data < 0xFFFF001F)
		{
			double percentForce = (4294901791 - data) / 10.0;
			game.wheel.ConstantWithRumble(Direction::FromRight, percentForce);
		}
		else if (data > 5 && data < 16)
		{
			double percentForce = (16 - data) / 10.0;
			game.wheel.ConstantWithRumble(Direction::FromLeft, percentForce);
		}
	}
	return 0;
}

// Not Memory::WriteJump: the jump is followed by NOPs up to `length` and the page protection is lifted
// for `length` bytes
bool FordRacingOther::Hook(void* toHook, void* detour, int length)
{
	if (length < 5) {
		return false;
	}

	DWORD curProtection;
	VirtualProtect(toHook, length, PAGE_EXECUTE_READWRITE, &curProtection);

	memset(toHook, 0x90, length);

	INT_PTR relativeAddress = ((INT_PTR)detour - (INT_PTR)toHook) - 5;

	*(BYTE*)toHook = 0xE9;
	*(DWORD*)((INT_PTR)toHook + 1) = (DWORD)relativeAddress;

	DWORD temp;
	VirtualProtect(toHook, length, curProtection, &temp);

	return true;
}

void FordRacingOther::Update()
{
	if (!hooked)
	{
		HMODULE hMod = GetModuleHandleA("inpout32.dll");
		if (hMod)
		{
			int hookLength = 6;
			HMODULE module = GetModuleHandle(L"inpout32.dll");
			if (module != NULL) {
				INT_PTR hookAddress = (INT_PTR)GetProcAddress(module, "Out32");
				if (hookAddress)
				{
					jumpBack = hookAddress + hookLength;
					Hook((void*)hookAddress, (void*)Out32Hook, hookLength);
					hooked = true;
				}
			}
		}
	}

	ApplyDamper();
}
