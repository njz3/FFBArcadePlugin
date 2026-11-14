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

#include "GRIDCustom.h"

using namespace ffb;

void GRIDCustom::Update()
{
	float wallSide = Memory::Read(WallSide);
	INT_PTR panel = Memory::Read(Panel);
	UINT8 collision = Memory::Read<UINT8>(panel + Collision);
	UINT8 wheels = Memory::Read<UINT8>(panel + Wheels);
	UINT8 skids = Memory::Read<UINT8>(panel + Skids);
	UINT8 ai = Memory::Read<UINT8>(panel + AI);
	UINT8 gear = Memory::Read(Gear);
	float speed = Memory::Read(Speed);

	UINT8 newGear = gear;

	wheel.KeptSpring(springStrength / 100.0);

	ApplyDamper();

	if ((oldGear != newGear) && (speed > 0))
	{
		gearShift = true;
	}

	if (gearShift)
	{
		Log::Write("gear change");
		double percentForce = gearChangeStrength / 100.0;
		gearChangeWheel->Sine(gearChangeLength, 0, percentForce, 150);
		gearChangeWheel->Rumble(0, percentForce, 150);
		gearShift = false;
	}

	UINT32 length_ms = 100;
	if (wheels > 0)
	{
		double percentForce = wheels / 37.0;
		wheel.Rumble(percentForce, percentForce, length_ms);
		wheel.Sine(80, 80, percentForce, length_ms);
	}

	if ((skids > 12) && (speed > 0))
	{
		double percentForce = ((skids - 12) / 8.0);
		wheel.Rumble(percentForce, 0, length_ms);
	}

	if ((ai > 0) && (collision > 0))
	{
		double percentForce = (collision) / 8.0;
		wheel.Rumble(percentForce, percentForce, length_ms);
	}

	if ((wallSide > 0) && (collision > 0))
	{
		double percentForce = (collision) / 8.0;
		wheel.Rumble(percentForce, 0, length_ms);
		wheel.Constant(Direction::FromRight, percentForce);
	}
	else if ((wallSide < 0) && (collision > 0))
	{
		double percentForce = (collision) / 8.0;
		wheel.Rumble(0, percentForce, length_ms);
		wheel.Constant(Direction::FromLeft, percentForce);
	}
	oldGear = newGear;
	gearChangeWheel = &wheel;
}
