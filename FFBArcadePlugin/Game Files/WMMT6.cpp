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

#include "WMMT6.h"

static const WMMTAddresses wmmt6Addresses = {
	0x1E3C610, // ffbValues
	0x1E3D4DC, // speed
	0x1E3D64C, // rpm
	0x1E5B5B0, // carState
	0x3A8,     // gearOffset
	0x1E5B5C8, // raceState
	0x1E5B894, // raceTimer
};

WMMT6::WMMT6() : WMMT(wmmt6Addresses)
{
}

void WMMT6::Init()
{
	StartInputThread();
}

// The race state starts with the total number of checkpoints
UINT8 WMMT6::FinishCheckpoint(Helpers* helpers, INT_PTR raceState)
{
	return helpers->ReadByte(raceState + 0x00, false);
}
