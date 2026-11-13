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
#include "WMMT.h"
#include "SDL.h"

extern SDL_Event e;
static int SpringStrength = GetSettingInt(TEXT("SpringStrength"), 100);
static int FrictionStrength = GetSettingInt(TEXT("FrictionStrength"), 0);
static int JointsAndStripesStrength = GetSettingInt(TEXT("JointsAndStripesStrength"), 100);
static int CollisionsStrength = GetSettingInt(TEXT("CollisionsStrength"), 100);
static int TiresSlipStrength = GetSettingInt(TEXT("TiresSlipStrength"), 100);
static int HighSpeedVibrationsStrength = GetSettingInt(TEXT("HighSpeedVibrationsStrength"), 100);
static int LimitBetweenHighSpeedVibrationsAndTiresSlip = GetSettingInt(TEXT("LimitBetweenHighSpeedVibrationsAndTiresSlip"), 75);
static int GearChangeStrength = GetSettingInt(TEXT("GearChangeStrength"), 20);
static int GearChangeDelay = GetSettingInt(TEXT("GearChangeDelay"), 250);
static int GearChangeLength = GetSettingInt(TEXT("GearChangeLength"), 200);
static int WheelSpinStrength = GetSettingInt(TEXT("WheelSpinStrength"), 100);
static int EnableForceFinish = GetSettingInt(TEXT("EnableForceFinish"), 0);
static int EnableForceTimeUp = GetSettingInt(TEXT("EnableForceTimeUp"), 0);
static int ForceFinishButton = GetSettingInt(TEXT("ForceFinishButton"), 99);
static int ForceTimeUpButton = GetSettingInt(TEXT("ForceTimeUpButton"), 99);
int WMMT::DisableRaceTimer = GetSettingInt(TEXT("DisableRaceTimer"), 0);

static const UINT32 length_ms = 150;

void WMMT::StartInputThread()
{
	SDL_CreateThread(InputThread, "InputThread", this);
}

int WMMT::InputThread(void* ptr)
{
	if (1 != EnableForceFinish && 1 != EnableForceTimeUp)
	{
		return 0;
	}

	WMMT* game = (WMMT*)ptr;
	Helpers* helpers = game->myHelpers;
	helpers->log("starting input thread");
	while (SDL_WaitEvent(&e) != 0)
	{
		if (e.type == SDL_JOYBUTTONDOWN)
		{
			helpers->log("button pressed");
			if (1 == EnableForceFinish && e.jbutton.button == ForceFinishButton)
			{
				INT_PTR raceState = helpers->ReadIntPtr(game->addresses.raceState, true);
				helpers->WriteByte(raceState + 0x28, game->FinishCheckpoint(helpers, raceState), false);
			}
			else if (1 == EnableForceTimeUp && e.jbutton.button == ForceTimeUpButton)
			{
				int tempDisableRaceTimer = DisableRaceTimer;
				DisableRaceTimer = 0;
				helpers->WriteFloat32(game->addresses.raceTimer, 0, true);
				if (1 == tempDisableRaceTimer)
				{
					Sleep(10000);
					DisableRaceTimer = tempDisableRaceTimer;
				}
			}
		}
	}
	helpers->log("input thread stopped");
	return 0;
}


// A light spring and friction keep the wheel centered until the game sends its FFB
void WMMT::PlaySpringAndFriction(bool gameFfbStarted, float spring, float friction)
{
	double percentForce;
	if (!gameFfbStarted)
	{
		myHelpers->log("fake spring+friction until game's FFB starts");
		percentForce = 0.3 * SpringStrength / 100.0;
		myTriggers->Spring(percentForce);
		percentForce = 0.5 * FrictionStrength / 100.0;
		myTriggers->Friction(percentForce);
	}
	else
	{
		percentForce = (1.0 * spring) * SpringStrength / 100.0;
		myTriggers->Spring(percentForce);
		percentForce = (1.0 * friction) * FrictionStrength / 100.0;
		myTriggers->Friction(percentForce);
	}
}

void WMMT::PlayCollisions(float collisions)
{
	double percentForce;
	if (0 < collisions)
	{
		if (0.209 <= collisions && 0.311 >= collisions)
		{
			myHelpers->log("joint/stripe on the right");
			percentForce = (1.0 * collisions) * JointsAndStripesStrength / 100.0;
			myTriggers->Sine(80, 80, percentForce, length_ms);
			myTriggers->Rumble(0, percentForce, length_ms);
		}
		else
		{
			myHelpers->log("collision on the right");
			percentForce = (1.0 * collisions) * CollisionsStrength / 100.0;
			myTriggers->Constant(myConstants->DIRECTION_FROM_RIGHT, percentForce);
			myTriggers->Rumble(0, percentForce, length_ms);
		}
	}
	else if (0 > collisions)
	{
		if (-0.209 >= collisions && -0.311 <= collisions)
		{
			myHelpers->log("joint/stripe on the left");
			percentForce = (1.0 * collisions) * JointsAndStripesStrength / 100.0;
			myTriggers->Sine(80, 80, percentForce, length_ms);
			myTriggers->Rumble(0, -1.0 * percentForce, length_ms);
		}
		else
		{
			myHelpers->log("collision on the left");
			percentForce = (-1.0 * collisions) * CollisionsStrength / 100.0;
			myTriggers->Constant(myConstants->DIRECTION_FROM_LEFT, percentForce);
			myTriggers->Rumble(0, percentForce, length_ms);
		}
	}
	else
	{
		myHelpers->log("resetting collision");
		myTriggers->Constant(myConstants->DIRECTION_FROM_LEFT, 0);
	}
}

void WMMT::PlayTiresSlip(float tiresSlip, float collisions, int speed)
{
	bool left = 0 < tiresSlip;
	if (!left && !(0 > tiresSlip))
		return;

	myHelpers->log(left ? "tires slip left" : "tires slip right");
	double slip = left ? (1.0 * tiresSlip) : (-1.0 * tiresSlip);
	bool highSpeedVibrations = (294 <= speed) && slip < (LimitBetweenHighSpeedVibrationsAndTiresSlip / 1000.0);
	double percentForce = (-1.0 * tiresSlip) * (highSpeedVibrations ? HighSpeedVibrationsStrength : TiresSlipStrength) / 100.0;
	myTriggers->Sine(100, 100, percentForce, length_ms);

	if (!highSpeedVibrations && ((0 == JointsAndStripesStrength && 0 == CollisionsStrength) || (0.001 > collisions && -0.001 < collisions)))
	{
		myTriggers->Rumble(0, left ? -1.0 * percentForce : percentForce, length_ms);
	}
}

void WMMT::PlayWheelSpin(UINT8 gear, int speed)
{
	UINT8 power = ReadPower(myHelpers);
	int rpm = myHelpers->ReadInt32(addresses.rpm, true);
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
		percentForce = (((100.0 - speed) / 100.0) * ((rpm * 100.0 / 8500.0) / 100.0)) * WheelSpinStrength / 100.0;
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
		percentForce = (((160.0 - speed) / 150.0) * ((rpm * 100.0 / 8500.0) / 100.0)) * WheelSpinStrength / 100.0;
	}
	else
		return;

	myTriggers->Sine(120, 120, percentForce, length_ms);
	myTriggers->Rumble(0, percentForce, length_ms);

	std::string msg = "tires spin: gear: " + std::to_string(gear) + " | speed: " + std::to_string(speed)
		+ " | rpm: " + std::to_string(rpm) + " | force: " + std::to_string(percentForce);
	myHelpers->log(msg.c_str());
}

// The jolt of a gear change, GearChangeDelay ms after it. FFBLoop runs every 16 ms, so the delay
// is waited for here rather than by a new thread for each gear change, which leaked.
void WMMT::PlayGearChange(UINT8 gear, int speed)
{
	if (gearChangeDue != 0 && GetTickCount64() >= gearChangeDue)
	{
		gearChangeDue = 0;
		myHelpers->log("gear change");
		double percentForce = GearChangeStrength / 100.0;
		myTriggers->Sine(GearChangeLength, GearChangeLength, percentForce, length_ms);
		myTriggers->Rumble(0, percentForce, length_ms);
	}

	INT_PTR raceState = myHelpers->ReadIntPtr(addresses.raceState, true);
	float time = myHelpers->ReadFloat32(raceState + 0x18, false);

	if (oldgear != gear && 0 < gear && 0 < time)
	{
		std::string msg = "oldgear: " + std::to_string(oldgear) + " | gear: " + std::to_string(gear)
			+ " | time: " + std::to_string(time) + " | speed: " + std::to_string(speed);
		myHelpers->log(msg.c_str());
	}

	if (oldgear != gear && 0 < gear && 0.5 < time && 0.1 <= speed)
	{
		gearChangeDue = GetTickCount64() + max(GearChangeDelay, 0); // a later change moves it
	}
	oldgear = gear;
}

void WMMT::FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers)
{
	if (!init)
	{
		init = true;
		myTriggers = triggers;
		myConstants = constants;
		myHelpers = helpers;
		Init();
	}

	float spring = helpers->ReadFloat32(addresses.ffbValues + 0x4, true);
	float friction = helpers->ReadFloat32(addresses.ffbValues + 0x8, true);
	float collisions = helpers->ReadFloat32(addresses.ffbValues + 0xC, true);
	float tiresSlip = helpers->ReadFloat32(addresses.ffbValues, true);
	int speed = helpers->ReadInt32(addresses.speed, true);
	std::string msg = "spring: " + std::to_string(spring) + " | friction: " + std::to_string(friction)
		+ " | collisions: " + std::to_string(collisions) + " | tires slip: " + std::to_string(tiresSlip)
		+ " | speed: " + std::to_string(speed);
	helpers->log(msg.c_str());

	ApplyDamper(triggers);

	if (!gameFfbStarted && !(0.001 > spring))
	{
		helpers->log("game's FFB started");
		gameFfbStarted = true;
	}

	PlaySpringAndFriction(gameFfbStarted, spring, friction);
	PlayCollisions(collisions);
	PlayTiresSlip(tiresSlip, collisions, speed);

	INT_PTR carState = helpers->ReadIntPtr(addresses.carState, true);
	UINT8 gear = helpers->ReadByte(carState + addresses.gearOffset, false);

	if (0 < WheelSpinStrength)
	{
		PlayWheelSpin(gear, speed);
	}

	if (0 < GearChangeStrength)
	{
		PlayGearChange(gear, speed);
	}
}
