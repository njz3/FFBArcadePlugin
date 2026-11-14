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

// Mach Storm (Namco, TeknoParrot): a vibration and its power are read in the game's memory, the 10 powers
// rumble with the strength (%) and length (ms) of their settings. The settings are read again at each
// update (DomeFix too, unused).
class Machstorm : public ffb::Game
{
public:
	void Update() override;

private:
	// 16842753 or 1 when vibrating, with its power in two encodings:
	static constexpr ffb::GameValue<int> Vibration{ ffb::Base::Module, 0x6390E9 };
	// 61542 to 61551 (vibration 16842753), or 16773366 to 16773375 (vibration 1), for the powers 1 to 10
	static constexpr ffb::GameValue<int> Power{ ffb::Base::Module, 0x639109 };

	static const int PowerCount = 10;
	static constexpr const wchar_t* StrengthKeys[PowerCount] = {
		L"Power1RumbleStrength", L"Power2RumbleStrength", L"Power3RumbleStrength", L"Power4RumbleStrength",
		L"Power5RumbleStrength", L"Power6RumbleStrength", L"Power7RumbleStrength", L"Power8RumbleStrength",
		L"Power9RumbleStrength", L"Power10RumbleStrength",
	};
	static constexpr const wchar_t* LengthKeys[PowerCount] = {
		L"Power1RumbleLength", L"Power2RumbleLength", L"Power3RumbleLength", L"Power4RumbleLength",
		L"Power5RumbleLength", L"Power6RumbleLength", L"Power7RumbleLength", L"Power8RumbleLength",
		L"Power9RumbleLength", L"Power10RumbleLength",
	};
};
