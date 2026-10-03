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
#include "../Common Files/Game.h"

// Addresses (relative to the game executable) of a Wangan Midnight Maximum Tune version
struct WMMTAddresses
{
	INT_PTR ffbValues;  // FFB computed by the game, 4 floats: tires slip, spring, friction, collisions
	INT_PTR speed;      // int
	INT_PTR rpm;        // int
	INT_PTR carState;   // pointer to the player car state
	INT_PTR gearOffset; // gear (UINT8) in the player car state
	INT_PTR raceState;  // pointer to the race state, with the race time (float) at +0x18
	INT_PTR raceTimer;  // remaining race time (float), for the versions supporting ForceTimeUp
};

// Wangan Midnight Maximum Tune games compute spring, friction, collisions and tires slip forces,
// the plugin adds wheel spin and gear change effects.
class WMMT : public Game {
public:
	void FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers);

protected:
	explicit WMMT(const WMMTAddresses& addresses) : addresses(addresses) {}

	// Called by the first FFBLoop
	virtual void Init() {}
	// Power tune of the car: with a low tune, tires spin at lower speeds
	virtual UINT8 ReadPower(Helpers* helpers) { return 16; }
	// Checkpoint written in the race state to finish the race (ForceFinishButton)
	virtual UINT8 FinishCheckpoint(Helpers* helpers, INT_PTR raceState) { return 8; }

	// Handles ForceFinishButton and ForceTimeUpButton
	void StartInputThread();

	void PlaySpringAndFriction(bool gameFfbStarted, float spring, float friction);
	void PlayCollisions(float collisions);
	void PlayTiresSlip(float tiresSlip, float collisions, int speed);

	const WMMTAddresses addresses;
	EffectTriggers* myTriggers = nullptr;
	EffectConstants* myConstants = nullptr;
	Helpers* myHelpers = nullptr;
	static int DisableRaceTimer;

private:
	void PlayWheelSpin(UINT8 gear, int speed);
	void PlayGearChange(UINT8 gear, int speed);
	static int InputThread(void* ptr);
	static int GearChangeThread(void* ptr);

	bool init = false;
	bool gameFfbStarted = false;
	UINT8 oldgear = 0;
};
