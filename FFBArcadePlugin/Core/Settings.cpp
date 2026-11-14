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

#include <Windows.h>
#include "Settings.h"

namespace ffb
{
	static std::wstring FullPathW(const wchar_t* fileName)
	{
		wchar_t path[MAX_PATH];
		DWORD length = GetFullPathNameW(fileName, MAX_PATH, path, nullptr);
		return (length == 0 || length >= MAX_PATH) ? fileName : path;
	}

	static std::string FullPathA(const char* fileName)
	{
		char path[MAX_PATH];
		DWORD length = GetFullPathNameA(fileName, MAX_PATH, path, nullptr);
		return (length == 0 || length >= MAX_PATH) ? fileName : path;
	}

	const std::wstring& Settings::Path()
	{
		static const std::wstring path = FullPathW(L".\\FFBPlugin.ini");
		return path;
	}

	const std::string& Settings::PathA()
	{
		static const std::string path = FullPathA(".\\FFBPlugin.ini");
		return path;
	}

	int Settings::Int(const wchar_t* key, int defaultValue)
	{
		return GetPrivateProfileIntW(L"Settings", key, defaultValue, Path().c_str());
	}

	std::wstring Settings::String(const wchar_t* key, const wchar_t* defaultValue)
	{
		wchar_t value[256];
		GetPrivateProfileStringW(L"Settings", key, defaultValue, value, 256, Path().c_str());
		return value;
	}

	std::string Settings::String(const char* key, const char* defaultValue)
	{
		char value[256];
		GetPrivateProfileStringA("Settings", key, defaultValue, value, 256, PathA().c_str());
		return value;
	}

	void Settings::Write(const char* key, const char* value)
	{
		WritePrivateProfileStringA("Settings", key, value, PathA().c_str());
	}
}
