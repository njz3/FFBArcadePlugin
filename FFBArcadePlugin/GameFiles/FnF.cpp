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

#include "FnF.h"
#include "../Core/Hooks.h"

using namespace ffb;

FastAndFuriousGame* FastAndFuriousGame::instance = nullptr;
int(__cdecl* FastAndFuriousGame::sendToLptOriginal)(uint16_t port, uint8_t value) = nullptr;
void(__cdecl* FastAndFuriousGame::ignoredOriginal)() = nullptr;

FastAndFuriousGame::FastAndFuriousGame(const Addresses& addresses)
	: addresses(addresses)
{
	instance = this;
}

double FastAndFuriousGame::Strength(LONG value)
{
	return value < 0x80 ? value / 127.0 : (value - 0x80) / 127.0;
}

int __cdecl FastAndFuriousGame::SendToLptHook(uint16_t port, uint8_t value)
{
	FastAndFuriousGame& game = *instance;
	if (port == 0x378 && InterlockedExchange(&game.lastValue, value) != value)
	{
		double strength = Strength(value);
		// both last until the next change; a force of 0 stops them
		game.wheel.Rumble(0, strength, RumbleLength);
		game.rumbleRenewed = GetTickCount64();
		game.wheel.KeptConstant(value < 0x80 ? Direction::FromRight : Direction::FromLeft, strength);
	}
	return 0;
}

void __cdecl FastAndFuriousGame::IgnoredHook()
{
}

void FastAndFuriousGame::Update()
{
	if (!hooked)
	{
		Memory::Write<INT_PTR>(addresses.ioEnabled, 1);

		Hooks::Initialize();
		Hooks::Create(addresses.sendToLpt, SendToLptHook, &sendToLptOriginal);
		for (INT_PTR io : addresses.otherLptIo)
			Hooks::Create(io, IgnoredHook, &ignoredOriginal);
		Hooks::Enable();

		hooked = true;
	}

	ApplyDamper();

	LONG value = lastValue;
	if (value >= 0 && Strength(value) > 0 && GetTickCount64() - rumbleRenewed >= RumbleRenewal)
	{
		wheel.Rumble(0, Strength(value), RumbleLength);
		rumbleRenewed = GetTickCount64();
	}
}
