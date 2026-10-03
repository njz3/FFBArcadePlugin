#include <Windows.h>
#include <fstream>
#include <string>
#include "Game.h"

typedef unsigned char U8;

int GetSettingInt(LPCWSTR name, int defaultValue)
{
	return GetPrivateProfileInt(TEXT("Settings"), name, defaultValue, TEXT(".\\FFBPlugin.ini"));
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

void Helpers::log(const char *msg) {
	if (enableLogging == 0) { return; }
	std::ofstream ofs("FFBlog.txt", std::ofstream::app);
	ofs << msg << std::endl;
}

void Helpers::logInt(int value) {
	log(std::to_string(value).c_str());
}

// reading memory
LPVOID Helpers::GetTranslatedOffset(INT_PTR offset)
{
	return reinterpret_cast<LPVOID>((INT_PTR)GetModuleHandle(NULL) + offset);
}

UINT8 Helpers::WriteNop(INT_PTR offset, int countBytes, bool isRelativeOffset)
{
	const U8 nop = 0x90;
	for (int i = 0; i < countBytes; i++)
		WriteByte(offset + i, nop, isRelativeOffset);
	return nop;
}

void Game::FFBLoop(EffectConstants * constants, Helpers * helpers, EffectTriggers * triggers)
{
	return;
}
