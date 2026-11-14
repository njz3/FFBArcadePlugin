#include <Windows.h>
#include <string>
#include "Game.h"

int GetSettingInt(LPCWSTR name, int defaultValue)
{
	return ffb::Settings::Int(name, defaultValue);
}

void RumbleWithMode(void(*rumble)(double lowfrequency, double highfrequency, UINT32 length_ms), int mode, double strength, UINT32 length_ms)
{
	if (mode == 0)
		rumble(strength, strength, length_ms);
	else if (mode == 1)
		rumble(0, strength, length_ms);
	else if (mode == 2)
		rumble(strength, 0, length_ms);
}

void ApplyDamper(EffectTriggers* triggers)
{
	extern int EnableDamper;
	extern int DamperStrength;

	if (EnableDamper)
		triggers->Damper(DamperStrength / 100.0);
}

void Helpers::log(const char *msg)
{
	ffb::Log::Write(msg);
}

void Helpers::logInt(int value)
{
	log(std::to_string(value).c_str());
}

LPVOID Helpers::GetTranslatedOffset(INT_PTR offset)
{
	return reinterpret_cast<LPVOID>(ffb::Memory::ModuleBase() + offset);
}

UINT8 Helpers::WriteNop(INT_PTR offset, int countBytes, bool isRelativeOffset)
{
	ffb::Memory::WriteNop(ResolveOffset(offset, isRelativeOffset), countBytes);
	return 0x90;
}

// The effects of the FFBLoop interface, played on the devices of Core/
namespace
{
	using ffb::Direction;

	void Constant(int direction, double strength) { ffb::MainWheel().Constant((Direction)direction, strength); }
	void ConstantInf(int direction, double strength) { ffb::MainWheel().KeptConstant((Direction)direction, strength); }
	void Spring(double strength) { ffb::MainWheel().Spring(strength); }
	void Friction(double strength) { ffb::MainWheel().Friction(strength); }
	void Sine(UINT16 period, UINT16 fade, double strength, UINT32 length) { ffb::MainWheel().Sine(period, fade, strength, length); }
	void SineDevice2(UINT16 period, UINT16 fade, double strength, UINT32 length) { ffb::SecondDevice().Sine(period, fade, strength, length); }
	void SineDevice3(UINT16 period, UINT16 fade, double strength, UINT32 length) { ffb::ThirdDevice().Sine(period, fade, strength, length); }
	void Rumble(double a, double b, UINT32 length) { ffb::MainWheel().Rumble(a, b, length); }
	void RumbleDevice2(double a, double b, UINT32 length) { ffb::SecondDevice().Rumble(a, b, length); }
	void RumbleDevice3(double a, double b, UINT32 length) { ffb::ThirdDevice().Rumble(a, b, length); }
	void RumbleTriggers(double left, double right, UINT32 length) { ffb::MainWheel().RumbleTriggers(left, right, length); }
	void LeftRight(double smallStrength, double largeStrength, UINT32 length) { ffb::MainWheel().LeftRight(smallStrength, largeStrength, length); }
	void LeftRightDevice2(double smallStrength, double largeStrength, UINT32 length) { ffb::SecondDevice().LeftRight(smallStrength, largeStrength, length); }
	void Springi(double strength) { ffb::MainWheel().KeptSpring(strength); }
	void Inertia(double strength) { ffb::MainWheel().Inertia(strength); }
	void Ramp(double start, double end, UINT32 length) { ffb::MainWheel().Ramp(start, end, length); }
	void Damper(double strength) { ffb::MainWheel().Damper(strength); }
	void SawtoothUp(double strength, UINT32 length) { ffb::MainWheel().SawtoothUp(strength, length); }
	void SawtoothDown(double strength, UINT32 length) { ffb::MainWheel().SawtoothDown(strength, length); }
	void Triangle(double strength, UINT32 length) { ffb::MainWheel().Triangle(strength, length); }

	EffectTriggers legacyTriggers = {
		Constant, ConstantInf, Spring, Friction, Sine, SineDevice2, SineDevice3, Rumble, RumbleDevice2, RumbleDevice3,
		RumbleTriggers, LeftRight, LeftRightDevice2, Springi, Inertia, Ramp, Damper, SawtoothUp, SawtoothDown, Triangle,
	};
	EffectConstants legacyConstants;
	Helpers legacyHelpers;
}

void Game::Update()
{
	FFBLoop(&legacyConstants, &legacyHelpers, &legacyTriggers);
}

void Game::FFBLoop(EffectConstants * constants, Helpers * helpers, EffectTriggers * triggers)
{
	return;
}
