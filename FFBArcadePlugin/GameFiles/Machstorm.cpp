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

#include "Machstorm.h"

using namespace ffb;

void Machstorm::Update()
{
	Settings::Int(L"DomeFix", 0); // read, unused
	int strengths[PowerCount];
	int lengths[PowerCount];
	for (int i = 0; i < PowerCount; i++)
		strengths[i] = Settings::Int(StrengthKeys[i], 0);
	for (int i = 0; i < PowerCount; i++)
		lengths[i] = Settings::Int(LengthKeys[i], 0);
	int vibration = Memory::Read(Vibration);
	int power = Memory::Read(Power);

	// index of the power, -1 if none
	int level = -1;
	if (vibration == 16842753 && power >= 61542 && power <= 61551)
		level = power - 61542;
	if (vibration == 1 && power >= 16773366 && power <= 16773375)
		level = power - 16773366;

	if (level >= 0)
	{
		UINT32 percentLength = (lengths[level]);
		double percentForce = ((strengths[level]) / 100.0);
		wheel.Rumble(percentForce, percentForce, percentLength);
	}
}
