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

#include <string.h>
#include "EmulatorGames.h"
#include "../Core/ForceProfile.h"
#include "../Hardware/Namco.h"
#include "../Hardware/Sega.h"
#include "../Hardware/SegaInitialD.h"
#include "../Hardware/Vibration.h"
#include "../Hardware/WheelMotors.h"

using namespace ffb;

namespace
{
	// the settings of the games (ForceProfile::Field)
	const unsigned Force = ForceProfile::Force;
	const unsigned Alternative = ForceProfile::Alternative;
	const unsigned FeedbackLength = ForceProfile::FeedbackLength;
	const unsigned PowerMode = ForceProfile::PowerMode;
	const unsigned Spring = ForceProfile::Spring;
	const unsigned Sine = ForceProfile::Sine;
	const unsigned Rumble = ForceProfile::Rumble;
	const unsigned Device2 = ForceProfile::Device2;
	const unsigned Device2Alternative = ForceProfile::Device2Alternative;
	const unsigned Device3 = ForceProfile::Device3;
	const unsigned Doubles = ForceProfile::Doubles;
	const unsigned FullSine = ForceProfile::FullSine;
	const unsigned HoldsAtZero = ForceProfile::HoldsAtZero;
	const unsigned Racing = ForceProfile::Racing;

	template <class Board>
	OutputBoard* Make() { return new Board; }

	OutputBoard* GunRecoil() { return new OnOffMotors(OnOffMotors::Kind::GunRecoil); }
	OutputBoard* WheelVibration() { return new OnOffMotors(OnOffMotors::Kind::WheelVibration); }
	OutputBoard* JalecoVibration() { return new OnOffMotors(OnOffMotors::Kind::JalecoVibration); }
	OutputBoard* SeatVibration() { return new OnOffMotors(OnOffMotors::Kind::SeatVibration); }
}

// roms, suffix of the settings, settings, force feedback hardware
static const EmulatorGame games[] = {
	// Supermodel
	{ { "dayto2pe", "daytona2" }, L"Daytona2", Racing, Make<Daytona2DriveBoard> },
	{ { "scud", "scuda", "scudau", "scuddx", "scudj", "scudplus", "scudplusa" }, L"Scud", Racing, Make<ScudRaceDriveBoard> },
	{ { "lemans24" }, L"LeMans", Racing, Make<ScudRaceDriveBoard> },
	{ { "dirtdvlsa", "dirtdvls", "dirtdvlsj", "dirtdvlsg", "dirtdvlsu", "dirtdvlsau" }, L"DirtDevils", Racing, Make<ScudRaceDriveBoard> },
	{ { "ecau", "eca", "ecap", "ecaj" }, L"ECA", Racing, Make<ScudRaceDriveBoard> },
	{ { "srally2", "srally2x", "srally2dx", "srally2p", "srally2pa" }, L"SRally2", Racing, Make<SegaRally2DriveBoard> },
	// MAME
	{ { "cartfury" }, L"CartFury", Racing, Make<SignedWheelMotor> },
	{ { "overrev", "overrevba", "overrevb" }, L"OverRev", Racing, Make<Model2DriveBoard> },
	{ { "vr" }, L"VirtuaRacing", Racing & ~PowerMode, Make<VirtuaRacingDriveBoard> },
	{ { "smgp", "smgp6", "smgp5", "smgpu", "smgpu1", "smgpu2", "smgpj", "smgpja", "smgpd", "smgp6d", "smgp5d", "smgpud", "smgpu1d", "smgpu2d", "smgpjd" }, L"SuperMonacoGP", Force | Alternative | FeedbackLength | Spring | Sine | FullSine, Make<SuperMonacoGPShaker> },
	{ { "roadedge" }, L"RoadEdge", Racing, Make<Hng64WheelMotor> },
	{ { "xrally" }, L"XRally", Racing, Make<Hng64WheelMotor> },
	{ { "thrilld", "thrillda", "thrilldj", "thrillde", "thrilldgej", "thrilldgeu", "thrilldgke", "thrilldgkk", "thrilldgmj", "thrilldgnj", "thrilldgpj", "thrilldgkee" }, L"ThrillD", Racing, Make<KonamiWheelMotor> },
	{ { "windheat", "windheata", "windheatj", "windheatu" }, L"Windheat", Racing, Make<KonamiWheelMotor> },
	{ { "gticlub", "gticluba", "gticlubj", "gticlubu" }, L"GTIClub", Racing, Make<KonamiWheelMotor> },
	{ { "midnrun", "midnruna", "midnrunj", "midnruna2" }, L"MidnRun", Racing, Make<KonamiWheelMotor> },
	{ { "racingj", "racingja", "racingje", "racingjj", "racingju" }, L"RacingJ", Racing, Make<KonamiWheelMotor> },
	{ { "racingj2", "racingj2a", "racingj2e", "racingj2j", "racingj2u" }, L"RacingJ2", Racing, Make<KonamiWheelMotor> },
	{ { "sfrush", "sfrusha" }, L"SFRush", Racing, Make<ReversedSignedWheelMotor> },
	{ { "sfrushrk", "sfrushrkwo" }, L"SFRushRock", Racing, Make<ReversedSignedWheelMotor> },
	{ { "crusnwld", "crusnwld24", "crusnwld23", "crusnwld20", "crusnwld19", "crusnwld17" }, L"CrusnWld", Racing, Make<ReversedSignedWheelMotor> },
	{ { "superchs", "superchsj", "superchsp", "superchsu" }, L"SuperChase", Racing, Make<SuperChaseWheelMotor> },
	{ { "offroadc", "offroadc4", "offroadc3", "offroadc1" }, L"OffRoadC", Racing, Make<ReversedSignedWheelMotor> },
	{ { "crusnusa", "crusnusa40", "crusnusa21" }, L"CrusnUSA", Racing, Make<ReversedSignedWheelMotor> },
	{ { "crusnexo" }, L"CrusnExotica", Racing, Make<CruisnExoticaWheelMotor> },
	{ { "calspeed", "calspeeda", "calspeedb" }, L"CalSpeed", Racing | HoldsAtZero, Make<SignedWheelMotor> },
	{ { "sf2049", "sf2049se", "sf2049te" }, L"SFRush2049", Racing, Make<SignedWheelMotor> },
	{ { "harddriv1", "harddrivb6", "harddrivb", "harddrivg4", "harddrivg", "harddrivj6", "harddrivj", "harddriv2", "harddriv3", "harddriv", "harddrivcb", "harddrivcg", "harddrivc1", "harddrivc", "hdrivairp", "hdrivair", "racedrivb1", "racedrivb4", "racedrivb", "racedrivg1", "racedrivg4", "racedrivg", "racedriv1", "racedriv2", "racedriv3", "racedriv4", "racedriv", "racedrivcb4", "racedrivcb", "racedrivcg4", "racedrivcg", "racedrivc2", "racedrivc4", "racedrivc", "racedrivpan" }, L"HardD", Racing, Make<HardDrivinWheelMotor> },
	{ { "revx", "revxp5" }, L"RevX", Force | Rumble | Device2 | Device3, GunRecoil },
	{ { "zombraid", "zombraidpj", "zombraidp" }, L"ZombRaid", Force | Rumble | Device2, GunRecoil },
	{ { "bbusters", "bbustersu", "bbustersua", "bbustersj" }, L"BBusters", Force | Rumble | Device2 | Device3, GunRecoil },
	{ { "mechatt", "mechattu", "mechattu1", "mechattj" }, L"Mechatt", Force | Rumble | Device2, GunRecoil },
	{ { "tshoot" }, L"TShoot", Force | Rumble | Device2, GunRecoil },
	{ { "gunbustru", "gunbustr", "gunbustrj" }, L"GunBuster", Force | Rumble | Device2, GunRecoil },
	{ { "alien3", "alien3u" }, L"Alien3", Force | Rumble | Device2, GunRecoil },
	{ { "ptblank2", "ptblank2ua" }, L"PointBlank2", Force | Rumble | Device2, GunRecoil },
	{ { "ghlpanic" }, L"GhoulPanic", Force | Rumble | Device2, GunRecoil },
	{ { "spacegun", "spacegunu", "spacegunj" }, L"Spacegun", Force | Rumble | Device2, GunRecoil },
	{ { "term2", "term2la1", "term2la2", "term2la3", "term2lg1" }, L"Term2", Force | Rumble | Device2, GunRecoil },
	{ { "rchase", "rchasej" }, L"Rchase", Force | Rumble | Device2, GunRecoil },
	{ { "lghost", "lghostd", "lghostu", "lghostud", "lghostj" }, L"LGhost", Force | Rumble | Device2 | Device3, GunRecoil },
	{ { "timecris", "timecrisa" }, L"TimeCris", Force | Rumble, GunRecoil },
	{ { "othunder", "othundero", "othunderuo", "othunderu", "othunderj" }, L"OThunder", Force | Rumble | Device2, GunRecoil },
	{ { "opwolf", "opwolfp", "opwolfj", "opwolfu", "opwolfa" }, L"OpWolf", Force | Rumble | Device2, GunRecoil },
	{ { "undrfire", "undrfireu", "undrfirej" }, L"UFire", Force | Rumble | Device2, GunRecoil },
	{ { "orunners", "orunnersu", "orunnersj" }, L"ORunners", Force | Alternative | FeedbackLength | Spring | Sine | Rumble | Device2 | Device2Alternative, WheelVibration },
	{ { "toutrun", "toutrund", "toutrunj", "toutrunjd" }, L"TOutrun", Force | Alternative | FeedbackLength | Spring | Sine | Rumble, WheelVibration },
	{ { "cbombers", "cbombersj", "cbombersp" }, L"CBombers", Force | Alternative | FeedbackLength | Spring | Sine | Rumble, WheelVibration },
	{ { "outrunra", "outrun", "outruneh" }, L"Outrun", Force | Alternative | FeedbackLength | Spring | Sine | Rumble, Make<OutRunMotors> },
	{ { "pdrift", "pdrifta", "pdrifte", "pdriftj", "pdriftl" }, L"PDrift", Force | Alternative | FeedbackLength | Spring | Sine | Rumble, Make<PowerDriftMotors> },
	{ { "dblaxle", "dblaxleu" }, L"DAxle", Force | Alternative | FeedbackLength | Spring | Sine | Rumble, WheelVibration },
	{ { "aburner2", "aburner2g" }, L"AfterBurner2", Force | Rumble, SeatVibration },
	{ { "cischeat" }, L"CisHeat", Force | Alternative | FeedbackLength | Spring | Sine | Rumble, JalecoVibration },
	{ { "f1gpstar", "f1gpstaro" }, L"F1GpStar", Force | Alternative | FeedbackLength | Spring | Sine | Rumble, JalecoVibration },
	{ { "f1gpstr2" }, L"F1GpStar2", Force | Alternative | FeedbackLength | Spring | Sine | Rumble, JalecoVibration },
	{ { "hyprdriv" }, L"HyperDrive", Racing & ~PowerMode, Make<SignedWheelMotor> },
	{ { "vaportrx", "vaportrp" }, L"VaporTrx", Racing & ~PowerMode, Make<SignedWheelMotor> },
	{ { "raveracw", "raveracj", "raveracja", "raveracej", "raveraceja", "raverace" }, L"RaveRacer", Racing, Make<RaveRacerWheelMotor> },
	{ { "daytona", "daytonas", "daytonase" }, L"Daytona", Racing, Make<Model2DriveBoard> },
	{ { "indy500", "indy500d", "indy500to" }, L"Indy500", Racing, Make<Model2DriveBoard> },
	{ { "sgt24h" }, L"SuperGT", Racing, Make<Model2DriveBoard> },
	{ { "srallyc", "srallycb", "srallycdx", "srallycdxa" }, L"SRally", Racing, Make<SegaRallyDriveBoard> },
	{ { "dirtdash", "dirtdasha", "dirtdashj" }, L"DirtDash", Racing, Make<DirtDashWheelMotor> },
	{ { "stcc", "stcca", "stccb" }, L"STCC", Racing, Make<Model2DriveBoard> },
	{ { "victlapw", "victlap" }, L"AceDriverVictory", Racing, Make<AceDriverWheelMotor> },
	{ { "acedrvrw", "acedrive" }, L"AceDriver", Racing, Make<AceDriverWheelMotor> },
	{ { "sidebs" }, L"SideBS", Racing, Make<SideBySideWheelMotor> },
	{ { "sidebs2", "sidebs2j", "sidebs2ja" }, L"SideBS2", Racing, Make<SideBySideWheelMotor> },
	{ { "drivedge" }, L"DriversEdge", Racing, Make<DriversEdgeWheelMotor> },
	{ { "speedup", "speedup10", "speedup12", "speedup20", "speedup20a", "speedup21" }, L"SpeedUp", Racing, Make<SignedWheelMotor> },
	// Flycast
	{ { "INITIAL D\n", "INITIAL D Ver.2\n", "INITIAL D Ver.3\n" }, L"InitialD", Racing | Doubles, Make<InitialDMidiBoard> },
	{ { "THE KING OF ROUTE66\n" }, L"KingOfRoute66", Racing | Doubles, Make<InitialDMidiBoard> },
	{ { "CLUB KART IN JAPAN\n" }, L"ClubKart", Racing | Doubles, Make<InitialDMidiBoard> },
	{ { "SEGA DRIVING SIMULATOR\n" }, L"SegaDrivingSimulator", Racing | Doubles, Make<InitialDMidiBoard> },
	{ { "MAXIMUM SPEED\n" }, L"MaximumSpeed", Racing, Make<MaximumSpeedWheelMotor> },
	{ { "FASTER THAN SPEED\n" }, L"FasterThanSpeed", Racing, Make<MaximumSpeedWheelMotor> },
	{ { "F355 CHALLENGE JAPAN\n" }, L"F355", Racing, Make<NaomiDriveBoard> },
	{ { "18WHEELER\n" }, L"18Wheeler", Racing, Make<NaomiDriveBoard> },
};

const EmulatorGame* FindEmulatorGame(const char* romName)
{
	for (const EmulatorGame& game : games)
	{
		for (const char* rom : game.roms)
		{
			if (strcmp(romName, rom) == 0)
				return &game;
		}
	}
	return nullptr;
}
