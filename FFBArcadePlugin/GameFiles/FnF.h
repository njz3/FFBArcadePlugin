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

// Fast & Furious and Fast & Furious Drift (Raw Thrills, TeknoParrot): the games write the force of the
// wheel to the parallel port (0x378), through a function the plugin replaces. The effects change only
// when that value changes: playing them at each write sent thousands of effects per second (442771 in
// a minute of FNF Drift at a high frame rate). 0x00-0x7F force from the right, 0x80-0xFF from the left.
class FastAndFuriousGame : public ffb::Game
{
public:
	void Update() override;

protected:
	// Addresses of a version of the game
	struct Addresses
	{
		INT_PTR ioEnabled;      // set to 1: I/O calls enabled
		INT_PTR sendToLpt;      // writes a byte to a port
		INT_PTR otherLptIo[4];  // the other I/O functions, emptied
	};

	explicit FastAndFuriousGame(const Addresses& addresses);

private:
	// The rumble lasts RumbleLength and is renewed every RumbleRenewal while it is not 0, so that it
	// ends by itself if the game is killed (the constant force is renewed by the plugin)
	static const UINT32 RumbleLength = 1000; // ms
	static const ULONGLONG RumbleRenewal = 300; // ms

	static FastAndFuriousGame* instance;
	static int(__cdecl* sendToLptOriginal)(uint16_t port, uint8_t value);
	static void(__cdecl* ignoredOriginal)();
	static int __cdecl SendToLptHook(uint16_t port, uint8_t value);
	static void __cdecl IgnoredHook();

	// Strength of a force, 0 to 1, in either direction
	static double Strength(LONG value);

	Addresses addresses;
	bool hooked = false;
	volatile LONG lastValue = -1; // written to the port
	ULONGLONG rumbleRenewed = 0;
};

class FnF : public FastAndFuriousGame
{
public:
	FnF() : FastAndFuriousGame({ 0x5B2ED4, 0x46DA10, { 0x46DD60, 0x46DDA0, 0x46DEC0, 0x46DF70 } }) {}
};
