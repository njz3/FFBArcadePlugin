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

#include "InitialD0v131.h"

using namespace ffb;

InitialDZeroGame::InitialDZeroGame(INT_PTR driveBoardOffset, const SegaInitialDDriveBoard::Scaling& scaling, bool loadOutputs)
	: driveBoardCommand{ Base::Module, driveBoardOffset }, driveBoard(wheel, scaling), loadOutputs(loadOutputs)
{
}

void InitialDZeroGame::Update()
{
	if (!outputsLoaded && loadOutputs)
	{
		outputsLoaded = true;
		LoadLibraryA("OutputBlaster.dll");
	}

	if (GetAsyncKeyState((VK_ESCAPE)) && (escapeKeyExitViaPlugin == 1))
		ExitProcess(0);

	ApplyDamper();

	DWORD command = Memory::Read(driveBoardCommand);
	const BYTE* bytes = reinterpret_cast<const BYTE*>(&command);

	driveBoard.Play(bytes[0], bytes[1], bytes[2]);
}
