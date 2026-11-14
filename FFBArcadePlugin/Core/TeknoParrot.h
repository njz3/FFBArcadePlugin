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
#include "Game.h"

namespace ffb
{
	// A game run by TeknoParrot, which publishes the FFB values of the game's I/O board in its
	// TeknoParrot_JvsState shared memory
	class TeknoParrotGame : public Game
	{
	protected:
		TeknoParrotGame();

		// The main FFB value (slot 2, or slot 6 for the games publishing it in the extended slots)
		int TeknoParrotFFB();
		// The extended FFB values (slots 7, 8 and 9)
		int TeknoParrotFFB2();
		int TeknoParrotFFB3();
		int TeknoParrotFFB4();

	private:
		HANDLE section;
		int* jvsState; // seen as an array of int slots
		int ReadSlot(int slot);
	};
}
