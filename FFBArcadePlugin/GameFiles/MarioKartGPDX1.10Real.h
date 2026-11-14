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

// Mario Kart Arcade GP DX (Namco, TeknoParrot) with the force feedback of the game itself: the force of
// the wheel is read in the game's memory, through two pointers. The versions differ only by the
// addresses (1.10, 1.18 and USA, see MarioKartGPDX1.18Real.h and MarioKartGPDXUSAReal.h); the Custom
// versions make up effects from other values of the game instead.
class MarioKartGPDXReal : public ffb::Game
{
public:
	void Update() override;

protected:
	// Addresses of a version of the game
	struct Addresses
	{
		INT_PTR forcePointer; // relative to the module: pointer (32 bits) to a structure whose pointer (32 bits) at 0x08 leads to the force
		int forceOffset;      // offset of the force of the wheel (float) there: > 0 from the left, < 0 from the right
	};

	explicit MarioKartGPDXReal(const Addresses& addresses);

private:
	static const int ForceStructure = 0x08; // offset of the pointer to the structure holding the force

	Addresses addresses;
	int springEnable = ffb::Settings::Int(L"SpringEnable", 0);
	int springStrength = ffb::Settings::Int(L"SpringStrength", 0); // %
};

// Mario Kart Arcade GP DX 1.10 with the FFB of the game
class MarioKartGPDX110Real : public MarioKartGPDXReal
{
public:
	MarioKartGPDX110Real() : MarioKartGPDXReal({ 0xA2FE20, 0xA44 }) {}
};
