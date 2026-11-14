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
#include "M2EmulatorInputs.h"

// Inputs of the wheel of SegaRally in M2Emulator, written in the memory of the emulated game
// (was M2EmulatorSegaRallyInputsEnabled)
class M2EmulatorSegaRallyInputs : public M2EmulatorInputs
{
public:
	using M2EmulatorInputs::M2EmulatorInputs;
	void Update() override;

private:
	INT_PTR imageBase {};
	bool testA {};
	bool serviceA {};
	bool startA {};
	bool vr1A {};
	bool shift1A {};
	bool shift2A {};
	bool shift3A {};
	bool shift4A {};
	bool shiftupA {};
	bool shiftdwnA {};
	bool coin1A {};
	bool coin2A {};
	bool ShiftUpPressed {};
	bool ShiftDownPressed {};
	bool Inputsinit = false;
	INT_PTR SteeringAddress {};
	INT_PTR AcclAddress {};
	INT_PTR BrakeAddress {};
	INT_PTR Button1Address {};
	INT_PTR Button2Address {};
	int SteeringValue {};
	int AcclValue {};
	int BrakeValue {};
	int GearValue {};
	int InputDeviceWheelSteeringAxis = ffb::Settings::Int(L"InputDeviceWheelSteeringAxis", 0);
	int InputDeviceWheelAcclAxis = ffb::Settings::Int(L"InputDeviceWheelAcclAxis", 0);
	int InputDeviceWheelBrakeAxis = ffb::Settings::Int(L"InputDeviceWheelBrakeAxis", 0);
	int InputDeviceWheelSteeringAxisDevice2 = ffb::Settings::Int(L"InputDeviceWheelSteeringAxisDevice2", 0);
	int InputDeviceWheelAcclAxisDevice2 = ffb::Settings::Int(L"InputDeviceWheelAcclAxisDevice2", 0);
	int InputDeviceWheelBrakeAxisDevice2 = ffb::Settings::Int(L"InputDeviceWheelBrakeAxisDevice2", 0);
	int InputDeviceWheelReverseAxis = ffb::Settings::Int(L"InputDeviceWheelReverseAxis", 0);
	int InputDeviceCombinedPedals = ffb::Settings::Int(L"InputDeviceCombinedPedals", 0);
	int SteeringDeadzone = ffb::Settings::Int(L"SteeringDeadzone", 0);
	int PedalDeadzone = ffb::Settings::Int(L"PedalDeadzone", 0);
	int ShowButtonNumbersForSetup = ffb::Settings::Int(L"ShowButtonNumbersForSetup", 0);
	int ShowKeyBoardNameForSetup = ffb::Settings::Int(L"ShowKeyBoardNameForSetup", 0);
	int ShowAxisForSetup = ffb::Settings::Int(L"ShowAxisForSetup", 0);
	int ExitButton = ffb::Settings::Int(L"ExitButton", 0);
	int TestButton = ffb::Settings::Int(L"TestButton", 0);
	int ServiceButton = ffb::Settings::Int(L"ServiceButton", 0);
	int Coin1Button = ffb::Settings::Int(L"Coin1Button", 0);
	int Coin2Button = ffb::Settings::Int(L"Coin2Button", 0);
	int VR1Button = ffb::Settings::Int(L"VR1Button", 0);
	int StartButton = ffb::Settings::Int(L"StartButton", 0);
	int Shift1Button = ffb::Settings::Int(L"Shift1Button", 0);
	int Shift2Button = ffb::Settings::Int(L"Shift2Button", 0);
	int Shift3Button = ffb::Settings::Int(L"Shift3Button", 0);
	int Shift4Button = ffb::Settings::Int(L"Shift4Button", 0);
	int ShiftUpButton = ffb::Settings::Int(L"ShiftUpButton", 0);
	int ShiftDownButton = ffb::Settings::Int(L"ShiftDownButton", 0);
	int ExitButtonDevice2 = ffb::Settings::Int(L"ExitButtonDevice2", 0);
	int TestButtonDevice2 = ffb::Settings::Int(L"TestButtonDevice2", 0);
	int ServiceButtonDevice2 = ffb::Settings::Int(L"ServiceButtonDevice2", 0);
	int Coin1ButtonDevice2 = ffb::Settings::Int(L"Coin1ButtonDevice2", 0);
	int Coin2ButtonDevice2 = ffb::Settings::Int(L"Coin2ButtonDevice2", 0);
	int VR1ButtonDevice2 = ffb::Settings::Int(L"VR1ButtonDevice2", 0);
	int StartButtonDevice2 = ffb::Settings::Int(L"StartButtonDevice2", 0);
	int Shift1ButtonDevice2 = ffb::Settings::Int(L"Shift1ButtonDevice2", 0);
	int Shift2ButtonDevice2 = ffb::Settings::Int(L"Shift2ButtonDevice2", 0);
	int Shift3ButtonDevice2 = ffb::Settings::Int(L"Shift3ButtonDevice2", 0);
	int Shift4ButtonDevice2 = ffb::Settings::Int(L"Shift4ButtonDevice2", 0);
	int ShiftUpButtonDevice2 = ffb::Settings::Int(L"ShiftUpButtonDevice2", 0);
	int ShiftDownButtonDevice2 = ffb::Settings::Int(L"ShiftDownButtonDevice2", 0);
};
