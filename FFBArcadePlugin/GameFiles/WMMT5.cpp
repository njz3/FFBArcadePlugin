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

#include "WMMT5.h"
#include "SDL.h"

using namespace ffb;

UINT8 WMMT5::ReadPower()
{
	return Memory::Read<UINT8>(Memory::Read(CarTune) + Power);
}

void WMMT5::StartThreads()
{
	StartInputThread();
	SDL_CreateThread(SpamThread, "SpamThread", this);
}

int WMMT5::SpamThread(void* ptr)
{
	WMMT5* game = (WMMT5*)ptr;
	if (1 != game->forceFullTune && 1 != game->disableRaceTimer)
	{
		return 0;
	}

	Sleep(5000); // To avoid crashes
	Log::Write("starting spam thread");
	while (1)
	{
		if (1 == game->forceFullTune)
		{
			INT_PTR carTune = Memory::Read(CarTune);
			UINT8 car = Memory::Read<UINT8>(carTune + Car);
			Log::Value("car:", car);

			if (0x00 < car)
			{
				UINT8 power = Memory::Read<UINT8>(carTune + Power);
				UINT8 handling = Memory::Read<UINT8>(carTune + Handling);
				Log::Printf("power: %d | handling: %d", power, handling);

				if (0x20 != (power + handling))
				{
					Log::Write("forcing full tune");
					Memory::Write<UINT8>(carTune + Power, 0x10);
					Memory::Write<UINT8>(carTune + Handling, 0x10);
				}
			}
		}

		if (1 == game->disableRaceTimer)
		{
			Memory::Write<float>(InModule<float>(game->addresses.raceTimer), 999.99f);
		}

		Sleep(500); // We don't need to spam too much
	}
}
