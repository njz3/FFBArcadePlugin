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
#include "SegaRally3Other.h"

using namespace ffb;

SegaRally3Other* SegaRally3Other::instance = nullptr;

SegaRally3Other::SegaRally3Other()
{
	instance = this;
}

int __stdcall SegaRally3Other::Out32Hook(DWORD port, DWORD data)
{
	SegaRally3Other& game = *instance;
	game.ApplyDamper();

	// data is unsigned: a value above 31 gives a huge force from the left
	if (data > 15)
		game.wheel.ConstantWithRumble(Direction::FromLeft, (31 - data) / 15.0);
	else if (data > 0)
		game.wheel.ConstantWithRumble(Direction::FromRight, (16 - data) / 15.0);
	return 0;
}

void SegaRally3Other::Update()
{
	if (hooked)
		return;

	HMODULE inpout = GetModuleHandleA("inpout32.dll");
	if (!inpout)
		return;

	INT_PTR out32 = (INT_PTR)GetProcAddress(GetModuleHandle(L"inpout32.dll"), "Out32");
	if (!out32)
		return;

	// jmp Out32Hook, then NOPs
	BYTE patch[HookLength];
	memset(patch, 0x90, HookLength);
	patch[0] = 0xE9;
	DWORD relative = (DWORD)(((INT_PTR)Out32Hook - out32) - 5);
	memcpy(patch + 1, &relative, sizeof(relative));
	Memory::Patch(out32, patch, HookLength);

	hooked = true;
}
