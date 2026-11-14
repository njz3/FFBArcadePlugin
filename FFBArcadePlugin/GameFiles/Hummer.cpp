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
#include "Hummer.h"
#include "../Core/Hooks.h"

using namespace ffb;

HummerGame* HummerGame::instance = nullptr;
void(__cdecl* HummerGame::setTrqCurveHalfPointOriginal)(void* device, int param_1, float param_2, float param_3) = nullptr;
void(__cdecl* HummerGame::setVibrateOriginal)(void* device, int param_1, float strength, float duration) = nullptr;
void(__cdecl* HummerGame::setViscosityOriginal)(void* device, int param_1, float viscosity, float param_3) = nullptr;
void(__cdecl* HummerGame::initCenterOriginal)(void* device, int param_1, unsigned char strength) = nullptr;
int(__cdecl* HummerGame::waitOriginal)(int param_1) = nullptr;

HummerGame::HummerGame(const Version& version)
	: version(version)
{
	instance = this;
}

void __cdecl HummerGame::SetTrqCurveHalfPointHook(void* device, int param_1, float param_2, float param_3)
{
	return setTrqCurveHalfPointOriginal(device, param_1, param_2, param_3);
}

void __cdecl HummerGame::SetVibrateHook(void* device, int param_1, float strength, float duration)
{
	HummerGame& game = *instance;
	if (game.version.traceVibrations)
	{
		std::string out = "Vibrate: " + std::to_string(strength) + " " + std::to_string(duration);
		OutputDebugStringA(out.c_str());
	}
	UINT32 length_ms = 100;
	game.wheel.Sine((UINT16)(duration * 100), 1, strength * 60.0, length_ms);
	game.wheel.Rumble(strength * 60.0, 0.0, (UINT32)(duration * 100));
	return setVibrateOriginal(device, param_1, strength, duration);
}

void __cdecl HummerGame::SetViscosityHook(void* device, int param_1, float viscosity, float param_3)
{
	HummerGame& game = *instance;
	double percentForce = (double)viscosity * game.version.viscosityScale;
	game.wheel.Friction(percentForce);
	return setViscosityOriginal(device, param_1, viscosity, param_3);
}

void __cdecl HummerGame::InitCenterHook(void* device, int param_1, unsigned char strength)
{
	HummerGame& game = *instance;
	double percentForce = (double)strength * game.version.springScale;
	game.wheel.KeptSpring(percentForce);
	return initCenterOriginal(device, param_1, strength);
}

int __cdecl HummerGame::WaitHook(int param_1)
{
	return 0;
}

void HummerGame::Update()
{
	if (!hooked)
	{
		if (version.hookedMessage)
			OutputDebugStringA(version.hookedMessage);
		hooked = true;
		Hooks::Initialize();
		// the wait for the wheel to be ready for an update
		Hooks::Create(version.wait, WaitHook, &waitOriginal);
		// the effects of the game
		Hooks::Create(version.setTrqCurveHalfPoint, SetTrqCurveHalfPointHook, &setTrqCurveHalfPointOriginal);
		Hooks::Create(version.setVibrate, SetVibrateHook, &setVibrateOriginal);
		Hooks::Create(version.setViscosity, SetViscosityHook, &setViscosityOriginal);
		Hooks::Create(version.initCenter, InitCenterHook, &initCenterOriginal);
		Hooks::Enable();
	}
}
