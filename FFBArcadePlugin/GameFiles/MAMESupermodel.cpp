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

#include <atomic>
#include <stdio.h>
#include <string.h>
#include "SDL.h"
#include "MAMESupermodel.h"
#include "EmulatorGames.h"
#include "../Core/Devices.h"
#include "../Core/ForceProfile.h"

extern SDL_Haptic* haptic2;
extern SDL_Haptic* haptic3;
extern int EnableForceSpringEffect;
extern int ForceSpringStrength;
extern int EnableDamper;
extern int DamperStrength;

// Rom name of the game, for the keys of the persistent max force (Config/PersistentValues.cpp)
char* romname;

namespace
{
	// Written by the thread of the outputs, read by the FFB thread. A Windows lock rather than
	// std::mutex: since VS 2022 17.10, std::mutex crashes when the process loads an older msvcp140.dll.
	char gameName[256];
	char lastOutput[256]; // name of the last output that changed
	SRWLOCK lastOutputLock = SRWLOCK_INIT;
	volatile int lastValue = 0;
	volatile bool stopped = false;

	// the hardware of the game, once it is known
	std::atomic<ffb::OutputBoard*> board{ nullptr };

	char romBuffer[256];
}

void MAMESupermodel::OnGameName(const char* name)
{
	sprintf_s(gameName, "%.255s", name);
}

void MAMESupermodel::OnOutput(const char* name, int value)
{
	AcquireSRWLockExclusive(&lastOutputLock);
	strncpy_s(lastOutput, name, _TRUNCATE);
	ReleaseSRWLockExclusive(&lastOutputLock);
	lastValue = value;

	if (ffb::OutputBoard* current = board.load())
		current->OnOutput(name, value);
}

void MAMESupermodel::OnStop()
{
	stopped = true;
	if (ffb::OutputBoard* current = board.load())
		current->OnStop();
}

void MAMESupermodel::Update()
{
	if (!devicesOpen)
	{
		OpenHapticDevice2();
		SDL_HapticSetGain(haptic2, 100);

		OpenHapticDevice3();
		SDL_HapticSetGain(haptic3, 100);

		// Flycast is ended with TerminateProcess by the ExitProcess hook of DllMain.cpp
		devicesOpen = true;
	}

	if (EnableForceSpringEffect)
		wheel.KeptSpring(ForceSpringStrength / 100.0);

	sprintf_s(romBuffer, "%.255s", gameName);
	romname = romBuffer;

	char name[256];
	AcquireSRWLockShared(&lastOutputLock);
	strcpy_s(name, lastOutput);
	ReleaseSRWLockShared(&lastOutputLock);
	int value = lastValue;

	ffb::OutputBoard* current = board.load();
	if (current == nullptr)
	{
		if (const EmulatorGame* game = FindEmulatorGame(romBuffer))
		{
			ffb::ForceProfile::Apply(game->suffix, game->fields);
			current = game->createBoard();
			if (stopped)
				current->OnStop();
			board.store(current);
		}

		ffb::Log::Printf("RomName = %s", romBuffer);
		ffb::Log::Printf("RunningFFB = %s", current ? current->Name() : "(null)");
	}

	if (current != nullptr)
	{
		ApplyDamper();
		current->Update(name, value);
	}
}
