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

#include <math.h>
#include "SegaRacingClassic.h"

using namespace ffb;

void SegaRacingClassic::Update()
{
	UINT8 ff = Memory::Read(DriveBoardCommand);
	Log::Value("got value:", ff);

	ApplyDamper();

	const UINT32 length_ms = 100;
	if ((ff > 0xD7) && (ff < 0xE0))
	{
		// Clutch
		double strength = (224 - ff) / 8.0;
		wheel.Friction(strength);
	}

	if ((ff > 0xBF) && (ff < 0xC8))
	{
		// Centering
		double strength = (200 - ff) / 8.0;
		wheel.Spring(pow(strength, 0.1));
	}

	if ((ff > 0xB7) && (ff < 0xC0))
	{
		// Uncentering
		double strength = (192 - ff) / 8.0;
		wheel.Rumble(strength, strength, length_ms);
		wheel.Sine(70, 80, strength, length_ms);
	}

	if ((ff > 0xA7) && (ff < 0xB0))
	{
		// Roll left
		double strength = (176 - ff) / 8.0;
		wheel.ConstantWithRumble(Direction::FromRight, strength);
	}

	if ((ff > 0x97) && (ff < 0xA0))
	{
		// Roll right
		double strength = (160 - ff) / 8.0;
		wheel.ConstantWithRumble(Direction::FromLeft, strength);
	}
}
