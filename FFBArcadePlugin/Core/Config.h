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
#include <string>

namespace ffb
{
	// A range of force, in % of the full force of a device
	struct ForceLimits
	{
		int min;
		int max;
	};

	// The settings of a device that scale its effects
	struct DeviceConfig
	{
		ForceLimits force;            // MinForce, MaxForce
		int alternativeFFB;           // AlternativeFFB: other limits for each side
		ForceLimits alternativeLeft;  // AlternativeMinForceLeft, AlternativeMaxForceLeft
		ForceLimits alternativeRight; // AlternativeMinForceRight, AlternativeMaxForceRight
		int enableRumble;             // EnableRumble: rumble motors of a game controller (cleared if it has none)
		int reverseRumble;            // ReverseRumble: the two motors swapped
	};

	// The settings of FFBPlugin.ini, read when the plugin is loaded. The force settings may then be
	// replaced by those of the game (ForceProfile) and the max force changed in game.
	struct Config
	{
		static Config& Get();

		int gameId;                // GameId, see GameIds.h
		std::wstring deviceGuid;   // DeviceGUID of device 1; devices 2 and 3 are in Device2GUID and Device3GUID
		DeviceConfig device1;      // the wheel
		DeviceConfig device2;      // ...Device2 settings
		DeviceConfig device3;      // ...Device3 settings
		int feedbackLength;        // FeedbackLength: length of the effects, ms
		int powerMode;             // PowerMode: square root of the constant force
		int enableRumbleTriggers;  // EnableRumbleTriggers
		int singleRumbleMotor;     // SingleRumbleMotor: both strengths on the low frequency motor
		int enableDamper;          // EnableDamper, DamperStrength: a damper played by the games
		int damperStrength;
		int defaultCentering;      // DefaultCentering, DefaultFriction: % played until the game changes them
		int defaultFriction;
		int enableForceSpringEffect; // EnableForceSpringEffect, ForceSpringStrength: a spring kept by the games
		int forceSpringStrength;
		int sinePeriod;            // SinePeriod, SineFadePeriod, SineStrength: the vibration of some games
		int sineFadePeriod;
		int sineStrength;
		int rumbleStrengthLeftMotor;  // RumbleStrengthLeftMotor, RumbleStrengthRightMotor: the rumble of some games
		int rumbleStrengthRightMotor;
		int doubleSine;            // DoubleSine, DoubleConstant, DoubleSpring, DoubleFriction: strength doubled
		int doubleConstant;
		int doubleSpring;
		int doubleFriction;
		int ffbOrRumble;           // FFBOrRumble
		int beepWhenHook;          // BeepWhenHook
		int forceShowDeviceGUIDMessageBox; // ForceShowDeviceGUIDMessageBox
		int ignoreFirstMatchingGUID;       // IgnoreFirstMatchingGUID (cleared once a device is skipped)
		int autoCloseWindowError;  // AutoCloseWindowError
		int inputDeviceWheelEnable; // InputDeviceWheelEnable: the plugin writes the inputs of the wheel
		int startDelay;            // StartDelay
		// Max force changed in game with buttons or keys
		int enableFFBStrengthDynamicAdjustment;
		int increaseFFBStrength;   // buttons
		int decreaseFFBStrength;
		int resetFFBStrength;
		int stepFFBStrength;
		std::wstring increaseFFBStrengthKey; // keys, named as by the GUI ("F5", "Keypad +"), empty if not used
		std::wstring decreaseFFBStrengthKey;
		std::wstring resetFFBStrengthKey;
		int enableFFBStrengthPersistence;  // the max force is saved in FFBPlugin.ini
		int enableFFBStrengthTextToSpeech; // the max force is spoken

	private:
		Config();
	};
}
