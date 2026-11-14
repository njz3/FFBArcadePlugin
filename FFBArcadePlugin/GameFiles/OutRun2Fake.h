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
#include "SDL.h"
#include "../Core/Game.h"

// OutRun 2 SP Special Tours Deluxe (Sega Lindbergh, TeknoParrot), the two versions of its force feedback:
// OutRun2Fake makes it up from values of the game's memory, OutRun2Real plays the commands the game sends
// to its drive board (OutRun2Real.h). Both play their effects in a thread of their own, every 16 ms:
// the FFB loop stays in the loop of the joystick events, where the buttons can change the gear
// (ChangeGearsViaPlugin, Gear1 to Gear6) or show their number (ShowButtonNumbersForSetup).
class OutRun2Game : public ffb::Game
{
protected:
	// Speed of the car in km/h; the effects are stronger with it (SpeedStrength)
	static constexpr ffb::GameValue<float> Speed{ ffb::Base::Absolute, 0x08273DF0 };
	// 1 in a race
	static constexpr ffb::GameValue<UINT8> InRace{ ffb::Base::Absolute, 0x08304ADC };
	// Gear of the car, 1 to 6 (written by the gear buttons)
	static constexpr ffb::GameValue<UINT8> Gear{ ffb::Base::Absolute, 0x0827A160 };
	// 1 with the manual transmission: the gear buttons work then
	static constexpr ffb::GameValue<UINT8> Transmission{ ffb::Base::Absolute, 0x082932C2 };

	// Strength of the effects for a speed, in %: 10 up to 80 km/h, 20 up to 130... 100 up to 1000,
	// 0 below 0.1 km/h, above 1000 km/h and between the steps (80 to 80.1 km/h...)
	static int SpeedStrength(float speed);

	// Starts the thread of the effects, the first time
	void StartThread();
	// A joystick event: shows the number of a button pressed, changes the gear
	void HandleEvent(const SDL_Event& event);

	// SpeedStrength of the speed, set by the thread
	int speedStrength = 0;

private:
	// The thread of the effects: ThreadLoop every 16 ms
	static DWORD WINAPI RunningLoop(LPVOID game);
	virtual void ThreadLoop() = 0;

	int showButtonNumbersForSetup = ffb::Settings::Int(L"ShowButtonNumbersForSetup", 0);
	int changeGearsViaPlugin = ffb::Settings::Int(L"ChangeGearsViaPlugin", 0);
	// The buttons of the gears 1 to 6
	int gearButtons[6] = {
		ffb::Settings::Int(L"Gear1", 0),
		ffb::Settings::Int(L"Gear2", 0),
		ffb::Settings::Int(L"Gear3", 0),
		ffb::Settings::Int(L"Gear4", 0),
		ffb::Settings::Int(L"Gear5", 0),
		ffb::Settings::Int(L"Gear6", 0),
	};
	bool threadStarted = false;
};

// OutRun 2 SP SDX, force feedback made up from values of the game's memory: the steering position in a
// race and in the menus, the hits on the sides, the crashes and the ground (vibrations), by speed.
class OutRun2Fake : public OutRun2Game
{
public:
	void Update() override;

private:
	// Ground under the car, it seems: 8, 4 and 16 play vibrations (logged)
	static constexpr ffb::GameValue<int> Ground{ ffb::Base::Absolute, 0x0827A1A0 };
	// Read, not used (named "ffwall")
	static constexpr ffb::GameValue<int> Wall{ ffb::Base::Absolute, 0x08273FAC };
	// Changes at a hit on a side of the car (the side in HitSide)
	static constexpr ffb::GameValue<int> SideHit{ ffb::Base::Absolute, 0x0827A1DA };
	// Changes at a crash: a vibration
	static constexpr ffb::GameValue<int> Crash{ ffb::Base::Absolute, 0x0827A35D };
	// Side of the hits: 1 force from the left, 2 from the right
	static constexpr ffb::GameValue<int> HitSide{ ffb::Base::Absolute, 0x0827A1D4 };
	// Steering: below 0x7F force from the left, above from the right (stronger away from 0x7F)
	static constexpr ffb::GameValue<UINT8> Steering{ ffb::Base::Absolute, 0x08670DC8 };
	// Read, not used
	static constexpr ffb::GameValue<float> UnusedValue{ ffb::Base::Absolute, 0x08273AD4 };
	// 1 in the menus
	static constexpr ffb::GameValue<UINT8> InMenu{ ffb::Base::Absolute, 0x086749CA };

	void ThreadLoop() override;
	// The steering as a constant force
	void PlaySteering(UINT8 steering);

	int lastSideHit = 0;
	int lastCrash = 0;
};
