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

// Dirty Drivin' (Raw Thrills, TeknoParrot): the force of the wheel is read in the game's memory. Its
// side is the other one on the tracks 8 to 14.
class DirtyDrivin : public ffb::Game
{
public:
	void Update() override;

private:
	// Force of the wheel: > 0 from the left, < 0 from the right (the other way on tracks 8 to 14)
	static constexpr ffb::GameValue<float> WheelForce{ ffb::Base::Absolute, 0x886EC4 };
	static constexpr ffb::GameValue<UINT8> Track{ ffb::Base::Absolute, 0x96F770 };
};
