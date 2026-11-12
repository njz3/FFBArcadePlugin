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
#include "WMMT6RR.h"

static int ReverseCollision = GetSettingInt(TEXT("ReverseCollision"), 0);
static int ReverseTiresSlip = GetSettingInt(TEXT("ReverseTiresSlip"), 0);

// TODO: wheel spin and gear change effects. Gear and race state pointers may be found from
// Wg6Enma_Release_IDL0.dll+29B3E0 (gear at +0x3AC or +0x3A8) and Wg6Enma_Release_IDL0.dll+29B3F8 (time at +0x18).
static const WMMTAddresses wmmt6RRAddresses = {
	0x1F25260, // ffbValues
	0x1F2612C, // speed
	0x1F2629C, // rpm
	0,         // carState
	0,         // gearOffset
	0,         // raceState
	0,         // raceTimer
};

WMMT6RR::WMMT6RR() : WMMT(wmmt6RRAddresses)
{
}

void WMMT6RR::FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers)
{
	myTriggers = triggers;
	myConstants = constants;
	myHelpers = helpers;

	float spring = helpers->ReadFloat32(addresses.ffbValues + 0x4, true);
	float friction = helpers->ReadFloat32(addresses.ffbValues + 0x8, true);
	float collisions = helpers->ReadFloat32(addresses.ffbValues + 0xC, true);
	float tiresSlip = helpers->ReadFloat32(addresses.ffbValues, true);
	int speed = helpers->ReadInt32(addresses.speed, true);
	int rpm = helpers->ReadInt32(addresses.rpm, true);
	std::string msg = "spring: " + std::to_string(spring) + " | friction: " + std::to_string(friction)
		+ " | collisions: " + std::to_string(collisions) + " | tires slip: " + std::to_string(tiresSlip)
		+ " | speed: " + std::to_string(speed) + " | rpm: " + std::to_string(rpm);
	helpers->log(msg.c_str());

	if (ReverseCollision)
	{
		collisions = -collisions;
	}
	if (ReverseTiresSlip)
	{
		tiresSlip = -tiresSlip;
	}

	ApplyDamper(triggers);

	bool gameFfbStarted = rpm || speed;
	if (gameFfbStarted)
	{
		helpers->log("game's FFB started");
	}

	PlaySpringAndFriction(gameFfbStarted, spring, friction);

	if (gameFfbStarted)
	{
		PlayCollisions(collisions);
		PlayTiresSlip(tiresSlip, collisions, speed);
	}
}
