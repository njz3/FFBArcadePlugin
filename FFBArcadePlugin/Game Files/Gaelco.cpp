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

#include "Gaelco.h"

static EffectTriggers* myTriggers;
static EffectConstants* myConstants;

void GaelcoGame::PlayMotorForce(double force)
{
	UINT32 length_ms = 100;
	if (force > 0)
	{
		double percentForce = force;

		if (percentForce > 1.0)
			percentForce = 1.0;

		myTriggers->Rumble(percentForce, 0, length_ms);
		myTriggers->Constant(myConstants->DIRECTION_FROM_LEFT, percentForce);
	}
	else if (force < 0)
	{
		double percentForce = -force;

		if (percentForce > 1.0)
			percentForce = 1.0;

		myTriggers->Rumble(0, percentForce, length_ms);
		myTriggers->Constant(myConstants->DIRECTION_FROM_RIGHT, percentForce);
	}
}

static int(__stdcall* FloatSetMotorOri)(DWORD* a1, float a2);
static int __stdcall FloatSetMotorHook(DWORD* a1, float a2)
{
	GaelcoGame::PlayMotorForce(a2);
	return FloatSetMotorOri(a1, a2);
}

void GaelcoGame::HookFloatSetMotor(INT_PTR address)
{
	MH_CreateHook((void*)address, FloatSetMotorHook, (void**)&FloatSetMotorOri);
}

void GaelcoGame::FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers)
{
	myTriggers = triggers;
	myConstants = constants;

	if (!init)
	{
		init = true;
		MH_Initialize();
		HookSetMotor();
		MH_EnableHook(MH_ALL_HOOKS);
	}

	ApplyDamper(triggers);
}
