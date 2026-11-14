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
#include "InitialD0v131.h"

// Initial D Zero v2.11 (Sega ALLS): see InitialDZeroGame. Vibrations of 100 ms; OutputBlaster.dll is
// loaded only when EnableOutputs is 1.
class InitialD0v211 : public InitialDZeroGame
{
public:
	InitialD0v211() : InitialDZeroGame(0x18A720C, ffb::SegaInitialDDriveBoard::DefaultScaling, ffb::Settings::Int(L"EnableOutputs", 0) == 1) {}
};
