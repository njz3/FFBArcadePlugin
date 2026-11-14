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

namespace ffb
{
	// The force settings of a game of an emulator, saved in FFBPlugin.ini with the suffix of the game
	// (MinForceDaytona2, AlternativeMaxForceLeftDaytona2...). Once the game is known, they replace
	// the settings of the same name without suffix.
	class ForceProfile
	{
	public:
		// The settings a game has
		enum Field : unsigned
		{
			Force = 1 << 0,              // MinForce, MaxForce
			Alternative = 1 << 1,        // AlternativeMinForceLeft, AlternativeMaxForceLeft, AlternativeMinForceRight, AlternativeMaxForceRight
			FeedbackLength = 1 << 2,
			PowerMode = 1 << 3,
			Damper = 1 << 4,             // EnableDamper, DamperStrength
			Spring = 1 << 5,             // EnableForceSpringEffect, ForceSpringStrength
			Sine = 1 << 6,               // SinePeriod, SineFadePeriod, SineStrength
			Rumble = 1 << 7,             // RumbleStrengthLeftMotor, RumbleStrengthRightMotor
			Device2 = 1 << 8,            // MinForce and MaxForce of device 2, e.g. MinForceRevXDevice2
			Device2Alternative = 1 << 9, // the alternative forces of device 2
			Device3 = 1 << 10,           // MinForce and MaxForce of device 3
			Doubles = 1 << 11,           // DoubleSine, DoubleConstant
			FullSine = 1 << 12,          // SinePeriod and SineStrength default to 100 instead of 0
			HoldsAtZero = 1 << 13,       // the KeptConstant is held at a level of 0 instead of stopped (ConstantInfHoldsAtZero)

			// most racing games
			Racing = Force | Alternative | FeedbackLength | PowerMode | Damper | Spring,
		};

		// Reads the settings of the fields with the suffix and puts them in place of the settings
		// without suffix
		static void Apply(const wchar_t* suffix, unsigned fields);
	};
}
