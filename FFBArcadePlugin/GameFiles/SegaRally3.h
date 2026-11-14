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

#pragma once
#include "../Core/TeknoParrot.h"

// Sega Rally 3 (Sega Europa-R, TeknoParrot): TeknoParrot publishes the force of the wheel as a value
// of the drive board: 16 to 31 from the left (16 the strongest), 1 to 15 from the right (1 the
// strongest), 0 no force
class SegaRally3 : public ffb::TeknoParrotGame
{
public:
	void Update() override;
};
