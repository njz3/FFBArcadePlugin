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

#include "SegaRally3.h"

using namespace ffb;

void SegaRally3::Update()
{
	const int ff = TeknoParrotFFB();
	Log::Value("got value:", ff);

	ApplyDamper();

	if (ff > 15)
		wheel.ConstantWithRumble(Direction::FromLeft, (31 - ff) / 15.0);
	else if (ff > 0)
		wheel.ConstantWithRumble(Direction::FromRight, (16 - ff) / 15.0);
}
