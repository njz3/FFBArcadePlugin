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
#include "SWDC2018.h"

using namespace ffb;

bool SWDC::IsGameName(DWORD value)
{
	char name[5] = { 0 };
	memcpy(name, &value, 4);
	return strcmp(name, "SDDS") == 0;
}

void SWDC::Update()
{
	if (GetAsyncKeyState((VK_ESCAPE)) && (escapeKeyExitViaPlugin == 1))
		ExitProcess(0);

	if (!versionFound)
	{
		DWORD name103 = Memory::Read(Name103);
		DWORD name230 = Memory::Read(Name230);

		if (IsGameName(name103))
		{
			version103 = true;
			versionFound = true;
		}

		if (IsGameName(name230))
		{
			version230 = true;
			versionFound = true;
		}
	}
	else
	{
		if (version103)
			command = Memory::Read(DriveBoardCommand103);

		if (version230)
			command = Memory::Read(DriveBoardCommand230);

		ApplyDamper();

		BYTE* bytes = reinterpret_cast<BYTE*>(&command);

		// the damper is played twice, as it always was
		ApplyDamper();

		driveBoard.Play(bytes[0], bytes[1], bytes[2]);
	}
}
