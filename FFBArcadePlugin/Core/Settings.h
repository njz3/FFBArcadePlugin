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
#include <string>

namespace ffb
{
	// The [Settings] section of FFBPlugin.ini, in the folder of the game.
	// Its full path is taken the first time a setting is read, when the plugin is loaded:
	// the game may change its current directory later.
	class Settings
	{
	public:
		static int Int(const wchar_t* key, int defaultValue);
		static std::wstring String(const wchar_t* key, const wchar_t* defaultValue = L"");
		static std::string String(const char* key, const char* defaultValue = "");

		// Writes a key of the section; a null value removes the key
		static void Write(const char* key, const char* value);

		static const std::wstring& Path();
		static const std::string& PathA();
	};
}
