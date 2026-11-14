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

// Hummer and Hummer Extreme (Sega, Lindbergh, TeknoParrot): the plugin hooks the functions of
// clSteerDualDevice, the wheel driver of the game, and plays the vibration, the viscosity (as a friction)
// and the centering spring the game gives them. The wait of the driver for the wheel is skipped.
class HummerGame : public ffb::Game
{
public:
	void Update() override;

protected:
	// A version of the game
	struct Version
	{
		// Functions of clSteerDualDevice
		INT_PTR wait;                 // waits for the wheel to be ready for an update: returns 0 at once
		INT_PTR setTrqCurveHalfPoint; // hooked, left as it is
		INT_PTR setVibrate;           // vibration: strength and duration
		INT_PTR setViscosity;         // viscosity, played as a friction
		INT_PTR initCenter;           // centering spring
		double viscosityScale;        // multiplies the viscosity
		double springScale;           // multiplies the centering spring
		const char* hookedMessage;    // debug output (OutputDebugString) before the hooks are made, or null
		bool traceVibrations;         // debug output of each vibration
	};

	explicit HummerGame(const Version& version);

private:
	static HummerGame* instance;
	static void(__cdecl* setTrqCurveHalfPointOriginal)(void* device, int param_1, float param_2, float param_3);
	static void(__cdecl* setVibrateOriginal)(void* device, int param_1, float strength, float duration);
	static void(__cdecl* setViscosityOriginal)(void* device, int param_1, float viscosity, float param_3);
	static void(__cdecl* initCenterOriginal)(void* device, int param_1, unsigned char strength);
	static int(__cdecl* waitOriginal)(int param_1);
	static void __cdecl SetTrqCurveHalfPointHook(void* device, int param_1, float param_2, float param_3);
	static void __cdecl SetVibrateHook(void* device, int param_1, float strength, float duration);
	static void __cdecl SetViscosityHook(void* device, int param_1, float viscosity, float param_3);
	static void __cdecl InitCenterHook(void* device, int param_1, unsigned char strength);
	static int __cdecl WaitHook(int param_1);

	Version version;
	bool hooked = false;
};

// Hummer: see HummerGame. The viscosity and the spring of the game are played as they are.
class Hummer : public HummerGame
{
public:
	Hummer() : HummerGame({ 0x82cdd1a, 0x82cd9ba, 0x82cdbfa, 0x82cdada, 0x82cd786, 1.0, 1.0, "Hummer FFB Plugin Loaded\n", false }) {}
};
