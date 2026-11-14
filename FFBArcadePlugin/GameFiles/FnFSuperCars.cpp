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

#include "FnFSuperCars.h"
#include "../Core/Hooks.h"

using namespace ffb;

FnFSuperCars* FnFSuperCars::instance = nullptr;
void(__cdecl* FnFSuperCars::setForceOriginal)(float force) = nullptr;
void(__cdecl* FnFSuperCars::ignoredOriginal)() = nullptr;

FnFSuperCars::FnFSuperCars()
{
	instance = this;
}

void __cdecl FnFSuperCars::SetForceHook(float force)
{
	FnFSuperCars& game = *instance;
	if (force == game.lastForce)
		return;
	game.lastForce = force;

	double strength = force >= 0 ? force : -force;
	// both last until the next change; a force of 0 stops them
	game.wheel.Rumble(0, strength, RumbleLength);
	game.rumbleRenewed = GetTickCount64();
	game.wheel.KeptConstant(force >= 0 ? Direction::FromRight : Direction::FromLeft, strength);
}

void __cdecl FnFSuperCars::IgnoredHook()
{
}

void FnFSuperCars::Update()
{
	if (!hooked)
	{
		OutputDebugStringA("FnFSuperCars::FFBLoop");

		Hooks::Initialize();
		Hooks::Create(SetForce, SetForceHook, &setForceOriginal);
		for (INT_PTR io : OtherLptIo)
			Hooks::Create(io, IgnoredHook, &ignoredOriginal);
		Hooks::Enable();

		hooked = true;
	}

	ApplyDamper();

	float force = lastForce;
	if (force != NoForceYet && force != 0.0f && GetTickCount64() - rumbleRenewed >= RumbleRenewal)
	{
		wheel.Rumble(0, force >= 0 ? force : -force, RumbleLength);
		rumbleRenewed = GetTickCount64();
	}
}
