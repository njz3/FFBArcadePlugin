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

#include "WMMT.h"
#include "SDL.h"

using namespace ffb;

extern SDL_Event e;

void WMMT::StartInputThread()
{
	SDL_CreateThread(InputThread, "InputThread", this);
}

int WMMT::InputThread(void* ptr)
{
	WMMT* game = (WMMT*)ptr;
	if (1 != game->enableForceFinish && 1 != game->enableForceTimeUp)
	{
		return 0;
	}

	Log::Write("starting input thread");
	while (SDL_WaitEvent(&e) != 0)
	{
		if (e.type == SDL_JOYBUTTONDOWN)
		{
			Log::Write("button pressed");
			if (1 == game->enableForceFinish && e.jbutton.button == game->forceFinishButton)
			{
				INT_PTR raceState = Memory::Read(InModule<INT_PTR>(game->addresses.raceState));
				Memory::Write<UINT8>(raceState + 0x28, game->FinishCheckpoint(raceState));
			}
			else if (1 == game->enableForceTimeUp && e.jbutton.button == game->forceTimeUpButton)
			{
				int tempDisableRaceTimer = game->disableRaceTimer;
				game->disableRaceTimer = 0;
				Memory::Write<float>(InModule<float>(game->addresses.raceTimer), 0);
				if (1 == tempDisableRaceTimer)
				{
					Sleep(10000);
					game->disableRaceTimer = tempDisableRaceTimer;
				}
			}
		}
	}
	Log::Write("input thread stopped");
	return 0;
}


// A light spring and friction keep the wheel centered until the game sends its FFB
void WMMT::PlaySpringAndFriction(bool gameFfbStarted, float spring, float friction)
{
	double percentForce;
	if (!gameFfbStarted)
	{
		Log::Write("fake spring+friction until game's FFB starts");
		percentForce = 0.3 * springStrength / 100.0;
		wheel.Spring(percentForce);
		percentForce = 0.5 * frictionStrength / 100.0;
		wheel.Friction(percentForce);
	}
	else
	{
		percentForce = (1.0 * spring) * springStrength / 100.0;
		wheel.Spring(percentForce);
		percentForce = (1.0 * friction) * frictionStrength / 100.0;
		wheel.Friction(percentForce);
	}
}

void WMMT::PlayCollisions(float collisions)
{
	double percentForce;
	if (0 < collisions)
	{
		if (0.209 <= collisions && 0.311 >= collisions)
		{
			Log::Write("joint/stripe on the right");
			percentForce = (1.0 * collisions) * jointsAndStripesStrength / 100.0;
			wheel.Sine(80, 80, percentForce, EffectLength);
			wheel.Rumble(0, percentForce, EffectLength);
		}
		else
		{
			Log::Write("collision on the right");
			percentForce = (1.0 * collisions) * collisionsStrength / 100.0;
			wheel.Constant(Direction::FromRight, percentForce);
			wheel.Rumble(0, percentForce, EffectLength);
		}
	}
	else if (0 > collisions)
	{
		if (-0.209 >= collisions && -0.311 <= collisions)
		{
			Log::Write("joint/stripe on the left");
			percentForce = (1.0 * collisions) * jointsAndStripesStrength / 100.0;
			wheel.Sine(80, 80, percentForce, EffectLength);
			wheel.Rumble(0, -1.0 * percentForce, EffectLength);
		}
		else
		{
			Log::Write("collision on the left");
			percentForce = (-1.0 * collisions) * collisionsStrength / 100.0;
			wheel.Constant(Direction::FromLeft, percentForce);
			wheel.Rumble(0, percentForce, EffectLength);
		}
	}
	else
	{
		Log::Write("resetting collision");
		wheel.Constant(Direction::FromLeft, 0);
	}
}

void WMMT::PlayTiresSlip(float tiresSlip, float collisions, int speed)
{
	bool left = 0 < tiresSlip;
	if (!left && !(0 > tiresSlip))
		return;

	Log::Write(left ? "tires slip left" : "tires slip right");
	double slip = left ? (1.0 * tiresSlip) : (-1.0 * tiresSlip);
	bool highSpeedVibrations = (294 <= speed) && slip < (limitBetweenHighSpeedVibrationsAndTiresSlip / 1000.0);
	double percentForce = (-1.0 * tiresSlip) * (highSpeedVibrations ? highSpeedVibrationsStrength : tiresSlipStrength) / 100.0;
	wheel.Sine(100, 100, percentForce, EffectLength);

	if (!highSpeedVibrations && ((0 == jointsAndStripesStrength && 0 == collisionsStrength) || (0.001 > collisions && -0.001 < collisions)))
	{
		wheel.Rumble(0, left ? -1.0 * percentForce : percentForce, EffectLength);
	}
}

void WMMT::PlayWheelSpin(UINT8 gear, int speed)
{
	UINT8 power = ReadPower();
	int rpm = Memory::Read(InModule<int>(addresses.rpm));
	int diff = 0x0A <= power ? 0 : 20;
	double percentForce;

	if (
		1 == gear && 10 < speed && (
			((30 - diff) > speed && 3500 < rpm)
			|| ((55 - diff) > speed && 5500 < rpm)
			|| ((75 - diff) > speed && 7000 < rpm)
			|| ((100 - diff) > speed && 7800 < rpm)
		)
	)
	{
		percentForce = (((100.0 - speed) / 100.0) * ((rpm * 100.0 / 8500.0) / 100.0)) * wheelSpinStrength / 100.0;
	}
	else if (
		2 == gear && 10 < speed && (
			((110 - (2 * diff)) > speed && 5000 < rpm)
			|| ((130 - (2 * diff)) > speed && 6000 < rpm)
			|| ((145 - (2 * diff)) > speed && 6500 < rpm)
			|| ((160 - (2 * diff)) > speed && 7000 < rpm)
		)
	)
	{
		percentForce = (((160.0 - speed) / 150.0) * ((rpm * 100.0 / 8500.0) / 100.0)) * wheelSpinStrength / 100.0;
	}
	else
		return;

	wheel.Sine(120, 120, percentForce, EffectLength);
	wheel.Rumble(0, percentForce, EffectLength);

	Log::Printf("tires spin: gear: %d | speed: %d | rpm: %d | force: %f", gear, speed, rpm, percentForce);
}

// The jolt of a gear change, GearChangeDelay ms after it. Update runs every 16 ms, so the delay
// is waited for here rather than by a new thread for each gear change, which leaked.
void WMMT::PlayGearChange(UINT8 gear, int speed)
{
	if (gearChangeDue != 0 && GetTickCount64() >= gearChangeDue)
	{
		gearChangeDue = 0;
		Log::Write("gear change");
		double percentForce = gearChangeStrength / 100.0;
		wheel.Sine(gearChangeLength, gearChangeLength, percentForce, EffectLength);
		wheel.Rumble(0, percentForce, EffectLength);
	}

	float time = Memory::Read(PointerChain<float, 1>{ InModule<INT_PTR>(addresses.raceState), { 0x18 } });

	if (oldGear != gear && 0 < gear && 0 < time)
	{
		Log::Printf("oldgear: %d | gear: %d | time: %f | speed: %d", oldGear, gear, time, speed);
	}

	if (oldGear != gear && 0 < gear && 0.5 < time && 0.1 <= speed)
	{
		gearChangeDue = GetTickCount64() + max(gearChangeDelay, 0); // a later change moves it
	}
	oldGear = gear;
}

void WMMT::Update()
{
	if (!started)
	{
		started = true;
		StartThreads();
	}

	float spring = Memory::Read(InModule<float>(addresses.ffbValues + 0x4));
	float friction = Memory::Read(InModule<float>(addresses.ffbValues + 0x8));
	float collisions = Memory::Read(InModule<float>(addresses.ffbValues + 0xC));
	float tiresSlip = Memory::Read(InModule<float>(addresses.ffbValues));
	int speed = Memory::Read(InModule<int>(addresses.speed));
	Log::Printf("spring: %f | friction: %f | collisions: %f | tires slip: %f | speed: %d",
		spring, friction, collisions, tiresSlip, speed);

	ApplyDamper();

	if (!gameFfbStarted && !(0.001 > spring))
	{
		Log::Write("game's FFB started");
		gameFfbStarted = true;
	}

	PlaySpringAndFriction(gameFfbStarted, spring, friction);
	PlayCollisions(collisions);
	PlayTiresSlip(tiresSlip, collisions, speed);

	UINT8 gear = Memory::Read(PointerChain<UINT8, 1>{ InModule<INT_PTR>(addresses.carState), { addresses.gearOffset } });

	if (0 < wheelSpinStrength)
	{
		PlayWheelSpin(gear, speed);
	}

	if (0 < gearChangeStrength)
	{
		PlayGearChange(gear, speed);
	}
}
