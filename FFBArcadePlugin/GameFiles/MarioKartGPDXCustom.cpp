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

#include "MarioKartGPDXCustom.h"

using namespace ffb;

void MarioKartGPDX100Custom::Update()
{
	Log::Write("in MKDX Ffbloop");

	int shake = Memory::Read(Shake);
	int kart = Memory::Read(Kart);
	int terrain = Memory::Read<int>(kart + Terrain);
	Memory::Read(Acceleration);
	int onGround = Memory::Read<int>(kart + OnGround);
	int pointer = Memory::Read(Race);
	for (int offset : RaceChain)
		pointer = Memory::Read<int>(pointer + offset);
	int race = Memory::Read<int>(pointer + InRace);
	int moving = Memory::Read<int>(kart + Moving);
	Log::Value("got value:", shake);

	ApplyDamper();

	bool driving = race == 1 && onGround == 1 && moving == 1065353216;

	// the length of the dirt rumble is the one of the shake when both play
	UINT32 length = 100;
	// Large Shake when hitting walls, other karts or getting hit by items
	if (shake == 4194308 && race == 1)
	{
		length = 500;
		wheel.Rumble(3.5, 3.5, length);
		wheel.Sine(200, 200, 1.0, length);
	}

	// small friction when driving on dirt while moving
	if (terrain == 3 && driving)
	{
		wheel.Rumble(0.3, 0, length);
		wheel.Friction(0.3);
	}
	// Small constant when hitting bumps
	if (terrain == 10 && driving)
	{
		length = 50;
		wheel.Rumble(3.0, 3.0, length);
		wheel.Constant(Direction::FromRight, 0.2);
	}
	// Wheel rumbles while driving on grass
	if (terrain == 4 && driving)
	{
		length = 50;
		wheel.Rumble(0, 0.2, length);
		wheel.Sine(50, 50, 0.2, length);
	}
	// wheel hard to turn while driving through water
	if (terrain == 7 && driving)
	{
		wheel.Friction(0.65);
	}
	// Wheel rumbles lightly when driving over tiles
	if (terrain == 12 && driving)
	{
		length = 150;
		wheel.Rumble(0.2, 0, length);
		wheel.Sine(70, 70, 0.1, length);
	}
	// Wheel rumbles lightly when driving over sand
	if (terrain == 14 && driving)
	{
		length = 50;
		wheel.Rumble(0.2, 0, length);
		wheel.Sine(70, 70, 0.1, length);
	}
	// Wheel rumbles lightly when driving over rough part of track
	if (terrain == 11 && driving)
	{
		length = 250;
		wheel.Rumble(0, 0.2, length);
		wheel.Sine(40, 50, 0.1, length);
	}
	// Wheel rumbles moderately when driving over wooden bridges
	if (terrain == 8 && driving)
	{
		length = 10;
		wheel.Rumble(0.4, 0.4, length);
		wheel.Sine(120, 120, 0.4, length);
	}
}
