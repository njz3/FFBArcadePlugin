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
	// Motors switched on (1) and off by an output, one per player, on the device of the player: gun
	// recoil solenoids, wheel or seat vibration motors. A motor on plays the rumble of the
	// RumbleStrengthLeftMotor and RumbleStrengthRightMotor settings, and for the wheel vibrations
	// the sine of the SineXxx settings. Played every 16 ms with the last state of the outputs.
	class OnOffMotors : public OutputBoard
	{
	public:
		enum class Kind
		{
			GunRecoil,       // light gun games, up to 3 players
			WheelVibration,  // Outrunners, Turbo Out Run, Cadillacs and Dinosaurs, Double Axle: up to 2 players
			JalecoVibration, // Cisco Heat, F1 Grand Prix Star 1 and 2: led2
			SeatVibration,   // After Burner II: lamp1
		};

		explicit OnOffMotors(Kind kind) : kind(kind) {}
		const char* Name() const override;
		void Update(const char* lastName, int lastValue) override;

	private:
		Kind kind;
		int state[3] = { 0, 0, 0 }; // last value of the output of each player

		int Players() const;
		bool IsOutputOf(int player, const char* name) const;
		Device& DeviceOf(int player);
	};
}
