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
#include "../Core/TeknoParrot.h"
#include "../Hardware/SegaInitialD.h"

// Initial D Arcade Stage 6 AA, 7 AAX and 8 Infinity (Sega RingWide / RingEdge, TeknoParrot): TeknoParrot
// publishes the command of the drive board with its bytes in reverse order (byte 2 the command
// without its 0x80 flag, byte 1 the parameter, byte 0 the strength). Spring and constant force
// commands with a strength above 0x7F are ignored.
class InitialDTeknoParrotGame : public ffb::TeknoParrotGame
{
public:
	void Update() override;

protected:
	InitialDTeknoParrotGame() = default;

private:
	ffb::SegaInitialDDriveBoard driveBoard{ wheel, { 127.0, 128, 127.0, 100, 0x7F } };
};

// Initial D Arcade Stage 6 AA (Sega RingWide, TeknoParrot)
class InitialD6 : public InitialDTeknoParrotGame
{
};
