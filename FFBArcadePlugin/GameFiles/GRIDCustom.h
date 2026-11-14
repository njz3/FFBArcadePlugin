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

// GRID (Codemasters, TeknoParrot), with forces made by the plugin from values read in the game's memory:
// a spring (SpringStrength), the collisions, the wheels off the road, the skids and the gear changes.
class GRIDCustom : public ffb::Game
{
public:
	void Update() override;

private:
	// Value read as the side of a collision with a wall: > 0 force from the right, < 0 from the left
	static constexpr ffb::PointerChain<float, 4> WallSide{ { ffb::Base::Module, 0xB1B7F0 }, { 0x730, 0x4, 0x4, 0x118 } };
	// Values of the player car (pointer), with these values (UINT8):
	static constexpr ffb::PointerChain<INT_PTR, 3> Panel{ { ffb::Base::Module, 0xA3FA34 }, { 0x678, 0x14, 0x30 } };
	static const INT_PTR Collision = 0x2C; // strength of a collision, up to 8
	static const INT_PTR Wheels = 0xB4;    // wheels off the road (rumble), up to 37
	static const INT_PTR Skids = 0x100;    // skids, played above 12, up to 20
	static const INT_PTR AI = 0x3D4;       // > 0: the collision is with a car of the AI
	// Gear of the car
	static constexpr ffb::GameValue<UINT8> Gear{ ffb::Base::Absolute, 0x414F7898 };
	// Speed of the car, > 0 when it moves
	static constexpr ffb::PointerChain<float, 4> Speed{ { ffb::Base::Module, 0x28C008 }, { 0xD0, 0x460, 0x184, 0x4F4 } };

	int springStrength = ffb::Settings::Int(L"SpringStrength", 0);
	int gearChangeStrength = ffb::Settings::Int(L"GearChangeStrength", 20);
	int gearChangeLength = ffb::Settings::Int(L"GearChangeLength", 200);

	bool gearShift = false;
	UINT8 oldGear = 0;
	// Bug kept from the old code (its myTriggers): the wheel of the gear change effect is set at the end
	// of Update, so a gear change seen by the first Update crashes (null pointer)
	ffb::Wheel* gearChangeWheel = nullptr;
};
