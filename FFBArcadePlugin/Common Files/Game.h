#pragma once
#include <Windows.h>
#include "MinHook.h"

// struct
struct EffectTriggers {
	void(*Constant)(int direction, double strength);
	void(*Spring)(double strength);
	void(*Friction)(double strength);
	void(*Sine)(UINT16 waveperiod_ms, UINT16 fadePeriod_ms, double strength, UINT32 length_ms);
	void(*SineDevice2)(UINT16 waveperiod_ms, UINT16 fadePeriod_ms, double strength, UINT32 length_ms);
	void(*SineDevice3)(UINT16 waveperiod_ms, UINT16 fadePeriod_ms, double strength, UINT32 length_ms);
	void(*Rumble)(double lowfrequency, double highfrequency, UINT32 length_ms);
	void(*RumbleDevice2)(double lowfrequency, double highfrequency, UINT32 length_ms);
	void(*RumbleDevice3)(double lowfrequency, double highfrequency, UINT32 length_ms);
	void(*RumbleTriggers)(double lefttrigger, double righttrigger, UINT32 length_ms);
	void(*LeftRight)(double smallstrength, double largestrength, UINT32 length_ms);
	void(*LeftRightDevice2)(double smallstrength, double largestrength, UINT32 length_ms);
	void(*Springi)(double strength);
	void(*Inertia)(double strength);
	void(*Ramp)(double start, double end, UINT32 length_ms);
	void(*Damper)(double strength);
	void(*SawtoothUp)(double strength, UINT32 length_ms);
	void(*SawtoothDown)(double strength, UINT32 length_ms);
	void(*Triangle)(double strength, UINT32 length_ms);
};

// classes
class EffectCollection {
public:
	int effect_constant_id;
	int effect_leftramp_id;
	int effect_rightramp_id;
	int effect_friction_id;
	int effect_leftright_id;
	int effect_sine_id;
	int effect_sine_id_device2;
	int effect_sine_id_device3;
	int effect_spring_id;
	int effect_vibration_id;
	int effect_inertia_id;
	int effect_ramp_id;
	int effect_damper_id;
	int effect_sawtoothup_id;
	int effect_sawtoothdown_id;
	int effect_triangle_id;
};

class EffectConstants {
public:
	// constants
	// Haptic forces are defined by the direction that generates the force
	// e.g. DIRECTION_FROM_LEFT causes wheel to go right.
	const int DIRECTION_FROM_LEFT = -1;
	const int DIRECTION_FROM_RIGHT = 1;
};

#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A

#define SDL_HAT_CENTERED    0x00
#define SDL_HAT_UP          0x01
#define SDL_HAT_RIGHT       0x02
#define SDL_HAT_DOWN        0x04
#define SDL_HAT_LEFT        0x08
#define SDL_HAT_RIGHTUP     (SDL_HAT_RIGHT|SDL_HAT_UP)
#define SDL_HAT_RIGHTDOWN   (SDL_HAT_RIGHT|SDL_HAT_DOWN)
#define SDL_HAT_LEFTUP      (SDL_HAT_LEFT|SDL_HAT_UP)
#define SDL_HAT_LEFTDOWN    (SDL_HAT_LEFT|SDL_HAT_DOWN)

// Reads an integer from the [Settings] section of FFBPlugin.ini
int GetSettingInt(LPCWSTR name, int defaultValue);

// Rumble selected by a HowtoRumbleXxxEffect setting: 0 both motors, 1 left motor only, 2 right motor only
void RumbleWithMode(void(*rumble)(double lowfrequency, double highfrequency, UINT32 length_ms), int mode, double strength, UINT32 length_ms);

// Plays the damper effect set by the EnableDamper and DamperStrength settings
void ApplyDamper(EffectTriggers* triggers);

class Helpers {
public:
	int enableLogging = 0;
	// logging
	void log(const char *msg);
	void logInt(int value);
	// reading memory
	LPVOID GetTranslatedOffset(INT_PTR offset);

	// isRelativeOffset: offset is relative to the main module base address instead of absolute.
	template <typename T>
	T Read(INT_PTR offset, bool isRelativeOffset)
	{
		T val{};
		SIZE_T read;
		ReadProcessMemory(GetCurrentProcess(), ResolveOffset(offset, isRelativeOffset), &val, sizeof(T), &read);
		return val;
	}

	template <typename T>
	T Write(INT_PTR offset, T val, bool isRelativeOffset)
	{
		SIZE_T written;
		WriteProcessMemory(GetCurrentProcess(), ResolveOffset(offset, isRelativeOffset), &val, sizeof(T), &written);
		return val;
	}

	int ReadInt32(INT_PTR offset, bool isRelativeOffset) { return Read<int>(offset, isRelativeOffset); }
	UINT8 ReadByte(INT_PTR offset, bool isRelativeOffset) { return Read<UINT8>(offset, isRelativeOffset); }
	WORD ReadWord(INT_PTR offset, bool isRelativeOffset) { return Read<WORD>(offset, isRelativeOffset); }
	INT_PTR ReadIntPtr(INT_PTR offset, bool isRelativeOffset) { return Read<INT_PTR>(offset, isRelativeOffset); }
	long long ReadLong(INT_PTR offset, bool isRelativeOffset) { return Read<long long>(offset, isRelativeOffset); }
	float ReadFloat32(INT_PTR offset, bool isRelativeOffset) { return Read<float>(offset, isRelativeOffset); }
	float WriteFloat32(INT_PTR offset, float val, bool isRelativeOffset) { return Write<float>(offset, val, isRelativeOffset); }
	UINT8 WriteByte(INT_PTR offset, UINT8 val, bool isRelativeOffset) { return Write<UINT8>(offset, val, isRelativeOffset); }
	INT_PTR WriteIntPtr(INT_PTR offset, INT_PTR val, bool isRelativeOffset) { return Write<INT_PTR>(offset, val, isRelativeOffset); }
	WORD WriteWord(INT_PTR offset, WORD val, bool isRelativeOffset) { return Write<WORD>(offset, val, isRelativeOffset); }
	UINT8 WriteNop(INT_PTR offset, int countBytes, bool isRelativeOffset);

private:
	LPVOID ResolveOffset(INT_PTR offset, bool isRelativeOffset)
	{
		return isRelativeOffset ? GetTranslatedOffset(offset) : (LPVOID)offset;
	}
};

class Game {
public:
	virtual void FFBLoop(EffectConstants *constants, Helpers *helpers, EffectTriggers *triggers);
};
