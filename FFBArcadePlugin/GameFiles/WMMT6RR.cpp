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

#include "WMMT6RR.h"

using namespace ffb;

void WMMT6RR::Update()
{
	float spring = Memory::Read(InModule<float>(addresses.ffbValues + 0x4));
	float friction = Memory::Read(InModule<float>(addresses.ffbValues + 0x8));
	float collisions = Memory::Read(InModule<float>(addresses.ffbValues + 0xC));
	float tiresSlip = Memory::Read(InModule<float>(addresses.ffbValues));
	int speed = Memory::Read(InModule<int>(addresses.speed));
	int rpm = Memory::Read(InModule<int>(addresses.rpm));
	Log::Printf("spring: %f | friction: %f | collisions: %f | tires slip: %f | speed: %d | rpm: %d",
		spring, friction, collisions, tiresSlip, speed, rpm);

	if (reverseCollision)
	{
		collisions = -collisions;
	}
	if (reverseTiresSlip)
	{
		tiresSlip = -tiresSlip;
	}

	ApplyDamper();

	bool gameFfbStarted = rpm || speed;
	if (gameFfbStarted)
	{
		Log::Write("game's FFB started");
	}

	PlaySpringAndFriction(gameFfbStarted, spring, friction);

	if (gameFfbStarted)
	{
		PlayCollisions(collisions);
		PlayTiresSlip(tiresSlip, collisions, speed);
	}
}
