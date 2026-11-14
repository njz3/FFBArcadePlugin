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

#include "TestGame.h"
#include "SDL.h"
#include "../Common Files/GameIds.h"

using namespace ffb;

void TestGame::Update()
{
	switch (gameId) {
	case TEST_GAME_CONST:
		// turn left
		wheel.Constant(Direction::FromRight, 1);
		break;
	case TEST_GAME_SINE:
		wheel.Sine(125, 100, 0.25, 100);
		break;
	case TEST_GAME_FRICTION:
		wheel.Friction(1);
		break;
	case TEST_GAME_SPRING:
		wheel.Spring(1);
		break;
	case TEST_GAME_HEAVY:
		wheel.Friction(1);
		wheel.Spring(1);
		break;
	case TEST_GAME_LOOSE:
		wheel.Friction(0);
		wheel.Spring(0);
		break;
	case TEST_GAME_RUMBLE:
		wheel.Rumble(1.0, 1.0, SDL_HAPTIC_INFINITY);
		break;
	case TEST_GAME_RUMBLE_TRIGGER:
		wheel.RumbleTriggers(1.0, 1.0, SDL_HAPTIC_INFINITY);
		break;
	}
}
