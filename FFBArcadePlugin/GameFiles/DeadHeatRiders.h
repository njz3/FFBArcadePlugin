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

// Dead Heat Riders (Namco, TeknoParrot): TeknoParrot publishes four FFB values of the game, as for Dead
// Heat (see NamcoWheelGame) but played without strength settings: spring (FFB, up to 500, kept),
// viscosity (FFB2), center offset (FFB3) and reflect (FFB4), 1 to 0x3F.
class DeadHeatRiders : public ffb::TeknoParrotGame
{
public:
	void Update() override;
};
