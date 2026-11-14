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

#include <sstream>
#include "SegaRaceTV.h"
#include "../Core/Config.h"

static ffb::Config& config = ffb::Config::Get();

using namespace ffb;


void SegaRaceTV::Update()
{
	if (!ioEnabled)
	{
		OutputDebugStringA("SegaRaceTV::FFBLoop");
		Memory::Write<INT_PTR>(IoCallsEnabled, 0x1);
		ioEnabled = true;
	}

	int ff = Memory::Read(WheelForce);

	ApplyDamper();
	if (config.enableForceSpringEffect)
		wheel.KeptSpring(config.forceSpringStrength / 100.0);

	if (ff > 0 && ff < 128)
	{
		double strength = ((128.0 - ff) / 127.0);

		std::ostringstream ss;
		ss << "LEFT: " << strength << " FF: " << ff;
		OutputDebugStringA(ss.str().c_str());

		// the second rumble motor for a force from the left
		wheel.Rumble(0, strength, 100);
		wheel.Constant(Direction::FromLeft, strength);
	}
	else if (ff > 128)
	{
		double strength = ((ff - 128.0) / 127.0);

		std::ostringstream ss;
		ss << "RIGHT: " << strength << " FF: " << ff;
		OutputDebugStringA(ss.str().c_str());

		// the first rumble motor for a force from the right
		wheel.Rumble(strength, 0, 100);
		wheel.Constant(Direction::FromRight, strength);
	}
}
