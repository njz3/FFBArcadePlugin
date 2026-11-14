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

#include "Vibration.h"
#include "../Core/Log.h"
#include "../Core/Config.h"

static ffb::Config& config = ffb::Config::Get();

// settings of the game, applied by its profile

namespace ffb
{
	const char* OnOffMotors::Name() const
	{
		switch (kind)
		{
		case Kind::GunRecoil: return "LightGunActive";
		case Kind::WheelVibration: return "RacingActive1";
		case Kind::JalecoVibration: return "RacingActive2";
		default: return "AfterburnerActive";
		}
	}

	int OnOffMotors::Players() const
	{
		return kind == Kind::GunRecoil ? 3 : kind == Kind::WheelVibration ? 2 : 1;
	}

	bool OnOffMotors::IsOutputOf(int player, const char* name) const
	{
		switch (kind)
		{
		case Kind::GunRecoil:
			if (player == 0)
				return Is(name, { "Player1_Recoil_Piston", "Player1_Gun_Recoil", "Left_Gun_Recoil", "P1_Gun_Recoil", "mcuout1", "P1_gun_recoil" });
			if (player == 1)
				return Is(name, { "Player2_Recoil_Piston", "Player2_Gun_Recoil", "Right_Gun_Recoil", "P2_Gun_Recoil", "P2_gun_recoil" });
			return Is(name, { "Player3_Gun_Recoil", "P3_Gun_Recoil" });
		case Kind::WheelVibration:
			if (player == 0)
				return Is(name, { "MA_Steering_Wheel_motor", "mainpcb:MA_Steering_Wheel_motor", "upright_wheel_motor", "Vibration_motor", "Wheel_vibration", "Wheel_Vibration", "genout2" });
			return Is(name, { "MB_Steering_Wheel_motor", "mainpcb:MB_Steering_Wheel_motor" });
		case Kind::JalecoVibration:
			return Is(name, { "led2" }); // kept apart: led2 could be another output in other games
		default:
			return Is(name, { "lamp1" });
		}
	}

	Device& OnOffMotors::DeviceOf(int player)
	{
		return player == 0 ? wheel : player == 1 ? device2 : device3;
	}

	void OnOffMotors::Update(const char* name, int value)
	{
		const int players = Players();

		for (int player = 0; player < players; player++)
		{
			if (IsOutputOf(player, name))
			{
				Log::Printf("P%d value: %d", player + 1, value);
				state[player] = value;
			}
		}

		const bool sine = kind == Kind::WheelVibration || kind == Kind::JalecoVibration;
		for (int player = 0; player < players; player++)
		{
			Device& device = DeviceOf(player);
			if (state[player] == 0x01)
			{
				if (sine)
					device.Sine(config.sinePeriod, config.sineFadePeriod, config.sineStrength / 100.0, 100);
				device.Rumble(config.rumbleStrengthLeftMotor / 100.0, config.rumbleStrengthRightMotor / 100.0, 100);
			}
			else
			{
				if (sine)
					device.Sine(0, 0, 0, 0);
				device.Rumble(0, 0, 0);
			}
		}
	}
}
