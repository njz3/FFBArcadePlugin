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

#include "InitialD4.h"

using namespace ffb;

InitialDLindberghGame::InitialDLindberghGame(INT_PTR driveBoardAddress, bool applyDamper)
	: driveBoardCommand{ Base::Absolute, driveBoardAddress }, applyDamper(applyDamper)
{
}

void InitialDLindberghGame::Update()
{
	if (applyDamper)
		ApplyDamper();

	DWORD command = Memory::Read(driveBoardCommand);
	const BYTE* bytes = reinterpret_cast<const BYTE*>(&command);

	driveBoard.Play(bytes[0], bytes[1], bytes[2]);
}
