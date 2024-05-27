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
#include "InitialD8.h"

extern int EnableDamper;
extern int DamperStrength;

void InitialD8::FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers)
{
	DWORD FFB = GetTeknoParrotFFB();

	BYTE* ffb = reinterpret_cast<BYTE*>(&FFB);

	if (EnableDamper)
		triggers->Damper(DamperStrength / 100.0);

	if (ffb[2] == 0x00 && ffb[0] == 0x01)
	{
		triggers->Spring(1.0);
	}

	UINT32 length_ms = 100;
	if (ffb[2] == 0x05 && ffb[1] > 0x00 && ffb[0] > 0x00)
	{
		double percentForce = ffb[0] / 127.0;
		double Period = ffb[1] / 127.0 * 120.0;
		triggers->Rumble(percentForce, percentForce, length_ms);
		triggers->Sine(static_cast<int>(Period), 0, percentForce, length_ms);
	}

	if (ffb[2] == 0x06 && ffb[0] > 0x00 && ffb[0] < 0x80)
	{
		double percentForce = ffb[0] / 127.0;
		triggers->Spring(percentForce);
	}

	if (ffb[2] == 0x04 && ffb[0] > 0x00 && ffb[0] < 0x80)
	{
		if (ffb[1] == 0x00)
		{
			double percentForce = (128 - ffb[0]) / 127.0;
			triggers->Rumble(percentForce, 0, length_ms);
			triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
		}
		else if (ffb[1] == 0x01)
		{
			double percentForce = (ffb[0] / 127.0);
			triggers->Rumble(0, percentForce, length_ms);
			triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
		}
	}
}