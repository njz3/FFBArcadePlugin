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

#include "SegaInitialD.h"
#include "../Core/Log.h"

namespace ffb
{
	const SegaInitialDDriveBoard::Scaling SegaInitialDDriveBoard::DefaultScaling = { 127.0, 128, 127.0, 100, 0xFFFFFFFF };

	void SegaInitialDDriveBoard::Play(BYTE command, BYTE parameter, UINT32 strength)
	{
		if (command == 0x80 && strength == 0x01)
			wheel.Spring(1.0);

		if (command == 0x85 && parameter > 0x00 && strength > 0x00) // vibration
		{
			double force = strength / scaling.strengthScale;
			double period = parameter / scaling.periodScale * 120.0;
			wheel.Rumble(force, force, 100);
			wheel.Sine(static_cast<int>(period), 0, force, scaling.sineLength_ms);
		}

		if (command == 0x86 && strength > 0x00 && strength <= scaling.maxStrength) // spring
			wheel.Spring(strength / scaling.strengthScale);

		if (command == 0x84 && strength > 0x00 && strength <= scaling.maxStrength) // constant force
		{
			if (parameter == 0x00)
				wheel.ConstantWithRumble(Direction::FromLeft, (scaling.leftStrengthBase - (int)strength) / scaling.strengthScale);
			else if (parameter == 0x01)
				wheel.ConstantWithRumble(Direction::FromRight, strength / scaling.strengthScale);
		}
	}

	void InitialDMidiBoard::OnOutput(const char* name, int value)
	{
		if (!Is(name, { "midiffb" }))
			return;

		Log::Value("got value:", value);

		const BYTE* bytes = reinterpret_cast<const BYTE*>(&value);
		driveBoard.Play(bytes[2], bytes[1], bytes[0]);
	}
}
