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

#include "ChaseHQ2.h"

using namespace ffb;

int ChaseHQ2::ForceLevel(int word)
{
	switch (word) {
	case 28672:
		return 30;
	case 24640:
		return 29;
	case 28736:
		return 28;
	case 16624:
		return 27;
	case 30720:
		return 26;
	case 26688:
		return 25;
	case 30784:
		return 24;
	case 24608:
		return 23;
	case 28704:
		return 22;
	case 24672:
		return 21;
	case 28768:
		return 20;
	case 26656:
		return 19;
	case 30752:
		return 18;
	case 26720:
		return 17;
	case 30816:
		return 16;

	case 20480:
		return 15;
	case 16448:
		return 14;
	case 20544:
		return 13;
	case 18432:
		return 12;
	case 22528:
		return 11;
	case 18496:
		return 10;
	case 22592:
		return 9;
	case 16416:
		return 8;
	case 20512:
		return 7;
	case 16480:
		return 6;
	case 20576:
		return 5;
	case 18464:
		return 4;
	case 22560:
		return 3;
	case 18528:
		return 2;
	case 22624:
		return 1;
	default:
		return 0;
	}
}

void ChaseHQ2::Update()
{
	long output = Memory::Read(Output);
	int word = Memory::Read<int>(output + ForceAndLamps);
	for (int lamp : Lamps)
	{
		if ((word & lamp) == lamp)
			word -= lamp;
	}
	int ff = ForceLevel(word);

	if (removeCelShadedFilter)
	{
		if (!celShadedFilterCodeRemoved)
		{
			Memory::WriteNop(Memory::Address(Base::Module, CelShadedFilterCode), 6);
			celShadedFilterCodeRemoved = true;
		}
		Memory::Write<UINT8>(CelShadedFilter, 0x00); // Remove Cel Shaded Filter
	}

	ApplyDamper();

	Log::Value("got value:", ff);

	if (ff > 15)
	{
		Log::Write("moving wheel right");
		wheel.ConstantWithRumble(Direction::FromLeft, (31 - ff) / 15.0);
	}
	else if (ff > 0)
	{
		Log::Write("moving wheel left");
		wheel.ConstantWithRumble(Direction::FromRight, (16 - ff) / 15.0);
	}
}
