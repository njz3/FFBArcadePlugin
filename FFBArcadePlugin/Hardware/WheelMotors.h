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

// Wheel motors driven by a force value on an output: the encodings of the value
#pragma once
#include "OutputBoard.h"

namespace ffb
{
	// Signed 8-bit force on the wheel output of the Midway and Atari games (California Speed, Rush
	// 2049, Hyperdrive, Vapor TRX, Cart Fury) and of Speed Up: 0x01-0x7F force from the right,
	// 0x80-0xFF from the left
	class SignedWheelMotor : public OutputBoard
	{
	public:
		const char* Name() const override { return "RacingFullValueActive1"; }
		void OnOutput(const char* name, int value) override;
	};

	// The same with the other sides, on the wheel output only (San Francisco Rush, Cruis'n USA, Cruis'n
	// World, Off Road Challenge): 0x01-0x7F force from the left, 0x81-0xFF from the right
	class ReversedSignedWheelMotor : public OutputBoard
	{
	public:
		const char* Name() const override { return "RacingFullValueActive2"; }
		void OnOutput(const char* name, int value) override;
	};

	// Cruis'n Exotica: signed 8-bit force, 0x80 neutral, multiplied by the FFBGainCrusnExotica setting (%)
	class CruisnExoticaWheelMotor : public OutputBoard
	{
	public:
		CruisnExoticaWheelMotor();
		const char* Name() const override { return "RacingFullValueExotica"; }
		void OnOutput(const char* name, int value) override;
	private:
		int gain;
	};

	// Konami wheel motor (Thrill Drive, Winding Heat, GTI Club, Midnight Run, Racing Jam 1 and 2):
	// bits 0-3 force level, bit 4 side
	class KonamiWheelMotor : public OutputBoard
	{
	public:
		const char* Name() const override { return "Konami"; }
		void OnOutput(const char* name, int value) override;
	};

	// Hyper Neo Geo 64 wheel motor (Roads Edge, Xtreme Rally): 129-191 and 193-255
	class Hng64WheelMotor : public OutputBoard
	{
	public:
		const char* Name() const override { return "hng64"; }
		void OnOutput(const char* name, int value) override;
	};

	// Driver's Edge wheel motor: signed force from -31 to 31. The ReverseDirection setting swaps its sides.
	class DriversEdgeWheelMotor : public OutputBoard
	{
	public:
		const char* Name() const override { return "DriversEdgeActive"; }
		void OnOutput(const char* name, int value) override;
	};

	// Atomiswave wheel of Maximum Speed and Faster Than Speed (Flycast awffb output): odd values force
	// from the left, even values from the right, in steps of 1/16
	class MaximumSpeedWheelMotor : public OutputBoard
	{
	public:
		const char* Name() const override { return "MaximumSpeedActive"; }
		void OnOutput(const char* name, int value) override;
	};

	// Hard Drivin' and Race Drivin': the force is sent in 5 values of the wheel output (frames)
	class HardDrivinWheelMotor : public OutputBoard
	{
	public:
		const char* Name() const override { return "HardDrivinActive"; }
		void OnOutput(const char* name, int value) override;
	private:
		bool framing = false; // the frames are read from the second output on
		int frame = 0;
		int values[5] = { 0 };
		int previous[4] = { 0 }; // last values of the wheel output, previous[0] the latest
	};

	// Side by Side 1 and 2: 5 bits, even values from the left, odd values from the right, the lowest the strongest
	class SideBySideWheelMotor : public OutputBoard
	{
	public:
		const char* Name() const override { return "SideBSActive"; }
		void OnOutput(const char* name, int value) override;
	};

	// Super Chase: no output, its force is read in the memory of the emulated game, found by a pattern scan
	class SuperChaseWheelMotor : public OutputBoard
	{
	public:
		const char* Name() const override { return "SuperChaseActive"; }
		void Update(const char* lastName, int lastValue) override;
	private:
		static DWORD WINAPI ScanThread(LPVOID board);

		bool waited = false;
		bool scanStarted = false;
		bool found = false;
		char* volatile pattern = nullptr; // set by the scan thread
		INT_PTR forceAddress = 0;
	};
}
