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

// Storm Racer G (TeknoParrot): TeknoParrot publishes the force of the wheel (FFB, / 1000: > 0 from the
// right, < 0 from the left), a shake (FFB2, > 0) and the ground under the car (FFB3: < 10000 road,
// < 300000 grass, < 700000 rough ground, else rumble strip, its strength in the thousands); the speed
// and the racing state are read in the game's memory.
class StormRacerG : public ffb::TeknoParrotGame
{
public:
	void Update() override;

private:
	// Speed of the car: the ground effects play above 1
	static constexpr ffb::GameValue<float> Speed{ ffb::Base::Module, 0x10BE22D8 };
	// 0x01 racing, 0x00 in the menus
	static constexpr ffb::GameValue<UINT8> Racing{ ffb::Base::Module, 0x10BD5B20 };

	int enableShakeEffectsWhileRacing = ffb::Settings::Int(L"EnableShakeEffectsWhileRacing", 0);
	int enableShakeEffectsinMenu = ffb::Settings::Int(L"EnableShakeEffectsinMenu", 0);
	int enableGroundEffects = ffb::Settings::Int(L"EnableGroundEffects", 0);
	int enableForceSpringEffect = ffb::Settings::Int(L"EnableForceSpringEffect", 0); // 1 always, 2 in the menus
	int forceSpringStrength = ffb::Settings::Int(L"ForceSpringStrength", 0);
	int minFFBConstantStrength = ffb::Settings::Int(L"MinFFBConstantStrength", 0);
	int ffbShakeSineStrength = ffb::Settings::Int(L"FFBShakeSineStrength", 0);
	int ffbShakeSinePeriod = ffb::Settings::Int(L"FFBShakeSinePeriod", 0);
	int ffbShakeFadeSinePeriod = ffb::Settings::Int(L"FFBShakeFadeSinePeriod", 0);
	int ffbGrassSinePeriod = ffb::Settings::Int(L"FFBGrassSinePeriod", 0);
	int ffbGrassSineStrength = ffb::Settings::Int(L"FFBGrassSineStrength", 0);
	int ffbGrassFadeSinePeriod = ffb::Settings::Int(L"FFBGrassFadeSinePeriod", 0);
	int ffbRoughTerrainSineStrength = ffb::Settings::Int(L"FFBRoughTerrainSineStrength", 0);
	int ffbRoughTerrainSinePeriod = ffb::Settings::Int(L"FFBRoughTerrainSinePeriod", 0);
	int ffbRoughTerrainFadeSinePeriod = ffb::Settings::Int(L"FFBRoughTerrainFadeSinePeriod", 0);
	int ffbRumbleStripSineStrength = ffb::Settings::Int(L"FFBRumbleStripSineStrength", 0);
	int ffbRumbleStripSinePeriod = ffb::Settings::Int(L"FFBRumbleStripSinePeriod", 0);
	int ffbRumbleStripFadeSinePeriod = ffb::Settings::Int(L"FFBRumbleStripFadeSinePeriod", 0);
};
