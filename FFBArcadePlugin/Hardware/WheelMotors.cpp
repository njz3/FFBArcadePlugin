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

#include <stdint.h>
#include "WheelMotors.h"
#include "../Core/Log.h"
#include "../Core/Memory.h"
#include "../Core/Settings.h"

namespace ffb
{
	void SignedWheelMotor::OnOutput(const char* name, int value)
	{
		if (!Is(name, { "wheel", "wheel_motor", "m_wheel_motor" }))
			return;

		Log::Value("got value:", value);

		if (value >= 0x80 && value < 0x100)
		{
			double strength = (256 - value) / 128.0;
			if (strength > 1.0)
				strength = 1.0;
			wheel.Rumble(strength, 0, 100);
			SendConstant(Direction::FromLeft, strength);
		}
		else if (value > 0x00 && value < 0x80)
		{
			double strength = value / 127.0;
			if (strength > 1.0)
				strength = 1.0;
			wheel.Rumble(0, strength, 100);
			SendConstant(Direction::FromRight, strength);
		}
		else
		{
			StopConstant();
		}
	}

	void ReversedSignedWheelMotor::OnOutput(const char* name, int value)
	{
		if (!Is(name, { "wheel" }))
			return;

		Log::Value("got value:", value);

		if (value > 0x80 && value < 0x100)
		{
			double strength = (256 - value) / 126.0;
			if (strength > 1.0)
				strength = 1.0;
			wheel.Rumble(0, strength, 100);
			SendConstant(Direction::FromRight, strength);
		}
		else if (value > 0x00 && value < 0x80)
		{
			double strength = value / 126.0;
			if (strength > 1.0)
				strength = 1.0;
			wheel.Rumble(strength, 0, 100);
			SendConstant(Direction::FromLeft, strength);
		}
		else
		{
			StopConstant();
		}
	}

	CruisnExoticaWheelMotor::CruisnExoticaWheelMotor()
		: gain(Settings::Int(L"FFBGainCrusnExotica", 400))
	{
	}

	void CruisnExoticaWheelMotor::OnOutput(const char* name, int value)
	{
		if (!Is(name, { "wheel", "wheel_motor" }))
			return;

		// a signed 8-bit value, 0x80 neutral as in the other Cruis'n games
		int force = static_cast<signed char>(static_cast<unsigned char>(value));
		if (force == -128)
			force = 0;

		int clampedGain = gain;
		if (clampedGain < 0) clampedGain = 0;
		if (clampedGain > 800) clampedGain = 800;

		int scaled = (force * clampedGain) / 100;
		if (scaled > 127) scaled = 127;
		if (scaled < -127) scaled = -127;

		// with the right shifter/wheel DIP switches, Exotica needs the sides swapped
		scaled = -scaled;

		Log::Printf("WheelActiveExotica raw: %i scaled: %i", value, scaled);

		if (scaled > 0)
		{
			double strength = scaled / 126.0;
			if (strength > 1.0)
				strength = 1.0;
			wheel.Rumble(strength, 0, 100);
			SendConstant(Direction::FromLeft, strength);
		}
		else if (scaled < 0)
		{
			double strength = (-scaled) / 126.0;
			if (strength > 1.0)
				strength = 1.0;
			wheel.Rumble(0, strength, 100);
			SendConstant(Direction::FromRight, strength);
		}
	}

	void KonamiWheelMotor::OnOutput(const char* name, int value)
	{
		if (!Is(name, { "wheel", "wheel_motor", "pcboutput0", "output0", "wheel0" }))
			return;

		const uint8_t command = static_cast<uint8_t>(value);
		Log::Value("got value:", (int)command);

		// Any value with a force level of 0 (0, but also 0x80 and 0x90) means no force
		const int level = command & 0x0F;
		if (level == 0)
		{
			StopConstant();
			return;
		}

		const double strength = level / 15.0;
		if ((command & 0x10) == 0)
		{
			wheel.Rumble(0, strength, 100);
			SendConstant(Direction::FromRight, strength);
		}
		else
		{
			wheel.Rumble(strength, 0, 100);
			SendConstant(Direction::FromLeft, strength);
		}
	}

	void Hng64WheelMotor::OnOutput(const char* name, int value)
	{
		if (!Is(name, { "wheel_motor" }))
			return;

		Log::Value("got value:", value);

		if (value >= 193 && value <= 255)
		{
			double strength = (value - 193) / 62.0;
			if (strength > 1.0)
				strength = 1.0;
			wheel.Rumble(0, strength, 100);
			SendConstant(Direction::FromLeft, strength);
		}
		else if (value >= 129 && value <= 191)
		{
			double strength = (value - 129) / 62.0;
			if (strength > 1.0)
				strength = 1.0;
			wheel.Rumble(strength, 0, 100);
			SendConstant(Direction::FromRight, strength);
		}
		else
		{
			StopConstant();
		}
	}

	void DriversEdgeWheelMotor::OnOutput(const char* name, int value)
	{
		if (!Is(name, { "wheel_motor" }))
			return;

		Log::Value("Driver's Edge wheel_motor:", value);

		int force = value;
		if (force > 31)
			force = 31;
		else if (force < -31)
			force = -31;

		if (force > 0)
			SendConstant(Reversible(Direction::FromRight), force / 31.0);
		else if (force < 0)
			SendConstant(Reversible(Direction::FromLeft), (-force) / 31.0);
		else
			StopConstant();
	}

	// awffb value to force: odd values 1 (0x01) to 16 (0x1F), even values 17 (0x02) to 31 (0x1E)
	static int MaximumSpeedForce(int raw)
	{
		if (raw < 0x01 || raw > 0x1F)
			return 0;
		return (raw & 1) ? (raw + 1) / 2 : 16 + raw / 2;
	}

	void MaximumSpeedWheelMotor::OnOutput(const char* name, int value)
	{
		if (!Is(name, { "awffb" }))
			return;

		Log::Value("got value:", value);

		UINT8 force = MaximumSpeedForce(value);
		if (force > 0x00 && force < 0x11)
			wheel.ConstantWithRumble(Direction::FromLeft, force / 16.0);
		else if (force > 0x10 && force < 0x20)
			wheel.ConstantWithRumble(Direction::FromRight, (force - 16) / 16.0);
	}

	void HardDrivinWheelMotor::OnOutput(const char* name, int value)
	{
		if (framing && Is(name, { "wheel" }))
		{
			previous[3] = previous[2];
			previous[2] = previous[1];
			previous[1] = previous[0];
			previous[0] = value;

			// 0xE0 and 0x00 alternating: start over
			bool stop = (value == 0xE0 && previous[1] == 0x00 && previous[2] == 0xE0 && previous[3] == 0x00) ||
				(value == 0x00 && previous[1] == 0xE0 && previous[2] == 0x00 && previous[3] == 0xE0);
			if (stop)
				frame = 0;
			if (frame > 4)
				frame = 0;
			// values[0] should always be < 200, values[1] > 200: otherwise the frame is invalid, start over
			if (frame > 2 && (values[0] > 200 || (values[1] < 200 && values[1] != 0)))
			{
				frame = 0;
				values[1] = 0;
				values[2] = 0;
				values[3] = 0;
			}
			if (!stop)
			{
				values[frame] = value;
				frame++;
			}
		}

		framing = true;

		if (frame == 4) // with any output
		{
			int force = (values[0] & 15) + ((values[3] & 7) << 5);

			if ((values[1] & 0xF0) == 0xF0)
				force |= 0x10;

			if ((values[2] & 0xF0) == 0xF0)
				force = -force;

			if (force > 100)
				force = 100;

			if (force < -100)
				force = -100;

			if (force >= 0)
			{
				double strength = force / 100.0;
				wheel.Rumble(strength, 0, 100);
				SendConstant(Direction::FromLeft, strength);
			}
			else
			{
				double strength = -force / 100.0;
				wheel.Rumble(0, strength, 100);
				SendConstant(Direction::FromRight, strength);
			}
		}
	}

	void SideBySideWheelMotor::OnOutput(const char* name, int value)
	{
		if (!Is(name, { "wheel_motor" }))
			return;

		Log::Value("got wheel_motor:", value);

		int v = value & 0x1F;

		// 0 (off) and 0x1E/0x1F (attract mode, idle): no force
		if (v == 0 || v == 0x1E || v == 0x1F)
		{
			StopConstant();
			return;
		}

		Direction direction = (v & 1) ? Direction::FromRight : Direction::FromLeft;

		int steps = (v + 1) / 2; // 1 to 15
		if (steps > 15)
			steps = 15;

		// the highest values are the weakest forces
		double strength = (double)(16 - steps) / 15.0;
		if (strength > 1.0)
			strength = 1.0;

		// no stale pull: both sides cleared before the new force
		StopConstant();
		SendConstant(direction, strength);
	}

	DWORD WINAPI SuperChaseWheelMotor::ScanThread(LPVOID parameter)
	{
		SuperChaseWheelMotor* board = (SuperChaseWheelMotor*)parameter;
		board->pattern = Memory::FindPattern("\x80\x38\x00\x50\x49\x5A\x05\x70\x11\x01\x4B\x54\x4F", "xxxxxxxxxxxxx", board->pattern);
		return 0;
	}

	void SuperChaseWheelMotor::Update(const char* lastName, int lastValue)
	{
		if (!found)
		{
			if (!waited)
			{
				Sleep(8000);
				waited = true;
			}

			if (!scanStarted)
			{
				CreateThread(NULL, 0, ScanThread, this, 0, NULL);
				scanStarted = true;
			}

			if (Memory::Read<UINT8>((INT_PTR)pattern + 0x12) == 0x5A)
			{
				forceAddress = (INT_PTR)pattern + 0x6C8;
				found = true;
			}
			return;
		}

		UINT8 force = Memory::Read<UINT8>(forceAddress);
		Log::Value("got value:", (int)force);

		if (force & 0x01)
			wheel.ConstantWithRumble(Direction::FromLeft, 1.0);

		if (force & 0x02)
			wheel.ConstantWithRumble(Direction::FromRight, 1.0);
	}
}
