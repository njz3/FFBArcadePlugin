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

// Golden Gun (Adrenaline Amusements, TeknoParrot): rumble of the guns of the two players (player 2 on
// Device2GUID) at the shots, the hits and when a player has no more health, read in the game's memory.
class GoldenGun : public ffb::Game
{
public:
	void Update() override;

private:
	// Structures of the players
	static constexpr ffb::PointerChain<INT_PTR, 2> Player1{ { ffb::Base::Module, 0x27034A0 }, { 0x24, 0x34 } };
	static constexpr ffb::PointerChain<INT_PTR, 2> Player2{ { ffb::Base::Module, 0x2703388 }, { 0x28, 0x38 } };
	// In the structure of a player (bytes)
	static const INT_PTR HealthOffset = 0x3C; // 0 with no health left
	static const INT_PTR AmmoOffset = 0x6C;   // changes at the shots
	static const INT_PTR StateOffset = 0x38;  // 3 playing; 12 or 14 with a health of 0: no health left
	// Start buttons: the rumble of the hits is played when it is not 0x80
	static constexpr ffb::GameValue<UINT8> StartButton1p{ ffb::Base::Module, 0x478F8C };
	static constexpr ffb::GameValue<UINT8> StartButton2p{ ffb::Base::Module, 0x478F94 };

	int feedbackLength = ffb::Settings::Int(L"FeedbackLength", 120);
	int howtoRumbleGunEffect = ffb::Settings::Int(L"HowtoRumbleGunEffect", 0);
	int howtoRumbleHealthEffect = ffb::Settings::Int(L"HowtoRumbleHealthEffect", 0);
	int gun1pStrength = ffb::Settings::Int(L"Gun1pStrength", 0);
	int health1pStrength = ffb::Settings::Int(L"Health1pStrength", 0);
	int gun2pStrength = ffb::Settings::Int(L"Gun2pStrength", 0);
	int health2pStrength = ffb::Settings::Int(L"Health2pStrength", 0);

	bool devicesOpen = false;
	// The rumble of no health left was played: until player 1 is hit again; for player 2, for good
	// (it is never reset, unlike for player 1)
	bool noHealth1pPlayed = false;
	bool noHealth2pPlayed = false;
	UINT8 lastHealth1p = 0;
	UINT8 lastAmmo1p = 0;
	UINT8 lastHealth2p = 0;
	UINT8 lastAmmo2p = 0;
};
