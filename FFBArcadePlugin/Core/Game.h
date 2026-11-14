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
#include <Windows.h>
#include "ForceFeedback.h"
#include "Log.h"
#include "Memory.h"
#include "Settings.h"

namespace ffb
{
	// A game (or an emulator) whose force feedback the plugin plays. It is created by the FFB thread
	// once the devices are open, then updated every 16 ms.
	class Game
	{
	public:
		virtual ~Game() = default;

		// Called by the FFB loop every 16 ms
		virtual void Update() = 0;

	protected:
		Game();

		Wheel& wheel;    // device 1
		Device& device2; // Device2GUID: second gun, second player...
		Device& device3; // Device3GUID

		// Damper set by the EnableDamper and DamperStrength settings
		void ApplyDamper();

		// Rumble set by a HowtoRumbleXxxEffect setting: 0 both motors, 1 the second one only, 2 the first one only
		static void RumbleWithMode(Device& device, int mode, double strength, UINT32 length_ms);
	};
}
