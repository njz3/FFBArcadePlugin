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

// Wasteland Racers 2071 (TeknoParrot): once the game has loaded sdc2.dll, the library of its wheel, the
// plugin replaces its functions: SetForce gives the strength of the centering spring (played at each
// loop), SetSteeringPos a force played at once; SetDigitalOut is emptied.
class WasteLandRacers2071 : public ffb::Game
{
public:
	WasteLandRacers2071();
	void Update() override;

private:
	static WasteLandRacers2071* instance;
	static int(__cdecl* setForceOriginal)(int force, int param_2, int param_3, int param_4);
	static int(__cdecl* setSteeringPosOriginal)(int position, int param_2, int param_3, int param_4);
	static int(__cdecl* setDigitalOutOriginal)(int param_1, int param_2, int param_3, int param_4);
	// force: the spring is force / 32768
	static int __cdecl SetForceHook(int force, int param_2, int param_3, int param_4);
	// position: >= 0 a force from the right of position / 128000, < 0 from the left
	static int __cdecl SetSteeringPosHook(int position, int param_2, int param_3, int param_4);
	static int __cdecl SetDigitalOutHook(int param_1, int param_2, int param_3, int param_4);

	bool hooked = false;
	double springStrength = 1; // set by SetForce
};
