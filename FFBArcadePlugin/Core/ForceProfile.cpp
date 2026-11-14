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

#include <string>
#include "ForceProfile.h"
#include "Settings.h"
#include "Config.h"
#include "PluginState.h"

static ffb::Config& config = ffb::Config::Get();


namespace ffb
{
	void ForceProfile::Apply(const wchar_t* suffix, unsigned fields)
	{
		// the setting <name><suffix><device>, e.g. AlternativeMinForceLeftORunnersDevice2
		auto read = [suffix](int& setting, const wchar_t* name, int defaultValue, const wchar_t* device = L"") {
			setting = Settings::Int((std::wstring(name) + suffix + device).c_str(), defaultValue);
		};

		if (fields & Force)
		{
			read(config.device1.force.min, L"MinForce", 0);
			read(config.device1.force.max, L"MaxForce", 100);
		}
		if (fields & Alternative)
		{
			read(config.device1.alternativeLeft.min, L"AlternativeMinForceLeft", 0);
			read(config.device1.alternativeLeft.max, L"AlternativeMaxForceLeft", 100);
			read(config.device1.alternativeRight.min, L"AlternativeMinForceRight", 0);
			read(config.device1.alternativeRight.max, L"AlternativeMaxForceRight", 100);
		}
		if (fields & FeedbackLength)
			read(config.feedbackLength, L"FeedbackLength", 120);
		if (fields & PowerMode)
			read(config.powerMode, L"PowerMode", 0);
		if (fields & Damper)
		{
			read(config.enableDamper, L"EnableDamper", 0);
			read(config.damperStrength, L"DamperStrength", 100);
		}
		if (fields & Spring)
		{
			read(config.enableForceSpringEffect, L"EnableForceSpringEffect", 0);
			read(config.forceSpringStrength, L"ForceSpringStrength", 0);
		}
		if (fields & Sine)
		{
			int sineDefault = (fields & FullSine) ? 100 : 0;
			read(config.sinePeriod, L"SinePeriod", sineDefault);
			read(config.sineFadePeriod, L"SineFadePeriod", 0);
			read(config.sineStrength, L"SineStrength", sineDefault);
		}
		if (fields & Rumble)
		{
			read(config.rumbleStrengthLeftMotor, L"RumbleStrengthLeftMotor", 0);
			read(config.rumbleStrengthRightMotor, L"RumbleStrengthRightMotor", 0);
		}
		if (fields & Device2)
		{
			read(config.device2.force.min, L"MinForce", 0, L"Device2");
			read(config.device2.force.max, L"MaxForce", 100, L"Device2");
		}
		if (fields & Device2Alternative)
		{
			read(config.device2.alternativeLeft.min, L"AlternativeMinForceLeft", 0, L"Device2");
			read(config.device2.alternativeLeft.max, L"AlternativeMaxForceLeft", 100, L"Device2");
			read(config.device2.alternativeRight.min, L"AlternativeMinForceRight", 0, L"Device2");
			read(config.device2.alternativeRight.max, L"AlternativeMaxForceRight", 100, L"Device2");
		}
		if (fields & Device3)
		{
			read(config.device3.force.min, L"MinForce", 0, L"Device3");
			read(config.device3.force.max, L"MaxForce", 100, L"Device3");
		}
		if (fields & Doubles)
		{
			read(config.doubleSine, L"DoubleSine", 0);
			read(config.doubleConstant, L"DoubleConstant", 0);
		}
		if (fields & HoldsAtZero)
			ConstantInfHoldsAtZero = true;
	}
}
