#include <Windows.h>
#include <cstring>
#include <vector>
#include "PersistentValues.h"
#include "../Common Files/GameIds.h"

extern int configGameId;
extern int configMinForce;
extern int configMaxForce;
extern int configAlternativeMinForceLeft;
extern int configAlternativeMaxForceLeft;
extern int configAlternativeMinForceRight;
extern int configAlternativeMaxForceRight;
extern char* romname;
extern char* romnameM2;
extern char* romnameDemul;
extern LPCSTR CustomAlternativeMaxForceLeft;
extern LPCSTR CustomAlternativeMaxForceRight;
extern LPCSTR CustomMaxForce;
extern int EnableFFBStrengthPersistence;

static int defaultMaxForce;
static int defaultAlternativeMaxForceLeft;
static int defaultAlternativeMaxForceRight;

// Settings keys used to persist the max force of the emulated games
struct PersistentMaxForceKeys
{
	int gameId;
	std::vector<const char*> romNames;
	LPCSTR maxForce;
	LPCSTR alternativeMaxForceLeft;
	LPCSTR alternativeMaxForceRight;
	int defaultMaxForce = 100; // restored by the ResetFFBStrength button
};

#define PERSISTENT_KEYS(suffix) "MaxForce" suffix, "AlternativeMaxForceLeft" suffix, "AlternativeMaxForceRight" suffix

static const PersistentMaxForceKeys persistentMaxForceKeys[] = {
	// SUPERMODEL_
	{ SUPERMODEL_, { "dayto2pe", "daytona2" }, PERSISTENT_KEYS("Daytona2") },
	{ SUPERMODEL_, { "scud", "scuda", "scudj", "scudplus", "scudplusa" }, PERSISTENT_KEYS("Scud") },
	{ SUPERMODEL_, { "lemans24" }, PERSISTENT_KEYS("LeMans") },
	{ SUPERMODEL_, { "dirtdvlsa", "dirtdvls", "dirtdvlsj", "dirtdvlsg" }, "MaxForceDirtDevils", "PersistentAlternativeMaxForceLeftDirtDevils", "PersistentAlternativeMaxForceRightDirtDevils" },
	{ SUPERMODEL_, { "srally2", "srally2x", "srally2p", "srally2pa" }, PERSISTENT_KEYS("SRally2") },
	{ SUPERMODEL_, { "ecau", "eca", "ecap", "ecaj" }, PERSISTENT_KEYS("ECA") },
	// MAME_
	{ MAME_, { "vr" }, PERSISTENT_KEYS("VirtuaRacing") },
	{ MAME_, { "sfrush", "sfrusha" }, PERSISTENT_KEYS("SFRush") },
	{ MAME_, { "sfrushrk", "sfrushrkwo" }, PERSISTENT_KEYS("SFRushRock") },
	{ MAME_, { "crusnwld", "crusnwld24", "crusnwld23", "crusnwld20", "crusnwld19", "crusnwld17" }, PERSISTENT_KEYS("CrusnWld") },
	{ MAME_, { "superchs", "superchsj", "superchsp", "superchsu" }, PERSISTENT_KEYS("SuperChase"), 80 },
	{ MAME_, { "offroadc", "offroadc4", "offroadc3", "offroadc1" }, PERSISTENT_KEYS("OffRoadC") },
	{ MAME_, { "crusnusa", "crusnusa40", "crusnusa21" }, PERSISTENT_KEYS("CrusnUSA") },
	{ MAME_, { "calspeed", "calspeeda", "calspeedb" }, PERSISTENT_KEYS("CalSpeed") },
	{ MAME_, { "sf2049", "sf2049se", "sf2049te" }, PERSISTENT_KEYS("SFRush2049") },
	{ MAME_, { "harddriv1", "harddrivb6", "harddrivb", "harddrivg4", "harddrivg", "harddrivj6", "harddrivj", "harddriv2", "harddriv3", "harddriv", "harddrivcb", "harddrivcg", "harddrivc1", "harddrivc", "hdrivairp", "hdrivair", "racedrivb1", "racedrivb4", "racedrivb", "racedrivg1", "racedrivg4", "racedrivg", "racedriv1", "racedriv2", "racedriv3", "racedriv4", "racedriv", "racedrivcb4", "racedrivcb", "racedrivcg4", "racedrivcg", "racedrivc2", "racedrivc4", "racedrivc", "racedrivpan" }, PERSISTENT_KEYS("HardD") },
	{ MAME_, { "orunners", "orunnersu", "orunnersj" }, PERSISTENT_KEYS("ORunners") },
	{ MAME_, { "toutrun", "toutrund", "toutrunj", "toutrunjd" }, PERSISTENT_KEYS("TOutrun") },
	{ MAME_, { "cbombers", "cbombersj", "cbombersp" }, PERSISTENT_KEYS("CBombers") },
	{ MAME_, { "outrunra", "outrun", "outruneh" }, PERSISTENT_KEYS("Outrun") },
	{ MAME_, { "pdrift", "pdrifta", "pdrifte", "pdriftj", "pdriftl" }, PERSISTENT_KEYS("PDrift") },
	{ MAME_, { "cischeat" }, PERSISTENT_KEYS("CisHeat") },
	{ MAME_, { "f1gpstar", "f1gpstaro" }, PERSISTENT_KEYS("F1GpStar") },
	{ MAME_, { "f1gpstr2" }, PERSISTENT_KEYS("F1GpStar2") },
	{ MAME_, { "hyprdriv" }, PERSISTENT_KEYS("HyperDrive") },
	{ MAME_, { "vaportrx", "vaportrp" }, PERSISTENT_KEYS("VaporTrx") },
	{ MAME_, { "raveracw", "raveracj", "raveracja" }, "MaxForceRaveRacer", "PersistentMaxForceLeftRaveRacer", "PersistentMaxForceRightRaveRacer" },
	{ MAME_, { "daytona", "daytonas", "daytonase" }, PERSISTENT_KEYS("Daytona") },
	{ MAME_, { "indy500", "indy500d", "indy500to" }, PERSISTENT_KEYS("Indy500") },
	{ MAME_, { "sgt24h" }, PERSISTENT_KEYS("SuperGT") },
	{ MAME_, { "srallyc", "srallycb", "srallycdx", "srallycdxa" }, PERSISTENT_KEYS("SRally") },
	{ MAME_, { "dirtdash" }, PERSISTENT_KEYS("DirtDash") },
	{ MAME_, { "victlapw" }, PERSISTENT_KEYS("AceDriverVictory") },
	{ MAME_, { "acedrvrw" }, PERSISTENT_KEYS("AceDriver") },
	// M2_Emulator
	{ M2_Emulator, { "Daytona USA", "Daytona USA '93 Edition", "Daytona USA (Saturn Ads)", "Daytona USA Special Edition", "Daytona USA Turbo", "Daytona USA Turbo (Rev A)", "Daytona USA: GTX 2004", "Daytona USA: To The Maxx" }, PERSISTENT_KEYS("Daytona") },
	{ M2_Emulator, { "Sega Rally Championship", "Sega Rally Championship (Rev B)", "Sega Rally Pro Drivin'" }, PERSISTENT_KEYS("SRally") },
	{ M2_Emulator, { "Indianapolis 500 (Rev A, Deluxe)", "Indianapolis 500 (Rev A, Twin, Newer rev)", "Indianapolis 500 (Rev A, Twin, Older rev)" }, PERSISTENT_KEYS("Indy500") },
	{ M2_Emulator, { "Sega Touring Car Championship", "Sega Touring Car Championship (Rev A)", "Sega Touring Car Championship (Rev B)" }, PERSISTENT_KEYS("STCC") },
	{ M2_Emulator, { "Over Rev", "Over Rev (Model 2B)" }, PERSISTENT_KEYS("OverRev") },
	{ M2_Emulator, { "Super GT 24h" }, PERSISTENT_KEYS("SuperGT") },
	// Demul_Emulator
	{ Demul_Emulator, { "Nascar" }, PERSISTENT_KEYS("NascarRacing") },
	{ Demul_Emulator, { "Initial D Arcade Stage" }, PERSISTENT_KEYS("InitialDDemul") },
	{ Demul_Emulator, { "Smashing Drive" }, PERSISTENT_KEYS("SmashingDrive") },
	{ Demul_Emulator, { "Maximum Speed" }, PERSISTENT_KEYS("MaximumSpeed") },
	{ Demul_Emulator, { "Faster Than Speed" }, PERSISTENT_KEYS("FasterSpeed") },
	{ Demul_Emulator, { "ATV Track" }, PERSISTENT_KEYS("ATVTrack") },
};

// Emulators have a max force setting per game
static bool HasPersistentKeysPerRom()
{
	return configGameId == SUPERMODEL_ || configGameId == MAME_ || configGameId == M2_Emulator || configGameId == Demul_Emulator;
}

static const char* CurrentRomName()
{
	switch (configGameId)
	{
	case MAME_:
	case SUPERMODEL_:
		return romname;
	case M2_Emulator:
		return romnameM2;
	case Demul_Emulator:
		return romnameDemul;
	default:
		return nullptr;
	}
}

static const PersistentMaxForceKeys* FindPersistentMaxForceKeys()
{
	const char* rom = CurrentRomName();
	if (rom == nullptr)
		return nullptr;

	for (const PersistentMaxForceKeys& keys : persistentMaxForceKeys)
	{
		if (keys.gameId != configGameId)
			continue;

		for (const char* romName : keys.romNames)
		{
			if (strcmp(rom, romName) == 0)
				return &keys;
		}
	}
	return nullptr;
}

static int PersistentDefaultMaxForce()
{
	if (configGameId == DAYTONA_3)
		return 90;

	if (configGameId == WACKY_RACES || configGameId == CHASE_HQ_2 || configGameId == Road_Fighters_3D)
		return 75;

	if (configGameId == MAME_)
	{
		const PersistentMaxForceKeys* keys = FindPersistentMaxForceKeys();
		if (keys != nullptr)
			return keys->defaultMaxForce;
	}
	return 100;
}

void DefaultConfigValues()
{
	if (EnableFFBStrengthPersistence == 1)
	{
		int maxForce = PersistentDefaultMaxForce();
		configMinForce = 0;
		configMaxForce = maxForce;
		configAlternativeMinForceLeft = 0;
		configAlternativeMaxForceLeft = -maxForce;
		configAlternativeMinForceRight = 0;
		configAlternativeMaxForceRight = maxForce;
	}
	else
	{
		configMaxForce = defaultMaxForce;
		configAlternativeMaxForceLeft = defaultAlternativeMaxForceLeft;
		configAlternativeMaxForceRight = defaultAlternativeMaxForceRight;
	}
}

void CustomFFBStrengthSetup()
{
	defaultMaxForce = configMaxForce;
	defaultAlternativeMaxForceLeft = configAlternativeMaxForceLeft;
	defaultAlternativeMaxForceRight = configAlternativeMaxForceRight;

	if (!HasPersistentKeysPerRom())
	{
		CustomMaxForce = "MaxForce";
		CustomAlternativeMaxForceLeft = "AlternativeMaxForceLeft";
		CustomAlternativeMaxForceRight = "AlternativeMaxForceRight";
		return;
	}

	const PersistentMaxForceKeys* keys = FindPersistentMaxForceKeys();
	if (keys != nullptr)
	{
		CustomMaxForce = keys->maxForce;
		CustomAlternativeMaxForceLeft = keys->alternativeMaxForceLeft;
		CustomAlternativeMaxForceRight = keys->alternativeMaxForceRight;
	}
}
