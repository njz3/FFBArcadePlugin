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

#include <string>
#include "WMMT5.h"
#include "SDL.h"

static int ForceFullTune = GetSettingInt(TEXT("ForceFullTune"), 0);

static const WMMTAddresses wmmt5Addresses = {
	0x196F188, // ffbValues
	0x196FEBC, // speed
	0x1970038, // rpm
	0x199A450, // carState
	0x398,     // gearOffset
	0x199A468, // raceState
	0x199AE18, // raceTimer
};

WMMT5::WMMT5() : WMMT(wmmt5Addresses)
{
}

// Tune of the player car: car at +0x2C, power at +0x98, handling at +0x9C
static INT_PTR ReadCarTune(Helpers* helpers)
{
	INT_PTR ptr1 = helpers->ReadIntPtr(0x1948F10, true);
	return helpers->ReadIntPtr(ptr1 + 0x180 + 0xa8 + 0x18, false);
}

UINT8 WMMT5::ReadPower(Helpers* helpers)
{
	return helpers->ReadByte(ReadCarTune(helpers) + 0x98, false);
}

void WMMT5::Init()
{
	StartInputThread();
	SDL_CreateThread(SpamThread, "SpamThread", this);
}

int WMMT5::SpamThread(void* ptr)
{
	if (1 != ForceFullTune && 1 != DisableRaceTimer)
	{
		return 0;
	}

	WMMT5* game = (WMMT5*)ptr;
	Helpers* helpers = game->myHelpers;
	Sleep(5000); // To avoid crashes
	helpers->log("starting spam thread");
	while (1)
	{
		if (1 == ForceFullTune)
		{
			INT_PTR carTune = ReadCarTune(helpers);
			UINT8 car = helpers->ReadByte(carTune + 0x2C, false);
			std::string msg = "car: " + std::to_string(car);
			helpers->log(msg.c_str());

			if (0x00 < car)
			{
				UINT8 power = helpers->ReadByte(carTune + 0x98, false);
				UINT8 handling = helpers->ReadByte(carTune + 0x9C, false);
				msg = "power: " + std::to_string(power) + " | handling: " + std::to_string(handling);
				helpers->log(msg.c_str());

				if (0x20 != (power + handling))
				{
					helpers->log("forcing full tune");
					helpers->WriteByte(carTune + 0x98, 0x10, false);
					helpers->WriteByte(carTune + 0x9C, 0x10, false);
				}
			}
		}

		if (1 == DisableRaceTimer)
		{
			helpers->WriteFloat32(game->addresses.raceTimer, 999.99f, true);
		}

		Sleep(500); // We don't need to spam too much
	}
}
