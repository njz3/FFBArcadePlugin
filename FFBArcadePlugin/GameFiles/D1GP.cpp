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

#include "D1GP.h"

using namespace ffb;

void D1GP::Update()
{
	float ffbConstant = TeknoParrotFFB() / 1000.0f;
	int effect = TeknoParrotFFB2();
	float speed = Memory::Read(Speed);

	const UINT32 length = 100;

	ApplyDamper();

	if (ffbConstant > 0)
	{
		double percentForce = ffbConstant * 5.0;

		if (percentForce > 1.0)
			percentForce = 1.0;

		wheel.ConstantWithRumble(Direction::FromRight, percentForce);
	}
	else if (ffbConstant < 0)
	{
		double percentForce = -ffbConstant * 5.0;

		if (percentForce > 1.0)
			percentForce = 1.0;

		wheel.ConstantWithRumble(Direction::FromLeft, percentForce);
	}

	double percentForce = speed / 200.0;

	if (percentForce > 1.0)
		percentForce = 1.0;

	if (effect != oldEffect)
	{
		switch (effect)
		{
		case 0x01:
			wheel.Spring(0.8);
			break;
		case 0x02:
			wheel.Rumble(0.3, 0.3, length);
			wheel.Sine(35, 0, 0.3, length);
			break;
		case 0x04:
			letsShake = true;
			break;
		}
	}

	if (enableGroundEffects == 1 && speed > 0.1)
	{
		switch (effect)
		{
		case 0x08:
		case 0x20:
		case 0x80:
		case 0x200: // grass
			wheel.Rumble(percentForce, percentForce, length);
			wheel.Sine(ffbGrassSinePeriod, ffbGrassFadeSinePeriod, percentForce, length);
			break;
		case 0x10:
		case 0x40:
		case 0x100:
		case 0x400: // rumble strip
			wheel.Rumble(percentForce, percentForce, length);
			wheel.Sine(ffbRumbleStripSinePeriod, ffbRumbleStripFadeSinePeriod, percentForce, length);
			break;
		}
	}

	if (letsShake)
	{
		++shakeCount;

		if (shakeCount >= 32)
		{
			shakeCount = 0;
			letsShake = false;
		}

		wheel.Rumble(0.6, 0.6, length);
		wheel.Sine(70, 60, 0.4, length);
	}

	oldEffect = effect;
}
