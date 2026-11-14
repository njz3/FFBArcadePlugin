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
#include "WMMT.h"

// Wangan Midnight Maximum Tune 6RR (Bandai Namco, TeknoParrot): see WMMT. Only the forces of the game, the
// collisions and the tires slip once the car runs (rpm or speed not 0); ReverseCollision and
// ReverseTiresSlip reverse their sides.
// TODO: wheel spin and gear change effects. Gear and race state pointers may be found from
// Wg6Enma_Release_IDL0.dll+29B3E0 (gear at +0x3AC or +0x3A8) and Wg6Enma_Release_IDL0.dll+29B3F8 (time at +0x18).
class WMMT6RR : public WMMT
{
public:
	WMMT6RR() : WMMT({
		0x1F25260, // ffbValues
		0x1F2612C, // speed
		0x1F2629C, // rpm
		0,         // carState
		0,         // gearOffset
		0,         // raceState
		0,         // raceTimer
	}) {}

	void Update() override;

private:
	int reverseCollision = ffb::Settings::Int(L"ReverseCollision", 0);
	int reverseTiresSlip = ffb::Settings::Int(L"ReverseTiresSlip", 0);
};
