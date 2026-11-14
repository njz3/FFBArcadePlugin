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

#include "CruisnBlast.h"
#include "../Core/Hooks.h"

using namespace ffb;

CruisnBlast* CruisnBlast::instance = nullptr;
int(__cdecl* CruisnBlast::wheelSetOriginal)(float force) = nullptr;

CruisnBlast::CruisnBlast()
{
	instance = this;
}

int __cdecl CruisnBlast::WheelSetHook(float raceForce)
{
	CruisnBlast& game = *instance;
	float menuForce = Memory::Read(MenuForce);

	if (Memory::Read(Screen) == 1)
	{
		if (!game.inRace)
		{
			// in a race: wheel effects on
			Memory::Write<UINT8>(WheelEffects, 1);
			game.inRace = true;
			game.force = 0;
		}
		else
		{
			game.force = raceForce;
		}
	}
	else
	{
		if (game.inRace)
		{
			// out of the race: wheel effects off
			Memory::Write<UINT8>(WheelEffects, 0);
			game.force = 0;
			game.inRace = false;
		}
		else
		{
			game.force = menuForce;
		}
	}

	// a force of 0 is played on both sides
	if (game.force >= 0)
	{
		game.wheel.Rumble(0, game.force, 100);
		game.wheel.Constant(Direction::FromRight, game.force);
	}
	if (game.force <= 0)
	{
		game.wheel.Rumble(0, game.force * -1, 100);
		game.wheel.Constant(Direction::FromLeft, game.force * -1);
	}
	return 0;
}

void CruisnBlast::Update()
{
	if (hooked)
		return;

	ApplyDamper();

	Memory::Write<UINT8>(CabinetType, 4);

	OutputDebugStringA("CruisnBlast FFB Loop");

	Hooks::Initialize();
	Hooks::Create(WheelSet, WheelSetHook, &wheelSetOriginal);
	// the wheel check of WHEEL_SET and its outputs, just in case
	Memory::WriteNop(WheelCheck, 2);
	for (INT_PTR output : WheelSetOutputs)
		Memory::WriteNop(output, 1);

	Hooks::Enable();
	hooked = true;
}
