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

// Starts the threads that read the network outputs of MAME, Supermodel and Flycast, and pass
// them to the same handlers as the Windows output protocol (MAME64.dll).
// gameId: the GameId of FFBPlugin.ini, which says how the emulator writes the values.
void StartMameNetworkOutputs(int gameId);
