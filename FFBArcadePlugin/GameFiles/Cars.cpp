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

#include "Cars.h"
#include "../Core/Hooks.h"

using namespace ffb;

Cars* Cars::instance = nullptr;
void(__thiscall* Cars::testMenuForceOriginal)(int object, float force, char unknown) = nullptr;
void(__thiscall* Cars::gameForceOriginal)(int object, float force, char unknown) = nullptr;
void(__thiscall* Cars::startRumbleOriginal)(int object, int rumble) = nullptr;

Cars::Cars()
{
	instance = this;
}

void Cars::PlayForce(float force)
{
	if (!ffbEnabled)
		return;

	if (force >= 0)
		wheel.Constant(Direction::FromRight, force);
	else
		wheel.Constant(Direction::FromLeft, force * -1);
}

void __fastcall Cars::TestMenuForceHook(int object, float force, char unknown)
{
	instance->PlayForce(force);
	testMenuForceOriginal(object, force, unknown);
}

void __fastcall Cars::GameForceHook(int object, float force, char unknown)
{
	instance->PlayForce(force);
	gameForceOriginal(object, force, unknown);
}

void __fastcall Cars::StartRumbleHook(int object, int rumble)
{
	Cars& game = *instance;
	if (game.ffbEnabled)
	{
		if (rumble == 0) // in the air
			game.wheel.Constant(Direction::FromRight, 0);

		if (rumble > 0) // after a collision
		{
			game.wheel.Rumble(0, 1, rumble);
			game.wheel.Sine(rumble, 0, 50, 150);
		}
	}
	startRumbleOriginal(object, rumble);
}

void Cars::Update()
{
	if (!hooked)
	{
		Hooks::Initialize();
		Hooks::Create(TestMenuForce, TestMenuForceHook, &testMenuForceOriginal);
		Hooks::Create(GameForce, GameForceHook, &gameForceOriginal);
		Hooks::Create(StartRumble, StartRumbleHook, &startRumbleOriginal);
		Hooks::Enable();
		hooked = true;
	}

	int status = Memory::Read(FFBEnabled);
	if (status == 1 && !ffbEnabled)
		ffbEnabled = true;
	else if (status == 0 && ffbEnabled)
		ffbEnabled = false;
}
