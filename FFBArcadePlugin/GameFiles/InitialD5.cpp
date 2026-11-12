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
#include "InitialD5.h"
#include "../Common Files/InitialDFFB.h"
#include "math.h"

void InitialD5::FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers) {

	ApplyDamper(triggers);

	DWORD FFB = helpers->ReadInt32(0x8CB6120, false);

	BYTE* ffb = reinterpret_cast<BYTE*>(&FFB);

	PlayInitialDFFB(ffb[0], ffb[1], ffb[2], InitialDFFBDefaultSettings, constants, triggers);
}
