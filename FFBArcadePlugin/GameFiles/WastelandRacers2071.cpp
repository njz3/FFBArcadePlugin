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

#include "WastelandRacers2071.h"
#include "../Core/Hooks.h"

using namespace ffb;

WasteLandRacers2071* WasteLandRacers2071::instance = nullptr;
int(__cdecl* WasteLandRacers2071::setForceOriginal)(int force, int param_2, int param_3, int param_4) = nullptr;
int(__cdecl* WasteLandRacers2071::setSteeringPosOriginal)(int position, int param_2, int param_3, int param_4) = nullptr;
int(__cdecl* WasteLandRacers2071::setDigitalOutOriginal)(int param_1, int param_2, int param_3, int param_4) = nullptr;

WasteLandRacers2071::WasteLandRacers2071()
{
	instance = this;
}

int __cdecl WasteLandRacers2071::SetForceHook(int force, int param_2, int param_3, int param_4)
{
	instance->springStrength = (double)force / 32768.0;

	return 0;
}

int __cdecl WasteLandRacers2071::SetSteeringPosHook(int position, int param_2, int param_3, int param_4)
{
	WasteLandRacers2071& game = *instance;
	double wheelPos = (double)position / 128000.0;

	// the rumble gets the position itself, not wheelPos, as it always did
	if (wheelPos >= 0) {
		game.wheel.Rumble(0, position, 100);
		game.wheel.Constant(Direction::FromRight, wheelPos);
	}
	else if (wheelPos <= 0) {
		game.wheel.Rumble(0, position * -1, 100);
		game.wheel.Constant(Direction::FromLeft, wheelPos * -1);
	}
	return 0;
}

int __cdecl WasteLandRacers2071::SetDigitalOutHook(int param_1, int param_2, int param_3, int param_4)
{
	return 0;
}

void WasteLandRacers2071::Update()
{
	// once the game has loaded sdc2.dll
	if (GetModuleHandleA("sdc2.dll") != NULL && !hooked) {
		hooked = true;
		Hooks::Initialize();
		Hooks::CreateApi(L"sdc2.dll", "SetForce", SetForceHook, &setForceOriginal);
		Hooks::CreateApi(L"sdc2.dll", "SetSteeringPos", SetSteeringPosHook, &setSteeringPosOriginal);
		Hooks::CreateApi(L"sdc2.dll", "SetDigitalOut", SetDigitalOutHook, &setDigitalOutOriginal);
		Hooks::Enable();
	}

	ApplyDamper();

	wheel.Spring(springStrength);
}
