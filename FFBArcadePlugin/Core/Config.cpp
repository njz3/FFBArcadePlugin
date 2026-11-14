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

#include "Config.h"
#include "Settings.h"

namespace ffb
{
	Config& Config::Get()
	{
		static Config config;
		return config;
	}

	static DeviceConfig ReadDevice(const wchar_t* device)
	{
		auto read = [device](const wchar_t* name, int defaultValue) {
			return Settings::Int((std::wstring(name) + device).c_str(), defaultValue);
		};
		DeviceConfig config;
		config.force = { read(L"MinForce", 0), read(L"MaxForce", 100) };
		config.alternativeFFB = read(L"AlternativeFFB", 0);
		config.alternativeLeft = { read(L"AlternativeMinForceLeft", 0), read(L"AlternativeMaxForceLeft", 100) };
		config.alternativeRight = { read(L"AlternativeMinForceRight", 0), read(L"AlternativeMaxForceRight", 100) };
		config.enableRumble = read(L"EnableRumble", 0);
		config.reverseRumble = read(L"ReverseRumble", 0);
		return config;
	}

	Config::Config()
	{
		gameId = Settings::Int(L"GameId", 0);
		deviceGuid = Settings::String(L"DeviceGUID");
		device1 = ReadDevice(L"");
		device2 = ReadDevice(L"Device2");
		device3 = ReadDevice(L"Device3");
		feedbackLength = Settings::Int(L"FeedbackLength", 120);
		powerMode = Settings::Int(L"PowerMode", 0);
		enableRumbleTriggers = Settings::Int(L"EnableRumbleTriggers", 0);
		singleRumbleMotor = Settings::Int(L"SingleRumbleMotor", 0);
		enableDamper = Settings::Int(L"EnableDamper", 0);
		damperStrength = Settings::Int(L"DamperStrength", 100);
		defaultCentering = Settings::Int(L"DefaultCentering", 0);
		defaultFriction = Settings::Int(L"DefaultFriction", 0);
		enableForceSpringEffect = Settings::Int(L"EnableForceSpringEffect", 0);
		forceSpringStrength = Settings::Int(L"ForceSpringStrength", 0);
		sinePeriod = Settings::Int(L"SinePeriod", 0);
		sineFadePeriod = Settings::Int(L"SineFadePeriod", 0);
		sineStrength = Settings::Int(L"SineStrength", 0);
		rumbleStrengthLeftMotor = Settings::Int(L"RumbleStrengthLeftMotor", 0);
		rumbleStrengthRightMotor = Settings::Int(L"RumbleStrengthRightMotor", 0);
		doubleSine = Settings::Int(L"DoubleSine", 0);
		doubleConstant = Settings::Int(L"DoubleConstant", 0);
		doubleSpring = Settings::Int(L"DoubleSpring", 0);
		doubleFriction = Settings::Int(L"DoubleFriction", 0);
		ffbOrRumble = Settings::Int(L"FFBOrRumble", 0);
		beepWhenHook = Settings::Int(L"BeepWhenHook", 0);
		forceShowDeviceGUIDMessageBox = Settings::Int(L"ForceShowDeviceGUIDMessageBox", 0);
		ignoreFirstMatchingGUID = Settings::Int(L"IgnoreFirstMatchingGUID", 0);
		autoCloseWindowError = Settings::Int(L"AutoCloseWindowError", 0);
		inputDeviceWheelEnable = Settings::Int(L"InputDeviceWheelEnable", 0);
		startDelay = Settings::Int(L"StartDelay", 0);
		enableFFBStrengthDynamicAdjustment = Settings::Int(L"EnableFFBStrengthDynamicAdjustment", 0);
		increaseFFBStrength = Settings::Int(L"IncreaseFFBStrength", 0);
		decreaseFFBStrength = Settings::Int(L"DecreaseFFBStrength", 0);
		resetFFBStrength = Settings::Int(L"ResetFFBStrength", 0);
		stepFFBStrength = Settings::Int(L"StepFFBStrength", 5);
		increaseFFBStrengthKey = Settings::String(L"IncreaseFFBStrengthKey");
		decreaseFFBStrengthKey = Settings::String(L"DecreaseFFBStrengthKey");
		resetFFBStrengthKey = Settings::String(L"ResetFFBStrengthKey");
		enableFFBStrengthPersistence = Settings::Int(L"EnableFFBStrengthPersistence", 0);
		enableFFBStrengthTextToSpeech = Settings::Int(L"EnableFFBStrengthTextToSpeech", 0);
	}
}
