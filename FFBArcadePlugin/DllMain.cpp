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

#include <Windows.h>
#include <math.h>
#include <dinput.h>
#include <stdio.h>
#include "SDL.h"
#include <string>
#include <chrono>
#include "IDirectInputDevice.h"
#include <sapi.h>
#include <atlcomcli.h>
#include <TlHelp32.h>
#include "Config/PersistentValues.h"
#include "Common Files/GameIds.h"
#include "WrapperExports.h"

// include all game header files here.
#include "Game Files/TestGame.h"
#include "Game Files/AliensExtermination.h"
#include "Game Files/Batman.h"
#include "Game Files/BG4JP.h"
#include "Game Files/Cars.h"
#include "Game Files/ChaseHQ2.h"
#include "Game Files/CrazyTaxi.h"
#include "Game Files/CruisnBlast.h"
#include "Game Files/D1GP.h"
#include "Game Files/Daytona3.h"
#include "Game Files/Daytona3NSE.h"
#include "Game Files/DeadHeat.h"
#include "Game Files/DeadHeatRiders.h"
#include "Game Files/DirtyDrivin.h"
#include "Game Files/FnF.h"
#include "Game Files/FnFDrift.h"
#include "Game Files/FnFSuperCars.h"
#include "Game Files/FordRacing.h"
#include "Game Files/FordRacingOther.h"
#include "Game Files/GaelcoTuningRace.h"
#include "Game Files/GRIDReal.h"
#include "Game Files/GRIDCustom.h"
#include "Game Files/GoldenGun.h"
#include "Game Files/Hummer.h"
#include "Game Files/HummerExtreme.h"
#include "Game Files/InitialD0v131.h"
#include "Game Files/InitialD0v211.h"
#include "Game Files/InitialD0v230.h"
#include "Game Files/InitialD4.h"
#include "Game Files/InitialD4Japan.h"
#include "Game Files/InitialD5.h"
#include "Game Files/InitialD6.h"
#include "Game Files/InitialD7.h"
#include "Game Files/InitialD8.h"
#include "Game Files/IDTAv231.h"
#include "Game Files/MarioKartGPDXCustom.h"
#include "Game Files/MarioKartGPDX1.10Custom.h"
#include "Game Files/MarioKartGPDX1.10Real.h"
#include "Game Files/MarioKartGPDX1.18Custom.h"
#include "Game Files/MarioKartGPDX1.18Real.h"
#include "Game Files/MarioKartGPDXUSACustom.h"
#include "Game Files/MarioKartGPDXUSAReal.h"
#include "Game Files/MAMESupermodel.h"
#include "Game Files/OutRun2Fake.h"
#include "Game Files/OutRun2Real.h"
#include "Game Files/SegaRaceTV.h"
#include "Game Files/SegaRacingClassic.h"
#include "Game Files/SegaRally3.h"
#include "Game Files/SegaRally3Other.h"
#include "Game Files/Showdown.h"
#include "Game Files/SnoCross.h"
#include "Game Files/WackyRaces.h"
#include "Game Files/WastelandRacers2071.h"
#include "Game Files/WMMT3.h"
#include "Game Files/WMMT5.h"
#include "Game Files/WMMT5DX.h"
#include "Game Files/WMMT5DX+.h"
#include "Game Files/WMMT6.h"
#include "Game Files/WMMT6R.h"
#include "Game Files/WMMT6RR.h"
#include "Game Files/Machstorm.h"
#include "Game Files/AfterburnerClimax.h"
#include "Game Files/PokkenTournament.h"
#include "Game Files/SonicSegaAllStarsRacing.h"
#include "Game Files/SpringEffect.h"
#include "Game Files/M2Emulator.h"
#include "Game Files/GTIClub3.h"
#include "Game Files/Demul.h"
#include "Game Files/ButtonRumble.h"
#include "Game Files/RingRiders.h"
#include "Game Files/RoadFighters3D.h"
#include "Game Files/LGI3D.h"
#include "Game Files/LGI.h"
#include "Game Files/KODrive.h"
#include "Game Files/HOTD4.h"
#include "Game Files/Rambo.h"
#include "Game Files/R-Tuned.h"
#include "Game Files/SWDC2018.h"
#include "Game Files/TokyoCop.h"
#include "Game Files/Transformers.h"
#include "Game Files/H2Overdrive.h"
#include "Game Files/StormRacerG.h"
#include "Game Files/ArcticThunder.h"

// DINPUT8 FUNCTION DEFINITIONS
typedef HRESULT(WINAPI* pfnDirectInputDirectInput8Create)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);
typedef HRESULT(WINAPI* pfnDirectInputDllRegisterServer)(void);
typedef HRESULT(WINAPI* pfnDirectInputDllUnregisterServer)(void);
typedef HRESULT(WINAPI* pfnDirectInputDllCanUnloadNow)(void);
typedef HRESULT(WINAPI* pfnDirectInputDllGetClassObject)(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
typedef LPCDIDATAFORMAT(WINAPI* pfnDirectInputGetdfDIJoystick)();

// DINPUT FUNCTION DEFINITIONS
typedef HRESULT(WINAPI* pfnDirectInputDirectInputCreateA)(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUTA* ppDI, LPUNKNOWN punkOuter);
typedef HRESULT(WINAPI* pfnDirectInputDirectInputCreateW)(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUTW* ppDI, LPUNKNOWN punkOuter);
typedef HRESULT(WINAPI* pfnDirectInputDirectInputCreateEx)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);

// DINPUT8 REAL FUNCTIONS
pfnDirectInputDirectInput8Create originalDirectInputDirectInput8Create = NULL;
pfnDirectInputDllRegisterServer originalDirectInputDllRegisterServer = NULL;
pfnDirectInputDllUnregisterServer originalDirectInputDllUnregisterServer = NULL;
pfnDirectInputDllCanUnloadNow originalDirectInputDllCanUnloadNow = NULL;
pfnDirectInputDllGetClassObject originalDirectInputDllGetClassObject = NULL;
pfnDirectInputGetdfDIJoystick originalGetdfDIJoystick = NULL;

// DINPUT REAL FUNCTIONS
pfnDirectInputDirectInputCreateA originalDirectInputDirectInputCreateA = NULL;
pfnDirectInputDirectInputCreateW originalDirectInputDirectInputCreateW = NULL;
pfnDirectInputDirectInputCreateEx originalDirectInputDirectInputCreateEx = NULL;

// REAL FUNCTIONS OF THE OTHER WRAPPED LIBRARIES (called by DLLWrapper.asm)
#define DECLARE_ORIGINAL(name) FARPROC original##name;
#define DECLARE_ORIGINAL_BY_ORDINAL(name, ordinal) FARPROC original##name;
extern "C"
{
	D3D11_EXPORTS(DECLARE_ORIGINAL)
	XINPUT_EXPORTS(DECLARE_ORIGINAL)
	XINPUT_ORDINAL_EXPORTS(DECLARE_ORIGINAL_BY_ORDINAL)
	D3D9_EXPORTS(DECLARE_ORIGINAL)
	WINMM_EXPORTS(DECLARE_ORIGINAL)
	OPENGL32_EXPORTS(DECLARE_ORIGINAL)
}

// SUPPORT FOR WRAPPING DIFFERENT LIBRARIES

enum lib
{
	unknown = 0,
	dinput8,
	d3d11,
	xinput1_3,
	d3d9,
	opengl32,
	winmm,
	dinput
};

lib currentLibrary = lib::unknown;

// DINPUT WRAPPER
HRESULT WINAPI DirectInputDirectInputCreateA(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUTA* ppDI, LPUNKNOWN punkOuter)
{
	return originalDirectInputDirectInputCreateA(hinst, dwVersion, ppDI, punkOuter);
}

HRESULT WINAPI DirectInputDirectInputCreateW(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUTW* ppDI, LPUNKNOWN punkOuter)
{
	return originalDirectInputDirectInputCreateW(hinst, dwVersion, ppDI, punkOuter);
}

HRESULT WINAPI DirectInputDirectInputCreateEx(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
	return originalDirectInputDirectInputCreateEx(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

// DINPUT8 WRAPPER
HRESULT WINAPI DirectInputDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
	LPVOID val;
	HRESULT res = originalDirectInputDirectInput8Create(hinst, dwVersion, riidltf, &val, punkOuter);
	*ppvOut = new DirectInputDeviceWrapper(val, (IID_IDirectInput8W == riidltf));
	return res;
}

HRESULT WINAPI DirectInputDllRegisterServer(void)
{
	return originalDirectInputDllRegisterServer();
}

HRESULT WINAPI DirectInputDllUnregisterServer(void)
{
	return originalDirectInputDllUnregisterServer();
}

HRESULT WINAPI DirectInputDllCanUnloadNow(void)
{
	return originalDirectInputDllCanUnloadNow();
}

HRESULT WINAPI DirectInputDllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return originalDirectInputDllGetClassObject(rclsid, riid, ppv);
}

LPCDIDATAFORMAT WINAPI DirectInputGetdfDIJoystick()
{
	return originalGetdfDIJoystick();
}

void MEMwrite(void* adr, void* ptr, int size)
{
	DWORD OldProtection;
	VirtualProtect(adr, size, PAGE_EXECUTE_READWRITE, &OldProtection);
	memcpy(adr, ptr, size);
	VirtualProtect(adr, size, OldProtection, &OldProtection);
}

extern DWORD WINAPI ThreadForOutputs(LPVOID lpParam);

// global variables 
SDL_Haptic* haptic;
SDL_Haptic* haptic2 = NULL;
SDL_Haptic* haptic3 = NULL;
EffectCollection effects;
EffectConstants effectConst;
Helpers hlp;
EffectTriggers t;

bool CustomStrength = false;
bool WaitForGame = false;
bool keepRunning = true;

SDL_Joystick* GameController = NULL;
SDL_Haptic* ControllerHaptic = NULL;
SDL_Joystick* GameController2 = NULL;
SDL_Haptic* ControllerHaptic2 = NULL;
SDL_Joystick* GameController3 = NULL;
SDL_Haptic* ControllerHaptic3 = NULL;
HINSTANCE gl_hOriginalDll = NULL;
HINSTANCE gl_hjgtDll = NULL;
HINSTANCE gl_cgGLDll = NULL;
HINSTANCE gl_hlibavs = NULL;
int joystick_index1;
int joystick1Index = -1;
int joystick_index2 = -1;
int joystick_index3 = -1;
static DWORD GameProcessID;

LPCSTR CustomAlternativeMaxForceLeft;
LPCSTR CustomAlternativeMaxForceRight;
LPCSTR CustomMaxForce;

// settings
static wchar_t* settingsFilename = TEXT(".\\FFBPlugin.ini");
int configMinForce = GetSettingInt(TEXT("MinForce"), 0);
int configMaxForce = GetSettingInt(TEXT("MaxForce"), 100);
int enableLogging = GetSettingInt(TEXT("Logging"), 0);
int PowerMode = GetSettingInt(TEXT("PowerMode"), 0);
int EnableRumble = GetSettingInt(TEXT("EnableRumble"), 0);
int EnableRumbleTriggers = GetSettingInt(TEXT("EnableRumbleTriggers"), 0);
int ReverseRumble = GetSettingInt(TEXT("ReverseRumble"), 0);
int SingleRumbleMotor = GetSettingInt(TEXT("SingleRumbleMotor"), 0);
int EnableDamper = GetSettingInt(TEXT("EnableDamper"), 0);
int DamperStrength = GetSettingInt(TEXT("DamperStrength"), 100);
static wchar_t deviceGUIDString[256];
int DeviceGUID = GetPrivateProfileString(TEXT("Settings"), TEXT("DeviceGUID"), NULL, deviceGUIDString, 256, settingsFilename);
int configFeedbackLength = GetSettingInt(TEXT("FeedbackLength"), 120);
int configGameId = GetSettingInt(TEXT("GameId"), 0);
int configDefaultCentering = GetSettingInt(TEXT("DefaultCentering"), 0);
int configDefaultFriction = GetSettingInt(TEXT("DefaultFriction"), 0);
int BeepWhenHook = GetSettingInt(TEXT("BeepWhenHook"), 0);
int AlternativeFFB = GetSettingInt(TEXT("AlternativeFFB"), 0);
int configAlternativeMinForceLeft = GetSettingInt(TEXT("AlternativeMinForceLeft"), 0);
int configAlternativeMaxForceLeft = GetSettingInt(TEXT("AlternativeMaxForceLeft"), 100);
int configAlternativeMinForceRight = GetSettingInt(TEXT("AlternativeMinForceRight"), 0);
int configAlternativeMaxForceRight = GetSettingInt(TEXT("AlternativeMaxForceRight"), 100);
int ForceShowDeviceGUIDMessageBox = GetSettingInt(TEXT("ForceShowDeviceGUIDMessageBox"), 0);
int configMinForceDevice2 = GetSettingInt(TEXT("MinForceDevice2"), 0);
int configMaxForceDevice2 = GetSettingInt(TEXT("MaxForceDevice2"), 100);
int EnableRumbleDevice2 = GetSettingInt(TEXT("EnableRumbleDevice2"), 0);
int ReverseRumbleDevice2 = GetSettingInt(TEXT("ReverseRumbleDevice2"), 0);
int AlternativeFFBDevice2 = GetSettingInt(TEXT("AlternativeFFBDevice2"), 0);
int configAlternativeMinForceLeftDevice2 = GetSettingInt(TEXT("AlternativeMinForceLeftDevice2"), 0);
int configAlternativeMaxForceLeftDevice2 = GetSettingInt(TEXT("AlternativeMaxForceLeftDevice2"), 100);
int configAlternativeMinForceRightDevice2 = GetSettingInt(TEXT("AlternativeMinForceRightDevice2"), 0);
int configAlternativeMaxForceRightDevice2 = GetSettingInt(TEXT("AlternativeMaxForceRightDevice2"), 100);
int configMinForceDevice3 = GetSettingInt(TEXT("MinForceDevice3"), 0);
int configMaxForceDevice3 = GetSettingInt(TEXT("MaxForceDevice3"), 100);
int EnableRumbleDevice3 = GetSettingInt(TEXT("EnableRumbleDevice3"), 0);
int ReverseRumbleDevice3 = GetSettingInt(TEXT("ReverseRumbleDevice3"), 0);
int AlternativeFFBDevice3 = GetSettingInt(TEXT("AlternativeFFBDevice3"), 0);
int configAlternativeMinForceLeftDevice3 = GetSettingInt(TEXT("AlternativeMinForceLeftDevice3"), 0);
int configAlternativeMaxForceLeftDevice3 = GetSettingInt(TEXT("AlternativeMaxForceLeftDevice3"), 100);
int configAlternativeMinForceRightDevice3 = GetSettingInt(TEXT("AlternativeMinForceRightDevice3"), 0);
int configAlternativeMaxForceRightDevice3 = GetSettingInt(TEXT("AlternativeMaxForceRightDevice3"), 100);
int SinePeriod = GetSettingInt(TEXT("SinePeriod"), 0);
int SineFadePeriod = GetSettingInt(TEXT("SineFadePeriod"), 0);
int SineStrength = GetSettingInt(TEXT("SineStrength"), 0);
int RumbleStrengthLeftMotor = GetSettingInt(TEXT("RumbleStrengthLeftMotor"), 0);
int RumbleStrengthRightMotor = GetSettingInt(TEXT("RumbleStrengthRightMotor"), 0);
int EnableForceSpringEffect = GetSettingInt(TEXT("EnableForceSpringEffect"), 0);
int ForceSpringStrength = GetSettingInt(TEXT("ForceSpringStrength"), 0);
int AutoCloseWindowError = GetSettingInt(TEXT("AutoCloseWindowError"), 0);
int EnableFFBStrengthDynamicAdjustment = GetSettingInt(TEXT("EnableFFBStrengthDynamicAdjustment"), 0);
int IncreaseFFBStrength = GetSettingInt(TEXT("IncreaseFFBStrength"), NULL);
int DecreaseFFBStrength = GetSettingInt(TEXT("DecreaseFFBStrength"), NULL);
int ResetFFBStrength = GetSettingInt(TEXT("ResetFFBStrength"), NULL);
int StepFFBStrength = GetSettingInt(TEXT("StepFFBStrength"), 5);
int EnableFFBStrengthPersistence = GetSettingInt(TEXT("EnableFFBStrengthPersistence"), 0);
int EnableFFBStrengthTextToSpeech = GetSettingInt(TEXT("EnableFFBStrengthTextToSpeech"), 0);
int InputDeviceWheelEnable = GetSettingInt(TEXT("InputDeviceWheelEnable"), 0);
int IgnoreFirstMatchingGUID = GetSettingInt(TEXT("IgnoreFirstMatchingGUID"), 0);
int DoubleSine = GetSettingInt(TEXT("DoubleSine"), 0);
int DoubleConstant = GetSettingInt(TEXT("DoubleConstant"), 0);
int DoubleSpring = GetSettingInt(TEXT("DoubleSpring"), 0);
int DoubleFriction = GetSettingInt(TEXT("DoubleFriction"), 0);
int FFBOrRumble = GetSettingInt(TEXT("FFBOrRumble"), 0);
int StartDelay = GetSettingInt(TEXT("StartDelay"), 0);

char chainedDLL[256];

SDL_Event e;
CComPtr<ISpVoice> cpVoice;

// Uploads an effect of the given type, its parameters being updated each time it is triggered.
static int CreateBaseEffect(SDL_Haptic* device, Uint16 type, Uint32 length_ms)
{
	SDL_HapticEffect effect;
	SDL_memset(&effect, 0, sizeof(SDL_HapticEffect));
	effect.type = type;
	effect.condition.direction.type = SDL_HAPTIC_CARTESIAN;
	effect.condition.length = length_ms;
	return SDL_HapticNewEffect(device, &effect);
}

void Initialize()
{
	hlp.log("in initialize");
	SDL_SetHint(SDL_HINT_JOYSTICK_RAWINPUT, "0");
	if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER) < 0)
		SDL_JoystickEventState(SDL_ENABLE);
	SDL_JoystickUpdate();
	char joystick_guid[256];
	sprintf(joystick_guid, "%S", deviceGUIDString);
	SDL_JoystickGUID guid = SDL_JoystickGetGUIDFromString(joystick_guid);
	hlp.log("numJoysticks = ");
	hlp.logInt(SDL_NumJoysticks());
	char firstJoystickSelectedText[256];
	SDL_Joystick* FirstGameController = NULL;
	for (int i = 0; i < SDL_NumJoysticks(); i++)
	{
		SDL_Joystick* js = SDL_JoystickOpen(i);
		const char* name = SDL_JoystickName(js);
		SDL_JoystickGUID dev_guid = SDL_JoystickGetGUID(js);
		char guid_str[1024];
		SDL_JoystickGetGUIDString(dev_guid, guid_str, sizeof(guid_str));
		if (ForceShowDeviceGUIDMessageBox == 1)
		{
			MessageBoxA(NULL, name, "", 0);
			MessageBoxA(NULL, guid_str, "", 0);
		}
		char text[256];
		sprintf(text, "Joystick: %d / Name: %s / GUID: %s\n", i, name, guid_str);
		hlp.log(text);
		SDL_JoystickClose(js);
		if (!memcmp(&guid, &dev_guid, sizeof(SDL_JoystickGUID)))
		{
			if (!IgnoreFirstMatchingGUID)
			{
				joystick1Index = i;
				GameController = SDL_JoystickOpen(i);
				joystick_index1 = SDL_JoystickInstanceID(GameController);
				ControllerHaptic = SDL_HapticOpenFromJoystick(GameController);

				if (FirstGameController == NULL)
				{
					sprintf(firstJoystickSelectedText, "No haptic joystick found, selecting first joystick matching the GUID: %d / Name: %s / GUID: %s\n", i, name, guid_str);
					FirstGameController = GameController;
				}

				if (ControllerHaptic != NULL)
				{
					sprintf(text, "Haptic joystick found: %d / Name: %s / GUID: %s\n", i, name, guid_str);
					hlp.log(text);
					break;
				}
			}
			IgnoreFirstMatchingGUID = 0;
		}
	}

	if (ControllerHaptic == NULL && FirstGameController != NULL) // If no haptic controller has been found, we select the first controller matching the guid.
	{
		GameController = FirstGameController;
		hlp.log(firstJoystickSelectedText);
	}
	haptic = ControllerHaptic;
	if ((SDL_HapticRumbleSupported(haptic) == SDL_TRUE && EnableRumble))
	{
		SDL_HapticRumbleInit(ControllerHaptic);
		hlp.log("Rumble Init");
	}
	hlp.log("disabling all current FFB effects");
	SDL_HapticStopAll(haptic);
	hlp.log("setting haptic gain to 100");
	SDL_HapticSetGain(haptic, 100); // HapticSetGain should be between 0 and 100 as per https://wiki.libsdl.org/SDL_HapticSetGain
	hlp.log("setting haptic auto center to 0");
	SDL_HapticSetAutocenter(haptic, 0); // 0 disables autocenter https://wiki.libsdl.org/SDL_HapticSetAutocenter

	hlp.log("creating base effects...");

	SDL_HapticEffect tempEffect;
	SDL_memset(&tempEffect, 0, sizeof(SDL_HapticEffect));
	tempEffect.type = SDL_HAPTIC_CONSTANT;
	tempEffect.constant.direction.type = SDL_HAPTIC_CARTESIAN;
	tempEffect.constant.direction.dir[0] = -1; //left => right is +1 as per this 2d array explanation: https://wiki.libsdl.org/SDL_HapticDirection
	tempEffect.constant.length = configFeedbackLength; // presumably is ms, but is not documented
	tempEffect.constant.delay = 0;
	tempEffect.constant.level = 9999; // this is an sint16 => -32768 to 32767
	effects.effect_constant_id = SDL_HapticNewEffect(haptic, &tempEffect); // Upload the effect

	effects.effect_friction_id = CreateBaseEffect(haptic, SDL_HAPTIC_FRICTION, 5000);
	effects.effect_sine_id = CreateBaseEffect(haptic, SDL_HAPTIC_SINE, 0);
	effects.effect_spring_id = CreateBaseEffect(haptic, SDL_HAPTIC_SPRING, 5000);
	effects.effect_inertia_id = CreateBaseEffect(haptic, SDL_HAPTIC_INERTIA, 5000);
	effects.effect_damper_id = CreateBaseEffect(haptic, SDL_HAPTIC_DAMPER, 5000);
	effects.effect_triangle_id = CreateBaseEffect(haptic, SDL_HAPTIC_TRIANGLE, 5000);
	effects.effect_ramp_id = CreateBaseEffect(haptic, SDL_HAPTIC_RAMP, 5000);
	effects.effect_sawtoothup_id = CreateBaseEffect(haptic, SDL_HAPTIC_SAWTOOTHUP, 5000);
	effects.effect_sawtoothdown_id = CreateBaseEffect(haptic, SDL_HAPTIC_SAWTOOTHDOWN, 5000);

	if (haptic2 != NULL)
		effects.effect_sine_id_device2 = CreateBaseEffect(haptic2, SDL_HAPTIC_SINE, 0);

	if (haptic3 != NULL)
		effects.effect_sine_id_device3 = CreateBaseEffect(haptic3, SDL_HAPTIC_SINE, 0);
}

using namespace std::chrono;

// FORCE COMPUTATION

// Min and max force (in % of the device force) applied to an effect
struct ForceLimits
{
	int minForce;
	int maxForce;
};

// AlternativeFFB is for wheels always turning the same way: they get separate limits for each direction.
static ForceLimits SelectForceLimits(int direction, int alternativeFFB, ForceLimits limits, ForceLimits leftLimits, ForceLimits rightLimits)
{
	if (!alternativeFFB)
		return limits;
	return direction == -1 ? leftLimits : rightLimits;
}

static ForceLimits DefaultForceLimits()
{
	return { configMinForce, configMaxForce };
}

static ForceLimits ForceLimitsDevice1(int direction)
{
	return SelectForceLimits(direction, AlternativeFFB, DefaultForceLimits(),
		{ configAlternativeMinForceLeft, configAlternativeMaxForceLeft },
		{ configAlternativeMinForceRight, configAlternativeMaxForceRight });
}

static ForceLimits ForceLimitsDevice2(int direction)
{
	return SelectForceLimits(direction, AlternativeFFBDevice2, { configMinForceDevice2, configMaxForceDevice2 },
		{ configAlternativeMinForceLeftDevice2, configAlternativeMaxForceLeftDevice2 },
		{ configAlternativeMinForceRightDevice2, configAlternativeMaxForceRightDevice2 });
}

static ForceLimits ForceLimitsDevice3(int direction)
{
	return SelectForceLimits(direction, AlternativeFFBDevice3, { configMinForceDevice3, configMaxForceDevice3 },
		{ configAlternativeMinForceLeftDevice3, configAlternativeMaxForceLeftDevice3 },
		{ configAlternativeMinForceRightDevice3, configAlternativeMaxForceRightDevice3 });
}

// Maps a strength between 0 and 1 onto [minForce, maxForce] of the haptic range.
static SHORT ScaleForce(double strength, ForceLimits limits, SHORT& range)
{
	SHORT minForce = (SHORT)(strength > 0.001 ? (limits.minForce / 100.0 * 32767.0) : 0); // strength is a double so we do an epsilon check of 0.001 instead of > 0.
	SHORT maxForce = (SHORT)(limits.maxForce / 100.0 * 32767.0);
	range = maxForce - minForce;
	return (SHORT)(strength * range + minForce);
}

// Level of a directional effect (constant, periodic): an overflowing level saturates in the direction of the range.
static SHORT ScaleDirectionalForce(double strength, ForceLimits limits)
{
	SHORT range;
	SHORT level = ScaleForce(strength, limits, range);

	if (range > 0 && level < 0)
		level = 32767;
	else if (range < 0 && level > 0)
		level = -32767;

	return level;
}

// Coefficient of a condition effect (spring, friction...): an overflowing coefficient saturates to the maximum.
static SHORT ScaleCoefficient(double strength, ForceLimits limits)
{
	SHORT range;
	SHORT coeff = ScaleForce(strength, limits, range);
	return coeff < 0 ? 32767 : coeff;
}

// Strength used when a DoubleXxx setting is enabled
static double DoubleStrength(double strength)
{
	strength = strength * 2.0;
	return strength > 1.0 ? 1.0 : strength;
}

// FFB EFFECTS

static void RunEffect(SDL_Haptic* device, int effectId, SDL_HapticEffect& effect)
{
	SDL_HapticUpdateEffect(device, effectId, &effect);
	SDL_HapticRunEffect(device, effectId, 1);
}

static SDL_HapticEffect ConditionEffect(Uint16 type, Uint32 length_ms, SHORT coeff)
{
	SDL_HapticEffect effect;
	SDL_memset(&effect, 0, sizeof(SDL_HapticEffect));
	effect.type = type;
	effect.condition.type = type;
	effect.condition.direction.type = SDL_HAPTIC_CARTESIAN;
	effect.condition.delay = 0;
	effect.condition.length = length_ms;
	effect.condition.left_coeff[0] = coeff;
	effect.condition.right_coeff[0] = coeff;
	return effect;
}

static void RunPeriodicEffect(Uint16 type, int effectId, int direction, SHORT magnitude, UINT32 length_ms)
{
	SDL_HapticEffect effect;
	SDL_memset(&effect, 0, sizeof(SDL_HapticEffect));
	effect.type = type;
	effect.periodic.type = type;
	effect.periodic.direction.type = SDL_HAPTIC_CARTESIAN;
	effect.periodic.direction.dir[0] = direction;
	effect.periodic.direction.dir[1] = 0; //Y Position
	effect.periodic.period = 500;
	effect.periodic.magnitude = magnitude;
	effect.periodic.length = length_ms;
	effect.periodic.attack_length = 1000;
	effect.periodic.fade_length = 1000;
	RunEffect(haptic, effectId, effect);
}

void TriggerConstantEffect(int direction, double strength)
{
	ForceLimits limits = ForceLimitsDevice1(direction);

	if (PowerMode)
		strength = pow(strength, 0.5);

	if (DoubleConstant)
		strength = DoubleStrength(strength);

	SDL_HapticEffect tempEffect;
	SDL_memset(&tempEffect, 0, sizeof(SDL_HapticEffect));
	tempEffect.type = SDL_HAPTIC_CONSTANT;
	tempEffect.constant.direction.type = SDL_HAPTIC_CARTESIAN;
	tempEffect.constant.direction.dir[0] = direction;
	tempEffect.constant.length = configFeedbackLength;
	tempEffect.constant.delay = 0;
	tempEffect.constant.level = ScaleDirectionalForce(strength, limits);
	RunEffect(haptic, effects.effect_constant_id, tempEffect);
}

void TriggerFrictionEffectWithDefaultOption(double strength, bool isDefault)
{
	if (DoubleFriction)
		strength = DoubleStrength(strength);

	SDL_HapticEffect tempEffect = ConditionEffect(SDL_HAPTIC_FRICTION, isDefault ? SDL_HAPTIC_INFINITY : configFeedbackLength, ScaleCoefficient(strength, DefaultForceLimits()));
	tempEffect.condition.left_sat[0] = 0xFFFF;
	tempEffect.condition.right_sat[0] = 0xFFFF;
	RunEffect(haptic, effects.effect_friction_id, tempEffect);
}

void TriggerFrictionEffect(double strength)
{
	TriggerFrictionEffectWithDefaultOption(strength, false);
}

void TriggerInertiaEffect(double strength)
{
	SHORT coeff = ScaleCoefficient(strength, DefaultForceLimits());
	SDL_HapticEffect tempEffect = ConditionEffect(SDL_HAPTIC_INERTIA, configFeedbackLength, coeff);
	tempEffect.condition.direction.dir[0] = 1;
	tempEffect.condition.direction.dir[1] = 1; //Y Position
	tempEffect.condition.left_sat[0] = coeff * 10;
	tempEffect.condition.right_sat[0] = coeff * 10;
	tempEffect.condition.center[0] = 0;
	RunEffect(haptic, effects.effect_inertia_id, tempEffect);
}

void TriggerDamperEffect(double strength)
{
	SHORT coeff = ScaleCoefficient(strength, DefaultForceLimits());
	SDL_HapticEffect tempEffect = ConditionEffect(SDL_HAPTIC_DAMPER, configFeedbackLength, coeff);
	tempEffect.condition.direction.dir[0] = 1; // not used
	tempEffect.condition.direction.dir[1] = 0; //Y Position
	tempEffect.condition.left_sat[0] = (DWORD)(coeff) * 2;
	tempEffect.condition.right_sat[0] = (DWORD)(coeff) * 2;
	RunEffect(haptic, effects.effect_damper_id, tempEffect);
}

static void RunSpringEffect(SHORT coeff, Uint32 length_ms, Sint32 directionY)
{
	SDL_HapticEffect tempEffect = ConditionEffect(SDL_HAPTIC_SPRING, length_ms, coeff);
	tempEffect.condition.direction.dir[0] = 1;
	tempEffect.condition.direction.dir[1] = directionY; //Y Position
	tempEffect.condition.left_sat[0] = (short)(coeff * 2.0); //Needed for Logitech G920 wheel
	tempEffect.condition.right_sat[0] = (short)(coeff * 2.0); //Needed for Logitech G920 wheel
	tempEffect.condition.center[0] = 0;
	RunEffect(haptic, effects.effect_spring_id, tempEffect);
}

void TriggerSpringEffectWithDefaultOption(double strength, bool isDefault)
{
	if (DoubleSpring)
		strength = DoubleStrength(strength);

	RunSpringEffect(ScaleCoefficient(strength, DefaultForceLimits()), isDefault ? SDL_HAPTIC_INFINITY : configFeedbackLength, 0);
}

void TriggerSpringEffect(double strength)
{
	TriggerSpringEffectWithDefaultOption(strength, false);
}

void TriggerSpringEffectInfinite(double strength)
{
	ForceLimits limits = EnableForceSpringEffect ? ForceLimits{ 0, ForceSpringStrength } : DefaultForceLimits();
	RunSpringEffect(ScaleCoefficient(strength, limits), SDL_HAPTIC_INFINITY, 1);
}

void TriggerTriangleEffect(double strength, UINT32 length_ms)
{
	int direction = 1;
	if (strength < -0.001) {
		strength *= -1;
		direction = -1;
	}

	RunPeriodicEffect(SDL_HAPTIC_TRIANGLE, effects.effect_triangle_id, direction, ScaleDirectionalForce(strength, ForceLimitsDevice1(direction)), length_ms);
}

void TriggerSawtoothUpEffect(double strength, UINT32 length_ms)
{
	RunPeriodicEffect(SDL_HAPTIC_SAWTOOTHUP, effects.effect_sawtoothup_id, 0, ScaleCoefficient(strength, DefaultForceLimits()), length_ms);
}

void TriggerSawtoothDownEffect(double strength, UINT32 length_ms)
{
	RunPeriodicEffect(SDL_HAPTIC_SAWTOOTHDOWN, effects.effect_sawtoothdown_id, 0, ScaleCoefficient(strength, DefaultForceLimits()), length_ms);
}

void TriggerRampEffect(double start, double end, UINT32 length_ms)
{
	SDL_HapticEffect tempEffect;
	SDL_memset(&tempEffect, 0, sizeof(SDL_HapticEffect));
	tempEffect.type = SDL_HAPTIC_RAMP;
	tempEffect.ramp.type = SDL_HAPTIC_RAMP;
	tempEffect.ramp.direction.type = SDL_HAPTIC_CARTESIAN;
	tempEffect.ramp.length = length_ms;
	tempEffect.ramp.delay = 0;
	tempEffect.ramp.start = ScaleCoefficient(start, DefaultForceLimits());
	tempEffect.ramp.end = -ScaleCoefficient(end, DefaultForceLimits());
	RunEffect(haptic, effects.effect_ramp_id, tempEffect);
}

// Last sine effect played on a device
struct SineEffectState
{
	milliseconds time = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	double strength = 0;
	double length_ms = 0;
};

static SineEffectState lastSineEffect;
static SineEffectState lastSineEffectDevice2;
static SineEffectState lastSineEffectDevice3;

static void RunSineEffect(SDL_Haptic* device, int effectId, SineEffectState& lastEffect, ForceLimits (*forceLimits)(int direction), bool doubleStrength, const char* logMessage,
	UINT16 period_ms, UINT16 fadePeriod_ms, double strength, UINT32 length_ms)
{
	milliseconds now = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	long long elapsedTime = (now - lastEffect.time).count();

	int direction = 1;
	if (strength < -0.001) {
		strength *= -1;
		direction = -1;
	}

	// if no strength, we do nothing
	if (strength <= 0.001)
		return;

	// we ignore the new effect until the last one is completed, unless the new one is significantly stronger
	if (elapsedTime < lastEffect.length_ms && strength < (lastEffect.strength * 1.5))
		return;

	hlp.log(logMessage);
	ForceLimits limits = forceLimits(direction);

	if (doubleStrength)
		strength = DoubleStrength(strength);

	SDL_HapticEffect tempEffect;
	SDL_memset(&tempEffect, 0, sizeof(SDL_HapticEffect));
	tempEffect.type = SDL_HAPTIC_SINE;
	tempEffect.periodic.direction.type = SDL_HAPTIC_CARTESIAN;
	tempEffect.periodic.direction.dir[0] = direction;
	tempEffect.periodic.direction.dir[1] = 0; //Y Position
	tempEffect.periodic.period = period_ms;
	tempEffect.periodic.magnitude = ScaleDirectionalForce(strength, limits);
	tempEffect.periodic.length = length_ms;
	tempEffect.periodic.attack_length = fadePeriod_ms;
	tempEffect.periodic.fade_length = fadePeriod_ms;
	RunEffect(device, effectId, tempEffect);

	lastEffect.time = now;
	lastEffect.strength = strength;
	lastEffect.length_ms = length_ms;
}

void TriggerSineEffect(UINT16 period_ms, UINT16 fadePeriod_ms, double strength, UINT32 length_ms)
{
	if (period_ms < 1)
		return;

	RunSineEffect(haptic, effects.effect_sine_id, lastSineEffect, ForceLimitsDevice1, DoubleSine, "Doing sine...", period_ms, fadePeriod_ms, strength, length_ms);
}

void TriggerSineEffectDevice2(UINT16 period_ms, UINT16 fadePeriod_ms, double strength, UINT32 length_ms)
{
	RunSineEffect(haptic2, effects.effect_sine_id_device2, lastSineEffectDevice2, ForceLimitsDevice2, false, "Doing Device2 sine...", period_ms, fadePeriod_ms, strength, length_ms);
}

void TriggerSineEffectDevice3(UINT16 period_ms, UINT16 fadePeriod_ms, double strength, UINT32 length_ms)
{
	RunSineEffect(haptic3, effects.effect_sine_id_device3, lastSineEffectDevice3, ForceLimitsDevice3, false, "Doing Device3 sine...", period_ms, fadePeriod_ms, strength, length_ms);
}

static void RunLeftRightEffect(SDL_Haptic* device, double smallstrength, double largestrength, UINT32 length_ms)
{
	if (!EnableRumble)
		return;

	if (ReverseRumble)
		std::swap(smallstrength, largestrength);

	SHORT range;
	SDL_HapticEffect tempEffect;
	SDL_memset(&tempEffect, 0, sizeof(SDL_HapticEffect));
	tempEffect.type = SDL_HAPTIC_LEFTRIGHT;
	tempEffect.leftright.length = length_ms;
	tempEffect.leftright.small_magnitude = ScaleForce(smallstrength, DefaultForceLimits(), range);
	tempEffect.leftright.large_magnitude = ScaleForce(largestrength, DefaultForceLimits(), range);
	RunEffect(device, effects.effect_leftright_id, tempEffect);
}

void TriggerLeftRightEffect(double smallstrength, double largestrength, UINT32 length_ms)
{
	RunLeftRightEffect(haptic, smallstrength, largestrength, length_ms);
}

void TriggerLeftRightDevice2Effect(double smallstrength, double largestrength, UINT32 length_ms)
{
	RunLeftRightEffect(haptic2, smallstrength, largestrength, length_ms);
}

// RUMBLE

// Maps a strength between 0 and 1 onto [MinForce, MaxForce] of the motor speed range (not clamped).
static DWORD ScaleMotorSpeed(double strength)
{
	DWORD minForce = (DWORD)(strength > 0.001 ? (configMinForce / 100.0 * 65535.0) : 0);
	DWORD maxForce = (DWORD)(configMaxForce / 100.0 * 65535.0);
	DWORD range = maxForce - minForce;
	return (DWORD)(strength * range + minForce);
}

static UINT16 ClampMotorSpeed(DWORD speed)
{
	return (UINT16)(speed > 0xFFFF ? 0xFFFF : speed);
}

// Rumble is disabled for a controller as soon as it reports it does not support it.
static void RunRumble(SDL_Joystick* controller, int& enableRumble, int reverseRumble, bool singleMotor, double highfrequency, double lowfrequency, UINT32 length_ms)
{
	if (!enableRumble)
		return;

	DWORD LowMotor = ScaleMotorSpeed(lowfrequency);
	DWORD HighMotor = ScaleMotorSpeed(highfrequency);

	if (singleMotor) // Both motors strengths are combined on the low frequency motor
	{
		double LowPercent = (LowMotor / 65535.0) * 100.0;
		double LowPercentRange = 100.0 - LowPercent;
		double HighPercent = (HighMotor / 65535.0) * 100.0;
		double Calculation = LowPercent + ((LowPercentRange * HighPercent) / 100.0);
		LowMotor = (DWORD)(65535.0 * (Calculation / 100.0));
	}

	UINT16 LowMotor16 = ClampMotorSpeed(LowMotor);
	UINT16 HighMotor16 = ClampMotorSpeed(HighMotor);

	int result = reverseRumble ? SDL_JoystickRumble(controller, HighMotor16, LowMotor16, length_ms) : SDL_JoystickRumble(controller, LowMotor16, HighMotor16, length_ms);

	if (result == -1)
		enableRumble = 0;
}

void TriggerRumbleEffect(double highfrequency, double lowfrequency, UINT32 length_ms)
{
	RunRumble(GameController, EnableRumble, ReverseRumble, SingleRumbleMotor, highfrequency, lowfrequency, length_ms);
}

void TriggerRumbleEffectDevice2(double highfrequency, double lowfrequency, UINT32 length_ms)
{
	RunRumble(GameController2, EnableRumbleDevice2, ReverseRumbleDevice2, false, highfrequency, lowfrequency, length_ms);
}

void TriggerRumbleEffectDevice3(double highfrequency, double lowfrequency, UINT32 length_ms)
{
	RunRumble(GameController3, EnableRumbleDevice3, ReverseRumbleDevice3, false, highfrequency, lowfrequency, length_ms);
}

void TriggerRumbleTriggerEffect(double lefttrigger, double righttrigger, UINT32 length_ms)
{
	if (!EnableRumbleTriggers)
		return;

	UINT16 LeftMotor16 = ClampMotorSpeed(ScaleMotorSpeed(lefttrigger));
	UINT16 RightMotor16 = ClampMotorSpeed(ScaleMotorSpeed(righttrigger));

	if (SDL_JoystickRumbleTriggers(GameController, LeftMotor16, RightMotor16, length_ms) == -1)
		EnableRumbleTriggers = 0;
}

int WorkaroundToFixRumble(void* ptr)
{
	while (SDL_WaitEvent(&e) != 0){}
	return 0;
}

// FFB STRENGTH ADJUSTMENT WITH JOYSTICK BUTTONS

void WritePersistentMaxForce()
{
	if (!EnableFFBStrengthPersistence)
		return;

	// A null key would make WritePrivateProfileString delete the whole [Settings] section
	if (AlternativeFFB)
	{
		if (CustomAlternativeMaxForceLeft && CustomAlternativeMaxForceRight)
		{
			WritePrivateProfileStringA("Settings", CustomAlternativeMaxForceLeft, std::to_string(configAlternativeMaxForceLeft).c_str(), ".\\FFBPlugin.ini");
			WritePrivateProfileStringA("Settings", CustomAlternativeMaxForceRight, std::to_string(configAlternativeMaxForceRight).c_str(), ".\\FFBPlugin.ini");
		}
	}
	else if (CustomMaxForce)
		WritePrivateProfileStringA("Settings", CustomMaxForce, std::to_string(configMaxForce).c_str(), ".\\FFBPlugin.ini");
}

// Moves maxForce by delta, unless it is out of [lowest, highest] or already at the bound it moves to.
static void StepMaxForce(int& maxForce, int delta, int lowest, int highest, bool towardHighest)
{
	bool canMove = towardHighest ? (maxForce >= lowest && maxForce < highest) : (maxForce > lowest && maxForce <= highest);

	if (canMove)
		maxForce = max(lowest, min(highest, maxForce + delta));
}

static void ChangeMaxForce(bool increase)
{
	int step = increase ? StepFFBStrength : -StepFFBStrength;

	if (AlternativeFFB)
	{
		StepMaxForce(configAlternativeMaxForceRight, step, 0, 100, increase);
		StepMaxForce(configAlternativeMaxForceLeft, -step, -100, 0, !increase); // left forces are negative
	}
	else
		StepMaxForce(configMaxForce, step, 0, 100, increase);

	WritePersistentMaxForce();
}

static void SpeakMaxForce()
{
	char text[256];
	wchar_t wideText[256];
	sprintf(text, "Max Force: %d", AlternativeFFB ? configAlternativeMaxForceRight : configMaxForce);
	mbstowcs(wideText, text, strlen(text) + 1);

	HRESULT hr = ::CoInitialize(nullptr);
	hr = cpVoice.CoCreateInstance(CLSID_SpVoice);

	if (SUCCEEDED(hr))
	{
		hr = cpVoice->SetRate(3);
		hr = cpVoice->SetOutput(NULL, TRUE);
		hr = cpVoice->Speak(wideText, SPF_PURGEBEFORESPEAK, NULL);
		::CoUninitialize();
	}
}

static void HandleFFBStrengthButton(const SDL_Event& event)
{
	if (event.type != SDL_JOYBUTTONDOWN || event.jbutton.which != joystick_index1)
		return;

	Uint8 button = event.jbutton.button;

	if (button == IncreaseFFBStrength)
		ChangeMaxForce(true);

	if (button == DecreaseFFBStrength)
		ChangeMaxForce(false);

	if (button == ResetFFBStrength)
	{
		DefaultConfigValues();
		WritePersistentMaxForce();
	}

	if (EnableFFBStrengthTextToSpeech && (button == IncreaseFFBStrength || button == DecreaseFFBStrength || button == ResetFFBStrength))
		SpeakMaxForce();
}

// Returns true once the game has started, after setting up the persistent max force keys.
static bool ReadyToAdjustFFBStrength()
{
	if (!WaitForGame)
	{
		if (configGameId != Demul_Emulator) // Demul sets WaitForGame once the game is detected
		{
			Sleep(4000);
			WaitForGame = true;
		}
		return false;
	}

	if (!CustomStrength)
	{
		CustomFFBStrengthSetup();
		CustomStrength = true;
	}
	return true;
}

// Polls SDL events itself
DWORD WINAPI AdjustFFBStrengthLoopWaitEvent(LPVOID lpParam)
{
	while (true)
	{
		if (ReadyToAdjustFFBStrength())
		{
			while (SDL_WaitEvent(&e) != 0)
				HandleFFBStrengthButton(e);
		}
		Sleep(16);
	}
}

// Reads the last SDL event polled by the game
DWORD WINAPI AdjustFFBStrengthLoopNoWaitEvent(LPVOID lpParam)
{
	while (true)
	{
		if (ReadyToAdjustFFBStrength())
			HandleFFBStrengthButton(e);
		Sleep(16);
	}
}

static DWORD MyGetProcessId(LPCTSTR ProcessName)
{
	PROCESSENTRY32 pt;
	HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pt.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hsnap, &pt)) {
		do {
			if (!lstrcmpi(pt.szExeFile, ProcessName)) {
				CloseHandle(hsnap);
				return pt.th32ProcessID;
			}
		} while (Process32Next(hsnap, &pt));
	}
	CloseHandle(hsnap);
	return 0;
}

static BOOL IsProcessRunning(DWORD pid)
{
	HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid);
	DWORD ret = WaitForSingleObject(process, 0);
	CloseHandle(process);
	return ret == WAIT_TIMEOUT;
}

// These games run their own SDL event loop (SDL_WaitEvent) in a dedicated thread
static bool GameHandlesSDLEvents()
{
	return configGameId == WMMT_5 || configGameId == OUTRUN_2Fake || configGameId == Button_Rumble || configGameId == Road_Fighters_3D || configGameId == OUTRUN_2Real;
}

// Special K disables rumble by default
static void EnableSpecialKRumble()
{
	char RumbleDisableChar[256];
	GetPrivateProfileStringA("Input.Gamepad", "DisableRumble", "", RumbleDisableChar, 256, ".\\dxgi.ini");

	if (strcmp(RumbleDisableChar, "true") == 0 || strcmp(RumbleDisableChar, "True") == 0 || strcmp(RumbleDisableChar, "TRUE") == 0)
		WritePrivateProfileStringA("Input.Gamepad", "DisableRumble", "false", ".\\dxgi.ini");
}

static Game* CreateGame(int gameId)
{
	switch (gameId) {
	case AFTER_BURNER_CLIMAX:
		return new AfterburnerClimax;
	case CHASE_HQ_2:
		return new ChaseHQ2;
	case CARS:
		return new Cars;
	case DAYTONA_3:
		return new Daytona3;
	case DAYTONA_3_NSE:
		return new Daytona3NSE;
	case MAME_:
	case SUPERMODEL_:
	case FLYCAST:
		return new MAMESupermodel;
	case FORD_RACING:
		return new FordRacing;
	case FORD_RACING_OTHER:
		return new FordRacingOther;
	case GRID_Real:
		return new GRIDReal;
	case Golden_Gun:
		return new GoldenGun;
	case Transformers_:
		return new Transformers;
	case INITIAL_D_0:
		return new InitialD0;
	case INITIAL_D_0_211:
		return new InitialD0v211;
	case INITIAL_D_0_230:
		return new InitialD0v230;
	case INITIAL_D_4:
		return new InitialD4;
	case INITIAL_D_4_Japan:
		return new InitialD4Japan;
	case INITIAL_D_5:
		return new InitialD5;
	case INITIAL_D_6:
		return new InitialD6;
	case INITIAL_D_7:
		return new InitialD7;
	case INITIAL_D_8:
		return new InitialD8;
	case SEGA_RACING_CLASSIC:
		return new SegaRacingClassic;
	case SEGA_RALLY_3:
		return new SegaRally3;
	case SEGA_RALLY_3_Other:
		return new SegaRally3Other;
	case WACKY_RACES:
		return new WackyRaces;
	case WMMT_5:
		return new WMMT5;
	case WMMT_5DX:
		return new WMMT5DX;
	case WMMT_5DXPlus:
		return new WMMT5DXPlus;
	case MARIO_KART_GPDX_CUSTOM:
		return new MarioKartGPDX100Custom;
	case OUTRUN_2Fake:
		return new OutRun2Fake;
	case OUTRUN_2Real:
		return new OutRun2Real;
	case BG4_JP:
		return new BG4JP;
	case MACH_STORM:
		return new Machstorm;
	case POKKEN_TOURNAMENT:
		return new PokkenTournament;
	case RAMBO_:
		return new Rambo;
	case MARIO_KART_GPDX_110_CUSTOM:
		return new MarioKartGPDX110Custom;
	case MARIO_KART_GPDX_118_CUSTOM:
		return new MarioKartGPDX118Custom;
	case MARIO_KART_GPDX_USA_CUSTOM:
		return new MarioKartGPDXUSACustom;
	case Sonic_Sega_AllStars_Racing:
		return new SonicSegaAllStarsRacing;
	case M2_Emulator:
		return new M2Emulator;
	case Demul_Emulator:
		return new Demul;
	case ALIENS_EXTERMINATION:
		return new AliensExtermination;
	case GTI_Club_3:
		return new GTIClub3;
	case Button_Rumble:
		return new ButtonRumble;
	case Road_Fighters_3D:
		return new RoadFighters3D;
	case LGI_3D:
		return new LGI3D;
	case LGI_:
		return new LGI;
	case KO_Drive:
		return new KODrive;
	case HOTD_4:
		return new HOTD4;
	case Dirty_Drivin:
		return new DirtyDrivin;
	case H2_Overdrive:
		return new H2Overdrive;
	case Sno_Cross:
		return new SnoCross;
	case Bat_man:
		return new Batman;
	case R_Tuned:
		return new RTuned;
	case SWDC_2018:
		return new SWDC;
	case WMMT_6:
		return new WMMT6;
	case WMMT_6_R:
		return new WMMT6R;
	case WMMT_6_RR:
		return new WMMT6RR;
	case SRG:
		return new StormRacerG;
	case D1_GP:
		return new D1GP;
	case Crazy_Taxi:
		return new CrazyTaxi;
	case WMMT_3:
		return new WMMT3;
	case DEAD_HEAT:
		return new DeadHeat;
	case DEAD_HEAT_RIDERS:
		return new DeadHeatRiders;
	case GAELCO_TUNING_RACE:
		return new GaelcoTuningRace;
	case TOKYO_COP:
		return new TokyoCop;
	case RING_RIDERS:
		return new RingRiders;
	case MARIO_KART_GPDX_118_REAL:
		return new MarioKartGPDX118Real;
	case MARIO_KART_GPDX_110_REAL:
		return new MarioKartGPDX110Real;
	case MARIO_KART_GPDX_USA_REAL:
		return new MarioKartGPDXUSAReal;
	case GRID_Custom:
		return new GRIDCustom;
	case SEGA_SHOWDOWN:
		return new Showdown;
	case SPRING_EFFECT:
		return new SpringEffect;
	case ARCTIC_THUNDER:
		return new ArcticThunder;
	case CRUISN_BLAST:
		return new CruisnBlast;
	case FNF:
		return new FnF;
	case FNF_DRIFT:
		return new FnFDrift;
	case FNF_SUPERCARS:
		return new FnFSuperCars;
	case SEGA_RACE_TV:
		return new SegaRaceTV;
	case HUMMER:
		return new Hummer;
	case HUMMER_EXTREME:
		return new HummerExtreme;
	case INITIAL_D_THEARCADE_V231:
		return new InitialDTA231;
	case WASTELAND_RACERS_2071:
		return new WasteLandRacers2071;
	case TEST_GAME_CONST:
	case TEST_GAME_FRICTION:
	case TEST_GAME_SINE:
	case TEST_GAME_SPRING:
	case TEST_GAME_HEAVY:
	case TEST_GAME_LOOSE:
	case TEST_GAME_RUMBLE:
		return new TestGame;
	default:
		return nullptr;
	}
}

DWORD WINAPI FFBLoop(LPVOID lpParam)
{
	hlp.log("In FFBLoop");

	if (StartDelay) //For games which need code to run quicker etc. Some games will crash if no sleep added
		Sleep(2500);

	Initialize();
	hlp.log("Initialize() complete");
	if (EnableRumble)
	{
		if (!EnableFFBStrengthDynamicAdjustment && !InputDeviceWheelEnable && !GameHandlesSDLEvents()) // Workaround for SDL_JoystickRumble rumble not stopping issue
			SDL_CreateThread(WorkaroundToFixRumble, "WorkaroundToFixRumble", (void*)NULL);

		EnableSpecialKRumble();
	}

	// assign FFB effects here
	t.Constant = &TriggerConstantEffect;
	t.Spring = &TriggerSpringEffect;
	t.Friction = &TriggerFrictionEffect;
	t.Sine = &TriggerSineEffect;
	t.SineDevice2 = &TriggerSineEffectDevice2;
	t.SineDevice3 = &TriggerSineEffectDevice3;
	t.Rumble = &TriggerRumbleEffect;
	t.RumbleDevice2 = &TriggerRumbleEffectDevice2;
	t.RumbleDevice3 = &TriggerRumbleEffectDevice3;
	t.RumbleTriggers = &TriggerRumbleTriggerEffect;
	t.LeftRight = &TriggerLeftRightEffect;
	t.LeftRightDevice2 = &TriggerLeftRightDevice2Effect;
	t.Springi = &TriggerSpringEffectInfinite;
	t.Damper = &TriggerDamperEffect;
	t.Inertia = &TriggerInertiaEffect;
	t.Ramp = &TriggerRampEffect;
	t.SawtoothUp = &TriggerSawtoothUpEffect;
	t.SawtoothDown = &TriggerSawtoothDownEffect;
	t.Triangle = &TriggerTriangleEffect;

	Game* game = CreateGame(configGameId);

	hlp.log("Setting DefaultCentering & DefaultFriction forces...");
	if (configDefaultCentering >= 0 && configDefaultCentering <= 100) {
		TriggerSpringEffectWithDefaultOption(configDefaultCentering / 100.0, true);
	}
	if (configDefaultFriction >= 0 && configDefaultFriction <= 100) {
		TriggerFrictionEffectWithDefaultOption(configDefaultFriction / 100.0, true);
	}

	if (EnableFFBStrengthDynamicAdjustment == 1)
	{
		// Only one thread can wait for SDL events
		if (GameHandlesSDLEvents() || (configGameId == Demul_Emulator && InputDeviceWheelEnable))
			CreateThread(NULL, 0, AdjustFFBStrengthLoopNoWaitEvent, NULL, 0, NULL);
		else
			CreateThread(NULL, 0, AdjustFFBStrengthLoopWaitEvent, NULL, 0, NULL);
	}

	hlp.log("Entering Game's FFBLoop loop");
	bool* kr = (bool*)lpParam;
	while (*kr)
	{
		if (game != nullptr)
			game->FFBLoop(&effectConst, &hlp, &t);
		Sleep(16);
	}
	hlp.log("about to exit FFBLoop");
	return 0;
}

void CreateFFBLoopThread()
{
	hlp.log("Before CreateThread");
	CreateThread(NULL, 0, FFBLoop, (LPVOID)&keepRunning, 0, NULL);
	hlp.log("After CreateThread");
}

void LeaveFFBLoopThread()
{
	hlp.log("Before LeaveFFBLoopThread");
	keepRunning = false;
	// Leave some time for thread to exit
	Sleep(32);
	hlp.log("After LeaveFFBLoopThread");
}

// System libraries this DLL can be renamed to: the original one is loaded from the system directory
struct WrappedLibrary
{
	const char* moduleName;
	lib type;
	const char* systemDll;
};

static const WrappedLibrary wrappedLibraries[] = {
	{ "DINPUT8", lib::dinput8, "\\dinput8.dll" }, // before DINPUT, which it contains
	{ "DINPUT", lib::dinput, "\\dinput.dll" },
	{ "D3D11", lib::d3d11, "\\d3d11.dll" },
	{ "XINPUT1_3", lib::xinput1_3, "\\xinput1_3.dll" },
	{ "XINPUT1_4", lib::xinput1_3, "\\xinput1_4.dll" },
	{ "OPENGL32", lib::opengl32, "\\opengl32.dll" },
	{ "D3D9", lib::d3d9, "\\d3d9.dll" },
	{ "WINMM", lib::winmm, "\\winmm.dll" },
};

static void LoadOriginalLibrary(HMODULE hModule)
{
	GetPrivateProfileStringA("Settings", "ChainLoad", "", chainedDLL, 256, ".\\FFBplugin.ini");
	OutputDebugStringA("FFBArcadePluginLoaded");

	if (0 != strlen(chainedDLL))
	{
		// e.g. ChainLoad = dinput8-old.dll (if we need to intercept another dinput8.dll file).
		hlp.log(chainedDLL);
		gl_hOriginalDll = LoadLibraryA(chainedDLL);
		return;
	}

	char buffer[MAX_PATH];
	GetSystemDirectoryA(buffer, MAX_PATH);

	char libName[256];
	GetModuleFileNameA(hModule, libName, 256);
	_strupr(libName);

	for (const WrappedLibrary& library : wrappedLibraries)
	{
		if (NULL != strstr(libName, library.moduleName))
		{
			currentLibrary = library.type;
			strcat_s(buffer, MAX_PATH, library.systemDll);
			break;
		}
	}

	hlp.log(buffer);

	gl_hOriginalDll = LoadLibraryA(buffer);

	if (configGameId == MAME_ || configGameId == SUPERMODEL_ || configGameId == FLYCAST)
		CreateThread(NULL, 0, ThreadForOutputs, NULL, 0, NULL);

	if (configGameId == SEGA_RALLY_3_Other)
		MEMwrite((void*)(0x57B2F0), (void*)"\x33\xC0\x40\xC3", 4);

	if (configGameId == Road_Fighters_3D)
	{
		gl_hjgtDll = LoadLibraryA("jgt.dll");
		gl_hlibavs = LoadLibraryA("libavs-win32-ea3.dll");
	}

	if (configGameId == HOTD_4 || configGameId == RAMBO_)
		gl_cgGLDll = LoadLibraryA("cgGL.dll");
}

#define LOAD_ORIGINAL(name) original##name = GetProcAddress(gl_hOriginalDll, #name);
#define LOAD_ORIGINAL_BY_ORDINAL(name, ordinal) original##name = GetProcAddress(gl_hOriginalDll, MAKEINTRESOURCEA(ordinal));

static void LoadOriginalFunctions()
{
	switch (currentLibrary)
	{
	case lib::dinput8:
		originalDirectInputDirectInput8Create = (pfnDirectInputDirectInput8Create)GetProcAddress(gl_hOriginalDll, "DirectInput8Create");
		originalDirectInputDllRegisterServer = (pfnDirectInputDllRegisterServer)GetProcAddress(gl_hOriginalDll, "DllRegisterServer");
		originalDirectInputDllUnregisterServer = (pfnDirectInputDllUnregisterServer)GetProcAddress(gl_hOriginalDll, "DllUnregisterServer");
		originalDirectInputDllCanUnloadNow = (pfnDirectInputDllCanUnloadNow)GetProcAddress(gl_hOriginalDll, "DllCanUnloadNow");
		originalDirectInputDllGetClassObject = (pfnDirectInputDllGetClassObject)GetProcAddress(gl_hOriginalDll, "DllGetClassObject");
		originalGetdfDIJoystick = (pfnDirectInputGetdfDIJoystick)GetProcAddress(gl_hOriginalDll, "GetdfDIJoystick");
		break;
	case lib::dinput:
		originalDirectInputDllRegisterServer = (pfnDirectInputDllRegisterServer)GetProcAddress(gl_hOriginalDll, "DllRegisterServer");
		originalDirectInputDllUnregisterServer = (pfnDirectInputDllUnregisterServer)GetProcAddress(gl_hOriginalDll, "DllUnregisterServer");
		originalDirectInputDllCanUnloadNow = (pfnDirectInputDllCanUnloadNow)GetProcAddress(gl_hOriginalDll, "DllCanUnloadNow");
		originalDirectInputDllGetClassObject = (pfnDirectInputDllGetClassObject)GetProcAddress(gl_hOriginalDll, "DllGetClassObject");
		originalDirectInputDirectInputCreateA = (pfnDirectInputDirectInputCreateA)GetProcAddress(gl_hOriginalDll, "DirectInputCreateA");
		originalDirectInputDirectInputCreateW = (pfnDirectInputDirectInputCreateW)GetProcAddress(gl_hOriginalDll, "DirectInputCreateW");
		originalDirectInputDirectInputCreateEx = (pfnDirectInputDirectInputCreateEx)GetProcAddress(gl_hOriginalDll, "DirectInputCreateEx");
		break;
	case lib::d3d11:
		D3D11_EXPORTS(LOAD_ORIGINAL)
		break;
	case lib::xinput1_3:
		XINPUT_EXPORTS(LOAD_ORIGINAL)
		XINPUT_ORDINAL_EXPORTS(LOAD_ORIGINAL_BY_ORDINAL)
		break;
	case lib::d3d9:
		D3D9_EXPORTS(LOAD_ORIGINAL)
		break;
	case lib::opengl32:
		OPENGL32_EXPORTS(LOAD_ORIGINAL)
		break;
	case lib::winmm:
		WINMM_EXPORTS(LOAD_ORIGINAL)
		break;
	}
}

static void CloseHaptic(SDL_Haptic* device)
{
	if (device)
	{
		SDL_HapticStopAll(device);
		SDL_HapticClose(device);
	}
}

static void CloseController(SDL_Joystick* controller, int stopRumble, int stopRumbleTriggers)
{
	if (controller)
	{
		if (stopRumble)
			SDL_JoystickRumble(controller, 0, 0, 0);

		if (stopRumbleTriggers)
			SDL_JoystickRumbleTriggers(controller, 0, 0, 0);

		SDL_JoystickClose(controller);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ulReasonForCall, LPVOID lpReserved)
{
	BOOL result = TRUE;
	hlp.enableLogging = enableLogging;
	char buff[MAX_PATH];
	hlp.log("DLLMAIN ENTERED");
	GetModuleFileNameA(NULL, buff, MAX_PATH);
	std::string processName = std::string(buff);
	hlp.log("process name:");
	hlp.log(processName.c_str());
	hlp.log("default centering & friction values:");
	hlp.logInt(configDefaultCentering);
	hlp.logInt(configDefaultFriction);

	switch (ulReasonForCall)
	{
	case DLL_PROCESS_ATTACH:
		// log, etc.
		hlp.log("dll process attach:");
		hlp.log(processName.c_str());
		hlp.logInt(configMinForce);
		hlp.logInt(configMaxForce);
		DisableThreadLibraryCalls(hModule);
		hlp.log("loading original library...");

		LoadOriginalLibrary(hModule);

		if (!gl_hOriginalDll)
		{
			hlp.log("library load process failed");
			ExitProcess(0);
		}
		else {
			hlp.log("library loaded");
		}

		LoadOriginalFunctions();

		if (configGameId == FLYCAST)
		{
			DWORD ProcessID = GetSettingInt(TEXT("ProcessID"), 0);

			if (IsProcessRunning(ProcessID))
				break;

			GameProcessID = MyGetProcessId(L"flycast.exe");

			if (GameProcessID)
				WritePrivateProfileStringA("Settings", "ProcessID", std::to_string(GameProcessID).c_str(), ".\\FFBPlugin.ini");
		}

		if (processName.find("FFBPluginGUI.exe") != std::string::npos)
		{
			hlp.log("hooked FFBPluginGUI.exe, aborting");
			break;
		}

		SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);

		hlp.log("creating ffb loop thread...");
		CreateFFBLoopThread();
		if (BeepWhenHook)
			MessageBeep(MB_ICONASTERISK);
		break;

	case DLL_THREAD_ATTACH:
		hlp.log("dll thread attach");
		break;

	case DLL_THREAD_DETACH:
		hlp.log("dll thread detach");
		break;

	case DLL_PROCESS_DETACH:
		hlp.log("detaching from process:");
		hlp.log(processName.c_str());

		LeaveFFBLoopThread();
		SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);

		if (configGameId == FLYCAST) {
			WritePrivateProfileStringA("Settings", "ProcessID", 0, ".\\FFBPlugin.ini");
		}

		hlp.log("Closing SDL haptic device");
		CloseHaptic(haptic);
		CloseHaptic(haptic2);
		CloseHaptic(haptic3);

		hlp.log("Closing game controller");
		CloseController(GameController, EnableRumble, EnableRumbleTriggers);
		CloseController(GameController2, EnableRumbleDevice2, false);
		CloseController(GameController3, EnableRumbleDevice3, false);
		hlp.log("All done, leaving");

		break;
	}

	return result;
}
