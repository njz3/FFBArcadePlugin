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

#include "WackyRaces.h"

using namespace ffb;

int WackyRaces::ForceLevel(int word)
{
	switch (word) {
		// moving left, from weakest to strongest (30 => 16).
	case 4096:
		return 30;
	case 64:
		return 29;
	case 4160:
		return 28;
	case 2048:
		return 27;
	case 6144:
		return 26;
	case 2112:
		return 25;
	case 6208:
		return 24;
	case 32:
		return 23;
	case 4128:
		return 22;
	case 96:
		return 21;
	case 4192:
		return 20;
	case 2080:
		return 19;
	case 6176:
		return 18;
	case 2144:
		return 17;
	case 6240:
		return 16;
		// moving right, from weakest to strongest (15 => 1)
	case 12288:
		return 15;
	case 8256:
		return 14;
	case 12352:
		return 13;
	case 10240:
		return 12;
	case 14336:
		return 11;
	case 10304:
		return 10;
	case 14400:
		return 9;
	case 8224:
		return 8;
	case 12320:
		return 7;
	case 8288:
		return 6;
	case 12384:
		return 5;
	case 10272:
		return 4;
	case 14368:
		return 3;
	case 10336:
		return 2;
	case 14432:
		return 1;
	default:
		return 0;
	}
}

void WackyRaces::Update()
{
	long output = Memory::Read(Output);
	int word = Memory::Read<int>(output + ForceAndLamps);
	for (int lamp : Lamps)
	{
		if ((word & lamp) == lamp)
			word -= lamp;
	}
	int ff = ForceLevel(word);

	ApplyDamper();

	if (ff > 15)
	{
		Log::Write("moving wheel left");
		// assume that 30 is the weakest and 16 is the strongest
		wheel.ConstantWithRumble(Direction::FromRight, (31 - ff) / 15.0);
	}
	else if (ff > 0)
	{
		Log::Write("moving wheel right");
		// assume that 1 is the strongest and 15 is the weakest
		wheel.ConstantWithRumble(Direction::FromLeft, (16 - ff) / 15.0);
	}
}
