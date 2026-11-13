
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
#include "FnFDrift.h"


static bool init;
static EffectTriggers* myTriggers;
static EffectConstants* myConstants;
static Helpers* myHelpers;
// longest rumble SDL plays (SDL_MAX_RUMBLE_DURATION_MS): the rumble is changed again at the next change
static const UINT32 RumbleUntilNextChange = 0xFFFF;

static int(__cdecl* FnFSendToLptOrig)(uint16_t addr, uint8_t FFB);
// The game writes the force to the parallel port (0x378) at each frame: the effects are changed, or
// stopped, only when that value changes. Playing a constant effect and a rumble at each write sent
// thousands of effects per second (442771 in a minute of FNF Drift at a high frame rate).
static volatile LONG lastFFB = -1;
static int __cdecl FnFSendToLpt(uint16_t addr, uint8_t FFB)
{
	if (addr == 0x378 && myTriggers != nullptr && InterlockedExchange(&lastFFB, FFB) != FFB)
	{
		bool isLeft = FFB < 0x80;
		double percentForce = isLeft ? FFB / 127.0 : (FFB - 0x80) / 127.0;
		// both last until the next change; a force of 0 stops them
		myTriggers->Rumble(0, percentForce, RumbleUntilNextChange);
		myTriggers->ConstantInf(isLeft ? myConstants->DIRECTION_FROM_RIGHT : myConstants->DIRECTION_FROM_LEFT, percentForce);
	}

	return 0;

}


static void(__cdecl* FnFIgnoreLPTOrig)();
static void __cdecl FnFignoreLPT() {
	return;
}

void FnFDrift::FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers) {
	// before the hooks, which use them
	myTriggers = triggers;
	myConstants = constants;
	myHelpers = helpers;

	if (!init)
	{
		//enable IO calls
		helpers->WriteIntPtr(0x05a3ca4, 1, false);
		MH_Initialize();
		MH_CreateHook((LPVOID)0x048a410, FnFSendToLpt, (LPVOID*)&FnFSendToLptOrig);

		MH_CreateHook((LPVOID)0x048a760, FnFignoreLPT, (LPVOID*)&FnFIgnoreLPTOrig);
		MH_CreateHook((LPVOID)0x048a7b0, FnFignoreLPT, (LPVOID*)&FnFIgnoreLPTOrig);
		MH_CreateHook((LPVOID)0x048a8f0, FnFignoreLPT, (LPVOID*)&FnFIgnoreLPTOrig);
		MH_CreateHook((LPVOID)0x048aa20, FnFignoreLPT, (LPVOID*)&FnFIgnoreLPTOrig);
		MH_EnableHook(MH_ALL_HOOKS);
		init = true;
	}

	ApplyDamper(triggers);
}