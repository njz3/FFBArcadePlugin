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

#include <string>
#include "SWDC2018.h"
#include "../Common Files/InitialDFFB.h"

static bool VersionFound;
static bool Version103;
static bool Version230;

static DWORD FFB;

static int EscapeKeyExitViaPlugin = GetSettingInt(TEXT("EscapeKeyExitViaPlugin"), 0);

static const char* SDDS = "SDDS";

static void getSDDS103(char* Name)
{
	if (strcmp(Name, SDDS) == 0)
	{
		Version103 = true;
		VersionFound = true;
	}
}

static void getSDDS230(char* Name)
{
	if (strcmp(Name, SDDS) == 0)
	{
		Version230 = true;
		VersionFound = true;
	}
}

void SWDC::FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers) {

	if (GetAsyncKeyState((VK_ESCAPE)) && (EscapeKeyExitViaPlugin == 1))
		ExitProcess(0);

	if (!VersionFound)
	{
		DWORD SDDS103 = helpers->ReadInt32(0x8E5CBD0, true);
		DWORD SDDS230 = helpers->ReadInt32(0x9BC7B20, true);

		char Version103[256] = { 0 };
		std::memcpy(Version103, &SDDS103, 4);
		std::string myString103(Version103);

		char Version230[256] = { 0 };
		std::memcpy(Version230, &SDDS230, 4);
		std::string myString230(Version230);

		getSDDS103((char*)myString103.c_str());
		getSDDS230((char*)myString230.c_str());
	}
	else
	{
		if (Version103)
			FFB = helpers->ReadInt32(0x8E5CCF4, true);

		if (Version230)
			FFB = helpers->ReadInt32(0x9BC7B18, true);

		ApplyDamper(triggers);

		BYTE* ffb = reinterpret_cast<BYTE*>(&FFB);

		ApplyDamper(triggers);

		PlayInitialDFFB(ffb[0], ffb[1], ffb[2], InitialDFFBDefaultSettings, constants, triggers);
	}
}