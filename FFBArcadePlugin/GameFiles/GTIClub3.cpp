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

#include "GTIClub3.h"

using namespace ffb;

void GTIClub3::Update()
{
	UINT8 force = Memory::Read(WheelForce);
	UINT8 vibration = Memory::Read(Vibration);
	UINT8 loggedValue = Memory::Read(LoggedValue);
	UINT8 menu = Memory::Read(Menu);
	Log::Value("got value:", loggedValue);

	ApplyDamper();

	if (!menu)
	{
		UINT32 length_ms = 100;
		if (vibration > 0x00 && vibration < 0x40)
		{
			double percentForce = vibration / 63.0;
			wheel.Rumble(percentForce, percentForce, length_ms);
			wheel.Sine(120, 0, percentForce, length_ms);
		}

		if (force > 0x80 && force < 0x101)
		{
			double percentForce = (257 - force) / 128.0;
			wheel.Rumble(percentForce, 0, length_ms);
			wheel.Constant(Direction::FromRight, percentForce);
		}
		else if (force > 0x00 && force < 0x80)
		{
			double percentForce = (force) / 127.0;
			wheel.Rumble(0, percentForce, length_ms);
			wheel.Constant(Direction::FromLeft, percentForce);
		}
	}
}
