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

#include "LGI.h"
#include "../Core/Devices.h"

using namespace ffb;

void LetsGoIslandGame::Update()
{
	UINT8 outputs = Memory::Read(PointerChain<UINT8, 1>{ { Base::Module, addresses.outputs }, { 0x44 } });
	INT_PTR players = Memory::Read(PointerChain<INT_PTR, 1>{ { Base::Module, addresses.players }, { 0x4 } });
	INT_PTR player1 = Memory::Read<INT_PTR>(players + addresses.player1);
	INT_PTR player2 = Memory::Read<INT_PTR>(players + addresses.player2);
	float health1p = Memory::Read<float>(player1 + 0x14);
	float health2p = Memory::Read<float>(player2 + 0x14);
	Log::Value("got value:", outputs);

	int feedbackLength = Settings::Int(L"FeedbackLength", 120);
	int howtoRumbleKnockEffect = Settings::Int(L"HowtoRumbleKnockEffect", 0);
	int howtoRumbleMotorEffect = Settings::Int(L"HowtoRumbleMotorEffect", 0);
	int howtoRumbleHealthEffect = Settings::Int(L"HowtoRumbleHealthEffect", 0);
	int knock1pStrength = Settings::Int(L"Knock1pStrength", 0);
	int motor1pStrength = Settings::Int(L"Motor1pStrength", 0);
	int health1pStrength = Settings::Int(L"Health1pStrength", 0);
	int knock2pStrength = Settings::Int(L"Knock2pStrength", 0);
	int motor2pStrength = Settings::Int(L"Motor2pStrength", 0);
	int health2pStrength = Settings::Int(L"Health2pStrength", 0);

	if (!devicesOpen)
	{
		OpenHapticDevice2();
		devicesOpen = true;
	}

	UINT32 length_ms = feedbackLength;
	if ((lastHealth1p != health1p) && (health1p != 1))
		RumbleWithMode(wheel, howtoRumbleHealthEffect, health1pStrength / 100.0, length_ms);

	if ((lastHealth2p != health2p) && (health2p != 1))
		RumbleWithMode(device2, howtoRumbleHealthEffect, health2pStrength / 100.0, length_ms);

	if (outputs & Knock1p)
		RumbleWithMode(wheel, howtoRumbleKnockEffect, knock1pStrength / 100.0, length_ms);

	if (outputs & Motor1p)
		RumbleWithMode(wheel, howtoRumbleMotorEffect, motor1pStrength / 100.0, length_ms);

	if (outputs & Knock2p)
		RumbleWithMode(device2, howtoRumbleKnockEffect, knock2pStrength / 100.0, length_ms);

	if (outputs & Motor2p)
		RumbleWithMode(device2, howtoRumbleMotorEffect, motor2pStrength / 100.0, length_ms);

	lastHealth1p = health1p;
	lastHealth2p = health2p;
}
