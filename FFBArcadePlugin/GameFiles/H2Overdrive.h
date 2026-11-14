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
#include "../Core/Game.h"

// H2Overdrive (Raw Thrills, TeknoParrot): the stiffness and the force of the wheel are read in the game's memory
class H2Overdrive : public ffb::Game
{
public:
	void Update() override;

private:
	// Centering spring of the wheel, up to 1.25
	static constexpr ffb::GameValue<float> Stiffness{ ffb::Base::Module, 0x392C54 };
	// Force of the wheel: > 0 from the left, < 0 from the right
	static constexpr ffb::GameValue<float> WheelForce{ ffb::Base::Module, 0x392C58 };
};
