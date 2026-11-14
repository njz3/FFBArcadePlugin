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

// Force feedback hardware of the Namco games emulated by MAME
#pragma once
#include "OutputBoard.h"

namespace ffb
{
	// Wheel motor of Ace Driver and Ace Driver: Victory Lap (System 22): a bit-reversed signed force on
	// wheel_motor; mcuout3 at 0 switches the motor off
	class AceDriverWheelMotor : public OutputBoard
	{
	public:
		const char* Name() const override { return "NamcoFFBNew"; }
		void OnOutput(const char* name, int value) override;
	};

	// Wheel motor of Rave Racer (System 22): a bit-reversed force on wheel_motor; mcuout3 at 0 switches
	// the motor off. The ReverseDirection setting swaps its sides.
	class RaveRacerWheelMotor : public OutputBoard
	{
	public:
		const char* Name() const override { return "RaveRacerNew"; }
		void OnOutput(const char* name, int value) override;
	};

	// Wheel motor of Dirt Dash (System 22): no output, its force is read in the memory of the emulated
	// game, found by a pattern scan once cpuled6 lights. mcuout1 switches the motor off (0) and on (1).
	// FFBDivideDirtDash is the force of a full turn.
	class DirtDashWheelMotor : public OutputBoard
	{
	public:
		DirtDashWheelMotor();
		const char* Name() const override { return "DirtDashFFB"; }
		void Update(const char* lastName, int lastValue) override;

	private:
		static DWORD WINAPI ScanThread(LPVOID board);

		double divide;
		int motorOn = 1; // assumed on until MAME sends mcuout1, so that the wheel is not dead at start
		bool scanAllowed = false;
		bool scanStarted = false;
		bool found = false;
		char* volatile pattern = nullptr; // set by the scan thread
		INT_PTR forceAddress = 0;
	};
}
