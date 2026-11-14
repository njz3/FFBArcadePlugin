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
#include "WMMT.h"

// Wangan Midnight Maximum Tune 5DX (Bandai Namco, TeknoParrot): see WMMT
class WMMT5DX : public WMMT
{
public:
	WMMT5DX() : WMMT({
		0x1C02750, // ffbValues
		0x1C0342C, // speed
		0x1C0359C, // rpm
		0x1C2C040, // carState
		0x398,     // gearOffset
		0x1C2C058, // raceState
		0,         // raceTimer
	}) {}
};
