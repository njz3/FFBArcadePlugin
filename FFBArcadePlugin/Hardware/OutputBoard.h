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
#include <initializer_list>
#include "../Core/ForceFeedback.h"

namespace ffb
{
	// The force feedback hardware of an emulated game (drive board, wheel motor, shaker, gun recoil...),
	// driven by the outputs of the emulator: MAME (-output windows or network), Supermodel, Flycast.
	class OutputBoard
	{
	public:
		virtual ~OutputBoard() = default;

		// Name of the board in the log
		virtual const char* Name() const = 0;

		// An output of the emulator changed. Called by the thread of the outputs.
		virtual void OnOutput(const char* name, int value) {}

		// Every 16 ms by the FFB loop, with the name and the value of the last output that changed
		virtual void Update(const char* lastName, int lastValue) {}

		// The emulator stopped (mame_stop)
		virtual void OnStop() {}

	protected:
		OutputBoard();

		Wheel& wheel;
		Device& device2;
		Device& device3;

		// True when name is one of the outputs
		static bool Is(const char* name, std::initializer_list<const char*> outputs);

		// The constant force of these boards: a KeptConstant, unless the UseConstantInf setting is 0
		void SendConstant(Direction direction, double strength);
		// SendConstant of 0 from both sides
		void StopConstant();
		// The other side when the ReverseDirection setting is set
		static Direction Reversible(Direction direction);
	};
}
