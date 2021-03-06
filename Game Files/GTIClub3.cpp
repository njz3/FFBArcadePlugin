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

#include <string>
#include "GTIClub3.h"
#include "math.h"

void GTIClub3::FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers) {

	UINT8 ff = helpers->ReadByte(0x00918CBC, /* isRelativeOffset */ false);
	UINT8 ff1 = helpers->ReadByte(0x00918CBD, /* isRelativeOffset */ false);
	UINT8 ff2 = helpers->ReadByte(0x00918CBE, /* isRelativeOffset */ false);
	UINT8 menu = helpers->ReadByte(0x518E8E, /* isRelativeOffset */ true);
	helpers->log("got value: ");
	std::string ffs = std::to_string(ff2);
	helpers->log((char*)ffs.c_str());

	if ((ff1 > 0x00)& (ff1 < 0x40)& (menu == 0))
	{
		double percentForce = (ff1) / 63.0;
		double percentLength = 100;
		triggers->Rumble(percentForce, percentForce, percentLength);
		triggers->Sine(120, 120, percentForce);
	}
	if ((ff > 0x80)& (ff < 0x101)& (menu == 0))
	{
		helpers->log("moving wheel right");
		double percentForce = (257 - ff) / 128.0;
		double percentLength = 100;
		triggers->Rumble(percentForce, 0, percentLength);
		triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
	}
	else if ((ff > 0x00)& (ff < 0x80)& (menu == 0))
	{
		helpers->log("moving wheel left");
		double percentForce = (ff) / 127.0;
		double percentLength = 100;
		triggers->Rumble(0, percentForce, percentLength);
		triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
	}
}