#pragma once
// Interface of the games not moved yet to the classes of Core/: they implement FFBLoop, which gets
// the effects as a structure of function pointers and the memory access as Helpers. Both forward
// to the classes of Core/ (ffb::Wheel, ffb::Memory, ffb::Log).
#include <Windows.h>
#include "MinHook.h"
#include "SDL.h"
#include "../Core/Game.h"

struct EffectTriggers {
	void(*Constant)(int direction, double strength);
	// Constant force that keeps playing until the next call, which updates it in place (a strength of 0 stops it)
	void(*ConstantInf)(int direction, double strength);
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

class EffectConstants {
public:
	// Haptic forces are defined by the direction that generates the force
	// e.g. DIRECTION_FROM_LEFT causes wheel to go right.
	const int DIRECTION_FROM_LEFT = -1;
	const int DIRECTION_FROM_RIGHT = 1;
};

// Reads an integer from the [Settings] section of FFBPlugin.ini
int GetSettingInt(LPCWSTR name, int defaultValue);

// Rumble selected by a HowtoRumbleXxxEffect setting: 0 both motors, 1 left motor only, 2 right motor only
void RumbleWithMode(void(*rumble)(double lowfrequency, double highfrequency, UINT32 length_ms), int mode, double strength, UINT32 length_ms);

// Plays the damper effect set by the EnableDamper and DamperStrength settings
void ApplyDamper(EffectTriggers* triggers);

class Helpers {
public:
	int enableLogging = 0; // unused: ffb::Log reads the Logging setting
	// logging
	void log(const char *msg);
	void logInt(int value);
	// reading memory
	LPVOID GetTranslatedOffset(INT_PTR offset);

	// isRelativeOffset: offset is relative to the main module base address instead of absolute.
	template <typename T>
	T Read(INT_PTR offset, bool isRelativeOffset)
	{
		return ffb::Memory::Read<T>(ResolveOffset(offset, isRelativeOffset));
	}

	template <typename T>
	T Write(INT_PTR offset, T val, bool isRelativeOffset)
	{
		ffb::Memory::Write<T>(ResolveOffset(offset, isRelativeOffset), val);
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
	INT_PTR ResolveOffset(INT_PTR offset, bool isRelativeOffset)
	{
		return isRelativeOffset ? (INT_PTR)GetTranslatedOffset(offset) : offset;
	}
};

// A game with the FFBLoop interface
class Game : public ffb::Game {
public:
	void Update() override;
	virtual void FFBLoop(EffectConstants *constants, Helpers *helpers, EffectTriggers *triggers);

protected:
	// the functions above, hidden by the members of ffb::Game of the same name
	static void ApplyDamper(EffectTriggers* triggers) { ::ApplyDamper(triggers); }
	static void RumbleWithMode(void(*rumble)(double lowfrequency, double highfrequency, UINT32 length_ms), int mode, double strength, UINT32 length_ms)
	{
		::RumbleWithMode(rumble, mode, strength, length_ms);
	}
};
