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

#include "GaelcoTuningRace.h"
#include "../Core/Hooks.h"

using namespace ffb;

int(__stdcall* GaelcoTuningRace::setMotorOriginal)(DWORD* force) = nullptr;

// The motor force (a float) is passed in the place of the first argument
int __stdcall GaelcoTuningRace::SetMotorHook(DWORD* a1)
{
	long double force = *(float*)&a1;
	PlayMotorForce(force);
	return setMotorOriginal(a1);
}

void GaelcoTuningRace::HookSetMotor()
{
	Hooks::Create(SetMotor, SetMotorHook, &setMotorOriginal);
}
