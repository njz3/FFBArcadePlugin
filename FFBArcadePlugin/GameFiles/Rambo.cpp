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

#include "Rambo.h"
#include "../Core/Devices.h"
#include "../Core/PluginState.h"

using namespace ffb;


void Rambo::Update()
{
	INT_PTR player1 = Memory::Read(PointerChain<INT_PTR, 4>{ { Base::Absolute, (INT_PTR)gl_cgGLDll + Player1Pointer }, { 0x5BC, 0x77C, 0x0, 0x20 } });
	UINT8 health1p = Memory::Read<UINT8>(player1 + HealthOffset);
	UINT8 ammo1p = Memory::Read<UINT8>(player1 + AmmoOffset);
	float rage1p = Memory::Read<float>(player1 + RageOffset);

	INT_PTR player2 = Memory::Read(PointerChain<INT_PTR, 4>{ { Base::Absolute, (INT_PTR)gl_cgGLDll + Player2Pointer }, { 0x4F4, 0x278, 0x1B0, 0x38 } });
	UINT8 health2p = Memory::Read<UINT8>(player2 + HealthOffset);
	UINT8 ammo2p = Memory::Read<UINT8>(player2 + AmmoOffset);
	float rage2p = Memory::Read<float>(player2 + RageOffset);

	if (!devicesOpen)
	{
		OpenHapticDevice2();
		devicesOpen = true;
	}

	UINT32 length_ms = feedbackLength;
	if (lastHealth1p != health1p)
		RumbleWithMode(wheel, howtoRumbleHealthEffect, health1pStrength / 100.0, length_ms);

	if (lastHealth2p != health2p)
		RumbleWithMode(device2, howtoRumbleHealthEffect, health2pStrength / 100.0, length_ms);

	if (lastAmmo1p != ammo1p)
		RumbleWithMode(wheel, howtoRumbleGunEffect, gun1pStrength / 100.0, length_ms);

	if (lastAmmo2p != ammo2p)
		RumbleWithMode(device2, howtoRumbleGunEffect, gun2pStrength / 100.0, length_ms);

	if (lastRage1p != rage1p)
		RumbleWithMode(wheel, howtoRumbleRageEffect, rage1pStrength / 100.0, length_ms);

	if (lastRage2p != rage2p)
		RumbleWithMode(device2, howtoRumbleRageEffect, rage2pStrength / 100.0, length_ms);

	lastHealth1p = health1p;
	lastAmmo1p = ammo1p;
	lastRage1p = rage1p;
	lastHealth2p = health2p;
	lastAmmo2p = ammo2p;
	lastRage2p = rage2p;
}
