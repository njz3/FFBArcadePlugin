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

#include <stdlib.h>
#include "Sega.h"
#include "../Core/Log.h"
#include "../Core/Config.h"

static ffb::Config& config = ffb::Config::Get();

// settings of the game, applied by its profile

namespace ffb
{
	void VirtuaRacingDriveBoard::OnOutput(const char* name, int value)
	{
		if (!Is(name, { "digit0" }))
			return;

		Log::Value("got value:", value);

		if (value == 0x03 || value == 0x07 || value == 0x09 || value == 0x10)
		{
			if (value == 0x07)
				uncenteringVibrates = true;

			if (value == 0x09)
				uncenteringVibrates = false;

			wheel.Spring(0.8);
			SendConstant(Direction::FromLeft, 0.0);
		}

		if (value == 0x20 || value == 0x28) // clutch
		{
			wheel.Friction(0.4);
			SendConstant(Direction::FromLeft, 0.0);
		}

		if (value > 0x2F && value < 0x40) // centering
		{
			wheel.Spring((value - 47) / 11.0);
			SendConstant(Direction::FromLeft, 0.0);
		}

		if (value == 0x40 || value == 0x46 || value == 0x4A) // uncentering
		{
			if (value == 0x40 || uncenteringVibrates)
			{
				wheel.Rumble(0.4, 0.4, 100);
				wheel.Sine(70, 30, 0.4, 100);
			}
			SendConstant(Direction::FromLeft, 0.0);
		}

		if (value == 0x50 || value == 0x5F) // roll left
		{
			wheel.Rumble(0, 0.5, 100);
			SendConstant(Direction::FromRight, 0.5);
		}
		else if (value == 0x60 || value == 0x6F) // roll right
		{
			wheel.Rumble(0.5, 0, 100);
			SendConstant(Direction::FromLeft, 0.5);
		}
		else
		{
			SendConstant(Direction::FromLeft, 0.0);
		}
	}

	void Model2DriveBoard::OnOutput(const char* name, int value)
	{
		if (!Is(name, { "wheel_motor" }))
			return;

		Log::Value("got value:", value);

		if (value > 0x09 && value < 0x18) // spring
			wheel.Spring((value - 15) / 8.0);

		if (value > 0x1F && value < 0x28) // clutch
			wheel.Friction((value - 31) / 8.0);

		if (value > 0x2F && value < 0x3D) // centering
			wheel.Spring((value - 47) / 13.0);

		if (value > 0x3F && value < 0x48) // uncentering
		{
			double strength = (value - 63) / 8.0;
			wheel.Sine(40, 0, strength, 100);
			wheel.Rumble(strength, strength, 100);
		}

		if (value > 0x4F && value < 0x58) // roll left
		{
			double strength = (value - 79) / 8.0;
			wheel.Rumble(0, strength, 100);
			SendConstant(Direction::FromRight, strength);
		}
		else if (value > 0x5F && value < 0x68) // roll right
		{
			double strength = (value - 95) / 8.0;
			wheel.Rumble(strength, 0, 100);
			SendConstant(Direction::FromLeft, strength);
		}
		else
		{
			SendConstant(Direction::FromLeft, 0.0);
		}
	}

	void SegaRallyDriveBoard::OnOutput(const char* name, int value)
	{
		if (!Is(name, { "wheel_motor" }))
			return;

		Log::Value("got value:", value);

		if (value == 0x00)
		{
			wheel.Rumble(0, 0, 0);
			StopConstant();
			return;
		}

		if (value >= 0xC0 && value <= 0xDF) // force from the right, the wheel turns left
		{
			double strength = (value - 0xC0) / 31.0;
			wheel.Rumble(0, strength, 100);
			SendConstant(Direction::FromRight, strength);
		}
		else if (value >= 0x80 && value <= 0x9F) // force from the left, the wheel turns right
		{
			double strength = (value - 0x80) / 31.0;
			wheel.Rumble(strength, 0, 100);
			SendConstant(Direction::FromLeft, strength);
		}
	}

	void Daytona2DriveBoard::OnOutput(const char* name, int value)
	{
		if (!Is(name, { "RawDrive" }))
			return;

		Log::Value("got value:", value);

		if (value >= 0x0A && value < 0x20) // spring
			wheel.Spring((value - 9) / 16.0);

		if (value >= 0x20 && value < 0x30) // clutch
			wheel.Friction((value - 0x1F) / 16.0);

		if (value >= 0x30 && value < 0x40) // centering
		{
			double strength = (value - 0x2F) / 16.0;
			wheel.Rumble(strength, strength, 100);
			wheel.Sine(40, 0, strength, 100);
		}

		if (value >= 0x40 && value < 0x50) // uncentering
			wheel.Spring(0);

		if (value >= 0x50 && value < 0x60) // roll right
			wheel.ConstantWithRumble(Direction::FromLeft, (value - 0x4F) / 16.0);
		else if (value >= 0x60 && value < 0x70) // roll left
			wheel.ConstantWithRumble(Direction::FromRight, (value - 0x5F) / 16.0);

		// test menu
		if (value == 0x80)
		{
			wheel.Rumble(0, 0, 0);
			wheel.StopConstant();
		}
		else if (value == 0x81)
		{
			wheel.ConstantWithRumble(Direction::FromLeft, 0.5);
		}
		else if (value == 0x82)
		{
			wheel.ConstantWithRumble(Direction::FromRight, 0.5);
		}
	}

	void ScudRaceDriveBoard::OnOutput(const char* name, int value)
	{
		if (!Is(name, { "RawDrive" }))
			return;

		Log::Value("got value:", value);

		if (value > 0x0 && value <= 0x09) // sequences: only 3 bits are relevant
		{
			int newSequence = value & 0x7;
			if (newSequence == 1 || newSequence == 2)
				sequenceDuration_ms = 1000; // unlimited time?
			sequence = newSequence;
			if (newSequence > 0)
			{
				sequenceStart_ms = now_ms;
				Log::Value("start sequence", newSequence);
			}
			else
			{
				Log::Write("sequence stopped");
			}
		}

		if (value >= 0x10 && value < 0x20) // spring: 0x10 stops it, 0x11 to 0x17 (strong), the same again from 0x18 to 0x1F
		{
			if (value - 0x10 > 0)
			{
				double strength = (value & 0x7) / 7.0;
				wheel.Spring(strength);
				Log::Value("spring", strength * 100);
			}
			else
			{
				Log::Write("spring stopped");
			}
		}

		if (value >= 0x20 && value < 0x30) // gain of the torque and vibration commands, 0x20 (almost none) to 0x2F (strong)
		{
			intensity = (double)(value - 0x1F) / 16.0; // 0.066 to 1.0
			Log::Value("intensity", intensity * 100);
		}

		if (value >= 0x30 && value < 0x40) // vibration: 0x30 stops it, 0x31 to 0x35 (strong)
		{
			int vibration = value - 0x30;
			if (vibration > 0)
			{
				double strength = (double)(vibration) / 5.0 * intensity;
				wheel.Rumble(strength, strength, 100);
				wheel.Sine(40, 0, strength, 100);
				Log::Value("vibration", strength * 100);
			}
			else
			{
				Log::Write("vibration stopped");
			}
		}

		if (value >= 0x40 && value < 0x50)
		{
			// Power slide: a sine of torque that pushes the wheel to one side then to the other, like
			// driving in a hole, for 2 to 3 seconds. 0x40 to 0x47 (strong) push first right, then left;
			// 0x48 to 0x4F first left, then right.
			double strength = (value & 0xF) / 8.0;
			if (value < 0x48)
				strength *= -1.0;
			wheel.Sine(2000, 0, strength, 2000); // 2 seconds, no fade
			Log::Value("power slide", strength * 100);
		}

		if (value >= 0x50 && value < 0x60) // torque right
		{
			double strength = (double)(value - 0x4F) / 16.0 * intensity;
			wheel.ConstantWithRumble(Direction::FromLeft, strength);
			Log::Value("torque Right", strength * 100);
		}
		else if (value >= 0x60 && value < 0x70) // torque left
		{
			double strength = (double)(value - 0x5F) / 16.0 * intensity;
			wheel.ConstantWithRumble(Direction::FromRight, strength);
			Log::Value("torque Left", strength * 100);
		}

		// test menu
		if (value == 0x80)
		{
			wheel.Rumble(0, 0, 0);
			wheel.StopConstant();
		}
		else if (value == 0x81)
		{
			wheel.ConstantWithRumble(Direction::FromLeft, 0.5);
		}
		else if (value == 0x82)
		{
			wheel.ConstantWithRumble(Direction::FromRight, 0.5);
		}
	}

	void ScudRaceDriveBoard::Update(const char* lastName, int lastValue)
	{
		if (sequence <= 0)
			return;

		now_ms += 16;
		if ((sequenceStart_ms - now_ms) > sequenceDuration_ms)
		{
			Log::Value("sequence done", sequence);
			sequence = 0;
		}

		// a sequence effect every 16 ms
		if (sequence == 1 || sequence == 2)
		{
			// light shocks (very short impulses of torque) to random sides, like driving on gravel
			double strength = sequence == 1 ? 0.2 : 0.5;
			const int frequency = 200;
			if ((rand() % frequency) == 0)
				wheel.Constant((rand() % 2) == 0 ? Direction::FromLeft : Direction::FromRight, strength);
		}
	}

	void SegaRally2DriveBoard::OnOutput(const char* name, int value)
	{
		if (!Is(name, { "RawDrive" }))
			return;

		Log::Value("got value:", value);

		if (value >= 0x00 && value <= 0x3F)
			wheel.ConstantWithRumble(Direction::FromRight, value / 64.0);
		else if (value >= 0x40 && value <= 0x7F)
			wheel.ConstantWithRumble(Direction::FromLeft, (value - 0x40) / 64.0);
	}

	void NaomiDriveBoard::OnOutput(const char* name, int value)
	{
		if (!Is(name, { "m3ffb" }))
			return;

		Log::Value("got value:", value);

		if (value == 0xD0)
			return;

		if (lastCommand != value && ((value > 0xB0 && value < 0xC0) || value == 0x9F))
		{
			vibration = value;
			vibrating = true;
		}

		if (value > 0x10 && value < 0x20)
			wheel.Spring((value - 10) / 15.0);

		if (value > 0x20 && value < 0x30)
			wheel.Friction((value - 32) / 15.0);

		if (value > 0x30 && value < 0x40)
		{
			double strength = (value - 48) / 15.0;
			wheel.Rumble(strength, strength, 100);
			wheel.Sine(40, 0, strength, 100);
		}

		if (value > 0x40 && value < 0x50)
			wheel.Spring((value - 64) / 15.0);

		if (value > 0x50 && value < 0x60)
			wheel.ConstantWithRumble(Direction::FromLeft, (value - 80) / 15.0);

		if (value > 0x60 && value < 0x70 && value != 0x68 && value != 0x69 && value != 0x6A && value != 0x6B)
			wheel.ConstantWithRumble(Direction::FromRight, (value - 96) / 15.0);

		if (value > 0x80 && value < 0x90)
			wheel.ConstantWithRumble(Direction::FromRight, (value - 128) / 15.0);

		if (value > 0x90 && value < 0xA0)
			wheel.ConstantWithRumble(Direction::FromLeft, (value - 144) / 15.0);

		if (value > 0xA0 && value < 0xB0)
			wheel.ConstantWithRumble(Direction::FromLeft, (value - 160) / 15.0);

		if (value == 0xBF)
			wheel.ConstantWithRumble(Direction::FromRight, 1.0);

		if (value > 0xC0 && value < 0xD0)
			wheel.Spring((value - 192) / 15.0);

		if (value > 0xD0 && value < 0xE0)
			wheel.Friction((value - 208) / 15.0);

		if (value > 0xE0 && value < 0xE9)
			wheel.ConstantWithRumble(Direction::FromLeft, (value - 224) / 8.0);

		if (value > 0xE8 && value < 0xF0)
		{
			// the rumble of a force from the left, with a force from the right
			double strength = (value - 232) / 8.0;
			wheel.Rumble(strength, 0, 100);
			wheel.Constant(Direction::FromRight, strength);
		}

		if (vibrating)
		{
			if (++vibrationCount >= 31)
			{
				vibrating = false;
				vibrationCount = 0;
			}

			double strength = (vibration - 176) / 15.0;
			wheel.Rumble(strength, strength, 100);
			wheel.Sine(60, 0, strength, 100);
		}

		if (value == 0xC0)
		{
			wheel.Rumble(0, 0, 100);
			wheel.Sine(60, 0, 0, 100);
			wheel.Friction(0);
			wheel.Constant(Direction::FromRight, 0);
			wheel.Constant(Direction::FromLeft, 0);
		}
		lastCommand = value;
	}

	void SuperMonacoGPShaker::OnOutput(const char* name, int value)
	{
		// the outputs only change the speed of the shaker, played by Update
		if (Is(name, { "mainpcb:lamp0" }))
			lamp0 = value ? 1 : 0;
		else if (Is(name, { "mainpcb:lamp1" }))
			lamp1 = value ? 1 : 0;
		else if (Is(name, { "mainpcb:lamp2" }))
			lamp2 = value ? 1 : 0;
	}

	void SuperMonacoGPShaker::Update(const char* lastName, int lastValue)
	{
		// Upright shaker speed: lamp2 = 1, lamp0 = 2, lamp1 = 4
		const int level = lamp2 | (lamp0 << 1) | (lamp1 << 2);

		if (level > 0)
		{
			// The original upright has seven motor speed stages. SinePeriod is the period of stage 1,
			// shortened as the motor stage rises.
			static const double periodScale[8] = { 0.0, 1.00, 0.80, 0.65, 0.52, 0.42, 0.34, 0.28 };

			int period = static_cast<int>((config.sinePeriod * periodScale[level]) + 0.5);
			if (period < 1)
				period = 1;

			const double strength = (config.sineStrength / 100.0) * (level / 7.0);
			wheel.Sine(static_cast<UINT16>(period), config.sineFadePeriod, strength, period);
		}
		else
		{
			wheel.Sine(0, 0, 0.0, 0);
		}
	}

	// Sine and rumble of the vibration motor, set by the SineXxx and RumbleStrengthXxx settings
	static void Vibrate(Wheel& wheel, bool on)
	{
		if (on)
		{
			wheel.Sine(config.sinePeriod, config.sineFadePeriod, config.sineStrength / 100.0, 100);
			wheel.Rumble(config.rumbleStrengthLeftMotor / 100.0, config.rumbleStrengthRightMotor / 100.0, 100);
		}
		else
		{
			wheel.Sine(0, 0, 0, 0);
			wheel.Rumble(0, 0, 0);
		}
	}

	void OutRunMotors::Update(const char* name, int value)
	{
		if (Is(name, { "Bank_Motor_Direction" }))
		{
			Log::Value("got value:", value);
			bankDirection = value;
		}

		if (Is(name, { "Vibration_motor" }))
		{
			Log::Value("got value:", value);
			vibration = value;
		}

		if (Is(name, { "Bank_Motor_Speed" }))
			bankSpeed = value;

		if (moving)
		{
			if (bankDirection == 0x01)
				wheel.Constant(Direction::FromLeft, (1 + bankSpeed) / 8.0);
			if (bankDirection == 0x02)
				wheel.Constant(Direction::FromRight, (1 + bankSpeed) / 8.0);
		}

		if (bankDirection == 1)
		{
			moving = true;
			stopping = false;
		}

		if (vibration == 1)
			moving = false;

		if (stopped)
			stopping = true;

		if (stopping)
		{
			wheel.StopConstant();
			moving = false;
			stopped = false;
			stopping = false;
		}

		if (!moving)
			Vibrate(wheel, vibration == 0x01);
	}

	void PowerDriftMotors::Update(const char* name, int value)
	{
		if (Is(name, { "bank_motor_position" }))
		{
			Log::Value("got value:", value);
			bankPosition = value;
		}

		if (Is(name, { "vibration_motor" }))
			vibration = value;

		if (Is(name, { "upright_wheel_motor" }))
		{
			Log::Value("got value:", value);
			wheelMotor = value;
		}

		if (moving)
		{
			if (bankPosition > 0x00 && bankPosition < 0x04)
				wheel.Constant(Direction::FromLeft, (4 - bankPosition) / 3.0);
			else if (bankPosition > 0x04 && bankPosition < 0x08)
				wheel.Constant(Direction::FromRight, (bankPosition - 4) / 3.0);

			if (vibration >= 0)
				wheel.Sine(60, 0, vibration / 7.0 / 3.0, 100);
		}

		if (bankPosition == 4)
		{
			moving = true;
			stopping = false;
		}

		if (wheelMotor == 1)
			moving = false;

		if (stopped)
			stopping = true;

		if (stopping)
		{
			wheel.StopConstant();
			moving = false;
			stopped = false;
			stopping = false;
		}

		if (!moving)
			Vibrate(wheel, wheelMotor == 0x01);
	}
}
