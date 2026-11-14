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

#include "IDTAv231.h"

using namespace ffb;

void InitialDTA231::Update()
{
	ApplyDamper();

	DWORD command = Memory::Read(DriveBoardCommand);
	WORD strength = Memory::Read(Strength);

	const BYTE* bytes = reinterpret_cast<const BYTE*>(&command);

	// The centering command checks the strength low byte only
	if (bytes[0] == 0x80 && bytes[2] == 0x01)
		wheel.Spring(1.0);
	else
		driveBoard.Play(bytes[0], bytes[1], strength);
}
