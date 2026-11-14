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

#include "BG4JP.h"

using namespace ffb;

void BG4JP::Update()
{
	UINT8 inRace = (UINT8)Memory::Read(InRace);
	Memory::Read(GroundContact);
	UINT8 wallContact = (UINT8)Memory::Read(WallContact);
	UINT8 carContact = (UINT8)Memory::Read(CarContact);
	UINT8 shiftEffect = (UINT8)Memory::Read(ShiftEffect);
	Memory::Read(CurrentView);
	float speed = Memory::Read(Speed);

	Log::Value("got value:", wallContact);

	ApplyDamper();

	double percentForce = speed / 180.00;
	if (percentForce > 1.0)
		percentForce = 1.0;

	if (inRace)
	{
		wheel.Spring(springStrength / 100.0);

		if (enableGearShiftEffect)
		{
			if (lastShiftEffect != shiftEffect && shiftEffect != 0x00)
			{
				double percentForceShift = gearChangeStrength / 100.0;
				wheel.Sine(gearChangeSinePeriod, 0, percentForceShift, 100);
				wheel.Rumble(0, percentForceShift, 100);
			}
		}

		if (enableBoostEffect)
		{
			if (wallContact & 0x02)
			{
				wheel.Sine(boostSinePeriod, boostFadeSinePeriod, percentForce, 100);
				wheel.Rumble(percentForce, percentForce, 100);
			}
		}

		if (carContact & 0x01)
		{
			wheel.Sine(120, 0, percentForce, 100);
			wheel.Rumble(percentForce, percentForce, 100);
		}

		if (wallContact & 0x10 || carContact & 0x08)
			wheel.ConstantWithRumble(Direction::FromLeft, percentForce);
		else if (wallContact & 0x20 || carContact & 0x02)
			wheel.ConstantWithRumble(Direction::FromRight, percentForce);
	}

	lastShiftEffect = shiftEffect;
}
