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

#include "PokkenTournament.h"

using namespace ffb;

void PokkenTournament::Update()
{
	Log::Write("in PokkenTournament Ffbloop");
	const int leftMotor = TeknoParrotFFB();
	const int rightMotor = TeknoParrotFFB2();
	Log::Value("left motor:", leftMotor);

	if (leftMotor > 3)
	{
		double percentForce = (leftMotor / 255.0);
		wheel.Rumble(0, percentForce, feedbackLength);
	}

	if (rightMotor > 3)
	{
		double percentForce = (rightMotor / 255.0);
		wheel.Rumble(percentForce, 0, feedbackLength);
	}
}
