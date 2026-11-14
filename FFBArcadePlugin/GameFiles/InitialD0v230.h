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

// Initial D Zero v2.30 (Sega ALLS): see InitialDZeroGame. OutputBlaster.dll is loaded when
// EnableOutputs is not 0.
class InitialD0v230 : public InitialDZeroGame
{
public:
	InitialD0v230() : InitialDZeroGame(0x18B54DC, LongVibrationScaling, ffb::Settings::Int(L"EnableOutputs", 0) != 0) {}
};
