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
#include <Windows.h>
#include "../Core/Settings.h"

// Addresses of the inputs in the memory of the game emulated by Demul, found by pattern scans
struct DemulInputAddresses
{
	INT_PTR steering = 0;
	INT_PTR accelerator = 0;
	INT_PTR brake = 0;
	INT_PTR startView = 0;
	INT_PTR shiftUpDown = 0;
	INT_PTR serviceTest = 0;
	INT_PTR coin = 0;
	INT_PTR card = 0;
	INT_PTR card2 = 0;
};

// The inputs of the wheel (InputDeviceWheelEnable) and of a second device of a game in Demul: the
// joystick events (SDL) are written to the memory of the emulated game
class DemulInputs
{
public:
	explicit DemulInputs(const DemulInputAddresses& addresses)
		: SteeringAddress(addresses.steering), AcclAddress(addresses.accelerator), BrakeAddress(addresses.brake),
		StartViewAddress(addresses.startView), ShiftUpDownAddress(addresses.shiftUpDown),
		ServiceTestAddress(addresses.serviceTest), CoinAddress(addresses.coin), CardAddress(addresses.card),
		CardAddress2(addresses.card2)
	{
	}
	virtual ~DemulInputs() = default;

	// Called by the FFB loop while the inputs are enabled
	virtual void Update() = 0;

protected:
	const INT_PTR& SteeringAddress;
	const INT_PTR& AcclAddress;
	const INT_PTR& BrakeAddress;
	const INT_PTR& StartViewAddress;
	const INT_PTR& ShiftUpDownAddress;
	const INT_PTR& ServiceTestAddress;
	const INT_PTR& CoinAddress;
	const INT_PTR& CardAddress;
	const INT_PTR& CardAddress2;
};
