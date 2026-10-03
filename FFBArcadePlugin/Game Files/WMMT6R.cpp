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

#include "WMMT6R.h"

static const WMMTAddresses wmmt6RAddresses = {
	0x2047128, // ffbValues
	0x204809C, // speed
	0x204820C, // rpm
	0x20681C0, // carState
	0x3AC,     // gearOffset
	0x20681D8, // raceState
	0,         // raceTimer
};

WMMT6R::WMMT6R() : WMMT(wmmt6RAddresses)
{
}
