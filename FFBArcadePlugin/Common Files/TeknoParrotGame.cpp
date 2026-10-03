#include <initializer_list>
#include "TeknoParrotGame.h"
#include "GameIds.h"

extern int configGameId;

// FFB values published by TeknoParrot in the JvsState shared memory
static const int FFB_SLOT = 2;
static const int FFB_EXTENDED_SLOT = 6; // followed by 3 more values in slots 7, 8 and 9

// Games publishing their main FFB value in the extended slot instead of the standard one
static bool UsesExtendedFFBSlot()
{
	return configGameId == POKKEN_TOURNAMENT || configGameId == SRG || configGameId == D1_GP || configGameId == WMMT_3 || configGameId == DEAD_HEAT;
}

TeknoParrotGame::TeknoParrotGame()
{
	jvsState = nullptr;
	hSection = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, 64, L"TeknoParrot_JvsState");
	if (hSection)
		jvsState = (int*)MapViewOfFile(hSection, FILE_MAP_ALL_ACCESS, 0, 0, 64);

	if (jvsState)
	{
		for (int slot : { FFB_SLOT, FFB_EXTENDED_SLOT, FFB_EXTENDED_SLOT + 1, FFB_EXTENDED_SLOT + 2, FFB_EXTENDED_SLOT + 3 })
		{
			if (jvsState[slot] > 0)
				jvsState[slot] = 0;
		}
	}
}

int TeknoParrotGame::ReadSlot(int slot)
{
	return jvsState ? jvsState[slot] : 0;
}

int TeknoParrotGame::GetTeknoParrotFFB()
{
	return ReadSlot(UsesExtendedFFBSlot() ? FFB_EXTENDED_SLOT : FFB_SLOT);
}

int TeknoParrotGame::GetTeknoParrotFFB2()
{
	return ReadSlot(FFB_EXTENDED_SLOT + 1);
}

int TeknoParrotGame::GetTeknoParrotFFB3()
{
	return ReadSlot(FFB_EXTENDED_SLOT + 2);
}

int TeknoParrotGame::GetTeknoParrotFFB4()
{
	return ReadSlot(FFB_EXTENDED_SLOT + 3);
}

void TeknoParrotGame::FFBLoop(EffectConstants *constants, Helpers *helpers, EffectTriggers* triggers) {
	helpers->log("TeknoParrot game not implemented");
}