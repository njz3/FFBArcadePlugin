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

// Tokyo Cop, Ring Riders and Gaelco Tuning Race (Gaelco, TeknoParrot): the games set the wheel motor with a
// force between -1 and 1 (> 0 from the left, < 0 from the right); the plugin replaces the game function
// doing it to play this force. The games differ by that function.
class GaelcoGame : public ffb::Game
{
public:
	void Update() override;

protected:
	GaelcoGame();

	// Hooks the game function setting the motor force, the hook calling PlayMotorForce
	virtual void HookSetMotor() = 0;
	// Hooks SetMotor(DWORD*, float force) of the game, at the given address
	static void HookFloatSetMotor(INT_PTR address);
	// Force of the motor: > 0 from the left, < 0 from the right, limited to 1
	static void PlayMotorForce(double force);

private:
	static GaelcoGame* instance;
	static int(__stdcall* floatSetMotorOriginal)(DWORD* motor, float force);
	static int __stdcall FloatSetMotorHook(DWORD* motor, float force);

	bool hooked = false;
};
