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

#include <initializer_list>
#include "TeknoParrot.h"
#include "GameIds.h"
#include "Config.h"

static ffb::Config& config = ffb::Config::Get();


namespace ffb
{
	// FFB values published by TeknoParrot in the JvsState shared memory
	static const int FFB_SLOT = 2;
	static const int FFB_EXTENDED_SLOT = 6; // followed by 3 more values in slots 7, 8 and 9

	// Games publishing their main FFB value in the extended slot instead of the standard one
	static bool UsesExtendedFFBSlot()
	{
		return config.gameId == POKKEN_TOURNAMENT || config.gameId == SRG || config.gameId == D1_GP || config.gameId == WMMT_3 || config.gameId == DEAD_HEAT;
	}

	TeknoParrotGame::TeknoParrotGame()
	{
		jvsState = nullptr;
		section = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, 64, L"TeknoParrot_JvsState");
		if (section)
			jvsState = (int*)MapViewOfFile(section, FILE_MAP_ALL_ACCESS, 0, 0, 64);

		if (jvsState)
		{
			for (int slot : { FFB_SLOT, FFB_EXTENDED_SLOT, FFB_EXTENDED_SLOT + 1, FFB_EXTENDED_SLOT + 2, FFB_EXTENDED_SLOT + 3 })
			{
				if (jvsState[slot] > 0)
					jvsState[slot] = 0;
			}
		}
	}

	int TeknoParrotGame::ReadSlot(int slot)
	{
		return jvsState ? jvsState[slot] : 0;
	}

	int TeknoParrotGame::TeknoParrotFFB()
	{
		return ReadSlot(UsesExtendedFFBSlot() ? FFB_EXTENDED_SLOT : FFB_SLOT);
	}

	int TeknoParrotGame::TeknoParrotFFB2()
	{
		return ReadSlot(FFB_EXTENDED_SLOT + 1);
	}

	int TeknoParrotGame::TeknoParrotFFB3()
	{
		return ReadSlot(FFB_EXTENDED_SLOT + 2);
	}

	int TeknoParrotGame::TeknoParrotFFB4()
	{
		return ReadSlot(FFB_EXTENDED_SLOT + 3);
	}
}
