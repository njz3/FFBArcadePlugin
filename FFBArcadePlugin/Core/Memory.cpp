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

	static char* FindInBuffer(const char* pattern, const char* mask, char* begin, SIZE_T size)
	{
		size_t patternLength = strlen(mask);
		if (size < patternLength)
			return nullptr;

		for (SIZE_T i = 0; i < size - patternLength; i++)
		{
			bool found = true;
			for (size_t j = 0; j < patternLength; j++)
			{
				if (mask[j] != '?' && pattern[j] != begin[i + j])
				{
					found = false;
					break;
				}
			}
			if (found)
				return begin + i;
		}
		return nullptr;
	}

	char* Memory::FindPattern(const char* pattern, const char* mask, const char* skip)
	{
		SYSTEM_INFO sysInfo;
		GetSystemInfo(&sysInfo);
		char* end = (char*)sysInfo.lpMaximumApplicationAddress;
		HANDLE process = GetCurrentProcess();
		char* chunk = nullptr;
		char* match = nullptr;

		while (chunk < end)
		{
			MEMORY_BASIC_INFORMATION mbi;
			if (!VirtualQueryEx(process, chunk, &mbi, sizeof(mbi)))
				return nullptr;

			if (mbi.State == MEM_COMMIT && mbi.Protect != PAGE_NOACCESS)
			{
				char* buffer = new char[mbi.RegionSize];
				DWORD oldProtect;
				SIZE_T bytesRead = 0;
				bool found = false;
				if (VirtualProtectEx(process, mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &oldProtect))
				{
					ReadProcessMemory(process, mbi.BaseAddress, buffer, mbi.RegionSize, &bytesRead);
					VirtualProtectEx(process, mbi.BaseAddress, mbi.RegionSize, oldProtect, &oldProtect);

					char* inBuffer = FindInBuffer(pattern, mask, buffer, bytesRead);
					if (inBuffer != nullptr)
					{
						match = chunk + (inBuffer - buffer);
						found = match != skip;
					}
				}
				delete[] buffer;
				if (found)
					break;
			}

			chunk += mbi.RegionSize;
		}
		return match;
	}
}
