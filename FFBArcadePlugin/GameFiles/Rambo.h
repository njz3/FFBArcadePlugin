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

#pragma once
#include "../Core/Game.h"

// Rambo (Sega Lindbergh, TeknoParrot): rumble of the guns of the two players (player 2 on Device2GUID)
// when their health, their ammo or their rage changes. The values are read through pointers from the
// base of cgGL.dll (gl_cgGLDll).
class Rambo : public ffb::Game
{
public:
	void Update() override;

private:
	// Offsets from the base of cgGL.dll of the pointers to the players: then
	// player 1: +0x5BC, +0x77C, +0x0, then +0x20 the pointer to its structure
	// player 2: +0x4F4, +0x278, +0x1B0, then +0x38 the pointer to its structure
	static const INT_PTR Player1Pointer = 0x1C4E8;
	static const INT_PTR Player2Pointer = 0x9144;
	// In the structure of a player
	static const INT_PTR HealthOffset = 0x3C; // byte
	static const INT_PTR AmmoOffset = 0x400;  // byte
	static const INT_PTR RageOffset = 0x68;   // float

	int feedbackLength = ffb::Settings::Int(L"FeedbackLength", 120);
	int howtoRumbleGunEffect = ffb::Settings::Int(L"HowtoRumbleGunEffect", 0);
	int howtoRumbleRageEffect = ffb::Settings::Int(L"HowtoRumbleRageEffect", 0);
	int howtoRumbleHealthEffect = ffb::Settings::Int(L"HowtoRumbleHealthEffect", 0);
	int gun1pStrength = ffb::Settings::Int(L"Gun1pStrength", 0);
	int rage1pStrength = ffb::Settings::Int(L"Rage1pStrength", 0);
	int health1pStrength = ffb::Settings::Int(L"Health1pStrength", 0);
	int gun2pStrength = ffb::Settings::Int(L"Gun2pStrength", 0);
	int rage2pStrength = ffb::Settings::Int(L"Rage2pStrength", 0);
	int health2pStrength = ffb::Settings::Int(L"Health2pStrength", 0);

	bool devicesOpen = false;
	UINT8 lastHealth1p = 0;
	UINT8 lastAmmo1p = 0;
	UINT8 lastHealth2p = 0;
	UINT8 lastAmmo2p = 0;
	float lastRage1p = 0;
	float lastRage2p = 0;
};
