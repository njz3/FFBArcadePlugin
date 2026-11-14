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
#include "Memory.h"

namespace ffb
{
	INT_PTR Memory::ModuleBase()
	{
		return (INT_PTR)GetModuleHandle(NULL);
	}

	void Memory::WriteNop(INT_PTR address, int count)
	{
		for (int i = 0; i < count; i++)
			Write<UINT8>(address + i, 0x90);
	}

	void Memory::Patch(INT_PTR address, const void* bytes, size_t size)
	{
		DWORD oldProtection;
		VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &oldProtection);
		memcpy((void*)address, bytes, size);
		VirtualProtect((LPVOID)address, size, oldProtection, &oldProtection);
	}

	void Memory::WriteJump(INT_PTR from, const void* to)
	{
		BYTE jump[5] = { 0xE9 };
		DWORD relative = (DWORD)((INT_PTR)to - from - 5);
		memcpy(jump + 1, &relative, sizeof(relative));
		Patch(from, jump, sizeof(jump));
	}
}
