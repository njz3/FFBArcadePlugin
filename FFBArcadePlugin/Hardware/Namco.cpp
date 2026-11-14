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

#include "Namco.h"
#include "../Core/Log.h"
#include "../Core/Memory.h"
#include "../Core/Settings.h"

namespace ffb
{
	// Ace Driver wheel_motor value to signed force (-62..62)
	static int AceDriverForce(int raw)
	{
		static const int map[256] = {
			/* 0x00 */  0,   62,  0,  -62,  0,   31,  0,  -31,
			/* 0x08 */  0,   47,  0,  -47,  0,   15,  0,  -15,
			/* 0x10 */  0,   55,  0,  -55,  0,   23,  0,  -23,
			/* 0x18 */  0,   39,  0,  -39,  0,    7,  0,   -7,
			/* 0x20 */  0,   59,  0,  -59,  0,   27,  0,  -27,
			/* 0x28 */  0,   43,  0,  -43,  0,   11,  0,  -11,
			/* 0x30 */  0,   51,  0,  -51,  0,   19,  0,  -19,
			/* 0x38 */  0,   35,  0,  -35,  0,    3,  0,   -3,

			/* 0x40 */  0,   61,  0,  -61,  0,   29,  0,  -29,
			/* 0x48 */  0,   45,  0,  -45,  0,   13,  0,  -13,
			/* 0x50 */  0,   53,  0,  -53,  0,   21,  0,  -21,
			/* 0x58 */  0,   37,  0,  -37,  0,    5,  0,   -5,
			/* 0x60 */  0,   57,  0,  -57,  0,   25,  0,  -25,
			/* 0x68 */  0,   41,  0,  -41,  0,    9,  0,   -9,
			/* 0x70 */  0,   49,  0,  -49,  0,   17,  0,  -17,
			/* 0x78 */  0,   33,  0,  -33,  0,    1,  0,   -1,

			/* 0x80 */  0,   62,  0,  -62,  0,   30,  0,  -30,
			/* 0x88 */  0,   46,  0,  -46,  0,   14,  0,  -14,
			/* 0x90 */  0,   54,  0,  -54,  0,   22,  0,  -22,
			/* 0x98 */  0,   38,  0,  -38,  0,    6,  0,   -6,
			/* 0xA0 */  0,   58,  0,  -58,  0,   26,  0,  -26,
			/* 0xA8 */  0,   42,  0,  -42,  0,   10,  0,  -10,
			/* 0xB0 */  0,   50,  0,  -50,  0,   18,  0,  -18,
			/* 0xB8 */  0,   34,  0,  -34,  0,    2,  0,   -2,

			/* 0xC0 */  0,   60,  0,  -60,  0,   28,  0,  -28,
			/* 0xC8 */  0,   44,  0,  -44,  0,   12,  0,  -12,
			/* 0xD0 */  0,   52,  0,  -52,  0,   20,  0,  -20,
			/* 0xD8 */  0,   36,  0,  -36,  0,    4,  0,   -4,
			/* 0xE0 */  0,   56,  0,  -56,  0,   24,  0,  -24,
			/* 0xE8 */  0,   40,  0,  -40,  0,    8,  0,   -8,
			/* 0xF0 */  0,   48,  0,  -48,  0,   16,  0,  -16,
			/* 0xF8 */  0,   32,  0,  -32,  0,    0,  0,    0  // 0xFF = true neutral
		};
		return map[raw & 0xFF];
	}

	// Rave Racer wheel_motor value to force (1..123), -1 when the value is not a force
	static int RaveRacerForce(int raw)
	{
		static const int map[256] = {
			/* 0x00 */ -1, -1, 62, -1, 30, -1, 94, -1,
			/* 0x08 */ 46, -1, 78, -1, 14, -1, 110, -1,
			/* 0x10 */ 54, -1, 70, -1, 22, -1, 102, -1,
			/* 0x18 */ 38, -1, 86, -1, 6,  -1, 118, -1,
			/* 0x20 */ 59, -1, 65, -1, 27, -1, 97, -1,
			/* 0x28 */ 43, -1, 81, -1, 11, -1, 113, -1,
			/* 0x30 */ 50, -1, 74, -1, 18, -1, 106, -1,
			/* 0x38 */ 34, -1, 90, -1, 2,  -1, 122, -1,
			/* 0x40 */ 60, -1, 64, -1, 28, -1, 96, -1,
			/* 0x48 */ 44, -1, 80, -1, 12, -1, 112, -1,
			/* 0x50 */ 52, -1, 72, -1, 20, -1, 104, -1,
			/* 0x58 */ 36, -1, 88, -1, 4,  -1, 120, -1,
			/* 0x60 */ 56, -1, 68, -1, 24, -1, 100, -1,
			/* 0x68 */ 40, -1, 84, -1, 8,  -1, 116, -1,
			/* 0x70 */ 48, -1, 76, -1, 16, -1, 108, -1,
			/* 0x78 */ 32, -1, 92, -1, 1,  -1, 123, -1,
			/* 0x80 */ 61, -1, 63, -1, 29, -1, 95, -1,
			/* 0x88 */ 45, -1, 79, -1, 13, -1, 111, -1,
			/* 0x90 */ 53, -1, 71, -1, 21, -1, 103, -1,
			/* 0x98 */ 37, -1, 87, -1, 5,  -1, 119, -1,
			/* 0xA0 */ 57, -1, 67, -1, 25, -1, 99,  -1,
			/* 0xA8 */ 41, -1, 83, -1, 9,  -1, 115, -1,
			/* 0xB0 */ 49, -1, 75, -1, 17, -1, 107, -1,
			/* 0xB8 */ 33, -1, 91, -1, -1, -1, -1, -1,
			/* 0xC0 */ 58, -1, 66, -1, 26, -1, 98,  -1,
			/* 0xC8 */ 42, -1, 82, -1, 10, -1, 114, -1,
			/* 0xD0 */ 51, -1, 73, -1, 19, -1, 105, -1,
			/* 0xD8 */ 35, -1, 89, -1, 3,  -1, 121, -1,
			/* 0xE0 */ 55, -1, 69, -1, 23, -1, 101, -1,
			/* 0xE8 */ 39, -1, 85, -1, 7,  -1, 117, -1,
			/* 0xF0 */ 47, -1, 77, -1, 15, -1, 109, -1,
			/* 0xF8 */ 31, -1, 93, -1, -1, -1, -1, -1
		};
		return map[raw & 0xFF];
	}

	void AceDriverWheelMotor::OnOutput(const char* name, int value)
	{
		if (Is(name, { "mcuout3" }) && value == 0)
			StopConstant();

		if (Is(name, { "wheel_motor" }))
		{
			int force = AceDriverForce((UINT8)value);

			if (force > 0)
			{
				double strength = force / 62.0;
				wheel.Rumble(strength, 0.0, 100);
				SendConstant(Direction::FromRight, strength);
			}
			else if (force < 0)
			{
				double strength = (-force) / 62.0;
				wheel.Rumble(0.0, strength, 100);
				SendConstant(Direction::FromLeft, strength);
			}
			else
			{
				StopConstant();
			}
		}
	}

	void RaveRacerWheelMotor::OnOutput(const char* name, int value)
	{
		if (Is(name, { "mcuout3" }) && value == 0)
			StopConstant();

		if (Is(name, { "wheel_motor" }))
		{
			UINT8 force = RaveRacerForce((UINT8)value); // 255 when not a force

			if (force > 0x3D && force < 0x7C)
			{
				double strength = (124 - force) / 61.0;
				wheel.Rumble(strength, 0.0, 100);
				SendConstant(Reversible(Direction::FromLeft), strength);
			}
			else if (force > 0x00 && force < 0x3E)
			{
				double strength = force / 61.0;
				wheel.Rumble(0.0, strength, 100);
				SendConstant(Reversible(Direction::FromRight), strength);
			}
		}
	}

	DirtDashWheelMotor::DirtDashWheelMotor()
		: divide(Settings::Int(L"FFBDivideDirtDash", 0) * 1.0)
	{
	}

	DWORD WINAPI DirtDashWheelMotor::ScanThread(LPVOID parameter)
	{
		DirtDashWheelMotor* board = (DirtDashWheelMotor*)parameter;
		board->pattern = Memory::FindPattern("\xC8\x00\xC8\x00\xC8\x00\xC8\x00\xC8\x00\xC8\x00\xC8\x00\xC8\x00\x14\x00\x14\x00\x14\x00\x14\x00\x14\x00\x14\x00\x14\x00\x14",
			"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", board->pattern);
		return 0;
	}

	void DirtDashWheelMotor::Update(const char* name, int value)
	{
		if (Is(name, { "mcuout1" }))
		{
			motorOn = value;
			Log::Value("DirtDashFFB: mcuout1 switched to", motorOn);
		}

		if (!found)
		{
			if (!scanAllowed)
			{
				if (Is(name, { "cpuled6" }) && value == 1)
				{
					Sleep(2000);
					scanAllowed = true;
				}
			}
			else
			{
				if (!scanStarted)
				{
					CreateThread(NULL, 0, ScanThread, this, 0, NULL);
					scanStarted = true;
				}

				if (Memory::Read<UINT8>((INT_PTR)pattern - 0x3E3) == 0x80)
				{
					forceAddress = (INT_PTR)pattern - 0x2C6;
					found = true;
				}
			}
			return;
		}

		DWORD force = Memory::Read<DWORD>(forceAddress);
		Log::Value("got value:", (int)force);

		if (motorOn == 0) // wheel motor switched off
		{
			wheel.Rumble(0, 0, 0);
			StopConstant();
		}
		else if (force >= 0x00 && force < 0x77A)
		{
			double strength = force / divide;
			if (strength > 1.0)
				strength = 1.0;
			wheel.Rumble(0, strength, 100);
			SendConstant(Direction::FromRight, strength);
		}
		else if (force > 0xF886 && force < 0x10000)
		{
			double strength = (65536 - force) / divide;
			if (strength > 1.0)
				strength = 1.0;
			wheel.Rumble(strength, 0, 100);
			SendConstant(Direction::FromLeft, strength);
		}
		else
		{
			wheel.Rumble(0, 0, 0);
			StopConstant();
		}
	}
}
