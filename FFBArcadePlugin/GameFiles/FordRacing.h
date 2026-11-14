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

// Ford Racing (TeknoParrot): the force of the wheel is the FFB value published by TeknoParrot:
// -65514 to -65506 from the right (strongest at -65514), 1 to 15 from the left ((16 - value) / 9, above 1
// below 7)
class FordRacing : public ffb::TeknoParrotGame
{
public:
	void Update() override;
};
