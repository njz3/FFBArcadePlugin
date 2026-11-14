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

// Wangan Midnight Maximum Tune 5DX+ (Bandai Namco, TeknoParrot): see WMMT. Its power tune is read for the
// wheel spin.
class WMMT5DXPlus : public WMMT
{
public:
	WMMT5DXPlus() : WMMT({
		0x1FA6F40, // ffbValues
		0x1FA7C7C, // speed
		0x1FA7DEC, // rpm
		0x1FD11B0, // carState
		0x398,     // gearOffset
		0x1FD11C8, // raceState
		0,         // raceTimer
	}) {}

protected:
	UINT8 ReadPower() override
	{
		return ffb::Memory::Read(Power);
	}

private:
	// Power tune of the player car, 0x10 full
	static constexpr ffb::PointerChain<UINT8, 2> Power{ { ffb::Base::Module, 0x1F7D578 }, { 0x268, 0xAC } };
};
