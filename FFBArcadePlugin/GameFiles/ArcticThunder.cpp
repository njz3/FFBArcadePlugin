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

#include "ArcticThunder.h"

extern int EnableForceSpringEffect;
extern int ForceSpringStrength;

using namespace ffb;

void ArcticThunder::Update()
{
	ApplyDamper();

	if (EnableForceSpringEffect)
		wheel.KeptSpring(ForceSpringStrength / 100.0);

	if (TeknoParrotFFB2() != 0xFF && TeknoParrotFFB2())
	{
		float vibration = ((float)TeknoParrotFFB2() / 255.0f);
		wheel.Sine(50, 0, vibration, 100);
		wheel.Rumble(0, vibration, 100);
	}

	int force = TeknoParrotFFB3();

	if (force)
	{
		if (force > 0x80)
		{
			double percentForce = ((force - 128.0) / 127.0);

			wheel.Rumble(0, percentForce, 100);
			wheel.Constant(Direction::FromLeft, percentForce);
		}
		else if (force < 0x80)
		{
			double percentForce = ((128.0 - force) / 127.0);

			wheel.Rumble(percentForce, 0, 100);
			wheel.Constant(Direction::FromRight, percentForce);
		}
	}
}
