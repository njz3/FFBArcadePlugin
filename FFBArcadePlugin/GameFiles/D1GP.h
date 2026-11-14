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

// D1 Grand Prix (TeknoParrot): TeknoParrot publishes the force of the wheel (FFB, / 1000: > 0 from the
// right, < 0 from the left) and an effect (FFB2: 0x01 spring, 0x02 vibration, 0x04 shake, ground effects
// on grass and rumble strips); the speed is read in the game's memory.
class D1GP : public ffb::TeknoParrotGame
{
public:
	void Update() override;

private:
	// Speed of the car: the strength of the ground effects is speed / 200, up to 1
	static constexpr ffb::GameValue<float> Speed{ ffb::Base::Module, 0x2150F4 };

	int enableGroundEffects = ffb::Settings::Int(L"EnableGroundEffects", 0);
	int ffbGrassSinePeriod = ffb::Settings::Int(L"FFBGrassSinePeriod", 0);
	int ffbGrassFadeSinePeriod = ffb::Settings::Int(L"FFBGrassFadeSinePeriod", 0);
	int ffbRumbleStripSinePeriod = ffb::Settings::Int(L"FFBRumbleStripSinePeriod", 0);
	int ffbRumbleStripFadeSinePeriod = ffb::Settings::Int(L"FFBRumbleStripFadeSinePeriod", 0);

	// the shake lasts 32 updates
	int shakeCount = 0;
	bool letsShake = false;
	UINT8 oldEffect = 0;
};
