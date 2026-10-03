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

#include "WMMT5DX+.h"

static const WMMTAddresses wmmt5DXPlusAddresses = {
	0x1FA6F40, // ffbValues
	0x1FA7C7C, // speed
	0x1FA7DEC, // rpm
	0x1FD11B0, // carState
	0x398,     // gearOffset
	0x1FD11C8, // raceState
	0,         // raceTimer
};

WMMT5DXPlus::WMMT5DXPlus() : WMMT(wmmt5DXPlusAddresses)
{
}

UINT8 WMMT5DXPlus::ReadPower(Helpers* helpers)
{
	INT_PTR ptr1 = helpers->ReadIntPtr(0x1F7D578, true);
	INT_PTR ptr2 = helpers->ReadIntPtr(ptr1 + 0x268, false);
	return helpers->ReadByte(ptr2 + 0xAC, false);
}
