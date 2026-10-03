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
#include "../Common Files/Game.h"

// Gaelco games set the wheel motor with a force between -1 and 1:
// the function doing it is hooked to play this force.
class GaelcoGame : public Game {
public:
	void FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers);
	static void PlayMotorForce(double force);

protected:
	// Hooks the game function setting the motor force, the hook calling PlayMotorForce
	virtual void HookSetMotor() = 0;
	// Hook of SetMotor(DWORD*, float force), called by the game at the given address
	static void HookFloatSetMotor(INT_PTR address);

private:
	bool init = false;
};
