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

#include "DirtyDrivin.h"

using namespace ffb;

void DirtyDrivin::Update()
{
	float force = Memory::Read(WheelForce);
	UINT8 track = Memory::Read(Track);
	Log::Value("got value:", force);

	bool reversed = track >= 8 && track <= 14;

	ApplyDamper();

	if (force > 0)
		wheel.ConstantWithRumble(reversed ? Direction::FromRight : Direction::FromLeft, force);
	else if (force < 0)
		wheel.ConstantWithRumble(reversed ? Direction::FromLeft : Direction::FromRight, -force);
}
