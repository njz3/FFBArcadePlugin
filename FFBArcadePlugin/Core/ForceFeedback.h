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

namespace ffb
{
	// Side a force comes from: a force from the left turns the wheel to the right
	enum class Direction : int
	{
		FromLeft = -1,
		FromRight = 1,
	};

	// The opposite side
	inline Direction Opposite(Direction direction)
	{
		return direction == Direction::FromLeft ? Direction::FromRight : Direction::FromLeft;
	}

	// Effects of a device. Strengths go from 0 to 1 and are scaled by the force settings of the device
	// (MinForce, MaxForce...); lengths are in ms.
	class Device
	{
	public:
		virtual ~Device() = default;

		// Vibration of the force feedback motor; a negative strength reverses it.
		virtual void Sine(UINT16 period_ms, UINT16 fadePeriod_ms, double strength, UINT32 length_ms) = 0;

		// Rumble motors of a game controller (EnableRumble)
		virtual void Rumble(double highFrequency, double lowFrequency, UINT32 length_ms) = 0;

		// Rumble effect of a haptic device
		virtual void LeftRight(double smallStrength, double largeStrength, UINT32 length_ms) = 0;
	};

	// Device 1, the wheel (DeviceGUID): all the effects
	class Wheel : public Device
	{
	public:
		// Constant force for FeedbackLength
		virtual void Constant(Direction direction, double strength) = 0;
		// Constant force that lasts until the next call changes it (a strength of 0 stops it). It is
		// renewed by the plugin and ends by itself within 1 s if the game is killed.
		virtual void KeptConstant(Direction direction, double strength) = 0;

		virtual void Spring(double strength) = 0;
		// Spring that lasts until another spring replaces it, scaled by ForceSpringStrength when
		// EnableForceSpringEffect is set
		virtual void KeptSpring(double strength) = 0;
		virtual void Friction(double strength) = 0;
		virtual void Inertia(double strength) = 0;
		virtual void Damper(double strength) = 0;

		virtual void Triangle(double strength, UINT32 length_ms) = 0;
		virtual void SawtoothUp(double strength, UINT32 length_ms) = 0;
		virtual void SawtoothDown(double strength, UINT32 length_ms) = 0;
		virtual void Ramp(double start, double end, UINT32 length_ms) = 0;

		// Rumble of the triggers of a game controller (EnableRumbleTriggers)
		virtual void RumbleTriggers(double leftTrigger, double rightTrigger, UINT32 length_ms) = 0;

		// The constant force with the rumble of the motor of its side, as most games play their torque:
		// the first rumble motor for a force from the left, the second one from the right.
		void ConstantWithRumble(Direction direction, double strength, UINT32 rumbleLength_ms = 100)
		{
			if (direction == Direction::FromLeft)
				Rumble(strength, 0, rumbleLength_ms);
			else
				Rumble(0, strength, rumbleLength_ms);
			Constant(direction, strength);
		}

		// Constant force of 0 from both sides
		void StopConstant()
		{
			Constant(Direction::FromLeft, 0);
			Constant(Direction::FromRight, 0);
		}
	};

	// The devices the games play their effects on, set by the plugin (or a test harness) before a game is created
	Wheel& MainWheel();
	Device& SecondDevice(); // Device2GUID
	Device& ThirdDevice();  // Device3GUID
	void UseDevices(Wheel& wheel, Device& second, Device& third);
}
