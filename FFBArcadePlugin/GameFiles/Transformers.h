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

// Transformers: Human Alliance (Sega, TeknoParrot): rumble of the guns of the two players (player 2 on
// Device2GUID) while they shoot and when their health changes, read in the game's memory.
class Transformers : public ffb::Game
{
public:
	void Update() override;

private:
	// Pointer to the players: then +0x34, +0x39C the pointer to player 1, its health (byte) at +0x54;
	// +0x20, +0x398, +0x1C the pointer to player 2, its health (byte) at +0x310. 100 is the full health.
	static constexpr ffb::GameValue<INT_PTR> Players{ ffb::Base::Module, 0x1A3CC1C };
	// Shots of the guns: 0x4B both players shoot, 0x42 player 1, 0x09 player 2; 0x03, 0x90, 0x93, 0x02 and
	// 0x12 stop them
	static constexpr ffb::GameValue<UINT8> Gun{ ffb::Base::Absolute, 0xD8CE14 };
	// Triggers of the guns: 0 released, the shots stop
	static constexpr ffb::GameValue<UINT8> Trigger1p{ ffb::Base::Absolute, 0xD8D110 };
	static constexpr ffb::GameValue<UINT8> Trigger2p{ ffb::Base::Absolute, 0xD8D112 };

	// The values of Gun that stop the shots
	static bool StopsShots(UINT8 gun);

	int feedbackLength = ffb::Settings::Int(L"FeedbackLength", 120);
	int howtoRumbleGunEffect = ffb::Settings::Int(L"HowtoRumbleGunEffect", 0);
	int howtoRumbleHealthEffect = ffb::Settings::Int(L"HowtoRumbleHealthEffect", 0);
	int gun1pStrength = ffb::Settings::Int(L"Gun1pStrength", 0);
	int health1pStrength = ffb::Settings::Int(L"Health1pStrength", 0);
	int gun2pStrength = ffb::Settings::Int(L"Gun2pStrength", 0);
	int health2pStrength = ffb::Settings::Int(L"Health2pStrength", 0);

	bool devicesOpen = false;
	bool shooting1p = false;
	bool shooting2p = false;
	UINT8 lastHealth1p = 0;
	UINT8 lastHealth2p = 0;
};
