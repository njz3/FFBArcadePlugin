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

#pragma once
#include "../Core/Game.h"

// Road Fighters 3D (Konami, e-Amusement): with InputDeviceWheelEnable, the plugin writes the inputs of
// the wheel and of a second device (gears, lever, buttons) to the memory of jgt.dll, removes the
// checks of its I/O board and sets the cabinet ID (CabinetID); a thread reads the force in jgt.dll
// every 16 ms. Only2D, HackToSkipMenuError and HackToCloseLibmovieErrorAuto are hacks of the game.
class RoadFighters3D : public ffb::Game
{
public:
	void Update() override;

private:
	int ThreadLoop();
	static DWORD WINAPI RunningLoop(LPVOID game);

	bool avoiderror = true;
	bool testbuttonA {};
	bool servicebuttonA {};
	bool threedeebuttonA {};
	bool viewbuttonA {};
	bool leverupA {};
	bool leverdownA {};
	bool leverleftA {};
	bool leverrightA {};
	bool init = false;
	int CabinetID = ffb::Settings::Int(L"CabinetID", 0);
	int Only2D = ffb::Settings::Int(L"Only2D", 0);
	int HackToSkipMenuError = ffb::Settings::Int(L"HackToSkipMenuError", 0);
	int HackToCloseLibmovieErrorAuto = ffb::Settings::Int(L"HackToCloseLibmovieErrorAuto", 0);
	int InputDeviceWheelEnable = ffb::Settings::Int(L"InputDeviceWheelEnable", 0);
	int InputDeviceWheelSteeringAxis = ffb::Settings::Int(L"InputDeviceWheelSteeringAxis", 0);
	int InputDeviceWheelAcclAxis = ffb::Settings::Int(L"InputDeviceWheelAcclAxis", 0);
	int InputDeviceWheelBrakeAxis = ffb::Settings::Int(L"InputDeviceWheelBrakeAxis", 0);
	int InputDeviceWheelReverseAxis = ffb::Settings::Int(L"InputDeviceWheelReverseAxis", 0);
	int InputDeviceCombinedPedals = ffb::Settings::Int(L"InputDeviceCombinedPedals", 0);
	int SteeringDeadzone = ffb::Settings::Int(L"SteeringDeadzone", 0);
	int PedalDeadzone = ffb::Settings::Int(L"PedalDeadzone", 0);
	int SequentialGears = ffb::Settings::Int(L"SequentialGears", 0);
	int ShowButtonNumbersForSetup = ffb::Settings::Int(L"ShowButtonNumbersForSetup", 0);
	int ShowAxisForSetup = ffb::Settings::Int(L"ShowAxisForSetup", 0);
	int ExitButton = ffb::Settings::Int(L"ExitButton", 0);
	int TestButton = ffb::Settings::Int(L"TestButton", 0);
	int ServiceButton = ffb::Settings::Int(L"ServiceButton", 0);
	int CreditButton = ffb::Settings::Int(L"CreditButton", 0);
	int ViewButton = ffb::Settings::Int(L"ViewButton", 0);
	int ThreeDimensionalButton = ffb::Settings::Int(L"ThreeDimensionalButton", 0);
	int leverUp = ffb::Settings::Int(L"leverUp", 0);
	int leverDown = ffb::Settings::Int(L"leverDown", 0);
	int leverLeft = ffb::Settings::Int(L"leverLeft", 0);
	int leverRight = ffb::Settings::Int(L"leverRight", 0);
	int Gear1Button = ffb::Settings::Int(L"Gear1Button", 0);
	int Gear2Button = ffb::Settings::Int(L"Gear2Button", 0);
	int Gear3Button = ffb::Settings::Int(L"Gear3Button", 0);
	int Gear4Button = ffb::Settings::Int(L"Gear4Button", 0);
	int Gear5Button = ffb::Settings::Int(L"Gear5Button", 0);
	int Gear6Button = ffb::Settings::Int(L"Gear6Button", 0);
	int ExitButtonDevice2 = ffb::Settings::Int(L"ExitButtonDevice2", 0);
	int TestButtonDevice2 = ffb::Settings::Int(L"TestButtonDevice2", 0);
	int ServiceButtonDevice2 = ffb::Settings::Int(L"ServiceButtonDevice2", 0);
	int CreditButtonDevice2 = ffb::Settings::Int(L"CreditButtonDevice2", 0);
	int ViewButtonDevice2 = ffb::Settings::Int(L"ViewButtonDevice2", 0);
	int ThreeDimensionalButtonDevice2 = ffb::Settings::Int(L"ThreeDimensionalButtonDevice2", 0);
	int leverUpDevice2 = ffb::Settings::Int(L"leverUpDevice2", 0);
	int leverDownDevice2 = ffb::Settings::Int(L"leverDownDevice2", 0);
	int leverLeftDevice2 = ffb::Settings::Int(L"leverLeftDevice2", 0);
	int leverRightDevice2 = ffb::Settings::Int(L"leverRightDevice2", 0);
	int Gear1ButtonDevice2 = ffb::Settings::Int(L"Gear1ButtonDevice2", 0);
	int Gear2ButtonDevice2 = ffb::Settings::Int(L"Gear2ButtonDevice2", 0);
	int Gear3ButtonDevice2 = ffb::Settings::Int(L"Gear3ButtonDevice2", 0);
	int Gear4ButtonDevice2 = ffb::Settings::Int(L"Gear4ButtonDevice2", 0);
	int Gear5ButtonDevice2 = ffb::Settings::Int(L"Gear5ButtonDevice2", 0);
	int Gear6ButtonDevice2 = ffb::Settings::Int(L"Gear6ButtonDevice2", 0);
};
