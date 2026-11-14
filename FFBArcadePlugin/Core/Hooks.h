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

#pragma once
#include <Windows.h>
#include "../Common Files/MinHook.h"

namespace ffb
{
	// Functions of the game (or of Windows) replaced by functions of the plugin, with MinHook.
	// A detour calls `original` to run the replaced function.
	class Hooks
	{
	public:
		static bool Initialize()
		{
			return MH_Initialize() == MH_OK;
		}

		template <typename F>
		static bool Create(LPVOID target, F detour, F* original)
		{
			return MH_CreateHook(target, (LPVOID)detour, (LPVOID*)original) == MH_OK;
		}

		template <typename F>
		static bool Create(INT_PTR target, F detour, F* original)
		{
			return Create((LPVOID)target, detour, original);
		}

		// A function exported by a DLL, e.g. CreateApi(L"user32.dll", "GetCursorPos", ...)
		template <typename F>
		static bool CreateApi(LPCWSTR module, LPCSTR function, F detour, F* original)
		{
			return MH_CreateHookApi(module, function, (LPVOID)detour, (LPVOID*)original) == MH_OK;
		}

		// Enables a hook, or all of them
		static bool Enable(LPVOID target = MH_ALL_HOOKS)
		{
			return MH_EnableHook(target) == MH_OK;
		}
	};
}
