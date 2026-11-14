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

#pragma once
#include "../Core/Game.h"

// Wangan Midnight Maximum Tune 5, 5DX, 5DX+, 6, 6R and 6RR (Bandai Namco, TeknoParrot): the games compute
// spring, friction, collisions and tires slip forces, read in their memory; the plugin adds wheel spin
// and gear change effects, made from the speed, the rpm and the gear. The versions share this code with
// addresses of their own. With EnableForceFinish/EnableForceTimeUp, a thread watching the joystick buttons
// ends the race (ForceFinishButton) or its time (ForceTimeUpButton), for the versions starting it.
class WMMT : public ffb::Game
{
public:
	void Update() override;

protected:
	// Addresses of a version, relative to the game executable
	struct Addresses
	{
		INT_PTR ffbValues;  // FFB computed by the game, 4 floats: tires slip, spring, friction, collisions
		INT_PTR speed;      // speed of the car (int)
		INT_PTR rpm;        // rpm of the engine (int), up to 8500
		INT_PTR carState;   // pointer to the player car state
		INT_PTR gearOffset; // gear (UINT8) in the player car state, 1 the first one
		INT_PTR raceState;  // pointer to the race state, with the race time (float) at +0x18
		INT_PTR raceTimer;  // remaining race time (float), for the versions supporting ForceTimeUp (else 0)
	};

	explicit WMMT(const Addresses& addresses) : addresses(addresses) {}

	// Called by the first Update: starts the threads of the version
	virtual void StartThreads() {}
	// Power tune of the car: with a low tune (below 0x0A), the tires spin at lower speeds
	virtual UINT8 ReadPower() { return 16; }
	// Checkpoint written in the race state to finish the race (ForceFinishButton)
	virtual UINT8 FinishCheckpoint(INT_PTR raceState) { return 8; }

	// Handles ForceFinishButton and ForceTimeUpButton
	void StartInputThread();

	// spring: 0 to 1, the game's FFB has started once it is 0.001 or more
	void PlaySpringAndFriction(bool gameFfbStarted, float spring, float friction);
	// collisions: > 0 on the right, < 0 on the left; 0.209 to 0.311 (either side) for road joints and stripes
	void PlayCollisions(float collisions);
	// tiresSlip: > 0 to the left, < 0 to the right
	void PlayTiresSlip(float tiresSlip, float collisions, int speed);

	// A value of the game executable, at an offset of the addresses
	template <typename T>
	static ffb::GameValue<T> InModule(INT_PTR offset)
	{
		return { ffb::Base::Module, offset };
	}

	const Addresses addresses;
	// 1: the remaining race time is kept at 999.99 (WMMT5); set to 0 by ForceTimeUpButton for 10 s
	int disableRaceTimer = ffb::Settings::Int(L"DisableRaceTimer", 0);

private:
	static const UINT32 EffectLength = 150; // ms

	void PlayWheelSpin(UINT8 gear, int speed);
	void PlayGearChange(UINT8 gear, int speed);
	static int InputThread(void* game);

	int springStrength = ffb::Settings::Int(L"SpringStrength", 100);
	int frictionStrength = ffb::Settings::Int(L"FrictionStrength", 0);
	int jointsAndStripesStrength = ffb::Settings::Int(L"JointsAndStripesStrength", 100);
	int collisionsStrength = ffb::Settings::Int(L"CollisionsStrength", 100);
	int tiresSlipStrength = ffb::Settings::Int(L"TiresSlipStrength", 100);
	int highSpeedVibrationsStrength = ffb::Settings::Int(L"HighSpeedVibrationsStrength", 100);
	int limitBetweenHighSpeedVibrationsAndTiresSlip = ffb::Settings::Int(L"LimitBetweenHighSpeedVibrationsAndTiresSlip", 75);
	int gearChangeStrength = ffb::Settings::Int(L"GearChangeStrength", 20);
	int gearChangeDelay = ffb::Settings::Int(L"GearChangeDelay", 250);
	int gearChangeLength = ffb::Settings::Int(L"GearChangeLength", 200);
	int wheelSpinStrength = ffb::Settings::Int(L"WheelSpinStrength", 100);
	int enableForceFinish = ffb::Settings::Int(L"EnableForceFinish", 0);
	int enableForceTimeUp = ffb::Settings::Int(L"EnableForceTimeUp", 0);
	int forceFinishButton = ffb::Settings::Int(L"ForceFinishButton", 99);
	int forceTimeUpButton = ffb::Settings::Int(L"ForceTimeUpButton", 99);

	bool started = false;
	bool gameFfbStarted = false;
	UINT8 oldGear = 0;
	// When the effect of the last gear change is due (GetTickCount64), 0 if none is waiting
	ULONGLONG gearChangeDue = 0;
};
