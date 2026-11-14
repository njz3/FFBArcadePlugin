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

#include <string.h>
#include "OutputBoard.h"
#include "../Core/Settings.h"

namespace ffb
{
	OutputBoard::OutputBoard()
		: wheel(MainWheel()), device2(SecondDevice()), device3(ThirdDevice())
	{
	}

	bool OutputBoard::Is(const char* name, std::initializer_list<const char*> outputs)
	{
		for (const char* output : outputs)
		{
			if (strcmp(name, output) == 0)
				return true;
		}
		return false;
	}

	void OutputBoard::SendConstant(Direction direction, double strength)
	{
		static const bool kept = Settings::Int(L"UseConstantInf", 1) != 0;
		if (kept)
			wheel.KeptConstant(direction, strength);
		else
			wheel.Constant(direction, strength);
	}

	void OutputBoard::StopConstant()
	{
		SendConstant(Direction::FromLeft, 0.0);
		SendConstant(Direction::FromRight, 0.0);
	}

	Direction OutputBoard::Reversible(Direction direction)
	{
		static const bool reverse = Settings::Int(L"ReverseDirection", 0) != 0;
		return reverse ? Opposite(direction) : direction;
	}
}
