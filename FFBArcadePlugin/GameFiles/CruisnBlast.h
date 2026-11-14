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

// Cruis'n Blast (Raw Thrills, TeknoParrot): the plugin replaces the WHEEL_SET function of the game,
// which gets the force of the wheel in a race; in the menus, the force is read in the game's memory.
class CruisnBlast : public ffb::Game
{
public:
	CruisnBlast();
	void Update() override;

private:
	static constexpr ffb::GameValue<UINT8> CabinetType{ ffb::Base::Absolute, 0xA0A7808 }; // set to 4
	static constexpr ffb::GameValue<float> MenuForce{ ffb::Base::Absolute, 0x8BABBAC };   // force of the wheel in the menus
	static constexpr ffb::GameValue<int> Screen{ ffb::Base::Absolute, 0x8BABBC0 };        // 0 menus, 1 race
	static constexpr ffb::GameValue<UINT8> WheelEffects{ ffb::Base::Absolute, 0x9C28504 }; // 1 on in a race, 0 off
	// WHEEL_SET, its wheel check (2 bytes) and its outputs (1 byte each)
	static const INT_PTR WheelSet = 0x8151B50;
	static const INT_PTR WheelCheck = 0x8151B61;
	static constexpr INT_PTR WheelSetOutputs[] = { 0x8151B90, 0x8151B96, 0x8151B9C, 0x8151B87 };

	static CruisnBlast* instance;
	static int(__cdecl* wheelSetOriginal)(float force);
	static int __cdecl WheelSetHook(float force);

	bool hooked = false;
	bool inRace = false;
	float force = 0;
};
