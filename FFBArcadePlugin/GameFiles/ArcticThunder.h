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

// Arctic Thunder (Midway, TeknoParrot): TeknoParrot publishes the vibration of the handlebar
// (TeknoParrotFFB2: 0x01 to 0xFE, 0xFF ignored) and its force (TeknoParrotFFB3: 0x80 none, above from
// the left, below from the right). The centering spring is the one of the EnableForceSpringEffect setting.
class ArcticThunder : public ffb::TeknoParrotGame
{
public:
	void Update() override;
};
