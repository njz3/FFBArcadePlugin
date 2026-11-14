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

// Cars (Raw Thrills, TeknoParrot): the plugin hooks the functions that give the force of the wheel
// (test menu and game) and start a rumble, while the FFB of the game is enabled.
class Cars : public ffb::Game
{
public:
	Cars();
	void Update() override;

private:
	static constexpr ffb::GameValue<UINT8> FFBEnabled{ ffb::Base::Absolute, 0x18D32A2 }; // 1 enabled, 0 disabled
	static const INT_PTR TestMenuForce = 0x451BA0;
	static const INT_PTR GameForce = 0x821D40;
	static const INT_PTR StartRumble = 0x455550;

	static Cars* instance;
	// The originals are __thiscall; the hooks are __fastcall, the force on the stack
	static void(__thiscall* testMenuForceOriginal)(int object, float force, char unknown);
	static void(__thiscall* gameForceOriginal)(int object, float force, char unknown);
	static void(__thiscall* startRumbleOriginal)(int object, int rumble);
	static void __fastcall TestMenuForceHook(int object, float force, char unknown);
	static void __fastcall GameForceHook(int object, float force, char unknown);
	static void __fastcall StartRumbleHook(int object, int rumble);

	void PlayForce(float force);

	bool hooked = false;
	int ffbEnabled = 0;
};
