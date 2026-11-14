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

// Wangan Midnight Maximum Tune 5 (Bandai Namco, TeknoParrot): see WMMT. Its power tune is read for the
// wheel spin. A thread keeps the player car at full tune (ForceFullTune) and the remaining race time at
// 999.99 (DisableRaceTimer), and the input thread handles ForceFinishButton and ForceTimeUpButton.
class WMMT5 : public WMMT
{
public:
	WMMT5() : WMMT({
		0x196F188, // ffbValues
		0x196FEBC, // speed
		0x1970038, // rpm
		0x199A450, // carState
		0x398,     // gearOffset
		0x199A468, // raceState
		0x199AE18, // raceTimer
	}) {}

protected:
	void StartThreads() override;
	UINT8 ReadPower() override;

private:
	// Tune of the player car (pointer)
	static constexpr ffb::PointerChain<INT_PTR, 1> CarTune{ { ffb::Base::Module, 0x1948F10 }, { 0x180 + 0xA8 + 0x18 } };
	// Values (UINT8) in the tune
	static const INT_PTR Car = 0x2C;      // the car, 0 none
	static const INT_PTR Power = 0x98;    // power tune, 0x10 full
	static const INT_PTR Handling = 0x9C; // handling tune, 0x10 full

	static int SpamThread(void* game);

	int forceFullTune = ffb::Settings::Int(L"ForceFullTune", 0);
};
