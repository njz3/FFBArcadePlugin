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

// Pokken Tournament (Bandai Namco, TeknoParrot): the rumble of the two motors of the controller, 0 to 255
// each, published by TeknoParrot (TeknoParrotFFB: left motor, TeknoParrotFFB2: right motor)
class PokkenTournament : public ffb::TeknoParrotGame
{
public:
	void Update() override;

private:
	int feedbackLength = ffb::Settings::Int(L"FeedbackLength", 120);
};
