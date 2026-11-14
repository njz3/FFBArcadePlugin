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

#include "DeadHeat.h"

using namespace ffb;

void NamcoWheelGame::Update()
{
	int spring = TeknoParrotFFB();
	int viscosity = TeknoParrotFFB2();
	int centerOffset = TeknoParrotFFB3();
	int reflect = TeknoParrotFFB4();

	ApplyDamper();

	if (spring)
	{
		double percentForce = (spring / 63.0) * (springStrength / 100.0);
		wheel.Spring(percentForce);
	}

	if (viscosity)
	{
		double percentForce = (viscosity / 63.0) * (viscosityStrength / 100.0);
		wheel.Friction(percentForce);
	}

	if (reflect == 0x00 && centerOffset == 0x00)
	{
		wheel.Rumble(0, 0, 0);
		wheel.StopConstant();
	}
	else
	{
		PlayReflect(reflect);

		if (centerOffset > 0x00 && centerOffset <= 0x3F)
		{
			double percentForce = (centerOffset / 63.0) * (centerOffsetStrength / 100.0);
			wheel.ConstantWithRumble(Direction::FromLeft, percentForce);
		}
		else if (centerOffset < 0x00)
		{
			// in unsigned arithmetic: -centerOffset
			double percentForce = (((0xFFFFFFFF - centerOffset) + 1) / 63.0) * (centerOffsetStrength / 100.0);
			wheel.ConstantWithRumble(Direction::FromRight, percentForce);
		}
	}
}

void DeadHeat::PlayReflect(int reflect)
{
	if (reflect > 0x00 && reflect <= 0x3F)
	{
		double percentForce = (reflect / 63.0) * (reflectStrength / 100.0);
		wheel.Rumble(percentForce, 0, 100);
		wheel.Constant(Direction::FromRight, percentForce);
	}
	else if (reflect > 0x3F)
	{
		double percentForce = ((0xFF - reflect) / 63.0) * (reflectStrength / 100.0);
		wheel.Rumble(0, percentForce, 100);
		wheel.Constant(Direction::FromLeft, percentForce);
	}
}
