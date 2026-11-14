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
#include "DemulInputs.h"

// Inputs of the wheel of SmashingDrive in Demul, written in the memory of the emulated game
// (was SmashingDriveInputsEnabled)
class DemulSmashingDriveInputs : public DemulInputs
{
public:
	using DemulInputs::DemulInputs;
	void Update() override;

private:
	bool testbuttonA {};
	bool servicebuttonA {};
	bool coinbuttonA {};
	bool hornbuttonA {};
	bool volupA {};
	bool voldownA {};
	bool acclpressed {};
	bool brakepressed {};
	bool Inputsinit = false;
	int AcclValue {};
	int BrakeValue {};
	int InputDeviceWheelSteeringAxis = ffb::Settings::Int(L"InputDeviceWheelSteeringAxis", 0);
	int InputDeviceWheelAcclAxis = ffb::Settings::Int(L"InputDeviceWheelAcclAxis", 0);
	int InputDeviceWheelBrakeAxis = ffb::Settings::Int(L"InputDeviceWheelBrakeAxis", 0);
	int InputDeviceWheelReverseAxis = ffb::Settings::Int(L"InputDeviceWheelReverseAxis", 0);
	int InputDeviceCombinedPedals = ffb::Settings::Int(L"InputDeviceCombinedPedals", 0);
	int SteeringDeadzone = ffb::Settings::Int(L"SteeringDeadzone", 0);
	int PedalDeadzone = ffb::Settings::Int(L"PedalDeadzone", 0);
	int ShowButtonNumbersForSetup = ffb::Settings::Int(L"ShowButtonNumbersForSetup", 0);
	int ShowAxisForSetup = ffb::Settings::Int(L"ShowAxisForSetup", 0);
	int ExitButton = ffb::Settings::Int(L"ExitButton", 0);
	int TestButton = ffb::Settings::Int(L"TestButton", 0);
	int ServiceButton = ffb::Settings::Int(L"ServiceButton", 0);
	int CreditButton = ffb::Settings::Int(L"CreditButton", 0);
	int HornButton = ffb::Settings::Int(L"HornButton", 0);
	int VolumeUp = ffb::Settings::Int(L"VolumeUp", 0);
	int VolumeDown = ffb::Settings::Int(L"VolumeDown", 0);
	int ExitButtonDevice2 = ffb::Settings::Int(L"ExitButtonDevice2", 0);
	int TestButtonDevice2 = ffb::Settings::Int(L"TestButtonDevice2", 0);
	int ServiceButtonDevice2 = ffb::Settings::Int(L"ServiceButtonDevice2", 0);
	int CreditButtonDevice2 = ffb::Settings::Int(L"CreditButtonDevice2", 0);
	int HornButtonDevice2 = ffb::Settings::Int(L"HornButtonDevice2", 0);
	int VolumeUpDevice2 = ffb::Settings::Int(L"VolumeUpDevice2", 0);
	int VolumeDownDevice2 = ffb::Settings::Int(L"VolumeDownDevice2", 0);
};
