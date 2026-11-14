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
#include "MarioKartGPDX1.10Real.h"

// Mario Kart Arcade GP DX USA with the FFB of the game: see MarioKartGPDXReal
class MarioKartGPDXUSAReal : public MarioKartGPDXReal
{
public:
	MarioKartGPDXUSAReal() : MarioKartGPDXReal({ 0xB47B48, 0x2FC }) {}
};
