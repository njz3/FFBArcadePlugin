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

#include "StormRacerG.h"

using namespace ffb;

void StormRacerG::Update()
{
	float ffbConstant = (float)TeknoParrotFFB() / 1000.0f;
	int shake = TeknoParrotFFB2();
	float ground = (float)TeknoParrotFFB3();
	float speed = Memory::Read(Speed);
	UINT8 racing = Memory::Read(Racing);

	ApplyDamper();

	if (enableForceSpringEffect == 1)
	{
		wheel.KeptSpring(forceSpringStrength / 100.0);
	}
	else if (enableForceSpringEffect == 2)
	{
		if (racing == 0x00)
		{
			wheel.Spring(forceSpringStrength / 100.0);
		}
	}

	if (ffbConstant > 0)
	{
		double percentForce = ffbConstant / (minFFBConstantStrength / 2.173913043478261);

		if (percentForce > 1.0)
			percentForce = 1.0;

		wheel.ConstantWithRumble(Direction::FromRight, percentForce);
	}
	else if (ffbConstant < 0)
	{
		double percentForce = -ffbConstant / (minFFBConstantStrength / 2.173913043478261);

		if (percentForce > 1.0)
			percentForce = 1.0;

		wheel.ConstantWithRumble(Direction::FromLeft, percentForce);
	}

	if (enableShakeEffectsWhileRacing == 1)
	{
		if (racing == 0x01)
		{
			if (shake > 0)
			{
				double percentForce = ((shake / 1500.0) * (ffbShakeSineStrength / 100.0));

				if (percentForce > 1.0)
					percentForce = 1.0;

				wheel.Rumble(percentForce, percentForce, 100);
				wheel.Sine(ffbShakeSinePeriod, ffbShakeFadeSinePeriod, percentForce, 100);
			}
		}
	}

	if (enableShakeEffectsinMenu == 1)
	{
		if (racing == 0x00)
		{
			if (shake > 0)
			{
				double percentForce = ((shake / 1500.0) * (ffbShakeSineStrength / 100.0));

				if (percentForce > 1.0)
					percentForce = 1.0;

				wheel.Rumble(percentForce, percentForce, 100);
				wheel.Sine(ffbShakeSinePeriod, ffbShakeFadeSinePeriod, percentForce, 100);
			}
		}
	}

	if (enableGroundEffects == 1)
	{
		if (speed > 1.0)
		{
			if (ground < 10000.0f) // Driving on Road (Not implemented yet)
			{
				ground = ground / 1000.0f;
			}
			else if (ground < 300000.0f) // Driving on Grass
			{
				ground = ((ground - 10000.0f) / 1000.0f);

				ground = ground * (ffbGrassSineStrength / 100.0f);

				wheel.Rumble(ground, ground, 100);
				wheel.Sine(ffbGrassSinePeriod, ffbGrassFadeSinePeriod, ground, 100);
			}
			else if (ground < 700000.0f) // Driving on Rough Ground
			{
				ground = ((ground - 300000.0f) / 1000.0f);

				ground = ground * (ffbRoughTerrainSineStrength / 100.0f);

				wheel.Rumble(ground, ground, 100);
				wheel.Sine(ffbRoughTerrainSinePeriod, ffbRoughTerrainFadeSinePeriod, ground, 100);
			}
			else
			{
				ground = ((ground - 700000.0f) / 1000.0f); // Driving on Rumble Strip

				ground = ground * (ffbRumbleStripSineStrength / 100.0f);

				wheel.Rumble(ground, ground, 100);
				wheel.Sine(ffbRumbleStripSinePeriod, ffbRumbleStripFadeSinePeriod, ground, 100);
			}
		}
	}
}
