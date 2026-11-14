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

// Fast & Furious SuperCars (Raw Thrills, TeknoParrot): the plugin replaces the function the game gives
// the force of the wheel to (-1 to 1, at each frame). The effects change only when it changes.
class FnFSuperCars : public ffb::Game
{
public:
	FnFSuperCars();
	void Update() override;

private:
	static const INT_PTR SetForce = 0x402CF0;
	static constexpr INT_PTR OtherLptIo[] = { 0x402CA0, 0x402C50, 0x402BC0 }; // emptied

	// The rumble lasts RumbleLength and is renewed every RumbleRenewal while it is not 0, so that it
	// ends by itself if the game is killed (the constant force is renewed by the plugin)
	static const UINT32 RumbleLength = 1000; // ms
	static const ULONGLONG RumbleRenewal = 300; // ms
	static constexpr float NoForceYet = 2.0f;

	static FnFSuperCars* instance;
	static void(__cdecl* setForceOriginal)(float force);
	static void(__cdecl* ignoredOriginal)();
	static void __cdecl SetForceHook(float force);
	static void __cdecl IgnoredHook();

	bool hooked = false;
	volatile float lastForce = NoForceYet;
	ULONGLONG rumbleRenewed = 0;
};
