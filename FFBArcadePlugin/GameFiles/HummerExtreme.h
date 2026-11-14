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
#include "Hummer.h"

// Hummer Extreme (Sega, Lindbergh, TeknoParrot): see HummerGame. The ViosityStrength and SpringStrength
// settings (in %) scale the viscosity and the spring of the game.
class HummerExtreme : public HummerGame
{
public:
	HummerExtreme()
		: HummerGame({ 0x80cffb4, 0x80d01aa, 0x80d004a, 0x80d00fa, 0x80d0302,
			ffb::Settings::Int(L"ViosityStrength", 100) / 100.0, ffb::Settings::Int(L"SpringStrength", 100) / 100.0, nullptr, true })
	{
	}
};
