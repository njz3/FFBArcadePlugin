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

// Aliens: Extermination (Global VR, TeknoParrot): rumble of the guns of the two players (player 2 on
// Device2GUID) at the shots, the flame thrower and the hits, read in the game's memory. With
// EscapeKeyExitViaPlugin, the Escape key closes the game (its window is "GLUT").
class AliensExtermination : public ffb::Game
{
public:
	void Update() override;

private:
	// Ammo of player 1, two bytes: a shot when the first one changes; both 0 or both 10 when the shots
	// are seen by Shoot1p instead
	static constexpr ffb::GameValue<UINT8> Ammo1pA{ ffb::Base::Module, 0x4E7AEA0 };
	static constexpr ffb::GameValue<UINT8> Ammo1pB{ ffb::Base::Module, 0x4E7AEA1 };
	// Flame thrower of player 1: > 0 when it fires, a rumble when it changes
	static constexpr ffb::GameValue<float> Flame1p{ ffb::Base::Module, 0x556CB30 };
	// Health of player 1: a rumble when it changes while > 0
	static constexpr ffb::GameValue<float> Health1p{ ffb::Base::Module, 0x556C884 };
	// Same for player 2
	static constexpr ffb::GameValue<UINT8> Ammo2pA{ ffb::Base::Module, 0x4E7AEA2 };
	static constexpr ffb::GameValue<UINT8> Ammo2pB{ ffb::Base::Module, 0x4E7AEA3 };
	static constexpr ffb::GameValue<float> Flame2p{ ffb::Base::Module, 0x556CEC4 };
	static constexpr ffb::GameValue<float> Health2p{ ffb::Base::Module, 0x556CC18 };
	// Trigger of the gun of player 1, bit 0x10 (a shot when it changes)
	static constexpr ffb::GameValue<UINT8> Shoot1p{ ffb::Base::Module, 0x556ACBC };
	static constexpr ffb::GameValue<UINT8> Shoot2p{ ffb::Base::Module, 0x556AFD8 };

	int escapeKeyExitViaPlugin = ffb::Settings::Int(L"EscapeKeyExitViaPlugin", 0);
	int feedbackLength = ffb::Settings::Int(L"FeedbackLength", 120);
	int howtoRumbleGunEffect = ffb::Settings::Int(L"HowtoRumbleGunEffect", 0);
	int howtoRumbleFlameEffect = ffb::Settings::Int(L"HowtoRumbleFlameEffect", 0);
	int howtoRumbleHealthEffect = ffb::Settings::Int(L"HowtoRumbleHealthEffect", 0);
	int gun1pStrength = ffb::Settings::Int(L"Gun1pStrength", 0);
	int flame1pStrength = ffb::Settings::Int(L"Flame1pStrength", 0);
	int health1pStrength = ffb::Settings::Int(L"Health1pStrength", 0);
	int gun2pStrength = ffb::Settings::Int(L"Gun2pStrength", 0);
	int flame2pStrength = ffb::Settings::Int(L"Flame2pStrength", 0);
	int health2pStrength = ffb::Settings::Int(L"Health2pStrength", 0);

	bool devicesOpen = false;
	// At their last change: ammo not 0 and 0, flame > 0, health not 0. A change plays a rumble only then.
	bool hadAmmo1p = false;
	bool hadAmmo2p = false;
	bool hadFlame1p = false;
	bool hadFlame2p = false;
	bool hadHealth1p = false;
	bool hadHealth2p = false;

	UINT8 lastAmmo = 0;
	UINT8 lastAmmo2 = 0;
	float lastFlame = 0;
	float lastFlame2 = 0;
	float lastHealth = 0;
	float lastHealth2 = 0;
	UINT8 lastShoot = 0;
	UINT8 lastShoot2 = 0;
};
