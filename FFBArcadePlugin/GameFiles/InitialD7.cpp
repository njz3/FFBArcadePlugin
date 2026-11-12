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
#include "InitialD7.h"
#include "../Common Files/InitialDFFB.h"

static const InitialDFFBSettings ffbSettings = { 127.0, 128, 127.0, 100, 0x7F };

void InitialD7::FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers)
{
	DWORD FFB = GetTeknoParrotFFB();

	BYTE* ffb = reinterpret_cast<BYTE*>(&FFB);

	ApplyDamper(triggers);

	// TeknoParrot gives the bytes in reverse order, the command without its 0x80 flag
	PlayInitialDFFB(ffb[2] ^ 0x80, ffb[1], ffb[0], ffbSettings, constants, triggers);
}
