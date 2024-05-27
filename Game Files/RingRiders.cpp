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

#include <string>
#include "RingRiders.h"
static EffectTriggers* myTriggers;
static EffectConstants* myConstants;
static Helpers* myHelpers;
extern int EnableDamper;
extern int DamperStrength;
static bool init;

static int(__stdcall* SetMotorOri)(DWORD* a1, float a2);
static int __stdcall SetMotorHook(DWORD* a1, float a2)
{
	UINT32 length_ms = 100;
	if (a2 > 0)
	{
		double percentForce = a2;

		if (percentForce > 1.0)
			percentForce = 1.0;

		myTriggers->Rumble(percentForce, 0, length_ms);
		myTriggers->Constant(myConstants->DIRECTION_FROM_LEFT, percentForce);
	}
	else if (a2 < 0)
	{
		double percentForce = -a2;

		if (percentForce > 1.0)
			percentForce = 1.0;

		myTriggers->Rumble(0, percentForce, length_ms);
		myTriggers->Constant(myConstants->DIRECTION_FROM_RIGHT, percentForce);
	}
	return SetMotorOri(a1, a2);
}

void RingRiders::FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers) {

	if (!init)
	{
		init = true;
		MH_Initialize();
		MH_CreateHook((void*)0x8107568, SetMotorHook, (void**)&SetMotorOri);
		MH_EnableHook(MH_ALL_HOOKS);
	}

	if (EnableDamper)
		triggers->Damper(DamperStrength / 100.0);

	myTriggers = triggers;
	myConstants = constants;
	myHelpers = helpers;
}