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
#include "Core/GameIds.h"
#include "Core/Devices.h"
#include "../Version.h"
#include "WrapperExports.h"

#include "Core/GameFactory.h"
#include "Core/ForceFeedback.h"
#include "Core/Log.h"
#include "Core/Settings.h"
#include "Core/MinHook.h"
#include "Core/Config.h"

static ffb::Config& config = ffb::Config::Get();

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
	LPVOID val = NULL;
	HRESULT res = originalDirectInputDirectInput8Create(hinst, dwVersion, riidltf, &val, punkOuter);
	// the wrapper deletes itself with the last Release
	*ppvOut = (SUCCEEDED(res) && val != NULL) ? new DirectInputDeviceWrapper(val, (IID_IDirectInput8W == riidltf)) : NULL;
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
// Effects uploaded to the haptic devices, -1 when not created (no device, or effect not supported)
class EffectCollection {
public:
	int effect_constant_id = -1;
	int effect_leftramp_id = -1;
	int effect_rightramp_id = -1;
	int effect_friction_id = -1;
	int effect_leftright_id = -1;
	int effect_sine_id = -1;
	int effect_sine_id_device2 = -1;
	int effect_sine_id_device3 = -1;
	int effect_spring_id = -1;
	int effect_vibration_id = -1;
	int effect_inertia_id = -1;
	int effect_ramp_id = -1;
	int effect_damper_id = -1;
	int effect_sawtoothup_id = -1;
	int effect_sawtoothdown_id = -1;
	int effect_triangle_id = -1;
};

EffectCollection effects;

bool CustomStrength = false;
bool WaitForGame = false;
volatile bool keepRunning = true;
static HANDLE ffbLoopThread = NULL;
static DWORD ffbLoopThreadId = 0;
// Set once StopPlugin has stopped the forces and closed the devices
static volatile LONG pluginStopped = 0;

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

char chainedDLL[256];

SDL_Event e;

// Uploads an effect of the given type, its parameters being updated each time it is triggered.
// Returns -1 when the device does not support it: some wheels (e.g. PXN V10) crash when asked for an unsupported effect.
static int CreateBaseEffect(SDL_Haptic* device, Uint16 type, Uint32 length_ms)
{
	if (!(SDL_HapticQuery(device) & type))
		return -1;

	SDL_HapticEffect effect;
	SDL_memset(&effect, 0, sizeof(SDL_HapticEffect));
	effect.type = type;
	effect.condition.direction.type = SDL_HAPTIC_CARTESIAN;
	effect.condition.length = length_ms;
	return SDL_HapticNewEffect(device, &effect);
}

void Initialize()
{
	ffb::Log::Write("in initialize");
	{
		SDL_version sdl;
		SDL_GetVersion(&sdl);
		char text[128];
		sprintf_s(text, "FFB Arcade Plugin %s, SDL %d.%d.%d", FFB_VERSION_STRING, sdl.major, sdl.minor, sdl.patch);
		ffb::Log::Write(text);
	}
	SDL_SetHint(SDL_HINT_JOYSTICK_RAWINPUT, "0");
	if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER) < 0)
	{
		ffb::Log::Write("SDL_Init failed");
		return;
	}
	SDL_JoystickEventState(SDL_ENABLE);
	SDL_JoystickUpdate();
	char joystick_guid[256];
	sprintf(joystick_guid, "%S", config.deviceGuid.c_str());
	SDL_JoystickGUID guid = SDL_JoystickGetGUIDFromString(joystick_guid);
	ffb::Log::Write("numJoysticks = ");
	ffb::Log::Printf("%d", SDL_NumJoysticks());
	char firstJoystickSelectedText[512];
	// First joystick matching the GUID, used when none of them has haptics; the other ones are closed
	SDL_Joystick* FirstGameController = NULL;
	int firstJoystickIndex = -1;
	for (int i = 0; i < SDL_NumJoysticks(); i++)
	{
		SDL_Joystick* js = SDL_JoystickOpen(i);
		const char* name = SDL_JoystickName(js);
		SDL_JoystickGUID dev_guid = SDL_JoystickGetGUID(js);
		char guid_str[1024];
		SDL_JoystickGetGUIDString(dev_guid, guid_str, sizeof(guid_str));
		if (config.forceShowDeviceGUIDMessageBox == 1)
		{
			MessageBoxA(NULL, name, "", 0);
			MessageBoxA(NULL, guid_str, "", 0);
		}
		char text[1024];
		sprintf_s(text, "Joystick: %d / Name: %s / GUID: %s\n", i, name ? name : "", guid_str);
		ffb::Log::Write(text);
		SDL_JoystickClose(js);
		if (!SameJoystickGUID(guid, dev_guid))
			continue;
		if (config.ignoreFirstMatchingGUID)
		{
			config.ignoreFirstMatchingGUID = 0;
			continue;
		}

		SDL_Joystick* candidate = SDL_JoystickOpen(i);
		SDL_Haptic* candidateHaptic = SDL_HapticOpenFromJoystick(candidate);
		if (candidateHaptic != NULL)
		{
			joystick1Index = i;
			GameController = candidate;
			joystick_index1 = SDL_JoystickInstanceID(candidate);
			ControllerHaptic = candidateHaptic;
			sprintf_s(text, "Haptic joystick found: %d / Name: %s / GUID: %s\n", i, name ? name : "", guid_str);
			ffb::Log::Write(text);
			break;
		}

		if (FirstGameController == NULL)
		{
			sprintf_s(firstJoystickSelectedText, "No haptic joystick found, selecting first joystick matching the GUID: %d / Name: %s / GUID: %s\n", i, name ? name : "", guid_str);
			FirstGameController = candidate;
			firstJoystickIndex = i;
		}
		else
			SDL_JoystickClose(candidate);
	}

	if (ControllerHaptic == NULL && FirstGameController != NULL) // If no haptic controller has been found, we select the first controller matching the guid.
	{
		joystick1Index = firstJoystickIndex;
		GameController = FirstGameController;
		joystick_index1 = SDL_JoystickInstanceID(FirstGameController);
		ffb::Log::Write(firstJoystickSelectedText);
	}
	else if (FirstGameController != NULL)
		SDL_JoystickClose(FirstGameController);
	haptic = ControllerHaptic;
	if (haptic == NULL)
		ffb::Log::Write("No haptic device available, FFB effects will be disabled");
	else
	{
		if ((SDL_HapticRumbleSupported(haptic) == SDL_TRUE && config.device1.enableRumble))
		{
			SDL_HapticRumbleInit(ControllerHaptic);
			ffb::Log::Write("Rumble Init");
		}
		ffb::Log::Write("disabling all current FFB effects");
		SDL_HapticStopAll(haptic);
		ffb::Log::Write("setting haptic gain to 100");
		SDL_HapticSetGain(haptic, 100); // HapticSetGain should be between 0 and 100 as per https://wiki.libsdl.org/SDL_HapticSetGain
		ffb::Log::Write("setting haptic auto center to 0");
		SDL_HapticSetAutocenter(haptic, 0); // 0 disables autocenter https://wiki.libsdl.org/SDL_HapticSetAutocenter

		ffb::Log::Write("creating base effects...");

		if (SDL_HapticQuery(haptic) & SDL_HAPTIC_CONSTANT)
		{
			SDL_HapticEffect tempEffect;
			SDL_memset(&tempEffect, 0, sizeof(SDL_HapticEffect));
			tempEffect.type = SDL_HAPTIC_CONSTANT;
			tempEffect.constant.direction.type = SDL_HAPTIC_CARTESIAN;
			tempEffect.constant.direction.dir[0] = -1; //left => right is +1 as per this 2d array explanation: https://wiki.libsdl.org/SDL_HapticDirection
			tempEffect.constant.length = config.feedbackLength; // presumably is ms, but is not documented
			tempEffect.constant.delay = 0;
			tempEffect.constant.level = 9999; // this is an sint16 => -32768 to 32767
			effects.effect_constant_id = SDL_HapticNewEffect(haptic, &tempEffect); // Upload the effect
		}

		effects.effect_friction_id = CreateBaseEffect(haptic, SDL_HAPTIC_FRICTION, 5000);
		effects.effect_sine_id = CreateBaseEffect(haptic, SDL_HAPTIC_SINE, 0);
		effects.effect_spring_id = CreateBaseEffect(haptic, SDL_HAPTIC_SPRING, 5000);
		effects.effect_inertia_id = CreateBaseEffect(haptic, SDL_HAPTIC_INERTIA, 5000);
		effects.effect_damper_id = CreateBaseEffect(haptic, SDL_HAPTIC_DAMPER, 5000);
		effects.effect_triangle_id = CreateBaseEffect(haptic, SDL_HAPTIC_TRIANGLE, 5000);
		effects.effect_ramp_id = CreateBaseEffect(haptic, SDL_HAPTIC_RAMP, 5000);
		effects.effect_sawtoothup_id = CreateBaseEffect(haptic, SDL_HAPTIC_SAWTOOTHUP, 5000);
		effects.effect_sawtoothdown_id = CreateBaseEffect(haptic, SDL_HAPTIC_SAWTOOTHDOWN, 5000);
	}

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
	return { config.device1.force.min, config.device1.force.max };
}

static ForceLimits ForceLimitsDevice1(int direction)
{
	return SelectForceLimits(direction, config.device1.alternativeFFB, DefaultForceLimits(),
		{ config.device1.alternativeLeft.min, config.device1.alternativeLeft.max },
		{ config.device1.alternativeRight.min, config.device1.alternativeRight.max });
}

static ForceLimits ForceLimitsDevice2(int direction)
{
	return SelectForceLimits(direction, config.device2.alternativeFFB, { config.device2.force.min, config.device2.force.max },
		{ config.device2.alternativeLeft.min, config.device2.alternativeLeft.max },
		{ config.device2.alternativeRight.min, config.device2.alternativeRight.max });
}

static ForceLimits ForceLimitsDevice3(int direction)
{
	return SelectForceLimits(direction, config.device3.alternativeFFB, { config.device3.force.min, config.device3.force.max },
		{ config.device3.alternativeLeft.min, config.device3.alternativeLeft.max },
		{ config.device3.alternativeRight.min, config.device3.alternativeRight.max });
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
	if (device == NULL || effectId < 0) // no haptic device, or effect not supported by it
		return;

	SDL_HapticUpdateEffect(device, effectId, &effect);
	SDL_HapticRunEffect(device, effectId, 1);
}

// EFFECTS THAT LAST UNTIL THEY ARE CHANGED
// The default centering and friction, Springi and ConstantInf last until the game changes or stops
// them. They used to be infinite: when the game is killed (TerminateProcess, as some launchers do),
// the plugin runs no more code, and an infinite effect stays on the device. They are now played for
// KeptEffectLength and renewed by the FFB loop every KeptEffectRenewal while they last, so they end
// by themselves within KeptEffectLength once the game is gone.
static const Uint32 KeptEffectLength = 1000; // ms
static const ULONGLONG KeptEffectRenewal = 300; // ms

struct KeptEffect
{
	const int* id; // in `effects`, set by Initialize
	bool running;
	ULONGLONG renewed; // GetTickCount64 of the last start
};

static KeptEffect keptConstant = { &effects.effect_constant_id, false, 0 };
static KeptEffect keptSpring = { &effects.effect_spring_id, false, 0 };
static KeptEffect keptFriction = { &effects.effect_friction_id, false, 0 };
// The effects of these slots are also triggered by other threads than the FFB loop (MAME outputs,
// game hooks): every use of the three slots holds this lock.
static SRWLOCK keptEffectsLock = SRWLOCK_INIT;

// Updates the effect of the slot, whose length is KeptEffectLength, and starts it if it is not running;
// RenewKeptEffects then starts it again before it ends. Called with keptEffectsLock held.
static void PlayKeptEffect(KeptEffect& kept, SDL_HapticEffect& effect)
{
	if (haptic == NULL || *kept.id < 0)
	{
		kept.running = false;
		return;
	}

	SDL_HapticUpdateEffect(haptic, *kept.id, &effect);
	if (!kept.running)
	{
		SDL_HapticRunEffect(haptic, *kept.id, 1);
		kept.renewed = GetTickCount64();
		kept.running = true;
	}
}

// Called with keptEffectsLock held
static void StopKeptEffect(KeptEffect& kept)
{
	if (kept.running && haptic != NULL && *kept.id >= 0)
		SDL_HapticStopEffect(haptic, *kept.id);
	kept.running = false;
}

// Plays a finite effect of the game in the slot of a kept one, which is no longer renewed
static void RunInKeptSlot(KeptEffect& kept, SDL_HapticEffect& effect)
{
	AcquireSRWLockExclusive(&keptEffectsLock);
	kept.running = false;
	RunEffect(haptic, *kept.id, effect);
	ReleaseSRWLockExclusive(&keptEffectsLock);
}

// Called by the FFB loop, every 16 ms
static void RenewKeptEffects()
{
	ULONGLONG now = GetTickCount64();
	AcquireSRWLockExclusive(&keptEffectsLock);
	for (KeptEffect* kept : { &keptConstant, &keptSpring, &keptFriction })
	{
		if (kept->running && haptic != NULL && *kept->id >= 0 && now - kept->renewed >= KeptEffectRenewal)
		{
			SDL_HapticRunEffect(haptic, *kept->id, 1);
			kept->renewed = now;
		}
	}
	ReleaseSRWLockExclusive(&keptEffectsLock);
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

static SDL_HapticEffect ConstantEffect(int direction, double strength)
{
	ForceLimits limits = ForceLimitsDevice1(direction);

	if (config.powerMode)
		strength = pow(strength, 0.5);

	if (config.doubleConstant)
		strength = DoubleStrength(strength);

	SDL_HapticEffect tempEffect;
	SDL_memset(&tempEffect, 0, sizeof(SDL_HapticEffect));
	tempEffect.type = SDL_HAPTIC_CONSTANT;
	tempEffect.constant.direction.type = SDL_HAPTIC_CARTESIAN;
	tempEffect.constant.direction.dir[0] = direction;
	tempEffect.constant.length = config.feedbackLength;
	tempEffect.constant.delay = 0;
	tempEffect.constant.level = ScaleDirectionalForce(strength, limits);
	return tempEffect;
}

void TriggerConstantEffect(int direction, double strength)
{
	SDL_HapticEffect tempEffect = ConstantEffect(direction, strength);
	RunInKeptSlot(keptConstant, tempEffect);
}

// Set for California Speed, whose raw motor value crosses 0 when the force changes direction:
// ConstantInf then holds the running effect at level 0 instead of stopping and restarting it.
bool ConstantInfHoldsAtZero = false;

// Constant effect that lasts until the next call, which only updates it (restarting it at each update
// makes some wheels jerk); it is renewed by the FFB loop (see KeptEffect). A strength of 0 stops it.
void TriggerConstantInfEffect(int direction, double strength)
{
	AcquireSRWLockExclusive(&keptEffectsLock);
	if (strength < 0.001 && !(ConstantInfHoldsAtZero && strength == 0.0))
		StopKeptEffect(keptConstant);
	else
	{
		SDL_HapticEffect tempEffect = ConstantEffect(direction, strength);
		tempEffect.constant.length = KeptEffectLength;
		PlayKeptEffect(keptConstant, tempEffect);
	}
	ReleaseSRWLockExclusive(&keptEffectsLock);
}

// isDefault: the DefaultFriction setting, which lasts until a friction of the game replaces it
void TriggerFrictionEffectWithDefaultOption(double strength, bool isDefault)
{
	if (config.doubleFriction)
		strength = DoubleStrength(strength);

	SHORT coeff = ScaleCoefficient(strength, DefaultForceLimits());
	SDL_HapticEffect tempEffect = ConditionEffect(SDL_HAPTIC_FRICTION, isDefault ? KeptEffectLength : config.feedbackLength, coeff);
	tempEffect.condition.left_sat[0] = 0xFFFF;
	tempEffect.condition.right_sat[0] = 0xFFFF;

	if (!isDefault)
		RunInKeptSlot(keptFriction, tempEffect);
	else
	{
		AcquireSRWLockExclusive(&keptEffectsLock);
		if (coeff == 0) // nothing to keep playing
			StopKeptEffect(keptFriction);
		else
			PlayKeptEffect(keptFriction, tempEffect);
		ReleaseSRWLockExclusive(&keptEffectsLock);
	}
}

void TriggerFrictionEffect(double strength)
{
	TriggerFrictionEffectWithDefaultOption(strength, false);
}

void TriggerInertiaEffect(double strength)
{
	SHORT coeff = ScaleCoefficient(strength, DefaultForceLimits());
	SDL_HapticEffect tempEffect = ConditionEffect(SDL_HAPTIC_INERTIA, config.feedbackLength, coeff);
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
	SDL_HapticEffect tempEffect = ConditionEffect(SDL_HAPTIC_DAMPER, config.feedbackLength, coeff);
	tempEffect.condition.direction.dir[0] = 1; // not used
	tempEffect.condition.direction.dir[1] = 0; //Y Position
	tempEffect.condition.left_sat[0] = (DWORD)(coeff) * 2;
	tempEffect.condition.right_sat[0] = (DWORD)(coeff) * 2;
	RunEffect(haptic, effects.effect_damper_id, tempEffect);
}

// kept: lasts until another spring replaces it (DefaultCentering, Springi), see KeptEffect
static void RunSpringEffect(SHORT coeff, bool kept, Sint32 directionY)
{
	SDL_HapticEffect tempEffect = ConditionEffect(SDL_HAPTIC_SPRING, kept ? KeptEffectLength : config.feedbackLength, coeff);
	tempEffect.condition.direction.dir[0] = 1;
	tempEffect.condition.direction.dir[1] = directionY; //Y Position
	tempEffect.condition.left_sat[0] = (short)(coeff * 2.0); //Needed for Logitech G920 wheel
	tempEffect.condition.right_sat[0] = (short)(coeff * 2.0); //Needed for Logitech G920 wheel
	tempEffect.condition.center[0] = 0;

	if (!kept)
		RunInKeptSlot(keptSpring, tempEffect);
	else
	{
		AcquireSRWLockExclusive(&keptEffectsLock);
		PlayKeptEffect(keptSpring, tempEffect);
		ReleaseSRWLockExclusive(&keptEffectsLock);
	}
}

// isDefault: the DefaultCentering setting, which lasts until a spring of the game replaces it
void TriggerSpringEffectWithDefaultOption(double strength, bool isDefault)
{
	if (config.doubleSpring)
		strength = DoubleStrength(strength);

	RunSpringEffect(ScaleCoefficient(strength, DefaultForceLimits()), isDefault, 0);
}

void TriggerSpringEffect(double strength)
{
	TriggerSpringEffectWithDefaultOption(strength, false);
}

void TriggerSpringEffectInfinite(double strength)
{
	ForceLimits limits = config.enableForceSpringEffect ? ForceLimits{ 0, config.forceSpringStrength } : DefaultForceLimits();
	RunSpringEffect(ScaleCoefficient(strength, limits), true, 1);
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
	if (device == NULL || effectId < 0)
		return;

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

	ffb::Log::Write(logMessage);
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

	RunSineEffect(haptic, effects.effect_sine_id, lastSineEffect, ForceLimitsDevice1, config.doubleSine, "Doing sine...", period_ms, fadePeriod_ms, strength, length_ms);
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
	if (!config.device1.enableRumble)
		return;

	if (config.device1.reverseRumble)
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
	DWORD minForce = (DWORD)(strength > 0.001 ? (config.device1.force.min / 100.0 * 65535.0) : 0);
	DWORD maxForce = (DWORD)(config.device1.force.max / 100.0 * 65535.0);
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
	RunRumble(GameController, config.device1.enableRumble, config.device1.reverseRumble, config.singleRumbleMotor, highfrequency, lowfrequency, length_ms);
}

void TriggerRumbleEffectDevice2(double highfrequency, double lowfrequency, UINT32 length_ms)
{
	RunRumble(GameController2, config.device2.enableRumble, config.device2.reverseRumble, false, highfrequency, lowfrequency, length_ms);
}

void TriggerRumbleEffectDevice3(double highfrequency, double lowfrequency, UINT32 length_ms)
{
	RunRumble(GameController3, config.device3.enableRumble, config.device3.reverseRumble, false, highfrequency, lowfrequency, length_ms);
}

void TriggerRumbleTriggerEffect(double lefttrigger, double righttrigger, UINT32 length_ms)
{
	if (!config.enableRumbleTriggers)
		return;

	UINT16 LeftMotor16 = ClampMotorSpeed(ScaleMotorSpeed(lefttrigger));
	UINT16 RightMotor16 = ClampMotorSpeed(ScaleMotorSpeed(righttrigger));

	if (SDL_JoystickRumbleTriggers(GameController, LeftMotor16, RightMotor16, length_ms) == -1)
		config.enableRumbleTriggers = 0;
}

// THE DEVICES OF THE GAMES

// Cleared by StopPlugin: the effects asked by the game are then ignored
static volatile bool effectsEnabled = true;

// Device 1, with all the effects
class HapticWheel : public ffb::Wheel
{
public:
	void Constant(ffb::Direction direction, double strength) override { if (effectsEnabled) TriggerConstantEffect((int)direction, strength); }
	void KeptConstant(ffb::Direction direction, double strength) override { if (effectsEnabled) TriggerConstantInfEffect((int)direction, strength); }
	void Spring(double strength) override { if (effectsEnabled) TriggerSpringEffect(strength); }
	void KeptSpring(double strength) override { if (effectsEnabled) TriggerSpringEffectInfinite(strength); }
	void Friction(double strength) override { if (effectsEnabled) TriggerFrictionEffect(strength); }
	void Inertia(double strength) override { if (effectsEnabled) TriggerInertiaEffect(strength); }
	void Damper(double strength) override { if (effectsEnabled) TriggerDamperEffect(strength); }
	void Triangle(double strength, UINT32 length_ms) override { if (effectsEnabled) TriggerTriangleEffect(strength, length_ms); }
	void SawtoothUp(double strength, UINT32 length_ms) override { if (effectsEnabled) TriggerSawtoothUpEffect(strength, length_ms); }
	void SawtoothDown(double strength, UINT32 length_ms) override { if (effectsEnabled) TriggerSawtoothDownEffect(strength, length_ms); }
	void Ramp(double start, double end, UINT32 length_ms) override { if (effectsEnabled) TriggerRampEffect(start, end, length_ms); }
	void RumbleTriggers(double left, double right, UINT32 length_ms) override { if (effectsEnabled) TriggerRumbleTriggerEffect(left, right, length_ms); }
	void Sine(UINT16 period_ms, UINT16 fadePeriod_ms, double strength, UINT32 length_ms) override { if (effectsEnabled) TriggerSineEffect(period_ms, fadePeriod_ms, strength, length_ms); }
	void Rumble(double highFrequency, double lowFrequency, UINT32 length_ms) override { if (effectsEnabled) TriggerRumbleEffect(highFrequency, lowFrequency, length_ms); }
	void LeftRight(double smallStrength, double largeStrength, UINT32 length_ms) override { if (effectsEnabled) TriggerLeftRightEffect(smallStrength, largeStrength, length_ms); }
};

// Device 2 (Device2GUID)
class HapticDevice2 : public ffb::Device
{
public:
	void Sine(UINT16 period_ms, UINT16 fadePeriod_ms, double strength, UINT32 length_ms) override { if (effectsEnabled) TriggerSineEffectDevice2(period_ms, fadePeriod_ms, strength, length_ms); }
	void Rumble(double highFrequency, double lowFrequency, UINT32 length_ms) override { if (effectsEnabled) TriggerRumbleEffectDevice2(highFrequency, lowFrequency, length_ms); }
	void LeftRight(double smallStrength, double largeStrength, UINT32 length_ms) override { if (effectsEnabled) TriggerLeftRightDevice2Effect(smallStrength, largeStrength, length_ms); }
};

// Device 3 (Device3GUID): no LeftRight effect
class HapticDevice3 : public ffb::Device
{
public:
	void Sine(UINT16 period_ms, UINT16 fadePeriod_ms, double strength, UINT32 length_ms) override { if (effectsEnabled) TriggerSineEffectDevice3(period_ms, fadePeriod_ms, strength, length_ms); }
	void Rumble(double highFrequency, double lowFrequency, UINT32 length_ms) override { if (effectsEnabled) TriggerRumbleEffectDevice3(highFrequency, lowFrequency, length_ms); }
	void LeftRight(double, double, UINT32) override {}
};

static HapticWheel hapticWheel;
static HapticDevice2 hapticDevice2;
static HapticDevice3 hapticDevice3;

int WorkaroundToFixRumble(void* ptr)
{
	while (SDL_WaitEvent(&e) != 0){}
	return 0;
}

// FFB STRENGTH ADJUSTMENT WITH JOYSTICK BUTTONS



void WritePersistentMaxForce()
{
	if (!config.enableFFBStrengthPersistence)
		return;

	// A null key would make WritePrivateProfileString delete the whole [Settings] section
	if (config.device1.alternativeFFB)
	{
		if (CustomAlternativeMaxForceLeft && CustomAlternativeMaxForceRight)
		{
			WritePrivateProfileStringA("Settings", CustomAlternativeMaxForceLeft, std::to_string(config.device1.alternativeLeft.max).c_str(), ffb::Settings::PathA().c_str());
			WritePrivateProfileStringA("Settings", CustomAlternativeMaxForceRight, std::to_string(config.device1.alternativeRight.max).c_str(), ffb::Settings::PathA().c_str());
		}
	}
	else if (CustomMaxForce)
		WritePrivateProfileStringA("Settings", CustomMaxForce, std::to_string(config.device1.force.max).c_str(), ffb::Settings::PathA().c_str());
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
	int step = increase ? config.stepFFBStrength : -config.stepFFBStrength;

	if (config.device1.alternativeFFB)
	{
		StepMaxForce(config.device1.alternativeRight.max, step, 0, 100, increase);
		StepMaxForce(config.device1.alternativeLeft.max, -step, -100, 0, !increase); // left forces are negative
	}
	else
		StepMaxForce(config.device1.force.max, step, 0, 100, increase);

	WritePersistentMaxForce();
}

static void SpeakMaxForce()
{
	char text[256];
	wchar_t wideText[256];
	sprintf(text, "Max Force: %d", config.device1.alternativeFFB ? config.device1.alternativeRight.max : config.device1.force.max);
	mbstowcs(wideText, text, strlen(text) + 1);

	// The voice is created and released each time, before CoUninitialize: it used to be a global
	// CComPtr, overwritten (leaked) by each new voice, released only when the DLL was unloaded,
	// after CoUninitialize and maybe after sapi.dll.
	HRESULT init = ::CoInitialize(nullptr);
	{
		CComPtr<ISpVoice> voice;
		if (SUCCEEDED(voice.CoCreateInstance(CLSID_SpVoice)))
		{
			voice->SetRate(3);
			voice->SetOutput(NULL, TRUE);
			voice->Speak(wideText, SPF_PURGEBEFORESPEAK, NULL); // synchronous: done when it returns
		}
	}
	if (SUCCEEDED(init))
		::CoUninitialize();
}

static void HandleFFBStrengthButton(const SDL_Event& event)
{
	if (event.type != SDL_JOYBUTTONDOWN || event.jbutton.which != joystick_index1)
		return;

	Uint8 button = event.jbutton.button;

	if (button == config.increaseFFBStrength)
		ChangeMaxForce(true);

	if (button == config.decreaseFFBStrength)
		ChangeMaxForce(false);

	if (button == config.resetFFBStrength)
	{
		DefaultConfigValues();
		WritePersistentMaxForce();
	}

	if (config.enableFFBStrengthTextToSpeech && (button == config.increaseFFBStrength || button == config.decreaseFFBStrength || button == config.resetFFBStrength))
		SpeakMaxForce();
}

// Returns true once the game has started, after setting up the persistent max force keys.
static bool ReadyToAdjustFFBStrength()
{
	if (!WaitForGame)
	{
		if (config.gameId != Demul_Emulator) // Demul sets WaitForGame once the game is detected
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

// FFB STRENGTH ADJUSTMENT WITH KEYBOARD KEYS

// Virtual key of a key named as by SDL_GetKeyName (the GUI stores these names), 0 if unknown
static int VirtualKeyFromName(const wchar_t* keyName)
{
	if (keyName[0] == L'\0')
		return 0;

	if (keyName[1] == L'\0') // single character key (A-Z, 0-9, punctuation)
	{
		SHORT vk = VkKeyScanW(keyName[0]);
		if (vk != -1)
			return LOBYTE(vk);
	}

	int functionKey = 0;
	if ((keyName[0] == L'F' || keyName[0] == L'f') && swscanf_s(keyName + 1, L"%d", &functionKey) == 1 && functionKey >= 1 && functionKey <= 24)
		return VK_F1 + functionKey - 1;

	static const struct
	{
		const wchar_t* name;
		int vk;
	} keys[] = {
		{ L"Return", VK_RETURN }, { L"Enter", VK_RETURN }, { L"Escape", VK_ESCAPE }, { L"Esc", VK_ESCAPE },
		{ L"Space", VK_SPACE }, { L"Tab", VK_TAB }, { L"Backspace", VK_BACK }, { L"Delete", VK_DELETE },
		{ L"Insert", VK_INSERT }, { L"Home", VK_HOME }, { L"End", VK_END },
		{ L"PageUp", VK_PRIOR }, { L"Page Up", VK_PRIOR }, { L"PageDown", VK_NEXT }, { L"Page Down", VK_NEXT },
		{ L"Up", VK_UP }, { L"Down", VK_DOWN }, { L"Left", VK_LEFT }, { L"Right", VK_RIGHT },
		{ L"Left Shift", VK_LSHIFT }, { L"Right Shift", VK_RSHIFT }, { L"Left Ctrl", VK_LCONTROL }, { L"Right Ctrl", VK_RCONTROL },
		{ L"Left Alt", VK_LMENU }, { L"Right Alt", VK_RMENU }, { L"Left GUI", VK_LWIN }, { L"Right GUI", VK_RWIN },
		{ L"CapsLock", VK_CAPITAL }, { L"NumLock", VK_NUMLOCK }, { L"ScrollLock", VK_SCROLL },
		{ L"Pause", VK_PAUSE }, { L"PrintScreen", VK_SNAPSHOT },
		{ L"Keypad 0", VK_NUMPAD0 }, { L"Keypad 1", VK_NUMPAD1 }, { L"Keypad 2", VK_NUMPAD2 }, { L"Keypad 3", VK_NUMPAD3 },
		{ L"Keypad 4", VK_NUMPAD4 }, { L"Keypad 5", VK_NUMPAD5 }, { L"Keypad 6", VK_NUMPAD6 }, { L"Keypad 7", VK_NUMPAD7 },
		{ L"Keypad 8", VK_NUMPAD8 }, { L"Keypad 9", VK_NUMPAD9 }, { L"Keypad +", VK_ADD }, { L"Keypad -", VK_SUBTRACT },
		{ L"Keypad *", VK_MULTIPLY }, { L"Keypad /", VK_DIVIDE }, { L"Keypad .", VK_DECIMAL }, { L"Keypad Enter", VK_RETURN },
	};

	for (const auto& key : keys)
	{
		if (lstrcmpiW(keyName, key.name) == 0)
			return key.vk;
	}
	return 0;
}

// Returns true when the key has just been pressed. The keys are read even when the game is not in the foreground.
static bool KeyPressed(int virtualKey, bool& wasDown)
{
	if (virtualKey == 0)
		return false;

	bool isDown = (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
	bool pressed = isDown && !wasDown;
	wasDown = isDown;
	return pressed;
}

// Polls the keys once the joystick buttons loop has set up the persistent max force keys
DWORD WINAPI AdjustFFBStrengthLoopKeyboard(LPVOID lpParam)
{
	int increaseKey = VirtualKeyFromName(config.increaseFFBStrengthKey.c_str());
	int decreaseKey = VirtualKeyFromName(config.decreaseFFBStrengthKey.c_str());
	int resetKey = VirtualKeyFromName(config.resetFFBStrengthKey.c_str());
	bool increaseDown = false;
	bool decreaseDown = false;
	bool resetDown = false;

	while (true)
	{
		if (CustomStrength)
		{
			bool increase = KeyPressed(increaseKey, increaseDown);
			bool decrease = KeyPressed(decreaseKey, decreaseDown);
			bool reset = KeyPressed(resetKey, resetDown);

			if (increase)
				ChangeMaxForce(true);

			if (decrease)
				ChangeMaxForce(false);

			if (reset)
			{
				DefaultConfigValues();
				WritePersistentMaxForce();
			}

			if (config.enableFFBStrengthTextToSpeech && (increase || decrease || reset))
				SpeakMaxForce();
		}
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
	return config.gameId == WMMT_5 || config.gameId == OUTRUN_2Fake || config.gameId == Button_Rumble || config.gameId == Road_Fighters_3D || config.gameId == OUTRUN_2Real;
}

// Special K disables rumble by default
static void EnableSpecialKRumble()
{
	char RumbleDisableChar[256];
	GetPrivateProfileStringA("Input.Gamepad", "DisableRumble", "", RumbleDisableChar, 256, ".\\dxgi.ini");

	if (strcmp(RumbleDisableChar, "true") == 0 || strcmp(RumbleDisableChar, "True") == 0 || strcmp(RumbleDisableChar, "TRUE") == 0)
		WritePrivateProfileStringA("Input.Gamepad", "DisableRumble", "false", ".\\dxgi.ini");
}


static void InstallExitProcessHook();

DWORD WINAPI FFBLoop(LPVOID lpParam)
{
	ffb::Log::Write("In FFBLoop");

	if (config.startDelay) //For games which need code to run quicker etc. Some games will crash if no sleep added
		Sleep(2500);

	Initialize();
	ffb::Log::Write("Initialize() complete");
	InstallExitProcessHook();
	if (config.device1.enableRumble)
	{
		if (!config.enableFFBStrengthDynamicAdjustment && !config.inputDeviceWheelEnable && !GameHandlesSDLEvents()) // Workaround for SDL_JoystickRumble rumble not stopping issue
			SDL_CreateThread(WorkaroundToFixRumble, "WorkaroundToFixRumble", (void*)NULL);

		EnableSpecialKRumble();
	}

	// the effects of the game are played on the devices opened by Initialize
	ffb::UseDevices(hapticWheel, hapticDevice2, hapticDevice3);

	ffb::Game* game = CreateGame(config.gameId);

	ffb::Log::Write("Setting DefaultCentering & DefaultFriction forces...");
	if (config.defaultCentering >= 0 && config.defaultCentering <= 100) {
		TriggerSpringEffectWithDefaultOption(config.defaultCentering / 100.0, true);
	}
	if (config.defaultFriction >= 0 && config.defaultFriction <= 100) {
		TriggerFrictionEffectWithDefaultOption(config.defaultFriction / 100.0, true);
	}

	if (config.enableFFBStrengthDynamicAdjustment == 1)
	{
		// Only one thread can wait for SDL events
		if (GameHandlesSDLEvents() || (config.gameId == Demul_Emulator && config.inputDeviceWheelEnable))
			CreateThread(NULL, 0, AdjustFFBStrengthLoopNoWaitEvent, NULL, 0, NULL);
		else
			CreateThread(NULL, 0, AdjustFFBStrengthLoopWaitEvent, NULL, 0, NULL);

		if (!config.increaseFFBStrengthKey.empty() || !config.decreaseFFBStrengthKey.empty() || !config.resetFFBStrengthKey.empty())
			CreateThread(NULL, 0, AdjustFFBStrengthLoopKeyboard, NULL, 0, NULL);
	}

	ffb::Log::Write("Entering Game's FFBLoop loop");
	while (keepRunning)
	{
		if (game != nullptr)
			game->Update();
		RenewKeptEffects();
		Sleep(16);
	}
	ffb::Log::Write("about to exit FFBLoop");
	return 0;
}

void CreateFFBLoopThread()
{
	ffb::Log::Write("Before CreateThread");
	// the handle is kept for StopPlugin to wait for the thread
	ffbLoopThread = CreateThread(NULL, 0, FFBLoop, NULL, 0, &ffbLoopThreadId);
	ffb::Log::Write("After CreateThread");
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
		ffb::Log::Write(chainedDLL);
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

	ffb::Log::Write(buffer);

	gl_hOriginalDll = LoadLibraryA(buffer);

	if (config.gameId == SEGA_RALLY_3_Other)
		MEMwrite((void*)(0x57B2F0), (void*)"\x33\xC0\x40\xC3", 4);

	if (config.gameId == Road_Fighters_3D)
	{
		gl_hjgtDll = LoadLibraryA("jgt.dll");
		gl_hlibavs = LoadLibraryA("libavs-win32-ea3.dll");
	}

	if (config.gameId == HOTD_4 || config.gameId == RAMBO_)
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

// Stops the effects of the game: the threads of the game code (the MAME outputs, for instance)
// may still trigger effects while the devices are closed.
static void DisableEffects()
{
	effectsEnabled = false;
}

// Stops the forces and closes the devices when the game exits, from ExitProcessHook: its threads
// still run and the loader lock is free, unlike in DLL_PROCESS_DETACH.
static void StopPlugin()
{
	if (InterlockedExchange(&pluginStopped, 1) != 0)
		return;

	ffb::Log::Write("Stopping the plugin");
	keepRunning = false;
	bool loopStopped = ffbLoopThread == NULL || GetCurrentThreadId() == ffbLoopThreadId
		|| WaitForSingleObject(ffbLoopThread, 500) == WAIT_OBJECT_0;
	DisableEffects();
	AcquireSRWLockExclusive(&keptEffectsLock);
	keptConstant.running = keptSpring.running = keptFriction.running = false; // no more renewals
	ReleaseSRWLockExclusive(&keptEffectsLock);

	// Stopping the forces first: a wheel may keep the last force once the game is gone
	SDL_Haptic* haptics[] = { haptic, haptic2, haptic3 };
	for (SDL_Haptic* device : haptics)
	{
		if (device)
			SDL_HapticStopAll(device);
	}

	if (loopStopped && GetCurrentThreadId() != ffbLoopThreadId)
	{
		Sleep(50); // an effect another thread was playing when the effects were disabled
		ffb::Log::Write("Closing SDL haptic devices and game controllers");
		CloseHaptic(haptic);
		CloseHaptic(haptic2);
		CloseHaptic(haptic3);
		haptic = haptic2 = haptic3 = ControllerHaptic = ControllerHaptic2 = ControllerHaptic3 = NULL;
		CloseController(GameController, config.device1.enableRumble, config.enableRumbleTriggers);
		CloseController(GameController2, config.device2.enableRumble, false);
		CloseController(GameController3, config.device3.enableRumble, false);
		GameController = GameController2 = GameController3 = NULL;
	}
	else
		ffb::Log::Write("The FFB loop did not stop: forces stopped, devices left open");

	if (config.gameId == FLYCAST)
		WritePrivateProfileStringA("Settings", "ProcessID", NULL, ".\\FFBPlugin.ini");
	ffb::Log::Write("Plugin stopped");
}

typedef VOID(WINAPI* ExitProcessFunction)(UINT exitCode);
static ExitProcessFunction originalExitProcess = NULL;

static VOID WINAPI ExitProcessHook(UINT exitCode)
{
	StopPlugin();
	// Flycast crashes when it exits with the plugin, so it is ended at once, as before (without
	// DLL_PROCESS_DETACH, whose work StopPlugin has done)
	if (config.gameId == FLYCAST)
		TerminateProcess(GetCurrentProcess(), exitCode);
	originalExitProcess(exitCode);
}

// Called by the FFB thread once the devices are open, before any game code: a game that hooks
// ExitProcess too (M2Emulator) replaces this hook.
static void InstallExitProcessHook()
{
	MH_STATUS status = MH_Initialize();
	if (status != MH_OK && status != MH_ERROR_ALREADY_INITIALIZED)
		return;

	LPVOID target = NULL;
	if (MH_CreateHookApiEx(L"kernel32.dll", "ExitProcess", &ExitProcessHook, (LPVOID*)&originalExitProcess, &target) == MH_OK
		&& MH_EnableHook(target) == MH_OK)
		ffb::Log::Write("ExitProcess hooked");
	else
		ffb::Log::Write("ExitProcess not hooked");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ulReasonForCall, LPVOID lpReserved)
{
	BOOL result = TRUE;
	char buff[MAX_PATH];
	ffb::Log::Write("DLLMAIN ENTERED");
	GetModuleFileNameA(NULL, buff, MAX_PATH);
	std::string processName = std::string(buff);
	ffb::Log::Write("process name:");
	ffb::Log::Write(processName.c_str());
	ffb::Log::Write("default centering & friction values:");
	ffb::Log::Printf("%d", config.defaultCentering);
	ffb::Log::Printf("%d", config.defaultFriction);

	switch (ulReasonForCall)
	{
	case DLL_PROCESS_ATTACH:
		// log, etc.
		ffb::Log::Write("dll process attach:");
		ffb::Log::Write(processName.c_str());
		ffb::Log::Printf("%d", config.device1.force.min);
		ffb::Log::Printf("%d", config.device1.force.max);
		DisableThreadLibraryCalls(hModule);
		{
			// Never unloaded before the process exits: FreeLibrary would leave the plugin's threads,
			// the MAMEInterop window, the game hooks and SDL running code that is gone
			HMODULE pinned;
			GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_PIN, (LPCWSTR)&DllMain, &pinned);
		}
		ffb::Log::Write("loading original library...");

		LoadOriginalLibrary(hModule);

		if (!gl_hOriginalDll)
		{
			ffb::Log::Write("library load process failed");
			ExitProcess(0);
		}
		else {
			ffb::Log::Write("library loaded");
		}

		LoadOriginalFunctions();

		if (config.gameId == FLYCAST)
		{
			DWORD ProcessID = ffb::Settings::Int(TEXT("ProcessID"), 0);

			if (IsProcessRunning(ProcessID))
				break;

			GameProcessID = MyGetProcessId(L"flycast.exe");

			if (GameProcessID)
				WritePrivateProfileStringA("Settings", "ProcessID", std::to_string(GameProcessID).c_str(), ".\\FFBPlugin.ini");
		}

		if (processName.find("FFBPluginGUI.exe") != std::string::npos)
		{
			ffb::Log::Write("hooked FFBPluginGUI.exe, aborting");
			break;
		}

		SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);

		// the outputs of the emulators, not in FFBPluginGUI.exe
		if (config.gameId == MAME_ || config.gameId == SUPERMODEL_ || config.gameId == FLYCAST)
		{
			HANDLE outputs = CreateThread(NULL, 0, ThreadForOutputs, NULL, 0, NULL);
			if (outputs)
				CloseHandle(outputs);
		}

		ffb::Log::Write("creating ffb loop thread...");
		CreateFFBLoopThread();
		if (config.beepWhenHook)
			MessageBeep(MB_ICONASTERISK);
		break;

	case DLL_THREAD_ATTACH:
		ffb::Log::Write("dll thread attach");
		break;

	case DLL_THREAD_DETACH:
		ffb::Log::Write("dll thread detach");
		break;

	case DLL_PROCESS_DETACH:
		ffb::Log::Write("detaching from process:");
		ffb::Log::Write(processName.c_str());

		// The module is pinned, so this is the process exiting. StopPlugin has normally been called
		// by ExitProcessHook. Otherwise, the other threads have already been killed, maybe in the
		// middle of an SDL call, holding its locks, and the loader lock is held: closing the devices
		// could hang the exit, so only stop the forces (no SDL lock with the DirectInput wheels).
		SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
		keepRunning = false;
		if (ffbLoopThread != NULL && pluginStopped == 0)
		{
			ffb::Log::Write("Plugin not stopped before the exit: stopping the forces");
			SDL_Haptic* haptics[] = { haptic, haptic2, haptic3 };
			for (SDL_Haptic* device : haptics)
			{
				if (device)
					SDL_HapticStopAll(device);
			}
		}
		ffb::Log::Write("All done, leaving");

		break;
	}

	return result;
}
