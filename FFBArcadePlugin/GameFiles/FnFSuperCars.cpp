
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

#include "FnFSuperCars.h"

static bool init;
static EffectTriggers* myTriggers;
static EffectConstants* myConstants;
static Helpers* myHelpers;
// longest rumble SDL plays (SDL_MAX_RUMBLE_DURATION_MS): the rumble is changed again at the next change
static const UINT32 RumbleUntilNextChange = 0xFFFF;

static void(__cdecl* FUN_00402cf0Orig)(float param_1);
// The game gives its force (-1 to 1) at each frame: the effects are changed, or stopped, only when it
// changes. They were played at each call, twice when the force was 0.
static float lastForce = 2.0f; // none yet
static void __cdecl FUN_00402cf0(float param_1) {
	if (myTriggers == nullptr || param_1 == lastForce)
		return;
	lastForce = param_1;

	double percentForce = param_1 >= 0 ? param_1 : -param_1;
	// both last until the next change; a force of 0 stops them
	myTriggers->Rumble(0, percentForce, RumbleUntilNextChange);
	myTriggers->ConstantInf(param_1 >= 0 ? myConstants->DIRECTION_FROM_RIGHT : myConstants->DIRECTION_FROM_LEFT, percentForce);
}

static void(__cdecl* FnFSCIgnoreLPTOrig)();
static void __cdecl FnFSCignoreLPT() {
	return ;
}

void FnFSuperCars::FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers) {
	// before the hooks, which use them
	myTriggers = triggers;
	myConstants = constants;
	myHelpers = helpers;

	if (!init)
	{
		OutputDebugStringA("FnFSuperCars::FFBLoop");


		MH_Initialize();
		MH_CreateHook((LPVOID)0x0402cf0, FUN_00402cf0, (LPVOID*)&FUN_00402cf0Orig);

		//remove unwanted instances
		MH_CreateHook((LPVOID)0x0402ca0, FnFSCignoreLPT, (LPVOID*)&FnFSCIgnoreLPTOrig);
		MH_CreateHook((LPVOID)0x0402c50, FnFSCignoreLPT, (LPVOID*)&FnFSCIgnoreLPTOrig);
		MH_CreateHook((LPVOID)0x0402bc0, FnFSCignoreLPT, (LPVOID*)&FnFSCIgnoreLPTOrig);	
		MH_EnableHook(MH_ALL_HOOKS);

		init = true;
	}

	ApplyDamper(triggers);

}