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
#include "Daytona3.h"

// Daytona Championship USA New Season Edition (Sega): the drive board command is always played, with
// no damper (see Daytona3Game)
class Daytona3NSE : public Daytona3Game
{
public:
	Daytona3NSE() : Daytona3Game({ 0x1334061, 0x15C98C4, 0x11C22A0 }) {}
	void Update() override { PlayDriveBoard(); }
};
