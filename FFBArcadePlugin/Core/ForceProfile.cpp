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

// the settings of DllMain.cpp the effects are scaled with
extern int configMinForce;
extern int configMaxForce;
extern int configAlternativeMinForceLeft;
extern int configAlternativeMaxForceLeft;
extern int configAlternativeMinForceRight;
extern int configAlternativeMaxForceRight;
extern int configFeedbackLength;
extern int PowerMode;
extern int EnableDamper;
extern int DamperStrength;
extern int EnableForceSpringEffect;
extern int ForceSpringStrength;
extern int SinePeriod;
extern int SineFadePeriod;
extern int SineStrength;
extern int RumbleStrengthLeftMotor;
extern int RumbleStrengthRightMotor;
extern int configMinForceDevice2;
extern int configMaxForceDevice2;
extern int configAlternativeMinForceLeftDevice2;
extern int configAlternativeMaxForceLeftDevice2;
extern int configAlternativeMinForceRightDevice2;
extern int configAlternativeMaxForceRightDevice2;
extern int configMinForceDevice3;
extern int configMaxForceDevice3;
extern int DoubleSine;
extern int DoubleConstant;
extern bool ConstantInfHoldsAtZero;

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
			read(configMinForce, L"MinForce", 0);
			read(configMaxForce, L"MaxForce", 100);
		}
		if (fields & Alternative)
		{
			read(configAlternativeMinForceLeft, L"AlternativeMinForceLeft", 0);
			read(configAlternativeMaxForceLeft, L"AlternativeMaxForceLeft", 100);
			read(configAlternativeMinForceRight, L"AlternativeMinForceRight", 0);
			read(configAlternativeMaxForceRight, L"AlternativeMaxForceRight", 100);
		}
		if (fields & FeedbackLength)
			read(configFeedbackLength, L"FeedbackLength", 120);
		if (fields & PowerMode)
			read(::PowerMode, L"PowerMode", 0);
		if (fields & Damper)
		{
			read(EnableDamper, L"EnableDamper", 0);
			read(DamperStrength, L"DamperStrength", 100);
		}
		if (fields & Spring)
		{
			read(EnableForceSpringEffect, L"EnableForceSpringEffect", 0);
			read(ForceSpringStrength, L"ForceSpringStrength", 0);
		}
		if (fields & Sine)
		{
			int sineDefault = (fields & FullSine) ? 100 : 0;
			read(SinePeriod, L"SinePeriod", sineDefault);
			read(SineFadePeriod, L"SineFadePeriod", 0);
			read(SineStrength, L"SineStrength", sineDefault);
		}
		if (fields & Rumble)
		{
			read(RumbleStrengthLeftMotor, L"RumbleStrengthLeftMotor", 0);
			read(RumbleStrengthRightMotor, L"RumbleStrengthRightMotor", 0);
		}
		if (fields & Device2)
		{
			read(configMinForceDevice2, L"MinForce", 0, L"Device2");
			read(configMaxForceDevice2, L"MaxForce", 100, L"Device2");
		}
		if (fields & Device2Alternative)
		{
			read(configAlternativeMinForceLeftDevice2, L"AlternativeMinForceLeft", 0, L"Device2");
			read(configAlternativeMaxForceLeftDevice2, L"AlternativeMaxForceLeft", 100, L"Device2");
			read(configAlternativeMinForceRightDevice2, L"AlternativeMinForceRight", 0, L"Device2");
			read(configAlternativeMaxForceRightDevice2, L"AlternativeMaxForceRight", 100, L"Device2");
		}
		if (fields & Device3)
		{
			read(configMinForceDevice3, L"MinForce", 0, L"Device3");
			read(configMaxForceDevice3, L"MaxForce", 100, L"Device3");
		}
		if (fields & Doubles)
		{
			read(DoubleSine, L"DoubleSine", 0);
			read(DoubleConstant, L"DoubleConstant", 0);
		}
		if (fields & HoldsAtZero)
			ConstantInfHoldsAtZero = true;
	}
}
