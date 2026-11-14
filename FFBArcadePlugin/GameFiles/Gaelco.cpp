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

#include "Gaelco.h"
#include "../Core/Hooks.h"

using namespace ffb;

GaelcoGame* GaelcoGame::instance = nullptr;
int(__stdcall* GaelcoGame::floatSetMotorOriginal)(DWORD* motor, float force) = nullptr;

GaelcoGame::GaelcoGame()
{
	instance = this;
}

void GaelcoGame::PlayMotorForce(double force)
{
	GaelcoGame& game = *instance;
	if (force > 0)
	{
		double percentForce = force;

		if (percentForce > 1.0)
			percentForce = 1.0;

		game.wheel.ConstantWithRumble(Direction::FromLeft, percentForce);
	}
	else if (force < 0)
	{
		double percentForce = -force;

		if (percentForce > 1.0)
			percentForce = 1.0;

		game.wheel.ConstantWithRumble(Direction::FromRight, percentForce);
	}
}

int __stdcall GaelcoGame::FloatSetMotorHook(DWORD* motor, float force)
{
	PlayMotorForce(force);
	return floatSetMotorOriginal(motor, force);
}

void GaelcoGame::HookFloatSetMotor(INT_PTR address)
{
	Hooks::Create(address, FloatSetMotorHook, &floatSetMotorOriginal);
}

void GaelcoGame::Update()
{
	if (!hooked)
	{
		hooked = true;
		Hooks::Initialize();
		HookSetMotor();
		Hooks::Enable();
	}

	ApplyDamper();
}
