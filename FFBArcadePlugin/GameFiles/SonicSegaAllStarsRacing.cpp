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

#include "SonicSegaAllStarsRacing.h"

using namespace ffb;

void SonicSegaAllStarsRacing::Update()
{
	UINT8 ff = Memory::Read(WheelForce);

	Memory::Write<UINT8>(ForceMode, 0x03);

	ApplyDamper();

	if (springEnable)
		wheel.KeptSpring(springStrength / 100.0);

	if (lastForce != ff)
	{
		if (ff > 0 && ff < 19)
		{
			Log::Write("moving wheel left");
			wheel.ConstantWithRumble(Direction::FromRight, ff / 18.0);
		}
		else if (ff > 237 && ff < 256)
		{
			Log::Write("moving wheel right");
			wheel.ConstantWithRumble(Direction::FromLeft, (256 - ff) / 18.0);
		}
		else
		{
			wheel.Rumble(0, 0, 80);
			wheel.StopConstant();
		}
	}
	lastForce = ff;
}
