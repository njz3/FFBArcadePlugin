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

#include "Showdown.h"
#include "../Core/Hooks.h"

using namespace ffb;

Showdown* Showdown::instance = nullptr;
int(__stdcall* Showdown::out32Original)(DWORD device, DWORD data) = nullptr;

Showdown::Showdown()
{
	instance = this;
}

int __stdcall Showdown::Out32Hook(DWORD device, DWORD data)
{
	if (device == 0x378)
	{
		Showdown& game = *instance;
		if (data > 15)
		{
			double percentForce = (31 - data) / 15.0;
			game.wheel.ConstantWithRumble(Direction::FromLeft, percentForce);
		}
		else if (data > 0)
		{
			double percentForce = (16 - data) / 15.0;
			game.wheel.ConstantWithRumble(Direction::FromRight, percentForce);
		}
	}

	return 0;
}

void Showdown::Update()
{
	if (!hooked)
	{
		hooked = true;

		Hooks::Initialize();
		Hooks::CreateApi(L"inpout32.dll", "Out32", Out32Hook, &out32Original);
		Hooks::Enable();
	}
}
