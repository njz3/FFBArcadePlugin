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

// Dead Heat and Wangan Midnight Maximum Tune 3 (Namco, TeknoParrot): TeknoParrot publishes four FFB values
// of the game: spring (FFB), viscosity (FFB2), center offset (FFB3) and reflect (FFB4), 1 to 0x3F, each
// scaled by its strength setting (%). The two games differ only by the reflect from the left (see
// PlayReflect). Dead Heat Riders plays the same values without strength settings (see DeadHeatRiders).
class NamcoWheelGame : public ffb::TeknoParrotGame
{
public:
	void Update() override;

protected:
	// Reflect, not 0 (or the center offset is not 0): 1 to 0x3F from the right, other values from the left
	virtual void PlayReflect(int reflect) = 0;

	int centerOffsetStrength = ffb::Settings::Int(L"CenterOffsetStrength", 100);
	int reflectStrength = ffb::Settings::Int(L"ReflectStrength", 100);
	int springStrength = ffb::Settings::Int(L"SpringStrength", 100);
	int viscosityStrength = ffb::Settings::Int(L"ViosityStrength", 100);
};

// Dead Heat: the reflect from the left is 0x40 to 0xFF (0xFF the weakest)
class DeadHeat : public NamcoWheelGame
{
protected:
	void PlayReflect(int reflect) override;
};
