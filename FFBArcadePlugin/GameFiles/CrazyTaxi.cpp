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

#include "CrazyTaxi.h"

using namespace ffb;

void CrazyTaxi::Update()
{
	UINT8 inGame = Memory::Read(InGame);
	INT_PTR timer = Memory::Read(Timer);
	UINT8 ground = Memory::Read(Ground);
	UINT8 originalRumble = Memory::Read(OriginalRumble);
	UINT8 crazyMoves1 = Memory::Read(CrazyMoves1);
	UINT8 crazyMoves2 = Memory::Read(CrazyMoves2);
	float speed = Memory::Read(Speed);
	float leftWall = Memory::Read(LeftWall);
	float rightWall = Memory::Read(RightWall);
	Log::Value("got value:", ground);

	ApplyDamper();

	if (!inGame && timer)
	{
		double percentForce = speed / 10.0;

		if (percentForce > 1.0)
			percentForce = 1.0;

		double percentForceGround = percentForce / 2.0;

		Memory::Write<UINT8>(NativeRumble, 0x00); // turns the rumble/FFB of the game off

		// an integer division, as it always was: 0 when SpringStrength is below 100
		wheel.KeptSpring(springStrength / 100);

		if (engineRumbleEffect)
		{
			if (speed < 0.001)
				wheel.Rumble(0, 0.012, 100);
			else
				wheel.Rumble(0, 0.012 + percentForceGround / 7.0, 100);
		}

		if (originalRumbleEffect)
		{
			if (originalRumble)
				wheel.Rumble(0, percentForce, 100);

			if (crazyMoves1 & 0x10)
				wheel.Rumble(0, 1.0, 100);

			if (crazyMoves2)
				wheel.Rumble(0, 1.0, 100);
		}

		// the rumble of a crazy move replaces the effects of the ground
		bool crazyMoveRumble = (originalRumbleEffect && (crazyMoves1 & 0x10)) || (originalRumbleEffect && crazyMoves2);
		if (!crazyMoveRumble)
		{
			switch (ground)
			{
			case 0x00: // road
				break;
			case 0x03: // grass
				if (enableGroundEffects && speed > 0.001)
				{
					wheel.Rumble(percentForceGround, percentForceGround, 100);
					wheel.Sine(grassSinePeriod, grassFadeSinePeriod, percentForceGround, 100);
				}
				break;
			case 0x05: // shoreline
				if (enableGroundEffects && speed > 0.001)
				{
					wheel.Rumble(percentForceGround, percentForceGround, 100);
					wheel.Sine(shorelineSinePeriod, shorelineFadeSinePeriod, percentForceGround, 100);
					wheel.Friction(shorelineFriction / 100.0);
				}
				break;
			case 0x09: // sand
				if (enableGroundEffects && speed > 0.001)
				{
					wheel.Rumble(percentForceGround, percentForceGround, 100);
					wheel.Sine(sandSinePeriod, sandFadeSinePeriod, percentForceGround, 100);
					wheel.Friction(sandFriction / 100.0);
				}
				break;
			case 0x0A: // footpath
				if (enableGroundEffects && speed > 0.001)
				{
					wheel.Rumble(percentForceGround, percentForceGround, 100);
					wheel.Sine(footpathSinePeriod, footpathFadeSinePeriod, percentForceGround, 100);
				}
				break;
			case 0x0E: // underwater
				if (enableGroundEffects && speed > 0.001)
				{
					wheel.Rumble(percentForceGround, percentForceGround, 100);
					wheel.Sine(underwaterSinePeriod, underwaterFadeSinePeriod, percentForceGround, 100);
					wheel.Friction(underwaterFriction / 100.0);
				}
				break;
			case 0x11: // stairs
			case 0x13:
				if (enableGroundEffects && speed > 0.001)
				{
					wheel.Rumble(0, percentForceGround, 100);
					wheel.Sine(stairsSinePeriod, stairsFadeSinePeriod, percentForceGround, 100);
				}
				break;
			case 0x12: // stairs
			case 0x14:
				if (enableGroundEffects && speed > 0.001)
				{
					wheel.Rumble(percentForceGround, 0, 100);
					wheel.Sine(stairsSinePeriod, stairsFadeSinePeriod, percentForceGround, 100);
				}
				break;
			case 0x15: // subway, indoors
				if (enableGroundEffects && speed > 0.001)
				{
					wheel.Rumble(percentForceGround, 0, 100);
					wheel.Sine(subwaySinePeriod, subwayFadeSinePeriod, percentForceGround, 100);
				}
				break;
			case 0xFF: // collision with a car or a wall
				if (rightWall > 0)
				{
					wheel.Rumble(0, percentForce, 100);

					if (!disableConstantEffects)
						wheel.Constant(Direction::FromRight, percentForce);
				}
				else if (leftWall > 0)
				{
					wheel.Rumble(percentForce, 0, 100);

					if (!disableConstantEffects)
						wheel.Constant(Direction::FromLeft, percentForce);
				}
				else if (ground != lastGround)
				{
					// a collision that has just started, on neither side
					collision = true;
				}
				break;
			}
		}

		if (collision)
		{
			wheel.Sine(150, 0, percentForce, 100);
			wheel.Rumble(percentForce, percentForce, 100);
			collision = false;
		}

		lastGround = ground;
	}
}
