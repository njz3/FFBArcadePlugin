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

#include <stdio.h>
#include "OutRun2Fake.h"

using namespace ffb;

extern SDL_Event e;

int OutRun2Game::SpeedStrength(float speed)
{
	// from, to (km/h), strength (%)
	static const struct { double from, to; int strength; } steps[] = {
		{ 0.1, 80, 10 }, { 80.1, 130, 20 }, { 130.1, 180, 30 }, { 180.1, 220, 40 }, { 220.1, 270, 50 },
		{ 270.1, 320, 60 }, { 320.1, 380, 70 }, { 380.1, 430, 80 }, { 430.1, 500, 90 }, { 500.1, 1000, 100 },
	};
	for (const auto& step : steps)
	{
		if ((speed >= step.from) && (speed <= step.to))
			return step.strength;
	}
	return 0;
}

void OutRun2Game::StartThread()
{
	if (!threadStarted)
	{
		CreateThread(NULL, 0, RunningLoop, this, 0, NULL);
		threadStarted = true;
	}
}

DWORD WINAPI OutRun2Game::RunningLoop(LPVOID game)
{
	while (true)
	{
		((OutRun2Game*)game)->ThreadLoop();
		Sleep(16);
	}
}

void OutRun2Game::HandleEvent(const SDL_Event& event)
{
	UINT8 transmission = Memory::Read(Transmission);

	if (showButtonNumbersForSetup == 1 && event.type == SDL_JOYBUTTONDOWN)
	{
		char text[100];
		sprintf_s(text, "Button %d Pressed", event.jbutton.button);
		MessageBoxA(NULL, text, "", MB_OK);
	}

	if (event.type == SDL_JOYBUTTONDOWN && changeGearsViaPlugin == 1 && transmission == 1)
	{
		// the first gear of the button
		for (int gear = 0; gear < 6; gear++)
		{
			if (event.jbutton.button == gearButtons[gear])
			{
				Memory::Write<UINT8>(Gear, (UINT8)(gear + 1));
				break;
			}
		}
	}
}

void OutRun2Fake::Update()
{
	StartThread();

	int ground = Memory::Read(Ground);
	Log::Value("got value:", ground);

	// SDL_WaitEvent returns when there is an event: the loop does not end
	while (SDL_WaitEvent(&e) != 0)
		HandleEvent(e);
}

void OutRun2Fake::PlaySteering(UINT8 steering)
{
	if (steering < 0x7F)
		wheel.ConstantWithRumble(Direction::FromLeft, (127 - steering) / 127.0);
	else if (steering > 0x7F)
		wheel.ConstantWithRumble(Direction::FromRight, (steering - 127) / 128.0);
}

void OutRun2Fake::ThreadLoop()
{
	int ground = Memory::Read(Ground);
	Memory::Read(Wall);
	int sideHit = Memory::Read(SideHit);
	int crash = Memory::Read(Crash);
	int hitSide = Memory::Read(HitSide);
	UINT8 steering = Memory::Read(Steering);
	Memory::Read(UnusedValue);
	UINT8 inRace = Memory::Read(InRace);
	UINT8 inMenu = Memory::Read(InMenu);
	Memory::Read(Gear);
	float speed = Memory::Read(Speed);

	speedStrength = SpeedStrength(speed);

	ApplyDamper();

	if (inRace == 1)
		PlaySteering(steering);
	if (inMenu == 1)
		PlaySteering(steering);

	if (hitSide == 2)
	{
		if (lastSideHit != sideHit)
			wheel.ConstantWithRumble(Direction::FromRight, speedStrength / 100.0);
	}
	else if (hitSide == 1)
	{
		if (lastSideHit != sideHit)
			wheel.ConstantWithRumble(Direction::FromLeft, speedStrength / 100.0);
	}

	if (lastCrash != crash)
	{
		double percentForce = speedStrength / 100.0;
		wheel.Rumble(percentForce, percentForce, 100);
		wheel.Sine(200, 200, percentForce, 100);
	}
	else if (ground == 8)
	{
		if (speedStrength > 0)
		{
			wheel.Rumble(0.1, 0.1, 100);
			wheel.Sine(70, 70, 0.1, 100);
		}
	}
	else if (ground == 4)
	{
		if (speedStrength > 0)
		{
			wheel.Rumble(0.2, 0.2, 100);
			wheel.Sine(50, 50, 0.2, 100);
		}
	}
	else if (ground == 16)
	{
		if (speedStrength > 0)
		{
			wheel.Rumble(0.2, 0.2, 50);
			wheel.Sine(100, 50, 0.2, 50);
		}
	}

	lastSideHit = sideHit;
	lastCrash = crash;
}
