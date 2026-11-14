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

// Inputs of the wheel of InitialD in Demul, written in the memory of the emulated game
// (was InitialDInputsEnabled)
class DemulInitialDInputs : public DemulInputs
{
public:
	using DemulInputs::DemulInputs;
	void Update() override;

private:
	bool testbuttonA {};
	bool servicebuttonA {};
	bool startbuttonA {};
	bool viewbuttonA {};
	bool shiftupA {};
	bool shiftdownA {};
	bool coinA {};
	bool cardA {};
	bool cardB {};
	bool Inputsinit = false;
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
	int ViewButton = ffb::Settings::Int(L"ViewButton", 0);
	int StartButton = ffb::Settings::Int(L"StartButton", 0);
	int ShiftUp = ffb::Settings::Int(L"ShiftUp", 0);
	int ShiftDown = ffb::Settings::Int(L"ShiftDown", 0);
	int InsertRemoveCard = ffb::Settings::Int(L"InsertRemoveCard", 0);
	int ExitButtonDevice2 = ffb::Settings::Int(L"ExitButtonDevice2", 0);
	int TestButtonDevice2 = ffb::Settings::Int(L"TestButtonDevice2", 0);
	int ServiceButtonDevice2 = ffb::Settings::Int(L"ServiceButtonDevice2", 0);
	int CreditButtonDevice2 = ffb::Settings::Int(L"CreditButtonDevice2", 0);
	int ViewButtonDevice2 = ffb::Settings::Int(L"ViewButtonDevice2", 0);
	int StartButtonDevice2 = ffb::Settings::Int(L"StartButtonDevice2", 0);
	int ShiftUpDevice2 = ffb::Settings::Int(L"ShiftUpDevice2", 0);
	int ShiftDownDevice2 = ffb::Settings::Int(L"ShiftDownDevice2", 0);
	int InsertRemoveCardDevice2 = ffb::Settings::Int(L"InsertRemoveCardDevice2", 0);
};
