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

#include "AfterburnerClimax.h"

using namespace ffb;

void AfterburnerClimax::Update()
{
	if (!versionFound)
	{
		DWORD oldMark = Memory::Read(OldVersionMark);
		DWORD newMark = Memory::Read(NewVersionMark);

		if (oldMark == VersionMark)
		{
			oldVersion = true;
			versionFound = true;
		}

		if (newMark == VersionMark)
		{
			newVersion = true;
			versionFound = true;
		}
	}

	if (newVersion)
		outputs = Memory::Read(NewVersionOutputs);

	if (oldVersion)
		outputs = Memory::Read(OldVersionOutputs);

	Log::Value("got value:", outputs);

	if (outputs & 0x40)
	{
		double strength = rumbleStrength / 100.0;
		wheel.Rumble(strength, strength, 100);
	}
}
