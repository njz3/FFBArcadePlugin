#pragma once
#include "Game.h"

// Scaling of the drive board values, which differs between games
struct InitialDFFBSettings
{
	double strengthScale;  // strength of a full force
	int leftStrengthBase;  // a constant force to the left is (leftStrengthBase - strength) / strengthScale
	double periodScale;    // a vibration period is param / periodScale * 120 ms
	UINT32 sineLength_ms;  // length of the vibrations sine effect
	UINT32 maxStrength;    // spring and constant force commands with a higher strength are ignored
};

// Settings used by most games
const InitialDFFBSettings InitialDFFBDefaultSettings = { 127.0, 128, 127.0, 100, 0xFFFFFFFF };

// Plays a FFB command of the drive board used by Initial D and other Sega games:
// 0x80 centering (strength 1), 0x84 constant force (param is the direction), 0x85 vibration (param is the period), 0x86 spring
void PlayInitialDFFB(BYTE command, BYTE param, UINT32 strength, const InitialDFFBSettings& settings, EffectConstants* constants, EffectTriggers* triggers);
