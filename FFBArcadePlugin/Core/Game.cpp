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

#include "Game.h"

extern int EnableDamper;
extern int DamperStrength;

namespace ffb
{
	static Wheel* mainWheel = nullptr;
	static Device* secondDevice = nullptr;
	static Device* thirdDevice = nullptr;

	Wheel& MainWheel() { return *mainWheel; }
	Device& SecondDevice() { return *secondDevice; }
	Device& ThirdDevice() { return *thirdDevice; }

	void UseDevices(Wheel& wheel, Device& second, Device& third)
	{
		mainWheel = &wheel;
		secondDevice = &second;
		thirdDevice = &third;
	}

	Game::Game()
		: wheel(MainWheel()), device2(SecondDevice()), device3(ThirdDevice())
	{
	}

	void Game::ApplyDamper()
	{
		if (EnableDamper)
			wheel.Damper(DamperStrength / 100.0);
	}

	void Game::RumbleWithMode(Device& device, int mode, double strength, UINT32 length_ms)
	{
		if (mode == 0)
			device.Rumble(strength, strength, length_ms);
		else if (mode == 1)
			device.Rumble(0, strength, length_ms);
		else if (mode == 2)
			device.Rumble(strength, 0, length_ms);
	}
}
