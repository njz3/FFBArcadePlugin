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

// The games of the emulators that send their outputs to the plugin: MAME (-output windows or
// -output network), Supermodel and Flycast. The game is found by its rom name in the table of
// EmulatorGames.cpp, which gives its force settings and its force feedback hardware.
// The outputs arrive in the thread of the outputs (MAMEOutputs.cpp, MAMENetworkOutputs.cpp).
class MAMESupermodel : public ffb::Game
{
public:
	void Update() override;

	// The rom name of the game, sent when it starts
	static void OnGameName(const char* name);
	// An output of the game changed
	static void OnOutput(const char* name, int value);
	// The emulator stopped
	static void OnStop();

private:
	bool devicesOpen = false;
};
