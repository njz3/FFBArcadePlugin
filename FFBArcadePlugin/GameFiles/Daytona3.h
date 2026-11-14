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

// Daytona Championship USA and its New Season Edition (Sega): the command of the drive board is
// read in the game's memory. On the tracks 2 and 4, in a race, the rolls are played the other way.
class Daytona3Game : public ffb::Game
{
protected:
	// Offsets in the game's executable of the values of a version of the game
	struct Addresses
	{
		// byte: 0x81-0x8F spring, 0x91-0x9F roll right, 0xA1-0xAF roll left, 0xB1-0xBF shaking,
		// 0xC1-0xCF spring and vibration, 0xD1-0xDF no effect, 0xE1-0xEF spring (big crash?),
		// 0xF1-0xFF spring (menus)
		INT_PTR driveBoardCommand;
		INT_PTR gameState;         // byte: 0x16 in a race
		INT_PTR trackData;         // 32 bits: address of the data of the selected track, whose byte 4 is its number
	};

	explicit Daytona3Game(const Addresses& addresses);

	// Reads the drive board command and plays it
	void PlayDriveBoard();

private:
	Addresses addresses;
};

// Daytona Championship USA: the drive board command is played with the OriginalFFB setting; without it,
// a force of the wheel read in the game's memory. The plugin can hide the cursor (HideCursor) and end
// the game with the Escape key (EscapeKeyExitViaPlugin).
class Daytona3 : public Daytona3Game
{
public:
	Daytona3() : Daytona3Game({ 0x131FEC9, 0x15B5744, 0x11B0148 }) {}
	void Update() override;

private:
	// Force of the wheel: 16 to 31 from the left (16 the strongest), 1 to 15 from the right (1 the
	// strongest), 0 no force
	static constexpr ffb::GameValue<UINT8> WheelForce{ ffb::Base::Absolute, 0x15AFC46 };

	int escapeKeyExitViaPlugin = ffb::Settings::Int(L"EscapeKeyExitViaPlugin", 0);
	int hideCursor = ffb::Settings::Int(L"HideCursor", 0);
	int originalFFB = ffb::Settings::Int(L"OriginalFFB", 0);
};
