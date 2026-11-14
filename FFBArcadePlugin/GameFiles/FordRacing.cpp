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

#include "FordRacing.h"

using namespace ffb;

void FordRacing::Update()
{
	Log::Write("in FR Ffbloop");
	const int ff = TeknoParrotFFB();
	Log::Printf("%d", ff);

	ApplyDamper();

	UINT32 length_ms = 50;
	if (ff < -65505 && ff > -65515)
	{
		Log::Write("moving wheel left");
		double percentForce = (-65505 - ff) / 9.0;
		wheel.Rumble(0, percentForce, length_ms);
		wheel.Constant(Direction::FromRight, percentForce);
	}
	else if (ff > 0 && ff < 16)
	{
		Log::Write("moving wheel right");
		double percentForce = (16 - ff) / 9.0;
		wheel.Rumble(percentForce, 0, length_ms);
		wheel.Constant(Direction::FromLeft, percentForce);
	}
}
