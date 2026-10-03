#pragma once
#include "Game.h"

class TeknoParrotGame : public Game {

	// TP-related
	HANDLE hSection;
	int* jvsState; // TeknoParrot_JvsState shared memory, seen as an array of int slots
	int ReadSlot(int slot);

protected:
	int GetTeknoParrotFFB();
	int GetTeknoParrotFFB2();
	int GetTeknoParrotFFB3();
	int GetTeknoParrotFFB4();
	TeknoParrotGame();

public:
	void FFBLoop(EffectConstants *constants, Helpers *helpers, EffectTriggers* triggers);
};