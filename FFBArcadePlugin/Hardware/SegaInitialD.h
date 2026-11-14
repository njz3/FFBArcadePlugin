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
#include "OutputBoard.h"

namespace ffb
{
	// Drive board of Initial D and other Sega games (Naomi, Lindbergh, RingEdge...): a command,
	// a parameter and a strength. 0x80 centering (strength 1), 0x84 constant force (parameter: 0 from
	// the left, 1 from the right), 0x85 vibration (parameter: period), 0x86 spring.
	class SegaInitialDDriveBoard
	{
	public:
		// Scaling of the values, which differs between games
		struct Scaling
		{
			double strengthScale;  // strength of a full force
			int leftStrengthBase;  // a constant force from the left is (leftStrengthBase - strength) / strengthScale
			double periodScale;    // a vibration period is parameter / periodScale * 120 ms
			UINT32 sineLength_ms;  // length of the vibration sine effect
			UINT32 maxStrength;    // spring and constant force commands with a higher strength are ignored
		};

		// Scaling of most games
		static const Scaling DefaultScaling;

		explicit SegaInitialDDriveBoard(Wheel& wheel, const Scaling& scaling = DefaultScaling)
			: wheel(wheel), scaling(scaling)
		{
		}

		void Play(BYTE command, BYTE parameter, UINT32 strength);

	private:
		Wheel& wheel;
		Scaling scaling;
	};

	// The drive board of Initial D, The King of Route 66, Club Kart and Sega Driving Simulator in
	// Flycast: its midiffb output holds the command in byte 2, the parameter in byte 1, the strength in byte 0
	class InitialDMidiBoard : public OutputBoard
	{
	public:
		const char* Name() const override { return "InitialDActive"; }
		void OnOutput(const char* name, int value) override;

	private:
		SegaInitialDDriveBoard driveBoard{ wheel, { 127.0, 128, 127.0, 100, 0x7F } };
	};
}
