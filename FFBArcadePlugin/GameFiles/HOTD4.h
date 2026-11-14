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

// The House of the Dead 4 (Sega Lindbergh, TeknoParrot): rumble of the guns of the two players (player 2
// on Device2GUID) at the shots, the hits, the grenades and when a player has no more health. The values
// are read through pointers from the base of cgGL.dll (gl_cgGLDll), by a thread of the plugin every 16 ms.
// The settings are read again at each iteration.
class HOTD4 : public ffb::Game
{
public:
	void Update() override;

private:
	// Offsets from the base of cgGL.dll of the pointers to:
	// the players: then +0x11C, +0x50, then +0x20 the pointer to player 1, +0x38 to player 2
	static const INT_PTR PlayersPointer = 0x9164;
	// the start buttons: then +0x130, then +0x234 the pointer to the buttons
	static const INT_PTR StartButtonsPointer = 0x3C40;
	// In the structure of a player (bytes)
	static const INT_PTR HealthOffset = 0x3C;      // changes when the player is hit
	static const INT_PTR BulletsOffset = 0x274;    // changes at the shots
	static const INT_PTR GrenadeOffset = 0x2B3;    // 0x42 when a grenade explodes
	static const INT_PTR StateOffset = 0x38;       // 3 playing; 11 with a health of 0: no health left
	// The start buttons (bytes): the effects of a player who plays are played when it is not 0x80
	static const INT_PTR StartButton1pOffset = 0x530;
	static const INT_PTR StartButton2pOffset = 0x538;

	// The thread of the effects: ThreadLoop every 16 ms
	static DWORD WINAPI RunningLoop(LPVOID game);
	void ThreadLoop();

	bool started = false;
	// The rumble of no health left was played, until the player plays again
	bool noHealth1pPlayed = false;
	bool noHealth2pPlayed = false;
	UINT8 lastHealth1p = 0;
	UINT8 lastBullets1p = 0;
	UINT8 lastHealth2p = 0;
	UINT8 lastBullets2p = 0;
};
