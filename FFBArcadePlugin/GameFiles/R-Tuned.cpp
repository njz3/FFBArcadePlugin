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

#include "R-Tuned.h"

using namespace ffb;

void RTuned::Update()
{
	UINT8 springCrash = Memory::Read(SpringCrash);
	UINT8 boostStrength = Memory::Read(BoostStrength);
	UINT8 boost = Memory::Read(Boost);
	Log::Value("got value:", boostStrength);

	ApplyDamper();

	if (Memory::Read(FFBState) == 0x01)
		Memory::Write<UINT8>(FFBState, 0x09);

	if (springCrash == 0xFF)
	{
		wheel.Sine(300, 300, 1.0, 100);
		wheel.Rumble(1.0, 1.0, 100);
	}
	else if (springCrash == 0x04)
	{
		wheel.Spring(springStrength / 100.0);
	}

	if (boost == 0x7F)
	{
		double percentForce = (boostStrength / 400.0);
		wheel.Rumble(percentForce, percentForce, 100);
		wheel.Sine(60, 60, percentForce, 100);
	}
}
