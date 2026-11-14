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

// Sonic & Sega All-Stars Racing Arcade (Sega): the force of the wheel is read in the game's memory,
// played when it changes
class SonicSegaAllStarsRacing : public ffb::Game
{
public:
	void Update() override;

private:
	// Force of the wheel: 1 to 18 from the right (18 a full force), 238 to 255 from the left (238 a
	// full force), other values no force
	static constexpr ffb::GameValue<UINT8> WheelForce{ ffb::Base::Module, 0x5CD864 };
	// Set to 3 at each update (meaning unknown)
	static constexpr ffb::GameValue<UINT8> ForceMode{ ffb::Base::Module, 0x5CD858 };

	int springEnable = ffb::Settings::Int(L"SpringEnable", 0);
	int springStrength = ffb::Settings::Int(L"SpringStrength", 0);

	UINT8 lastForce = 0;
};
