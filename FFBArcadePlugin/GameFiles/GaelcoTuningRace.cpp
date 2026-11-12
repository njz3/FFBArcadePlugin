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

// The motor force (a float) is passed in the place of the first argument
static int(__stdcall* SetMotorOri)(DWORD* a1);
static int __stdcall SetMotorHook(DWORD* a1)
{
	long double force = *(float*)&a1;
	GaelcoGame::PlayMotorForce(force);
	return SetMotorOri(a1);
}

void GaelcoTuningRace::HookSetMotor()
{
	MH_CreateHook((void*)0x8188ABC, SetMotorHook, (void**)&SetMotorOri);
}
