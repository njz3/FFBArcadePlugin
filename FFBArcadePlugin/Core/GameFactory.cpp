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

// Creates the game of the GameId setting
#include "GameFactory.h"
#include "../Common Files/GameIds.h"
#include "../GameFiles/TestGame.h"
#include "../GameFiles/AliensExtermination.h"
#include "../GameFiles/Batman.h"
#include "../GameFiles/BG4JP.h"
#include "../GameFiles/Cars.h"
#include "../GameFiles/ChaseHQ2.h"
#include "../GameFiles/CrazyTaxi.h"
#include "../GameFiles/CruisnBlast.h"
#include "../GameFiles/D1GP.h"
#include "../GameFiles/Daytona3.h"
#include "../GameFiles/Daytona3NSE.h"
#include "../GameFiles/DeadHeat.h"
#include "../GameFiles/DeadHeatRiders.h"
#include "../GameFiles/DirtyDrivin.h"
#include "../GameFiles/FnF.h"
#include "../GameFiles/FnFDrift.h"
#include "../GameFiles/FnFSuperCars.h"
#include "../GameFiles/FordRacing.h"
#include "../GameFiles/FordRacingOther.h"
#include "../GameFiles/GaelcoTuningRace.h"
#include "../GameFiles/GRIDReal.h"
#include "../GameFiles/GRIDCustom.h"
#include "../GameFiles/GoldenGun.h"
#include "../GameFiles/Hummer.h"
#include "../GameFiles/HummerExtreme.h"
#include "../GameFiles/InitialD0v131.h"
#include "../GameFiles/InitialD0v211.h"
#include "../GameFiles/InitialD0v230.h"
#include "../GameFiles/InitialD4.h"
#include "../GameFiles/InitialD4Japan.h"
#include "../GameFiles/InitialD5.h"
#include "../GameFiles/InitialD6.h"
#include "../GameFiles/InitialD7.h"
#include "../GameFiles/InitialD8.h"
#include "../GameFiles/IDTAv231.h"
#include "../GameFiles/MarioKartGPDXCustom.h"
#include "../GameFiles/MarioKartGPDX1.10Custom.h"
#include "../GameFiles/MarioKartGPDX1.10Real.h"
#include "../GameFiles/MarioKartGPDX1.18Custom.h"
#include "../GameFiles/MarioKartGPDX1.18Real.h"
#include "../GameFiles/MarioKartGPDXUSACustom.h"
#include "../GameFiles/MarioKartGPDXUSAReal.h"
#include "../GameFiles/MAMESupermodel.h"
#include "../GameFiles/OutRun2Fake.h"
#include "../GameFiles/OutRun2Real.h"
#include "../GameFiles/SegaRaceTV.h"
#include "../GameFiles/SegaRacingClassic.h"
#include "../GameFiles/SegaRally3.h"
#include "../GameFiles/SegaRally3Other.h"
#include "../GameFiles/Showdown.h"
#include "../GameFiles/SnoCross.h"
#include "../GameFiles/WackyRaces.h"
#include "../GameFiles/WastelandRacers2071.h"
#include "../GameFiles/WMMT3.h"
#include "../GameFiles/WMMT5.h"
#include "../GameFiles/WMMT5DX.h"
#include "../GameFiles/WMMT5DX+.h"
#include "../GameFiles/WMMT6.h"
#include "../GameFiles/WMMT6R.h"
#include "../GameFiles/WMMT6RR.h"
#include "../GameFiles/Machstorm.h"
#include "../GameFiles/AfterburnerClimax.h"
#include "../GameFiles/PokkenTournament.h"
#include "../GameFiles/SonicSegaAllStarsRacing.h"
#include "../GameFiles/SpringEffect.h"
#include "../GameFiles/M2Emulator.h"
#include "../GameFiles/GTIClub3.h"
#include "../GameFiles/Demul.h"
#include "../GameFiles/ButtonRumble.h"
#include "../GameFiles/RingRiders.h"
#include "../GameFiles/RoadFighters3D.h"
#include "../GameFiles/LGI3D.h"
#include "../GameFiles/LGI.h"
#include "../GameFiles/KODrive.h"
#include "../GameFiles/HOTD4.h"
#include "../GameFiles/Rambo.h"
#include "../GameFiles/R-Tuned.h"
#include "../GameFiles/SWDC2018.h"
#include "../GameFiles/TokyoCop.h"
#include "../GameFiles/Transformers.h"
#include "../GameFiles/H2Overdrive.h"
#include "../GameFiles/StormRacerG.h"
#include "../GameFiles/ArcticThunder.h"

ffb::Game* CreateGame(int gameId)
{
	switch (gameId) {
	case AFTER_BURNER_CLIMAX:
		return new AfterburnerClimax;
	case CHASE_HQ_2:
		return new ChaseHQ2;
	case CARS:
		return new Cars;
	case DAYTONA_3:
		return new Daytona3;
	case DAYTONA_3_NSE:
		return new Daytona3NSE;
	case MAME_:
	case SUPERMODEL_:
	case FLYCAST:
		return new MAMESupermodel;
	case FORD_RACING:
		return new FordRacing;
	case FORD_RACING_OTHER:
		return new FordRacingOther;
	case GRID_Real:
		return new GRIDReal;
	case Golden_Gun:
		return new GoldenGun;
	case Transformers_:
		return new Transformers;
	case INITIAL_D_0:
		return new InitialD0;
	case INITIAL_D_0_211:
		return new InitialD0v211;
	case INITIAL_D_0_230:
		return new InitialD0v230;
	case INITIAL_D_4:
		return new InitialD4;
	case INITIAL_D_4_Japan:
		return new InitialD4Japan;
	case INITIAL_D_5:
		return new InitialD5;
	case INITIAL_D_6:
		return new InitialD6;
	case INITIAL_D_7:
		return new InitialD7;
	case INITIAL_D_8:
		return new InitialD8;
	case SEGA_RACING_CLASSIC:
		return new SegaRacingClassic;
	case SEGA_RALLY_3:
		return new SegaRally3;
	case SEGA_RALLY_3_Other:
		return new SegaRally3Other;
	case WACKY_RACES:
		return new WackyRaces;
	case WMMT_5:
		return new WMMT5;
	case WMMT_5DX:
		return new WMMT5DX;
	case WMMT_5DXPlus:
		return new WMMT5DXPlus;
	case MARIO_KART_GPDX_CUSTOM:
		return new MarioKartGPDX100Custom;
	case OUTRUN_2Fake:
		return new OutRun2Fake;
	case OUTRUN_2Real:
		return new OutRun2Real;
	case BG4_JP:
		return new BG4JP;
	case MACH_STORM:
		return new Machstorm;
	case POKKEN_TOURNAMENT:
		return new PokkenTournament;
	case RAMBO_:
		return new Rambo;
	case MARIO_KART_GPDX_110_CUSTOM:
		return new MarioKartGPDX110Custom;
	case MARIO_KART_GPDX_118_CUSTOM:
		return new MarioKartGPDX118Custom;
	case MARIO_KART_GPDX_USA_CUSTOM:
		return new MarioKartGPDXUSACustom;
	case Sonic_Sega_AllStars_Racing:
		return new SonicSegaAllStarsRacing;
	case M2_Emulator:
		return new M2Emulator;
	case Demul_Emulator:
		return new Demul;
	case ALIENS_EXTERMINATION:
		return new AliensExtermination;
	case GTI_Club_3:
		return new GTIClub3;
	case Button_Rumble:
		return new ButtonRumble;
	case Road_Fighters_3D:
		return new RoadFighters3D;
	case LGI_3D:
		return new LGI3D;
	case LGI_:
		return new LGI;
	case KO_Drive:
		return new KODrive;
	case HOTD_4:
		return new HOTD4;
	case Dirty_Drivin:
		return new DirtyDrivin;
	case H2_Overdrive:
		return new H2Overdrive;
	case Sno_Cross:
		return new SnoCross;
	case Bat_man:
		return new Batman;
	case R_Tuned:
		return new RTuned;
	case SWDC_2018:
		return new SWDC;
	case WMMT_6:
		return new WMMT6;
	case WMMT_6_R:
		return new WMMT6R;
	case WMMT_6_RR:
		return new WMMT6RR;
	case SRG:
		return new StormRacerG;
	case D1_GP:
		return new D1GP;
	case Crazy_Taxi:
		return new CrazyTaxi;
	case WMMT_3:
		return new WMMT3;
	case DEAD_HEAT:
		return new DeadHeat;
	case DEAD_HEAT_RIDERS:
		return new DeadHeatRiders;
	case GAELCO_TUNING_RACE:
		return new GaelcoTuningRace;
	case TOKYO_COP:
		return new TokyoCop;
	case RING_RIDERS:
		return new RingRiders;
	case MARIO_KART_GPDX_118_REAL:
		return new MarioKartGPDX118Real;
	case MARIO_KART_GPDX_110_REAL:
		return new MarioKartGPDX110Real;
	case MARIO_KART_GPDX_USA_REAL:
		return new MarioKartGPDXUSAReal;
	case GRID_Custom:
		return new GRIDCustom;
	case SEGA_SHOWDOWN:
		return new Showdown;
	case SPRING_EFFECT:
		return new SpringEffect;
	case ARCTIC_THUNDER:
		return new ArcticThunder;
	case CRUISN_BLAST:
		return new CruisnBlast;
	case FNF:
		return new FnF;
	case FNF_DRIFT:
		return new FnFDrift;
	case FNF_SUPERCARS:
		return new FnFSuperCars;
	case SEGA_RACE_TV:
		return new SegaRaceTV;
	case HUMMER:
		return new Hummer;
	case HUMMER_EXTREME:
		return new HummerExtreme;
	case INITIAL_D_THEARCADE_V231:
		return new InitialDTA231;
	case WASTELAND_RACERS_2071:
		return new WasteLandRacers2071;
	case TEST_GAME_CONST:
	case TEST_GAME_FRICTION:
	case TEST_GAME_SINE:
	case TEST_GAME_SPRING:
	case TEST_GAME_HEAVY:
	case TEST_GAME_LOOSE:
	case TEST_GAME_RUMBLE:
		return new TestGame;
	default:
		return nullptr;
	}
}
