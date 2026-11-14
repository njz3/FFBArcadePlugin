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
#include <stdarg.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include "Log.h"
#include "Settings.h"

namespace ffb
{
	static const std::string& LogPath()
	{
		static const std::string path = [] {
			char full[MAX_PATH];
			DWORD length = GetFullPathNameA("FFBlog.txt", MAX_PATH, full, nullptr);
			return (length == 0 || length >= MAX_PATH) ? std::string("FFBlog.txt") : std::string(full);
		}();
		return path;
	}

	bool Log::Enabled()
	{
		static const bool enabled = Settings::Int(L"Logging", 0) != 0;
		return enabled;
	}

	void Log::Write(const char* text)
	{
		if (!Enabled())
			return;
		std::ofstream ofs(LogPath(), std::ofstream::app);
		ofs << text << std::endl;
	}

	void Log::Printf(const char* format, ...)
	{
		if (!Enabled())
			return;
		char text[1024];
		va_list args;
		va_start(args, format);
		vsnprintf(text, sizeof(text), format, args);
		va_end(args);
		Write(text);
	}

	void Log::Value(const char* label, int value)
	{
		Printf("%s %d", label, value);
	}

	void Log::Value(const char* label, double value)
	{
		Printf("%s %f", label, value);
	}
}
