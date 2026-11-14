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

// Let's Go Island and Let's Go Island 3D (Sega Lindbergh, TeknoParrot): rumble for the two players
// (player 2 on Device2GUID) from the outputs of the cabinet (knockers and motors) and when the health of
// a player changes, read in the game's memory. The settings are read again at each update.
class LetsGoIslandGame : public ffb::Game
{
public:
	void Update() override;

protected:
	// Addresses of a version of the game
	struct Addresses
	{
		INT_PTR outputs; // offset from the module of the pointer to the outputs: their bits at +0x44
		INT_PTR players; // offset from the module of a pointer: at +0x4 the pointer to the players
		INT_PTR player1; // offset in the players of the pointer to player 1: its health (a float) at +0x14
		INT_PTR player2; // same for player 2
	};

	explicit LetsGoIslandGame(const Addresses& addresses) : addresses(addresses) {}

private:
	// The bits of the outputs
	static const UINT8 Knock1p = 0x20;
	static const UINT8 Motor1p = 0x40;
	static const UINT8 Knock2p = 0x04;
	static const UINT8 Motor2p = 0x08;

	Addresses addresses;
	bool devicesOpen = false;
	float lastHealth1p = 0.0;
	float lastHealth2p = 0.0;
};

// Let's Go Island
class LGI : public LetsGoIslandGame
{
public:
	LGI() : LetsGoIslandGame({ 0x0063BF5C, 0x00820024, 0x58, 0x5C }) {}
};
