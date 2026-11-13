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

#include "MAMESupermodel.h"
#include "MAMENetworkOutputs.h"
#include "../Common Files/Devices.h"
#include <string>
#include <tchar.h>
#include <atlstr.h>
#include "SDL.h"
#include "../Common Files/SignatureScanning.h"

static char GameName[256];

static EffectConstants* myconstants;
static Helpers* myhelpers;
static EffectTriggers* mytriggers;

//Supermodel Emulator Games
std::string dayto2pe("dayto2pe");
std::string daytona2("daytona2");
std::string dirtdvlsa("dirtdvlsa");
std::string dirtdvlsau("dirtdvlsau");
std::string dirtdvls("dirtdvls");
std::string dirtdvlsu("dirtdvlsu");
std::string dirtdvlsj("dirtdvlsj");
std::string dirtdvlsg("dirtdvlsg");
std::string ecau("ecau");
std::string eca("eca");
std::string ecap("ecap");
std::string ecaj("ecaj");
std::string lemans24("lemans24");
std::string scud("scud");
std::string scuda("scuda");
std::string scuddx("scuddx");
std::string scudau("scudau");
std::string scudj("scudj");
std::string scudplus("scudplus");
std::string scudplusa("scudplusa");
std::string srally2("srally2");
std::string srally2x("srally2x");
std::string srally2dx("srally2dx");
std::string srally2p("srally2p");
std::string srally2pa("srally2pa");

//MAME Games
std::string aburner2("aburner2");
std::string aburner2g("aburner2g");
std::string acedrive("acedrive");
std::string acedrvrw("acedrvrw");
std::string vformula("vformula");
std::string vr("vr");
std::string smgp("smgp");
std::string smgp6("smgp6");
std::string smgp5("smgp5");
std::string smgpu("smgpu");
std::string smgpu1("smgpu1");
std::string smgpu2("smgpu2");
std::string smgpj("smgpj");
std::string smgpja("smgpja");
std::string smgpd("smgpd");
std::string smgp6d("smgp6d");
std::string smgp5d("smgp5d");
std::string smgpud("smgpud");
std::string smgpu1d("smgpu1d");
std::string smgpu2d("smgpu2d");
std::string smgpjd("smgpjd");
std::string sfrush("sfrush");
std::string sfrushrk("sfrushrk");
std::string sfrushrkwo("sfrushrkwo");
std::string sfrusha("sfrusha");
std::string crusnwld("crusnwld");
std::string crusnwld24("crusnwld24");
std::string crusnwld23("crusnwld23");
std::string crusnwld20("crusnwld20");
std::string crusnwld19("crusnwld19");
std::string crusnwld17("crusnwld17");
std::string crusnwld13("crusnwld13");
std::string daytona("daytona");
std::string daytonas("daytonas");
std::string daytonase("daytonase");
std::string dirtdash("dirtdash");
std::string dirtdasha("dirtdasha");
std::string dirtdashj("dirtdashj");
std::string drivedge("drivedge");
std::string offroadc("offroadc");
std::string offroadc4("offroadc4");
std::string offroadc3("offroadc3");
std::string offroadc1("offroadc1");
std::string crusnusa("crusnusa");
std::string crusnusa40("crusnusa40");
std::string crusnusa21("crusnusa21");
std::string crusnexo("crusnexo");
std::string calspeed("calspeed");
std::string calspeeda("calspeeda");
std::string calspeedb("calspeedb");
std::string indy500("indy500");
std::string indy500d("indy500d");
std::string indy500to("indy500to");
std::string outrunra("outrunra");
std::string outrun("outrun");
std::string outruneh("outruneh");
std::string toutrun("toutrun");
std::string toutrund("toutrund");
std::string toutrunj("toutrunj");
std::string toutrunjd("toutrunjd");
std::string pdrift("pdrift");
std::string pdrifta("pdrifta");
std::string pdrifte("pdrifte");
std::string pdriftj("pdriftj");
std::string pdriftl("pdriftl");
std::string orunners("orunners");
std::string orunnersu("orunnersu");
std::string orunnersj("orunnersj");
std::string sf2049("sf2049");
std::string sf2049se("sf2049se");
std::string sf2049te("sf2049te");
std::string harddriv("harddriv");
std::string harddriv1("harddriv1");
std::string harddrivb6("harddrivb6");
std::string harddrivb("harddrivb");
std::string harddrivg4("harddrivg4");
std::string harddrivg("harddrivg");
std::string harddrivj6("harddrivj6");
std::string harddrivj("harddrivj");
std::string harddriv2("harddriv2");
std::string harddriv3("harddriv3");
std::string harddrivcb("harddrivcb");
std::string harddrivcg("harddrivcg");
std::string harddrivc1("harddrivc1");
std::string harddrivc("harddrivc");
std::string hdrivairp("hdrivairp");
std::string hdrivair("hdrivair");
std::string racedrivb1("racedrivb1");
std::string racedrivb4("racedrivb4");
std::string racedrivb("racedrivb");
std::string racedrivg1("racedrivg1");
std::string racedrivg4("racedrivg4");
std::string racedrivg("racedrivg");
std::string racedriv1("racedriv1");
std::string racedriv2("racedriv2");
std::string racedriv3("racedriv3");
std::string racedriv4("racedriv4");
std::string racedriv("racedriv");
std::string racedrivcb4("racedrivcb4");
std::string racedrivcb("racedrivcb");
std::string racedrivcg4("racedrivcg4");
std::string racedrivcg("racedrivcg");
std::string racedrivc2("racedrivc2");
std::string racedrivc4("racedrivc4");
std::string racedrivc("racedrivc");
std::string racedrivpan("racedrivpan");
std::string raverace("raverace");
std::string raveracw("raveracw");
std::string raveracj("raveracj");
std::string raveracja("raveracja");
std::string raveracej("raveracej");
std::string raveraceja("raveraceja");
std::string othunder("othunder");
std::string othundero("othundero");
std::string othunderuo("othunderuo");
std::string othunderu("othunderu");
std::string othunderj("othunderj");
std::string opwolf("opwolf");
std::string opwolfp("opwolfp");
std::string opwolfj("opwolfj");
std::string opwolfu("opwolfu");
std::string opwolfa("opwolfa");
std::string revx("revx");
std::string revxp5("revxp5");
std::string term2("term2");
std::string term2la1("term2la1");
std::string term2pa2("term2pa2");
std::string term2la2("term2la2");
std::string term2la3("term2la3");
std::string term2lg1("term2lg1");
std::string zombraid("zombraid");
std::string zombraidpj("zombraidpj");
std::string zombraidp("zombraidp");
std::string bbusters("bbusters");
std::string bbustersu("bbustersu");
std::string bbustersua("bbustersua");
std::string bbustersj("bbustersj");
std::string mechatt("mechatt");
std::string mechattu("mechattu");
std::string mechattu1("mechattu1");
std::string mechattj("mechattj");
std::string tshoot("tshoot");
std::string gunbustru("gunbustru");
std::string gunbustr("gunbustr");
std::string gunbustrj("gunbustrj");
std::string alien3("alien3");
std::string alien3u("alien3u");
std::string ptblank2("ptblank2");
std::string ptblank2ua("ptblank2ua");
std::string ghlpanic("ghlpanic");
std::string sgt24h("sgt24h");
std::string srallyc("srallyc");
std::string srallycb("srallycb");
std::string srallycdxa("srallycdxa");
std::string srallycdx("srallycdx");
std::string spacegun("spacegun");
std::string spacegunu("spacegunu");
std::string spacegunj("spacegunj");
std::string speedup("speedup");
std::string speedup21("speedup21");
std::string speedup20a("speedup20a");
std::string speedup20("speedup20");
std::string speedup12("speedup12");
std::string speedup10("speedup10");
std::string superchs("superchs");
std::string superchsp("superchsp");
std::string superchsj("superchsj");
std::string superchsu("superchsu");
std::string stcc("stcc");
std::string stcca("stcca");
std::string stccb("stccb");
std::string rchase("rchase");
std::string rchasej("rchasej");
std::string lghost("lghost");
std::string lghostd("lghostd");
std::string lghostu("lghostu");
std::string lghostud("lghostud");
std::string lghostj("lghostj");
std::string timecris("timecris");
std::string timecrisa("timecrisa");
std::string cischeat("cischeat");
std::string f1gpstar("f1gpstar");
std::string f1gpstaro("f1gpstaro");
std::string f1gpstr2("f1gpstr2");
std::string undrfire("undrfire");
std::string undrfireu("undrfireu");
std::string undrfirej("undrfirej");
std::string cbombers("cbombers");
std::string cbombersj("cbombersj");
std::string cbombersp("cbombersp");
std::string hyprdriv("hyprdriv");
std::string vaportrx("vaportrx");
std::string vaportrp("vaportrp");
std::string victlapw("victlapw");
std::string victlap("victlap");
std::string dblaxle("dblaxle");
std::string dblaxleu("dblaxleu");
std::string windheat("windheat");
std::string windheatj("windheatj");
std::string windheatu("windheatu");
std::string windheata("windheata");
std::string gticlub("gticlub");
std::string gticlubj("gticlubj");
std::string gticlubu("gticlubu");
std::string gticluba("gticluba");
std::string midnrun("midnrun");
std::string midnruna("midnruna");
std::string midnruna2("midnruna2");
std::string midnrunj("midnrunj");
std::string roadedge("roadedge");
std::string xrally("xrally");
std::string cartfury("cartfury");
std::string thrilld("thrilld");
std::string thrillda("thrillda");
std::string thrillde("thrillde");
std::string thrilldj("thrilldj");
std::string thrilldgej("thrilldgej");
std::string thrilldgeu("thrilldgeu");
std::string thrilldgke("thrilldgke");
std::string thrilldgkk("thrilldgkk");
std::string thrilldgmj("thrilldgmj");
std::string thrilldgnj("thrilldgnj");
std::string thrilldgpj("thrilldgpj");
std::string thrilldgkee("thrilldgkee");
std::string racingj("racingj");
std::string racingja("racingja");
std::string racingje("racingje");
std::string racingju("racingju");
std::string racingjj("racingjj");
std::string racingj2("racingj2");
std::string racingj2a("racingj2a");
std::string racingj2e("racingj2e");
std::string racingj2u("racingj2u");
std::string racingj2j("racingj2j");
std::string overrev("overrev");
std::string overrevb("overrevb");
std::string overrevba("overrevba");
std::string sidebs("sidebs");
std::string sidebs2("sidebs2");
std::string sidebs2j("sidebs2j");
std::string sidebs2ja("sidebs2ja");

//Flycast Below
std::string InitialDVer1("INITIAL D\n");
std::string InitialDVer2("INITIAL D Ver.2\n");
std::string InitialDVer3("INITIAL D Ver.3\n");
std::string EighteenWheeler("18WHEELER\n");
std::string MaximumSPEED("MAXIMUM SPEED\n");
std::string FasterThanSPEED("FASTER THAN SPEED\n");
std::string F355ChallengeJapan("F355 CHALLENGE JAPAN\n");
std::string ClubKartInJapan("CLUB KART IN JAPAN\n");
std::string TheKingOfRoute66("THE KING OF ROUTE66\n");
std::string SegaDrivingSimulator("SEGA DRIVING SIMULATOR\n");

//Our string to load game from
std::string M2Active("M2Active");
std::string Daytona2Active("Daytona2Active");
std::string ScudRaceActive("ScudRaceActive");
std::string DirtDevilsActive("DirtDevilsActive");
std::string Srally2Active("Srally2Active");
std::string VirtuaRacingActive("VirtuaRacingActive");
std::string SuperMonacoGPActive("SuperMonacoGPActive");
std::string HardDrivinActive("HardDrivinActive");
std::string LightGunActive("LightGunActive");
std::string DirtDashFFB("DirtDashFFB");
std::string NamcoFFBNew("NamcoFFBNew");
std::string RacingFullValueActive1("RacingFullValueActive1");
std::string RacingFullValueActive2("RacingFullValueActive2");
std::string RacingFullValueExotica("RacingFullValueExotica");
std::string RacingActive1("RacingActive1");
std::string RacingActive2("RacingActive2");
std::string AfterburnerActive("AfterburnerActive");
std::string OutrunActive("OutrunActive");
std::string PDriftActive("PDriftActive");
std::string SuperChaseActive("SuperChaseActive");
std::string MaximumSpeedActive("MaximumSpeedActive");
std::string InitialDActive("InitialDActive");
std::string NaomiFFBActive("NaomiFFBActive");
std::string srallynew("srallynew");
std::string m2new("m2new");
std::string RaveRacerNew("RaveRacerNew");
std::string Konami("Konami");
std::string hng64("hng64");
std::string SideBSActive("SideBSActive");
std::string DriversEdgeActive("DriversEdgeActive");

//Names of FFB Outputs
std::string RawDrive("RawDrive");
std::string digit0("digit0");
std::string wheel("wheel");
std::string wheel0("wheel0");
std::string pcboutput0("pcboutput0");
std::string output0("output0");
std::string wheel_motor("wheel_motor");
std::string m_wheel_motor("m_wheel_motor");
std::string lamp1("lamp1");
// Recent MAME versions name the outputs of the X-Board and Multi 32 games after the main PCB device
std::string mainpcb_lamp0("mainpcb:lamp0");
std::string mainpcb_lamp1("mainpcb:lamp1");
std::string mainpcb_lamp2("mainpcb:lamp2");
std::string led2("led2");
std::string cpuled6("cpuled6");
std::string Vibration_motor("Vibration_motor");
std::string vibration_motor("vibration_motor");
std::string Wheel_vibration("Wheel_vibration");
std::string Wheel_Vibration("Wheel_Vibration");
std::string upright_wheel_motor("upright_wheel_motor");
std::string MA_Steering_Wheel_motor("MA_Steering_Wheel_motor");
std::string MB_Steering_Wheel_motor("MB_Steering_Wheel_motor");
std::string mainpcb_MA_Steering_Wheel_motor("mainpcb:MA_Steering_Wheel_motor");
std::string mainpcb_MB_Steering_Wheel_motor("mainpcb:MB_Steering_Wheel_motor");
std::string Left_Gun_Recoil("Left_Gun_Recoil");
std::string Right_Gun_Recoil("Right_Gun_Recoil");
std::string Player1_Recoil_Piston("Player1_Recoil_Piston");
std::string Player2_Recoil_Piston("Player2_Recoil_Piston");
std::string Player1_Gun_Recoil("Player1_Gun_Recoil");
std::string Player2_Gun_Recoil("Player2_Gun_Recoil");
std::string Player3_Gun_Recoil("Player3_Gun_Recoil");
std::string P1_Gun_Recoil("P1_Gun_Recoil");
std::string P2_Gun_Recoil("P2_Gun_Recoil");
std::string P3_Gun_Recoil("P3_Gun_Recoil");
std::string P1_gun_recoil("P1_gun_recoil");
std::string P2_gun_recoil("P2_gun_recoil");
std::string mcuout1("mcuout1");
std::string mcuout3("mcuout3");
std::string Bank_Motor_Speed("Bank_Motor_Speed");
std::string Bank_Motor_Direction("Bank_Motor_Direction");
std::string bank_motor_position("bank_motor_position");
std::string genout2("genout2");

//Flycast
std::string awffb("awffb");
std::string midiffb("midiffb");
std::string m3ffb("m3ffb");

HINSTANCE ProcDLL = NULL;
extern SDL_Haptic* haptic2;
extern SDL_Haptic* haptic3;

//Config Settings
extern int enableLogging;
extern int configFeedbackLength;
extern int configGameId;
extern int configMinForce;
extern int configMaxForce;
extern int PowerMode;
extern int configFeedbackLength;
extern int configAlternativeMinForceLeft;
extern int configAlternativeMaxForceLeft;
extern int configAlternativeMinForceRight;
extern int configAlternativeMaxForceRight;
extern int configMinForceDevice2;
extern int configMaxForceDevice2;
extern int configAlternativeMinForceLeftDevice2;
extern int configAlternativeMaxForceLeftDevice2;
extern int configAlternativeMinForceRightDevice2;
extern int configAlternativeMaxForceRightDevice2;
extern int configMinForceDevice3;
extern int configMaxForceDevice3;
extern int SinePeriod;
extern int SineFadePeriod;
extern int SineStrength;
extern int RumbleStrengthLeftMotor;
extern int RumbleStrengthRightMotor;
extern int EnableForceSpringEffect;
extern int ForceSpringStrength;
extern int EnableDamper;
extern int DamperStrength;
extern int DoubleConstant;
extern int DoubleSine;
extern bool ConstantInfHoldsAtZero;

// Use the ConstantInf effect (infinite constant force updated in place) instead of Constant
static int UseConstantInf = GetSettingInt(TEXT("UseConstantInf"), 1);
// Swaps the force direction for the games whose wheel pulls the wrong way
static int ReverseDirection = GetSettingInt(TEXT("ReverseDirection"), 0);

static int configMinForceDaytona2 = GetSettingInt(TEXT("MinForceDaytona2"), 0);
static int configMaxForceDaytona2 = GetSettingInt(TEXT("MaxForceDaytona2"), 100);
static int configAlternativeMinForceLeftDaytona2 = GetSettingInt(TEXT("AlternativeMinForceLeftDaytona2"), 0);
static int configAlternativeMaxForceLeftDaytona2 = GetSettingInt(TEXT("AlternativeMaxForceLeftDaytona2"), 100);
static int configAlternativeMinForceRightDaytona2 = GetSettingInt(TEXT("AlternativeMinForceRightDaytona2"), 0);
static int configAlternativeMaxForceRightDaytona2 = GetSettingInt(TEXT("AlternativeMaxForceRightDaytona2"), 100);
static int configFeedbackLengthDaytona2 = GetSettingInt(TEXT("FeedbackLengthDaytona2"), 120);
static int PowerModeDaytona2 = GetSettingInt(TEXT("PowerModeDaytona2"), 0);
static int EnableForceSpringEffectDaytona2 = GetSettingInt(TEXT("EnableForceSpringEffectDaytona2"), 0);
static int ForceSpringStrengthDaytona2 = GetSettingInt(TEXT("ForceSpringStrengthDaytona2"), 0);
static int EnableDamperDaytona2 = GetSettingInt(TEXT("EnableDamperDaytona2"), 0);
static int DamperStrengthDaytona2 = GetSettingInt(TEXT("DamperStrengthDaytona2"), 100);

static int configMinForceScud = GetSettingInt(TEXT("MinForceScud"), 0);
static int configMaxForceScud = GetSettingInt(TEXT("MaxForceScud"), 100);
static int configAlternativeMinForceLeftScud = GetSettingInt(TEXT("AlternativeMinForceLeftScud"), 0);
static int configAlternativeMaxForceLeftScud = GetSettingInt(TEXT("AlternativeMaxForceLeftScud"), 100);
static int configAlternativeMinForceRightScud = GetSettingInt(TEXT("AlternativeMinForceRightScud"), 0);
static int configAlternativeMaxForceRightScud = GetSettingInt(TEXT("AlternativeMaxForceRightScud"), 100);
static int configFeedbackLengthScud = GetSettingInt(TEXT("FeedbackLengthScud"), 120);
static int PowerModeScud = GetSettingInt(TEXT("PowerModeScud"), 0);
static int EnableForceSpringEffectScud = GetSettingInt(TEXT("EnableForceSpringEffectScud"), 0);
static int ForceSpringStrengthScud = GetSettingInt(TEXT("ForceSpringStrengthScud"), 0);
static int EnableDamperScud = GetSettingInt(TEXT("EnableDamperScud"), 0);
static int DamperStrengthScud = GetSettingInt(TEXT("DamperStrengthScud"), 100);

static int configMinForceLeMans = GetSettingInt(TEXT("MinForceLeMans"), 0);
static int configMaxForceLeMans = GetSettingInt(TEXT("MaxForceLeMans"), 100);
static int configAlternativeMinForceLeftLeMans = GetSettingInt(TEXT("AlternativeMinForceLeftLeMans"), 0);
static int configAlternativeMaxForceLeftLeMans = GetSettingInt(TEXT("AlternativeMaxForceLeftLeMans"), 100);
static int configAlternativeMinForceRightLeMans = GetSettingInt(TEXT("AlternativeMinForceRightLeMans"), 0);
static int configAlternativeMaxForceRightLeMans = GetSettingInt(TEXT("AlternativeMaxForceRightLeMans"), 100);
static int configFeedbackLengthLeMans = GetSettingInt(TEXT("FeedbackLengthLeMans"), 120);
static int PowerModeLeMans = GetSettingInt(TEXT("PowerModeLeMans"), 0);
static int EnableForceSpringEffectLeMans = GetSettingInt(TEXT("EnableForceSpringEffectLeMans"), 0);
static int ForceSpringStrengthLeMans = GetSettingInt(TEXT("ForceSpringStrengthLeMans"), 0);
static int EnableDamperLeMans = GetSettingInt(TEXT("EnableDamperLeMans"), 0);
static int DamperStrengthLeMans = GetSettingInt(TEXT("DamperStrengthLeMans"), 100);

static int configMinForceDirtDevils = GetSettingInt(TEXT("MinForceDirtDevils"), 0);
static int configMaxForceDirtDevils = GetSettingInt(TEXT("MaxForceDirtDevils"), 100);
static int configAlternativeMinForceLeftDirtDevils = GetSettingInt(TEXT("AlternativeMinForceLeftDirtDevils"), 0);
static int configAlternativeMaxForceLeftDirtDevils = GetSettingInt(TEXT("AlternativeMaxForceLeftDirtDevils"), 100);
static int configAlternativeMinForceRightDirtDevils = GetSettingInt(TEXT("AlternativeMinForceRightDirtDevils"), 0);
static int configAlternativeMaxForceRightDirtDevils = GetSettingInt(TEXT("AlternativeMaxForceRightDirtDevils"), 100);
static int configFeedbackLengthDirtDevils = GetSettingInt(TEXT("FeedbackLengthDirtDevils"), 120);
static int PowerModeDirtDevils = GetSettingInt(TEXT("PowerModeDirtDevils"), 0);
static int EnableForceSpringEffectDirtDevils = GetSettingInt(TEXT("EnableForceSpringEffectDirtDevils"), 0);
static int ForceSpringStrengthDirtDevils = GetSettingInt(TEXT("ForceSpringStrengthDirtDevils"), 0);
static int EnableDamperDirtDevils = GetSettingInt(TEXT("EnableDamperDirtDevils"), 0);
static int DamperStrengthDirtDevils = GetSettingInt(TEXT("DamperStrengthDirtDevils"), 100);

static int configMinForceSRally2 = GetSettingInt(TEXT("MinForceSRally2"), 0);
static int configMaxForceSRally2 = GetSettingInt(TEXT("MaxForceSRally2"), 100);
static int configAlternativeMinForceLeftSRally2 = GetSettingInt(TEXT("AlternativeMinForceLeftSRally2"), 0);
static int configAlternativeMaxForceLeftSRally2 = GetSettingInt(TEXT("AlternativeMaxForceLeftSRally2"), 100);
static int configAlternativeMinForceRightSRally2 = GetSettingInt(TEXT("AlternativeMinForceRightSRally2"), 0);
static int configAlternativeMaxForceRightSRally2 = GetSettingInt(TEXT("AlternativeMaxForceRightSRally2"), 100);
static int configFeedbackLengthSRally2 = GetSettingInt(TEXT("FeedbackLengthSRally2"), 120);
static int PowerModeSRally2 = GetSettingInt(TEXT("PowerModeSRally2"), 0);
static int EnableForceSpringEffectSRally2 = GetSettingInt(TEXT("EnableForceSpringEffectSRally2"), 0);
static int ForceSpringStrengthSRally2 = GetSettingInt(TEXT("ForceSpringStrengthSRally2"), 0);
static int EnableDamperSRally2 = GetSettingInt(TEXT("EnableDamperSRally2"), 0);
static int DamperStrengthSRally2 = GetSettingInt(TEXT("DamperStrengthSRally2"), 100);

static int configMinForceECA = GetSettingInt(TEXT("MinForceECA"), 0);
static int configMaxForceECA = GetSettingInt(TEXT("MaxForceECA"), 100);
static int configAlternativeMinForceLeftECA = GetSettingInt(TEXT("AlternativeMinForceLeftECA"), 0);
static int configAlternativeMaxForceLeftECA = GetSettingInt(TEXT("AlternativeMaxForceLeftECA"), 100);
static int configAlternativeMinForceRightECA = GetSettingInt(TEXT("AlternativeMinForceRightECA"), 0);
static int configAlternativeMaxForceRightECA = GetSettingInt(TEXT("AlternativeMaxForceRightECA"), 100);
static int configFeedbackLengthECA = GetSettingInt(TEXT("FeedbackLengthECA"), 120);
static int PowerModeECA = GetSettingInt(TEXT("PowerModeECA"), 0);
static int EnableForceSpringEffectECA = GetSettingInt(TEXT("EnableForceSpringEffectECA"), 0);
static int ForceSpringStrengthECA = GetSettingInt(TEXT("ForceSpringStrengthECA"), 0);
static int EnableDamperECA = GetSettingInt(TEXT("EnableDamperECA"), 0);
static int DamperStrengthECA = GetSettingInt(TEXT("DamperStrengthECA"), 100);

static int configMinForceVirtuaRacing = GetSettingInt(TEXT("MinForceVirtuaRacing"), 0);
static int configMaxForceVirtuaRacing = GetSettingInt(TEXT("MaxForceVirtuaRacing"), 100);
static int configAlternativeMinForceLeftVirtuaRacing = GetSettingInt(TEXT("AlternativeMinForceLeftVirtuaRacing"), 0);
static int configAlternativeMaxForceLeftVirtuaRacing = GetSettingInt(TEXT("AlternativeMaxForceLeftVirtuaRacing"), 100);
static int configAlternativeMinForceRightVirtuaRacing = GetSettingInt(TEXT("AlternativeMinForceRightVirtuaRacing"), 0);
static int configAlternativeMaxForceRightVirtuaRacing = GetSettingInt(TEXT("AlternativeMaxForceRightVirtuaRacing"), 100);
static int configFeedbackLengthVirtuaRacing = GetSettingInt(TEXT("FeedbackLengthVirtuaRacing"), 120);
static int EnableForceSpringEffectVirtuaRacing = GetSettingInt(TEXT("EnableForceSpringEffectVirtuaRacing"), 0);
static int ForceSpringStrengthVirtuaRacing = GetSettingInt(TEXT("ForceSpringStrengthVirtuaRacing"), 0);
static int EnableDamperVirtuaRacing = GetSettingInt(TEXT("EnableDamperVirtuaRacing"), 0);
static int DamperStrengthVirtuaRacing = GetSettingInt(TEXT("DamperStrengthVirtuaRacing"), 100);

static int configMinForceSuperMonacoGP = GetSettingInt(TEXT("MinForceSuperMonacoGP"), 0);
static int configMaxForceSuperMonacoGP = GetSettingInt(TEXT("MaxForceSuperMonacoGP"), 100);
static int configAlternativeMinForceLeftSuperMonacoGP = GetSettingInt(TEXT("AlternativeMinForceLeftSuperMonacoGP"), 0);
static int configAlternativeMaxForceLeftSuperMonacoGP = GetSettingInt(TEXT("AlternativeMaxForceLeftSuperMonacoGP"), 100);
static int configAlternativeMinForceRightSuperMonacoGP = GetSettingInt(TEXT("AlternativeMinForceRightSuperMonacoGP"), 0);
static int configAlternativeMaxForceRightSuperMonacoGP = GetSettingInt(TEXT("AlternativeMaxForceRightSuperMonacoGP"), 100);
static int configFeedbackLengthSuperMonacoGP = GetSettingInt(TEXT("FeedbackLengthSuperMonacoGP"), 120);
static int EnableForceSpringEffectSuperMonacoGP = GetSettingInt(TEXT("EnableForceSpringEffectSuperMonacoGP"), 0);
static int ForceSpringStrengthSuperMonacoGP = GetSettingInt(TEXT("ForceSpringStrengthSuperMonacoGP"), 0);
static int SinePeriodSuperMonacoGP = GetSettingInt(TEXT("SinePeriodSuperMonacoGP"), 100);
static int SineFadePeriodSuperMonacoGP = GetSettingInt(TEXT("SineFadePeriodSuperMonacoGP"), 0);
static int SineStrengthSuperMonacoGP = GetSettingInt(TEXT("SineStrengthSuperMonacoGP"), 100);

static int configMinForceSFRush = GetSettingInt(TEXT("MinForceSFRush"), 0);
static int configMaxForceSFRush = GetSettingInt(TEXT("MaxForceSFRush"), 100);
static int configAlternativeMinForceLeftSFRush = GetSettingInt(TEXT("AlternativeMinForceLeftSFRush"), 0);
static int configAlternativeMaxForceLeftSFRush = GetSettingInt(TEXT("AlternativeMaxForceLeftSFRush"), 100);
static int configAlternativeMinForceRightSFRush = GetSettingInt(TEXT("AlternativeMinForceRightSFRush"), 0);
static int configAlternativeMaxForceRightSFRush = GetSettingInt(TEXT("AlternativeMaxForceRightSFRush"), 100);
static int configFeedbackLengthSFRush = GetSettingInt(TEXT("FeedbackLengthSFRush"), 120);
static int PowerModeSFRush = GetSettingInt(TEXT("PowerModeSFRush"), 0);
static int EnableForceSpringEffectSFRush = GetSettingInt(TEXT("EnableForceSpringEffectSFRush"), 0);
static int ForceSpringStrengthSFRush = GetSettingInt(TEXT("ForceSpringStrengthSFRush"), 0);
static int EnableDamperSFRush = GetSettingInt(TEXT("EnableDamperSFRush"), 0);
static int DamperStrengthSFRush = GetSettingInt(TEXT("DamperStrengthSFRush"), 100);

static int configMinForceSFRushRock = GetSettingInt(TEXT("MinForceSFRushRock"), 0);
static int configMaxForceSFRushRock = GetSettingInt(TEXT("MaxForceSFRushRock"), 100);
static int configAlternativeMinForceLeftSFRushRock = GetSettingInt(TEXT("AlternativeMinForceLeftSFRushRock"), 0);
static int configAlternativeMaxForceLeftSFRushRock = GetSettingInt(TEXT("AlternativeMaxForceLeftSFRushRock"), 100);
static int configAlternativeMinForceRightSFRushRock = GetSettingInt(TEXT("AlternativeMinForceRightSFRushRock"), 0);
static int configAlternativeMaxForceRightSFRushRock = GetSettingInt(TEXT("AlternativeMaxForceRightSFRushRock"), 100);
static int configFeedbackLengthSFRushRock = GetSettingInt(TEXT("FeedbackLengthSFRushRock"), 120);
static int PowerModeSFRushRock = GetSettingInt(TEXT("PowerModeSFRushRock"), 0);
static int EnableForceSpringEffectSFRushRock = GetSettingInt(TEXT("EnableForceSpringEffectSFRushRock"), 0);
static int ForceSpringStrengthSFRushRock = GetSettingInt(TEXT("ForceSpringStrengthSFRushRock"), 0);
static int EnableDamperSFRushRock = GetSettingInt(TEXT("EnableDamperSFRushRock"), 0);
static int DamperStrengthSFRushRock = GetSettingInt(TEXT("DamperStrengthSFRushRock"), 100);

static int configMinForceCrusnWld = GetSettingInt(TEXT("MinForceCrusnWld"), 0);
static int configMaxForceCrusnWld = GetSettingInt(TEXT("MaxForceCrusnWld"), 100);
static int configAlternativeMinForceLeftCrusnWld = GetSettingInt(TEXT("AlternativeMinForceLeftCrusnWld"), 0);
static int configAlternativeMaxForceLeftCrusnWld = GetSettingInt(TEXT("AlternativeMaxForceLeftCrusnWld"), 100);
static int configAlternativeMinForceRightCrusnWld = GetSettingInt(TEXT("AlternativeMinForceRightCrusnWld"), 0);
static int configAlternativeMaxForceRightCrusnWld = GetSettingInt(TEXT("AlternativeMaxForceRightCrusnWld"), 100);
static int configFeedbackLengthCrusnWld = GetSettingInt(TEXT("FeedbackLengthCrusnWld"), 120);
static int PowerModeCrusnWld = GetSettingInt(TEXT("PowerModeCrusnWld"), 0);
static int EnableForceSpringEffectCrusnWld = GetSettingInt(TEXT("EnableForceSpringEffectCrusnWld"), 0);
static int ForceSpringStrengthCrusnWld = GetSettingInt(TEXT("ForceSpringStrengthCrusnWld"), 0);
static int EnableDamperCrusnWld = GetSettingInt(TEXT("EnableDamperCrusnWld"), 0);
static int DamperStrengthCrusnWld = GetSettingInt(TEXT("DamperStrengthCrusnWld"), 100);

static int configMinForceOffRoadC = GetSettingInt(TEXT("MinForceOffRoadC"), 0);
static int configMaxForceOffRoadC = GetSettingInt(TEXT("MaxForceOffRoadC"), 100);
static int configAlternativeMinForceLeftOffRoadC = GetSettingInt(TEXT("AlternativeMinForceLeftOffRoadC"), 0);
static int configAlternativeMaxForceLeftOffRoadC = GetSettingInt(TEXT("AlternativeMaxForceLeftOffRoadC"), 100);
static int configAlternativeMinForceRightOffRoadC = GetSettingInt(TEXT("AlternativeMinForceRightOffRoadC"), 0);
static int configAlternativeMaxForceRightOffRoadC = GetSettingInt(TEXT("AlternativeMaxForceRightOffRoadC"), 100);
static int configFeedbackLengthOffRoadC = GetSettingInt(TEXT("FeedbackLengthOffRoadC"), 120);
static int PowerModeOffRoadC = GetSettingInt(TEXT("PowerModeOffRoadC"), 0);
static int EnableForceSpringEffectOffRoadC = GetSettingInt(TEXT("EnableForceSpringEffectOffRoadC"), 0);
static int ForceSpringStrengthOffRoadC = GetSettingInt(TEXT("ForceSpringStrengthOffRoadC"), 0);
static int EnableDamperOffRoadC = GetSettingInt(TEXT("EnableDamperOffRoadC"), 0);
static int DamperStrengthOffRoadC = GetSettingInt(TEXT("DamperStrengthOffRoadC"), 100);

static int configMinForceCrusnUSA = GetSettingInt(TEXT("MinForceCrusnUSA"), 0);
static int configMaxForceCrusnUSA = GetSettingInt(TEXT("MaxForceCrusnUSA"), 100);
static int configAlternativeMinForceLeftCrusnUSA = GetSettingInt(TEXT("AlternativeMinForceLeftCrusnUSA"), 0);
static int configAlternativeMaxForceLeftCrusnUSA = GetSettingInt(TEXT("AlternativeMaxForceLeftCrusnUSA"), 100);
static int configAlternativeMinForceRightCrusnUSA = GetSettingInt(TEXT("AlternativeMinForceRightCrusnUSA"), 0);
static int configAlternativeMaxForceRightCrusnUSA = GetSettingInt(TEXT("AlternativeMaxForceRightCrusnUSA"), 100);
static int configFeedbackLengthCrusnUSA = GetSettingInt(TEXT("FeedbackLengthCrusnUSA"), 120);
static int PowerModeCrusnUSA = GetSettingInt(TEXT("PowerModeCrusnUSA"), 0);
static int EnableForceSpringEffectCrusnUSA = GetSettingInt(TEXT("EnableForceSpringEffectCrusnUSA"), 0);
static int ForceSpringStrengthCrusnUSA = GetSettingInt(TEXT("ForceSpringStrengthCrusnUSA"), 0);
static int EnableDamperCrusnUSA = GetSettingInt(TEXT("EnableDamperCrusnUSA"), 0);
static int DamperStrengthCrusnUSA = GetSettingInt(TEXT("DamperStrengthCrusnUSA"), 100);

// Cruis'n Exotica (Zeus): accept both "wheel" and "wheel_motor" outputs.
// The raw game signal is much weaker than the V-Unit games, so use a separate
// configurable gain. 400% matches the measured Exotica signal range closely.
static int configFeedbackLengthCrusnExotica = GetSettingInt(TEXT("FeedbackLengthCrusnExotica"), 120);
static int configMinForceCrusnExotica = GetSettingInt(TEXT("MinForceCrusnExotica"), 0);
static int configMaxForceCrusnExotica = GetSettingInt(TEXT("MaxForceCrusnExotica"), 100);
static int configAlternativeMinForceLeftCrusnExotica = GetSettingInt(TEXT("AlternativeMinForceLeftCrusnExotica"), 0);
static int configAlternativeMaxForceLeftCrusnExotica = GetSettingInt(TEXT("AlternativeMaxForceLeftCrusnExotica"), 100);
static int configAlternativeMinForceRightCrusnExotica = GetSettingInt(TEXT("AlternativeMinForceRightCrusnExotica"), 0);
static int configAlternativeMaxForceRightCrusnExotica = GetSettingInt(TEXT("AlternativeMaxForceRightCrusnExotica"), 100);
static int PowerModeCrusnExotica = GetSettingInt(TEXT("PowerModeCrusnExotica"), 0);
static int EnableForceSpringEffectCrusnExotica = GetSettingInt(TEXT("EnableForceSpringEffectCrusnExotica"), 0);
static int ForceSpringStrengthCrusnExotica = GetSettingInt(TEXT("ForceSpringStrengthCrusnExotica"), 0);
static int EnableDamperCrusnExotica = GetSettingInt(TEXT("EnableDamperCrusnExotica"), 0);
static int DamperStrengthCrusnExotica = GetSettingInt(TEXT("DamperStrengthCrusnExotica"), 100);
static int configFFBGainCrusnExotica = GetSettingInt(TEXT("FFBGainCrusnExotica"), 400);

static int configMinForceCalSpeed = GetSettingInt(TEXT("MinForceCalSpeed"), 0);
static int configMaxForceCalSpeed = GetSettingInt(TEXT("MaxForceCalSpeed"), 100);
static int configAlternativeMinForceLeftCalSpeed = GetSettingInt(TEXT("AlternativeMinForceLeftCalSpeed"), 0);
static int configAlternativeMaxForceLeftCalSpeed = GetSettingInt(TEXT("AlternativeMaxForceLeftCalSpeed"), 100);
static int configAlternativeMinForceRightCalSpeed = GetSettingInt(TEXT("AlternativeMinForceRightCalSpeed"), 0);
static int configAlternativeMaxForceRightCalSpeed = GetSettingInt(TEXT("AlternativeMaxForceRightCalSpeed"), 100);
static int configFeedbackLengthCalSpeed = GetSettingInt(TEXT("FeedbackLengthCalSpeed"), 120);
static int PowerModeCalSpeed = GetSettingInt(TEXT("PowerModeCalSpeed"), 0);
static int EnableForceSpringEffectCalSpeed = GetSettingInt(TEXT("EnableForceSpringEffectCalSpeed"), 0);
static int ForceSpringStrengthCalSpeed = GetSettingInt(TEXT("ForceSpringStrengthCalSpeed"), 0);
static int EnableDamperCalSpeed = GetSettingInt(TEXT("EnableDamperCalSpeed"), 0);
static int DamperStrengthCalSpeed = GetSettingInt(TEXT("DamperStrengthCalSpeed"), 100);

static int configMinForceSFRush2049 = GetSettingInt(TEXT("MinForceSFRush2049"), 0);
static int configMaxForceSFRush2049 = GetSettingInt(TEXT("MaxForceSFRush2049"), 100);
static int configAlternativeMinForceLeftSFRush2049 = GetSettingInt(TEXT("AlternativeMinForceLeftSFRush2049"), 0);
static int configAlternativeMaxForceLeftSFRush2049 = GetSettingInt(TEXT("AlternativeMaxForceLeftSFRush2049"), 100);
static int configAlternativeMinForceRightSFRush2049 = GetSettingInt(TEXT("AlternativeMinForceRightSFRush2049"), 0);
static int configAlternativeMaxForceRightSFRush2049 = GetSettingInt(TEXT("AlternativeMaxForceRightSFRush2049"), 100);
static int configFeedbackLengthSFRush2049 = GetSettingInt(TEXT("FeedbackLengthSFRush2049"), 120);
static int PowerModeSFRush2049 = GetSettingInt(TEXT("PowerModeSFRush2049"), 0);
static int EnableForceSpringEffectSFRush2049 = GetSettingInt(TEXT("EnableForceSpringEffectSFRush2049"), 0);
static int ForceSpringStrengthSFRush2049 = GetSettingInt(TEXT("ForceSpringStrengthSFRush2049"), 0);
static int EnableDamperSFRush2049 = GetSettingInt(TEXT("EnableDamperSFRush2049"), 0);
static int DamperStrengthSFRush2049 = GetSettingInt(TEXT("DamperStrengthSFRush2049"), 100);

static int configMinForceHardD = GetSettingInt(TEXT("MinForceHardD"), 0);
static int configMaxForceHardD = GetSettingInt(TEXT("MaxForceHardD"), 100);
static int configAlternativeMinForceLeftHardD = GetSettingInt(TEXT("AlternativeMinForceLeftHardD"), 0);
static int configAlternativeMaxForceLeftHardD = GetSettingInt(TEXT("AlternativeMaxForceLeftHardD"), 100);
static int configAlternativeMinForceRightHardD = GetSettingInt(TEXT("AlternativeMinForceRightHardD"), 0);
static int configAlternativeMaxForceRightHardD = GetSettingInt(TEXT("AlternativeMaxForceRightHardD"), 100);
static int configFeedbackLengthHardD = GetSettingInt(TEXT("FeedbackLengthHardD"), 120);
static int PowerModeHardD = GetSettingInt(TEXT("PowerModeHardD"), 0);
static int EnableForceSpringEffectHardD = GetSettingInt(TEXT("EnableForceSpringEffectHardD"), 0);
static int ForceSpringStrengthHardD = GetSettingInt(TEXT("ForceSpringStrengthHardD"), 0);
static int EnableDamperHardD = GetSettingInt(TEXT("EnableDamperHardD"), 0);
static int DamperStrengthHardD = GetSettingInt(TEXT("DamperStrengthHardD"), 100);

static int configMinForceRevX = GetSettingInt(TEXT("MinForceRevX"), 0);
static int configMaxForceRevX = GetSettingInt(TEXT("MaxForceRevX"), 100);
static int configMinForceRevXDevice2 = GetSettingInt(TEXT("MinForceRevXDevice2"), 0);
static int configMaxForceRevXDevice2 = GetSettingInt(TEXT("MaxForceRevXDevice2"), 100);
static int configMinForceRevXDevice3 = GetSettingInt(TEXT("MinForceRevXDevice3"), 0);
static int configMaxForceRevXDevice3 = GetSettingInt(TEXT("MaxForceRevXDevice3"), 100);
static int RumbleStrengthLeftMotorRevX = GetSettingInt(TEXT("RumbleStrengthLeftMotorRevX"), 0);
static int RumbleStrengthRightMotorRevX = GetSettingInt(TEXT("RumbleStrengthRightMotorRevX"), 0);

static int configMinForceZombRaid = GetSettingInt(TEXT("MinForceZombRaid"), 0);
static int configMaxForceZombRaid = GetSettingInt(TEXT("MaxForceZombRaid"), 100);
static int configMinForceZombRaidDevice2 = GetSettingInt(TEXT("MinForceZombRaidDevice2"), 0);
static int configMaxForceZombRaidDevice2 = GetSettingInt(TEXT("MaxForceZombRaidDevice2"), 100);
static int RumbleStrengthLeftMotorZombRaid = GetSettingInt(TEXT("RumbleStrengthLeftMotorZombRaid"), 0);
static int RumbleStrengthRightMotorZombRaid = GetSettingInt(TEXT("RumbleStrengthRightMotorZombRaid"), 0);

static int configMinForceBBusters = GetSettingInt(TEXT("MinForceBBusters"), 0);
static int configMaxForceBBusters = GetSettingInt(TEXT("MaxForceBBusters"), 100);
static int configMinForceBBustersDevice2 = GetSettingInt(TEXT("MinForceBBustersDevice2"), 0);
static int configMaxForceBBustersDevice2 = GetSettingInt(TEXT("MaxForceBBustersDevice2"), 100);
static int configMinForceBBustersDevice3 = GetSettingInt(TEXT("MinForceBBustersDevice3"), 0);
static int configMaxForceBBustersDevice3 = GetSettingInt(TEXT("MaxForceBBustersDevice3"), 100);
static int RumbleStrengthLeftMotorBBusters = GetSettingInt(TEXT("RumbleStrengthLeftMotorBBusters"), 0);
static int RumbleStrengthRightMotorBBusters = GetSettingInt(TEXT("RumbleStrengthRightMotorBBusters"), 0);

static int configMinForceMechatt = GetSettingInt(TEXT("MinForceMechatt"), 0);
static int configMaxForceMechatt = GetSettingInt(TEXT("MaxForceMechatt"), 100);
static int configMinForceMechattDevice2 = GetSettingInt(TEXT("MinForceMechattDevice2"), 0);
static int configMaxForceMechattDevice2 = GetSettingInt(TEXT("MaxForceMechattDevice2"), 100);
static int RumbleStrengthLeftMotorMechatt = GetSettingInt(TEXT("RumbleStrengthLeftMotorMechatt"), 0);
static int RumbleStrengthRightMotorMechatt = GetSettingInt(TEXT("RumbleStrengthRightMotorMechatt"), 0);

static int configMinForceTShoot = GetSettingInt(TEXT("MinForceTShoot"), 0);
static int configMaxForceTShoot = GetSettingInt(TEXT("MaxForceTShoot"), 100);
static int configMinForceTShootDevice2 = GetSettingInt(TEXT("MinForceTShootDevice2"), 0);
static int configMaxForceTShootDevice2 = GetSettingInt(TEXT("MaxForceTShootDevice2"), 100);
static int RumbleStrengthLeftMotorTShoot = GetSettingInt(TEXT("RumbleStrengthLeftMotorTShoot"), 0);
static int RumbleStrengthRightMotorTShoot = GetSettingInt(TEXT("RumbleStrengthRightMotorTShoot"), 0);

static int configMinForceGunBuster = GetSettingInt(TEXT("MinForceGunBuster"), 0);
static int configMaxForceGunBuster = GetSettingInt(TEXT("MaxForceGunBuster"), 100);
static int configMinForceGunBusterDevice2 = GetSettingInt(TEXT("MinForceGunBusterDevice2"), 0);
static int configMaxForceGunBusterDevice2 = GetSettingInt(TEXT("MaxForceGunBusterDevice2"), 100);
static int RumbleStrengthLeftMotorGunBuster = GetSettingInt(TEXT("RumbleStrengthLeftMotorGunBuster"), 0);
static int RumbleStrengthRightMotorGunBuster = GetSettingInt(TEXT("RumbleStrengthRightMotorGunBuster"), 0);

static int configMinForceAlien3 = GetSettingInt(TEXT("MinForceAlien3"), 0);
static int configMaxForceAlien3 = GetSettingInt(TEXT("MaxForceAlien3"), 100);
static int configMinForceAlien3Device2 = GetSettingInt(TEXT("MinForceAlien3Device2"), 0);
static int configMaxForceAlien3Device2 = GetSettingInt(TEXT("MaxForceAlien3Device2"), 100);
static int RumbleStrengthLeftMotorAlien3 = GetSettingInt(TEXT("RumbleStrengthLeftMotorAlien3"), 0);
static int RumbleStrengthRightMotorAlien3 = GetSettingInt(TEXT("RumbleStrengthRightMotorAlien3"), 0);

static int configMinForcePointBlank2 = GetSettingInt(TEXT("MinForcePointBlank2"), 0);
static int configMaxForcePointBlank2 = GetSettingInt(TEXT("MaxForcePointBlank2"), 100);
static int configMinForcePointBlank2Device2 = GetSettingInt(TEXT("MinForcePointBlank2Device2"), 0);
static int configMaxForcePointBlank2Device2 = GetSettingInt(TEXT("MaxForcePointBlank2Device2"), 100);
static int RumbleStrengthLeftMotorPointBlank2 = GetSettingInt(TEXT("RumbleStrengthLeftMotorPointBlank2"), 0);
static int RumbleStrengthRightMotorPointBlank2 = GetSettingInt(TEXT("RumbleStrengthRightMotorPointBlank2"), 0);

static int configMinForceGhoulPanic = GetSettingInt(TEXT("MinForceGhoulPanic"), 0);
static int configMaxForceGhoulPanic = GetSettingInt(TEXT("MaxForceGhoulPanic"), 100);
static int configMinForceGhoulPanicDevice2 = GetSettingInt(TEXT("MinForceGhoulPanicDevice2"), 0);
static int configMaxForceGhoulPanicDevice2 = GetSettingInt(TEXT("MaxForceGhoulPanicDevice2"), 100);
static int RumbleStrengthLeftMotorGhoulPanic = GetSettingInt(TEXT("RumbleStrengthLeftMotorGhoulPanic"), 0);
static int RumbleStrengthRightMotorGhoulPanic = GetSettingInt(TEXT("RumbleStrengthRightMotorGhoulPanic"), 0);

static int configMinForceSpacegun = GetSettingInt(TEXT("MinForceSpacegun"), 0);
static int configMaxForceSpacegun = GetSettingInt(TEXT("MaxForceSpacegun"), 100);
static int configMinForceSpacegunDevice2 = GetSettingInt(TEXT("MinForceSpacegunDevice2"), 0);
static int configMaxForceSpacegunDevice2 = GetSettingInt(TEXT("MaxForceSpacegunDevice2"), 100);
static int RumbleStrengthLeftMotorSpacegun = GetSettingInt(TEXT("RumbleStrengthLeftMotorSpacegun"), 0);
static int RumbleStrengthRightMotorSpacegun = GetSettingInt(TEXT("RumbleStrengthRightMotorSpacegun"), 0);

static int configMinForceTerm2 = GetSettingInt(TEXT("MinForceTerm2"), 0);
static int configMaxForceTerm2 = GetSettingInt(TEXT("MaxForceTerm2"), 100);
static int configMinForceTerm2Device2 = GetSettingInt(TEXT("MinForceTerm2Device2"), 0);
static int configMaxForceTerm2Device2 = GetSettingInt(TEXT("MaxForceTerm2Device2"), 100);
static int RumbleStrengthLeftMotorTerm2 = GetSettingInt(TEXT("RumbleStrengthLeftMotorTerm2"), 0);
static int RumbleStrengthRightMotorTerm2 = GetSettingInt(TEXT("RumbleStrengthRightMotorTerm2"), 0);

static int configMinForceRchase = GetSettingInt(TEXT("MinForceRchase"), 0);
static int configMaxForceRchase = GetSettingInt(TEXT("MaxForceRchase"), 100);
static int configMinForceRchaseDevice2 = GetSettingInt(TEXT("MinForceRchaseDevice2"), 0);
static int configMaxForceRchaseDevice2 = GetSettingInt(TEXT("MaxForceRchaseDevice2"), 100);
static int RumbleStrengthLeftMotorRchase = GetSettingInt(TEXT("RumbleStrengthLeftMotorRchase"), 0);
static int RumbleStrengthRightMotorRchase = GetSettingInt(TEXT("RumbleStrengthRightMotorRchase"), 0);

static int configMinForceLGhost = GetSettingInt(TEXT("MinForceLGhost"), 0);
static int configMaxForceLGhost = GetSettingInt(TEXT("MaxForceLGhost"), 100);
static int configMinForceLGhostDevice2 = GetSettingInt(TEXT("MinForceLGhostDevice2"), 0);
static int configMaxForceLGhostDevice2 = GetSettingInt(TEXT("MaxForceLGhostDevice2"), 100);
static int configMinForceLGhostDevice3 = GetSettingInt(TEXT("MinForceLGhostDevice3"), 0);
static int configMaxForceLGhostDevice3 = GetSettingInt(TEXT("MaxForceLGhostDevice3"), 100);
static int RumbleStrengthLeftMotorLGhost = GetSettingInt(TEXT("RumbleStrengthLeftMotorLGhost"), 0);
static int RumbleStrengthRightMotorLGhost = GetSettingInt(TEXT("RumbleStrengthRightMotorLGhost"), 0);

static int configMinForceTimeCris = GetSettingInt(TEXT("MinForceTimeCris"), 0);
static int configMaxForceTimeCris = GetSettingInt(TEXT("MaxForceTimeCris"), 100);
static int RumbleStrengthLeftMotorTimeCris = GetSettingInt(TEXT("RumbleStrengthLeftMotorTimeCris"), 0);
static int RumbleStrengthRightMotorTimeCris = GetSettingInt(TEXT("RumbleStrengthRightMotorTimeCris"), 0);

static int configMinForceOThunder = GetSettingInt(TEXT("MinForceOThunder"), 0);
static int configMaxForceOThunder = GetSettingInt(TEXT("MaxForceOThunder"), 100);
static int configMinForceOThunderDevice2 = GetSettingInt(TEXT("MinForceOThunderDevice2"), 0);
static int configMaxForceOThunderDevice2 = GetSettingInt(TEXT("MaxForceOThunderDevice2"), 100);
static int RumbleStrengthLeftMotorOThunder = GetSettingInt(TEXT("RumbleStrengthLeftMotorOThunder"), 0);
static int RumbleStrengthRightMotorOThunder = GetSettingInt(TEXT("RumbleStrengthRightMotorOThunder"), 0);

static int configMinForceOpWolf = GetSettingInt(TEXT("MinForceOpWolf"), 0);
static int configMaxForceOpWolf = GetSettingInt(TEXT("MaxForceOpWolf"), 100);
static int configMinForceOpWolfDevice2 = GetSettingInt(TEXT("MinForceOpWolfDevice2"), 0);
static int configMaxForceOpWolfDevice2 = GetSettingInt(TEXT("MaxForceOpWolfDevice2"), 100);
static int RumbleStrengthLeftMotorOpWolf = GetSettingInt(TEXT("RumbleStrengthLeftMotorOpWolf"), 0);
static int RumbleStrengthRightMotorOpWolf = GetSettingInt(TEXT("RumbleStrengthRightMotorOpWolf"), 0);

static int configMinForceUFire = GetSettingInt(TEXT("MinForceUFire"), 0);
static int configMaxForceUFire = GetSettingInt(TEXT("MaxForceUFire"), 100);
static int configMinForceUFireDevice2 = GetSettingInt(TEXT("MinForceUFireDevice2"), 0);
static int configMaxForceUFireDevice2 = GetSettingInt(TEXT("MaxForceUFireDevice2"), 100);
static int RumbleStrengthLeftMotorUFire = GetSettingInt(TEXT("RumbleStrengthLeftMotorUFire"), 0);
static int RumbleStrengthRightMotorUFire = GetSettingInt(TEXT("RumbleStrengthRightMotorUFire"), 0);

static int configMinForceORunners = GetSettingInt(TEXT("MinForceORunners"), 0);
static int configMaxForceORunners = GetSettingInt(TEXT("MaxForceORunners"), 100);
static int configAlternativeMinForceLeftORunners = GetSettingInt(TEXT("AlternativeMinForceLeftORunners"), 0);
static int configAlternativeMaxForceLeftORunners = GetSettingInt(TEXT("AlternativeMaxForceLeftORunners"), 100);
static int configAlternativeMinForceRightORunners = GetSettingInt(TEXT("AlternativeMinForceRightORunners"), 0);
static int configAlternativeMaxForceRightORunners = GetSettingInt(TEXT("AlternativeMaxForceRightORunners"), 100);
static int configMinForceORunnersDevice2 = GetSettingInt(TEXT("MinForceORunnersDevice2"), 0);
static int configMaxForceORunnersDevice2 = GetSettingInt(TEXT("MaxForceORunnersDevice2"), 100);
static int configAlternativeMinForceLeftORunnersDevice2 = GetSettingInt(TEXT("AlternativeMinForceLeftORunnersDevice2"), 0);
static int configAlternativeMaxForceLeftORunnersDevice2 = GetSettingInt(TEXT("AlternativeMaxForceLeftORunnersDevice2"), 100);
static int configAlternativeMinForceRightORunnersDevice2 = GetSettingInt(TEXT("AlternativeMinForceRightORunnersDevice2"), 0);
static int configAlternativeMaxForceRightORunnersDevice2 = GetSettingInt(TEXT("AlternativeMaxForceRightORunnersDevice2"), 100);
static int configFeedbackLengthORunners = GetSettingInt(TEXT("FeedbackLengthORunners"), 120);
static int EnableForceSpringEffectORunners = GetSettingInt(TEXT("EnableForceSpringEffectORunners"), 0);
static int ForceSpringStrengthORunners = GetSettingInt(TEXT("ForceSpringStrengthORunners"), 0);
static int SinePeriodORunners = GetSettingInt(TEXT("SinePeriodORunners"), 0);
static int SineFadePeriodORunners = GetSettingInt(TEXT("SineFadePeriodORunners"), 0);
static int SineStrengthORunners = GetSettingInt(TEXT("SineStrengthORunners"), 0);
static int RumbleStrengthLeftMotorORunners = GetSettingInt(TEXT("RumbleStrengthLeftMotorORunners"), 0);
static int RumbleStrengthRightMotorORunners = GetSettingInt(TEXT("RumbleStrengthRightMotorORunners"), 0);

static int configMinForceTOutrun = GetSettingInt(TEXT("MinForceTOutrun"), 0);
static int configMaxForceTOutrun = GetSettingInt(TEXT("MaxForceTOutrun"), 100);
static int configAlternativeMinForceLeftTOutrun = GetSettingInt(TEXT("AlternativeMinForceLeftTOutrun"), 0);
static int configAlternativeMaxForceLeftTOutrun = GetSettingInt(TEXT("AlternativeMaxForceLeftTOutrun"), 100);
static int configAlternativeMinForceRightTOutrun = GetSettingInt(TEXT("AlternativeMinForceRightTOutrun"), 0);
static int configAlternativeMaxForceRightTOutrun = GetSettingInt(TEXT("AlternativeMaxForceRightTOutrun"), 100);
static int configFeedbackLengthTOutrun = GetSettingInt(TEXT("FeedbackLengthTOutrun"), 120);
static int EnableForceSpringEffectTOutrun = GetSettingInt(TEXT("EnableForceSpringEffectTOutrun"), 0);
static int ForceSpringStrengthTOutrun = GetSettingInt(TEXT("ForceSpringStrengthTOutrun"), 0);
static int SinePeriodTOutrun = GetSettingInt(TEXT("SinePeriodTOutrun"), 0);
static int SineFadePeriodTOutrun = GetSettingInt(TEXT("SineFadePeriodTOutrun"), 0);
static int SineStrengthTOutrun = GetSettingInt(TEXT("SineStrengthTOutrun"), 0);
static int RumbleStrengthLeftMotorTOutrun = GetSettingInt(TEXT("RumbleStrengthLeftMotorTOutrun"), 0);
static int RumbleStrengthRightMotorTOutrun = GetSettingInt(TEXT("RumbleStrengthRightMotorTOutrun"), 0);

static int configMinForceCBombers = GetSettingInt(TEXT("MinForceCBombers"), 0);
static int configMaxForceCBombers = GetSettingInt(TEXT("MaxForceCBombers"), 100);
static int configAlternativeMinForceLeftCBombers = GetSettingInt(TEXT("AlternativeMinForceLeftCBombers"), 0);
static int configAlternativeMaxForceLeftCBombers = GetSettingInt(TEXT("AlternativeMaxForceLeftCBombers"), 100);
static int configAlternativeMinForceRightCBombers = GetSettingInt(TEXT("AlternativeMinForceRightCBombers"), 0);
static int configAlternativeMaxForceRightCBombers = GetSettingInt(TEXT("AlternativeMaxForceRightCBombers"), 100);
static int configFeedbackLengthCBombers = GetSettingInt(TEXT("FeedbackLengthCBombers"), 120);
static int EnableForceSpringEffectCBombers = GetSettingInt(TEXT("EnableForceSpringEffectCBombers"), 0);
static int ForceSpringStrengthCBombers = GetSettingInt(TEXT("ForceSpringStrengthCBombers"), 0);
static int SinePeriodCBombers = GetSettingInt(TEXT("SinePeriodCBombers"), 0);
static int SineFadePeriodCBombers = GetSettingInt(TEXT("SineFadePeriodCBombers"), 0);
static int SineStrengthCBombers = GetSettingInt(TEXT("SineStrengthCBombers"), 0);
static int RumbleStrengthLeftMotorCBombers = GetSettingInt(TEXT("RumbleStrengthLeftMotorCBombers"), 0);
static int RumbleStrengthRightMotorCBombers = GetSettingInt(TEXT("RumbleStrengthRightMotorCBombers"), 0);

static int configMinForceOutrun = GetSettingInt(TEXT("MinForceOutrun"), 0);
static int configMaxForceOutrun = GetSettingInt(TEXT("MaxForceOutrun"), 100);
static int configAlternativeMinForceLeftOutrun = GetSettingInt(TEXT("AlternativeMinForceLeftOutrun"), 0);
static int configAlternativeMaxForceLeftOutrun = GetSettingInt(TEXT("AlternativeMaxForceLeftOutrun"), 100);
static int configAlternativeMinForceRightOutrun = GetSettingInt(TEXT("AlternativeMinForceRightOutrun"), 0);
static int configAlternativeMaxForceRightOutrun = GetSettingInt(TEXT("AlternativeMaxForceRightOutrun"), 100);
static int configFeedbackLengthOutrun = GetSettingInt(TEXT("FeedbackLengthOutrun"), 120);
static int EnableForceSpringEffectOutrun = GetSettingInt(TEXT("EnableForceSpringEffectOutrun"), 0);
static int ForceSpringStrengthOutrun = GetSettingInt(TEXT("ForceSpringStrengthOutrun"), 0);
static int SinePeriodOutrun = GetSettingInt(TEXT("SinePeriodOutrun"), 0);
static int SineFadePeriodOutrun = GetSettingInt(TEXT("SineFadePeriodOutrun"), 0);
static int SineStrengthOutrun = GetSettingInt(TEXT("SineStrengthOutrun"), 0);
static int RumbleStrengthLeftMotorOutrun = GetSettingInt(TEXT("RumbleStrengthLeftMotorOutrun"), 0);
static int RumbleStrengthRightMotorOutrun = GetSettingInt(TEXT("RumbleStrengthRightMotorOutrun"), 0);

static int configMinForcePDrift = GetSettingInt(TEXT("MinForcePDrift"), 0);
static int configMaxForcePDrift = GetSettingInt(TEXT("MaxForcePDrift"), 100);
static int configAlternativeMinForceLeftPDrift = GetSettingInt(TEXT("AlternativeMinForceLeftPDrift"), 0);
static int configAlternativeMaxForceLeftPDrift = GetSettingInt(TEXT("AlternativeMaxForceLeftPDrift"), 100);
static int configAlternativeMinForceRightPDrift = GetSettingInt(TEXT("AlternativeMinForceRightPDrift"), 0);
static int configAlternativeMaxForceRightPDrift = GetSettingInt(TEXT("AlternativeMaxForceRightPDrift"), 100);
static int configFeedbackLengthPDrift = GetSettingInt(TEXT("FeedbackLengthPDrift"), 120);
static int EnableForceSpringEffectPDrift = GetSettingInt(TEXT("EnableForceSpringEffectPDrift"), 0);
static int ForceSpringStrengthPDrift = GetSettingInt(TEXT("ForceSpringStrengthPDrift"), 0);
static int SinePeriodPDrift = GetSettingInt(TEXT("SinePeriodPDrift"), 0);
static int SineFadePeriodPDrift = GetSettingInt(TEXT("SineFadePeriodPDrift"), 0);
static int SineStrengthPDrift = GetSettingInt(TEXT("SineStrengthPDrift"), 0);
static int RumbleStrengthLeftMotorPDrift = GetSettingInt(TEXT("RumbleStrengthLeftMotorPDrift"), 0);
static int RumbleStrengthRightMotorPDrift = GetSettingInt(TEXT("RumbleStrengthRightMotorPDrift"), 0);

static int configMinForceDAxle = GetSettingInt(TEXT("MinForceDAxle"), 0);
static int configMaxForceDAxle = GetSettingInt(TEXT("MaxForceDAxle"), 100);
static int configAlternativeMinForceLeftDAxle = GetSettingInt(TEXT("AlternativeMinForceLeftDAxle"), 0);
static int configAlternativeMaxForceLeftDAxle = GetSettingInt(TEXT("AlternativeMaxForceLeftDAxle"), 100);
static int configAlternativeMinForceRightDAxle = GetSettingInt(TEXT("AlternativeMinForceRightDAxle"), 0);
static int configAlternativeMaxForceRightDAxle = GetSettingInt(TEXT("AlternativeMaxForceRightDAxle"), 100);
static int configFeedbackLengthDAxle = GetSettingInt(TEXT("FeedbackLengthDAxle"), 120);
static int EnableForceSpringEffectDAxle = GetSettingInt(TEXT("EnableForceSpringEffectDAxle"), 0);
static int ForceSpringStrengthDAxle = GetSettingInt(TEXT("ForceSpringStrengthDAxle"), 0);
static int SinePeriodDAxle = GetSettingInt(TEXT("SinePeriodDAxle"), 0);
static int SineFadePeriodDAxle = GetSettingInt(TEXT("SineFadePeriodDAxle"), 0);
static int SineStrengthDAxle = GetSettingInt(TEXT("SineStrengthDAxle"), 0);
static int RumbleStrengthLeftMotorDAxle = GetSettingInt(TEXT("RumbleStrengthLeftMotorDAxle"), 0);
static int RumbleStrengthRightMotorDAxle = GetSettingInt(TEXT("RumbleStrengthRightMotorDAxle"), 0);

static int configMinForceAfterBurner2 = GetSettingInt(TEXT("MinForceAfterBurner2"), 0);
static int configMaxForceAfterBurner2 = GetSettingInt(TEXT("MaxForceAfterBurner2"), 100);
static int RumbleStrengthLeftMotorAfterBurner2 = GetSettingInt(TEXT("RumbleStrengthLeftMotorAfterBurner2"), 0);
static int RumbleStrengthRightMotorAfterBurner2 = GetSettingInt(TEXT("RumbleStrengthRightMotorAfterBurner2"), 0);

static int configMinForceCisHeat = GetSettingInt(TEXT("MinForceCisHeat"), 0);
static int configMaxForceCisHeat = GetSettingInt(TEXT("MaxForceCisHeat"), 100);
static int configAlternativeMinForceLeftCisHeat = GetSettingInt(TEXT("AlternativeMinForceLeftCisHeat"), 0);
static int configAlternativeMaxForceLeftCisHeat = GetSettingInt(TEXT("AlternativeMaxForceLeftCisHeat"), 100);
static int configAlternativeMinForceRightCisHeat = GetSettingInt(TEXT("AlternativeMinForceRightCisHeat"), 0);
static int configAlternativeMaxForceRightCisHeat = GetSettingInt(TEXT("AlternativeMaxForceRightCisHeat"), 100);
static int configFeedbackLengthCisHeat = GetSettingInt(TEXT("FeedbackLengthCisHeat"), 120);
static int EnableForceSpringEffectCisHeat = GetSettingInt(TEXT("EnableForceSpringEffectCisHeat"), 0);
static int ForceSpringStrengthCisHeat = GetSettingInt(TEXT("ForceSpringStrengthCisHeat"), 0);
static int SinePeriodCisHeat = GetSettingInt(TEXT("SinePeriodCisHeat"), 0);
static int SineFadePeriodCisHeat = GetSettingInt(TEXT("SineFadePeriodCisHeat"), 0);
static int SineStrengthCisHeat = GetSettingInt(TEXT("SineStrengthCisHeat"), 0);
static int RumbleStrengthLeftMotorCisHeat = GetSettingInt(TEXT("RumbleStrengthLeftMotorCisHeat"), 0);
static int RumbleStrengthRightMotorCisHeat = GetSettingInt(TEXT("RumbleStrengthRightMotorCisHeat"), 0);

static int configMinForceF1GpStar = GetSettingInt(TEXT("MinForceF1GpStar"), 0);
static int configMaxForceF1GpStar = GetSettingInt(TEXT("MaxForceF1GpStar"), 100);
static int configAlternativeMinForceLeftF1GpStar = GetSettingInt(TEXT("AlternativeMinForceLeftF1GpStar"), 0);
static int configAlternativeMaxForceLeftF1GpStar = GetSettingInt(TEXT("AlternativeMaxForceLeftF1GpStar"), 100);
static int configAlternativeMinForceRightF1GpStar = GetSettingInt(TEXT("AlternativeMinForceRightF1GpStar"), 0);
static int configAlternativeMaxForceRightF1GpStar = GetSettingInt(TEXT("AlternativeMaxForceRightF1GpStar"), 100);
static int configFeedbackLengthF1GpStar = GetSettingInt(TEXT("FeedbackLengthF1GpStar"), 120);
static int EnableForceSpringEffectF1GpStar = GetSettingInt(TEXT("EnableForceSpringEffectF1GpStar"), 0);
static int ForceSpringStrengthF1GpStar = GetSettingInt(TEXT("ForceSpringStrengthF1GpStar"), 0);
static int SinePeriodF1GpStar = GetSettingInt(TEXT("SinePeriodF1GpStar"), 0);
static int SineFadePeriodF1GpStar = GetSettingInt(TEXT("SineFadePeriodF1GpStar"), 0);
static int SineStrengthF1GpStar = GetSettingInt(TEXT("SineStrengthF1GpStar"), 0);
static int RumbleStrengthLeftMotorF1GpStar = GetSettingInt(TEXT("RumbleStrengthLeftMotorF1GpStar"), 0);
static int RumbleStrengthRightMotorF1GpStar = GetSettingInt(TEXT("RumbleStrengthRightMotorF1GpStar"), 0);

static int configMinForceF1GpStar2 = GetSettingInt(TEXT("MinForceF1GpStar2"), 0);
static int configMaxForceF1GpStar2 = GetSettingInt(TEXT("MaxForceF1GpStar2"), 100);
static int configAlternativeMinForceLeftF1GpStar2 = GetSettingInt(TEXT("AlternativeMinForceLeftF1GpStar2"), 0);
static int configAlternativeMaxForceLeftF1GpStar2 = GetSettingInt(TEXT("AlternativeMaxForceLeftF1GpStar2"), 100);
static int configAlternativeMinForceRightF1GpStar2 = GetSettingInt(TEXT("AlternativeMinForceRightF1GpStar2"), 0);
static int configAlternativeMaxForceRightF1GpStar2 = GetSettingInt(TEXT("AlternativeMaxForceRightF1GpStar2"), 100);
static int configFeedbackLengthF1GpStar2 = GetSettingInt(TEXT("FeedbackLengthF1GpStar2"), 120);
static int EnableForceSpringEffectF1GpStar2 = GetSettingInt(TEXT("EnableForceSpringEffectF1GpStar2"), 0);
static int ForceSpringStrengthF1GpStar2 = GetSettingInt(TEXT("ForceSpringStrengthF1GpStar2"), 0);
static int SinePeriodF1GpStar2 = GetSettingInt(TEXT("SinePeriodF1GpStar2"), 0);
static int SineFadePeriodF1GpStar2 = GetSettingInt(TEXT("SineFadePeriodF1GpStar2"), 0);
static int SineStrengthF1GpStar2 = GetSettingInt(TEXT("SineStrengthF1GpStar2"), 0);
static int RumbleStrengthLeftMotorF1GpStar2 = GetSettingInt(TEXT("RumbleStrengthLeftMotorF1GpStar2"), 0);
static int RumbleStrengthRightMotorF1GpStar2 = GetSettingInt(TEXT("RumbleStrengthRightMotorF1GpStar2"), 0);

static int configMinForceHyperDrive = GetSettingInt(TEXT("MinForceHyperDrive"), 0);
static int configMaxForceHyperDrive = GetSettingInt(TEXT("MaxForceHyperDrive"), 100);
static int configAlternativeMinForceLeftHyperDrive = GetSettingInt(TEXT("AlternativeMinForceLeftHyperDrive"), 0);
static int configAlternativeMaxForceLeftHyperDrive = GetSettingInt(TEXT("AlternativeMaxForceLeftHyperDrive"), 100);
static int configAlternativeMinForceRightHyperDrive = GetSettingInt(TEXT("AlternativeMinForceRightHyperDrive"), 0);
static int configAlternativeMaxForceRightHyperDrive = GetSettingInt(TEXT("AlternativeMaxForceRightHyperDrive"), 100);
static int configFeedbackLengthHyperDrive = GetSettingInt(TEXT("FeedbackLengthHyperDrive"), 120);
static int EnableForceSpringEffectHyperDrive = GetSettingInt(TEXT("EnableForceSpringEffectHyperDrive"), 0);
static int ForceSpringStrengthHyperDrive = GetSettingInt(TEXT("ForceSpringStrengthHyperDrive"), 0);
static int EnableDamperHyperDrive = GetSettingInt(TEXT("EnableDamperHyperDrive"), 0);
static int DamperStrengthHyperDrive = GetSettingInt(TEXT("DamperStrengthHyperDrive"), 100);

static int configMinForceVaporTrx = GetSettingInt(TEXT("MinForceVaporTrx"), 0);
static int configMaxForceVaporTrx = GetSettingInt(TEXT("MaxForceVaporTrx"), 100);
static int configAlternativeMinForceLeftVaporTrx = GetSettingInt(TEXT("AlternativeMinForceLeftVaporTrx"), 0);
static int configAlternativeMaxForceLeftVaporTrx = GetSettingInt(TEXT("AlternativeMaxForceLeftVaporTrx"), 100);
static int configAlternativeMinForceRightVaporTrx = GetSettingInt(TEXT("AlternativeMinForceRightVaporTrx"), 0);
static int configAlternativeMaxForceRightVaporTrx = GetSettingInt(TEXT("AlternativeMaxForceRightVaporTrx"), 100);
static int configFeedbackLengthVaporTrx = GetSettingInt(TEXT("FeedbackLengthVaporTrx"), 120);
static int EnableForceSpringEffectVaporTrx = GetSettingInt(TEXT("EnableForceSpringEffectVaporTrx"), 0);
static int ForceSpringStrengthVaporTrx = GetSettingInt(TEXT("ForceSpringStrengthVaporTrx"), 0);
static int EnableDamperVaporTrx = GetSettingInt(TEXT("EnableDamperVaporTrx"), 0);
static int DamperStrengthVaporTrx = GetSettingInt(TEXT("DamperStrengthVaporTrx"), 100);

static int configMinForceRaveRacer = GetSettingInt(TEXT("MinForceRaveRacer"), 0);
static int configMaxForceRaveRacer = GetSettingInt(TEXT("MaxForceRaveRacer"), 100);
static int configAlternativeMinForceLeftRaveRacer = GetSettingInt(TEXT("AlternativeMinForceLeftRaveRacer"), 0);
static int configAlternativeMaxForceLeftRaveRacer = GetSettingInt(TEXT("AlternativeMaxForceLeftRaveRacer"), 100);
static int configAlternativeMinForceRightRaveRacer = GetSettingInt(TEXT("AlternativeMinForceRightRaveRacer"), 0);
static int configAlternativeMaxForceRightRaveRacer = GetSettingInt(TEXT("AlternativeMaxForceRightRaveRacer"), 100);
static int PowerModeRaveRacer = GetSettingInt(TEXT("PowerModeRaveRacer"), 0);
static int configFeedbackLengthRaveRacer = GetSettingInt(TEXT("FeedbackLengthRaveRacer"), 120);
static int EnableForceSpringEffectRaveRacer = GetSettingInt(TEXT("EnableForceSpringEffectRaveRacer"), 0);
static int ForceSpringStrengthRaveRacer = GetSettingInt(TEXT("ForceSpringStrengthRaveRacer"), 0);
static int EnableDamperRaveRacer= GetSettingInt(TEXT("EnableDamperRaveRacer"), 0);
static int DamperStrengthRaveRacer = GetSettingInt(TEXT("DamperStrengthRaveRacer"), 100);

static int configMinForceDaytonaMAME = GetSettingInt(TEXT("MinForceDaytona"), 0);
static int configMaxForceDaytonaMAME = GetSettingInt(TEXT("MaxForceDaytona"), 100);
static int configAlternativeMinForceLeftDaytonaMAME = GetSettingInt(TEXT("AlternativeMinForceLeftDaytona"), 0);
static int configAlternativeMaxForceLeftDaytonaMAME = GetSettingInt(TEXT("AlternativeMaxForceLeftDaytona"), 100);
static int configAlternativeMinForceRightDaytonaMAME = GetSettingInt(TEXT("AlternativeMinForceRightDaytona"), 0);
static int configAlternativeMaxForceRightDaytonaMAME = GetSettingInt(TEXT("AlternativeMaxForceRightDaytona"), 100);
static int PowerModeDaytonaMAME = GetSettingInt(TEXT("PowerModeDaytona"), 0);
static int configFeedbackLengthDaytonaMAME = GetSettingInt(TEXT("FeedbackLengthDaytona"), 120);
static int EnableForceSpringEffectDaytonaMAME = GetSettingInt(TEXT("EnableForceSpringEffectDaytona"), 0);
static int ForceSpringStrengthDaytonaMAME = GetSettingInt(TEXT("ForceSpringStrengthDaytona"), 0);
static int EnableDamperDaytonaMAME = GetSettingInt(TEXT("EnableDamperDaytona"), 0);
static int DamperStrengthDaytonaMAME = GetSettingInt(TEXT("DamperStrengthDaytona"), 100);

static int configMinForceSRallyMAME = GetSettingInt(TEXT("MinForceSRally"), 0);
static int configMaxForceSRallyMAME = GetSettingInt(TEXT("MaxForceSRally"), 100);
static int configAlternativeMinForceLeftSRallyMAME = GetSettingInt(TEXT("AlternativeMinForceLeftSRally"), 0);
static int configAlternativeMaxForceLeftSRallyMAME = GetSettingInt(TEXT("AlternativeMaxForceLeftSRally"), 100);
static int configAlternativeMinForceRightSRallyMAME = GetSettingInt(TEXT("AlternativeMinForceRightSRally"), 0);
static int configAlternativeMaxForceRightSRallyMAME = GetSettingInt(TEXT("AlternativeMaxForceRightSRally"), 100);
static int PowerModeSRallyMAME = GetSettingInt(TEXT("PowerModeSRally"), 0);
static int configFeedbackLengthSRallyMAME = GetSettingInt(TEXT("FeedbackLengthSRally"), 120);
static int EnableForceSpringEffectSRallyMAME = GetSettingInt(TEXT("EnableForceSpringEffectSRally"), 0);
static int ForceSpringStrengthSRallyMAME = GetSettingInt(TEXT("ForceSpringStrengthSRally"), 0);
static int EnableDamperSRallyMAME = GetSettingInt(TEXT("EnableDamperSRally"), 0);
static int DamperStrengthSRallyMAME = GetSettingInt(TEXT("DamperStrengthSRally"), 100);

static int configMinForceIndy500MAME = GetSettingInt(TEXT("MinForceIndy500"), 0);
static int configMaxForceIndy500MAME = GetSettingInt(TEXT("MaxForceIndy500"), 100);
static int configAlternativeMinForceLeftIndy500MAME = GetSettingInt(TEXT("AlternativeMinForceLeftIndy500"), 0);
static int configAlternativeMaxForceLeftIndy500MAME = GetSettingInt(TEXT("AlternativeMaxForceLeftIndy500"), 100);
static int configAlternativeMinForceRightIndy500MAME = GetSettingInt(TEXT("AlternativeMinForceRightIndy500"), 0);
static int configAlternativeMaxForceRightIndy500MAME = GetSettingInt(TEXT("AlternativeMaxForceRightIndy500"), 100);
static int PowerModeIndy500MAME = GetSettingInt(TEXT("PowerModeIndy500"), 0);
static int configFeedbackLengthIndy500MAME = GetSettingInt(TEXT("FeedbackLengthIndy500"), 120);
static int EnableForceSpringEffectIndy500MAME = GetSettingInt(TEXT("EnableForceSpringEffectIndy500"), 0);
static int ForceSpringStrengthIndy500MAME = GetSettingInt(TEXT("ForceSpringStrengthIndy500"), 0);
static int EnableDamperIndy500MAME = GetSettingInt(TEXT("EnableDamperIndy500"), 0);
static int DamperStrengthIndy500MAME = GetSettingInt(TEXT("DamperStrengthIndy500"), 100);

static int configMinForceSuperGTMAME = GetSettingInt(TEXT("MinForceSuperGT"), 0);
static int configMaxForceSuperGTMAME = GetSettingInt(TEXT("MaxForceSuperGT"), 100);
static int configAlternativeMinForceLeftSuperGTMAME = GetSettingInt(TEXT("AlternativeMinForceLeftSuperGT"), 0);
static int configAlternativeMaxForceLeftSuperGTMAME = GetSettingInt(TEXT("AlternativeMaxForceLeftSuperGT"), 100);
static int configAlternativeMinForceRightSuperGTMAME = GetSettingInt(TEXT("AlternativeMinForceRightSuperGT"), 0);
static int configAlternativeMaxForceRightSuperGTMAME = GetSettingInt(TEXT("AlternativeMaxForceRightSuperGT"), 100);
static int PowerModeSuperGTMAME = GetSettingInt(TEXT("PowerModeSuperGT"), 0);
static int configFeedbackLengthSuperGTMAME = GetSettingInt(TEXT("FeedbackLengthSuperGT"), 120);
static int EnableForceSpringEffectSuperGTMAME = GetSettingInt(TEXT("EnableForceSpringEffectSuperGT"), 0);
static int ForceSpringStrengthSuperGTMAME = GetSettingInt(TEXT("ForceSpringStrengthSuperGT"), 0);
static int EnableDamperSuperGTMAME = GetSettingInt(TEXT("EnableDamperSuperGT"), 0);
static int DamperStrengthSuperGTMAME = GetSettingInt(TEXT("DamperStrengthSuperGT"), 100);

static int configMinForceSTCCMAME = GetSettingInt(TEXT("MinForceSTCC"), 0);
static int configMaxForceSTCCMAME = GetSettingInt(TEXT("MaxForceSTCC"), 100);
static int configAlternativeMinForceLeftSTCCMAME = GetSettingInt(TEXT("AlternativeMinForceLeftSTCC"), 0);
static int configAlternativeMaxForceLeftSTCCMAME = GetSettingInt(TEXT("AlternativeMaxForceLeftSTCC"), 100);
static int configAlternativeMinForceRightSTCCMAME = GetSettingInt(TEXT("AlternativeMinForceRightSTCC"), 0);
static int configAlternativeMaxForceRightSTCCMAME = GetSettingInt(TEXT("AlternativeMaxForceRightSTCC"), 100);
static int PowerModeSTCCMAME = GetSettingInt(TEXT("PowerModeSTCC"), 0);
static int configFeedbackLengthSTCCMAME = GetSettingInt(TEXT("FeedbackLengthSTCC"), 120);
static int EnableForceSpringEffectSTCCMAME = GetSettingInt(TEXT("EnableForceSpringEffectSTCC"), 0);
static int ForceSpringStrengthSTCCMAME = GetSettingInt(TEXT("ForceSpringStrengthSTCC"), 0);
static int EnableDamperSTCCMAME = GetSettingInt(TEXT("EnableDamperSTCC"), 0);
static int DamperStrengthSTCCMAME = GetSettingInt(TEXT("DamperStrengthSTCC"), 100);

static int configMinForceSuperChase = GetSettingInt(TEXT("MinForceSuperChase"), 0);
static int configMaxForceSuperChase = GetSettingInt(TEXT("MaxForceSuperChase"), 100);
static int configAlternativeMinForceLeftSuperChase = GetSettingInt(TEXT("AlternativeMinForceLeftSuperChase"), 0);
static int configAlternativeMaxForceLeftSuperChase = GetSettingInt(TEXT("AlternativeMaxForceLeftSuperChase"), 100);
static int configAlternativeMinForceRightSuperChase = GetSettingInt(TEXT("AlternativeMinForceRightSuperChase"), 0);
static int configAlternativeMaxForceRightSuperChase = GetSettingInt(TEXT("AlternativeMaxForceRightSuperChase"), 100);
static int PowerModeSuperChase = GetSettingInt(TEXT("PowerModeSuperChase"), 0);
static int configFeedbackLengthSuperChase = GetSettingInt(TEXT("FeedbackLengthSuperChase"), 120);
static int EnableForceSpringEffectSuperChase = GetSettingInt(TEXT("EnableForceSpringEffectSuperChase"), 0);
static int ForceSpringStrengthSuperChase = GetSettingInt(TEXT("ForceSpringStrengthSuperChase"), 0);
static int EnableDamperSuperChase = GetSettingInt(TEXT("EnableDamperSuperChase"), 0);
static int DamperStrengthSuperChase = GetSettingInt(TEXT("DamperStrengthSuperChase"), 100);

static int configMinForceDirtDash = GetSettingInt(TEXT("MinForceDirtDash"), 0);
static int configMaxForceDirtDash = GetSettingInt(TEXT("MaxForceDirtDash"), 100);
static int configAlternativeMinForceLeftDirtDash = GetSettingInt(TEXT("AlternativeMinForceLeftDirtDash"), 0);
static int configAlternativeMaxForceLeftDirtDash = GetSettingInt(TEXT("AlternativeMaxForceLeftDirtDash"), 100);
static int configAlternativeMinForceRightDirtDash = GetSettingInt(TEXT("AlternativeMinForceRightDirtDash"), 0);
static int configAlternativeMaxForceRightDirtDash = GetSettingInt(TEXT("AlternativeMaxForceRightDirtDash"), 100);
static int PowerModeDirtDash = GetSettingInt(TEXT("PowerModeDirtDash"), 0);
static int configFeedbackLengthDirtDash = GetSettingInt(TEXT("FeedbackLengthDirtDash"), 120);
static int EnableForceSpringEffectDirtDash = GetSettingInt(TEXT("EnableForceSpringEffectDirtDash"), 0);
static int ForceSpringStrengthDirtDash = GetSettingInt(TEXT("ForceSpringStrengthDirtDash"), 0);
static int FFBDivideDirtDash = GetSettingInt(TEXT("FFBDivideDirtDash"), 0);
static int EnableDamperDirtDash = GetSettingInt(TEXT("EnableDamperDirtDash"), 0);
static int DamperStrengthDirtDash = GetSettingInt(TEXT("DamperStrengthDirtDash"), 100);

static int configMinForceAceDriverVictory= GetSettingInt(TEXT("MinForceAceDriverVictory"), 0);
static int configMaxForceAceDriverVictory = GetSettingInt(TEXT("MaxForceAceDriverVictory"), 100);
static int configAlternativeMinForceLeftAceDriverVictory = GetSettingInt(TEXT("AlternativeMinForceLeftAceDriverVictory"), 0);
static int configAlternativeMaxForceLeftAceDriverVictory = GetSettingInt(TEXT("AlternativeMaxForceLeftAceDriverVictory"), 100);
static int configAlternativeMinForceRightAceDriverVictory = GetSettingInt(TEXT("AlternativeMinForceRightAceDriverVictory"), 0);
static int configAlternativeMaxForceRightAceDriverVictory = GetSettingInt(TEXT("AlternativeMaxForceRightAceDriverVictory"), 100);
static int PowerModeAceDriverVictory = GetSettingInt(TEXT("PowerModeAceDriverVictory"), 0);
static int configFeedbackLengthAceDriverVictory = GetSettingInt(TEXT("FeedbackLengthAceDriverVictory"), 120);
static int EnableForceSpringEffectAceDriverVictory = GetSettingInt(TEXT("EnableForceSpringEffectAceDriverVictory"), 0);
static int ForceSpringStrengthAceDriverVictory = GetSettingInt(TEXT("ForceSpringStrengthAceDriverVictory"), 0);
static int FFBDivideAceDriverVictory = GetSettingInt(TEXT("FFBDivideAceDriverVictory"), 0);
static int EnableDamperAceDriverVictory = GetSettingInt(TEXT("EnableDamperAceDriverVictory"), 0);
static int DamperStrengthAceDriverVictory = GetSettingInt(TEXT("DamperStrengthAceDriverVictory"), 100);

static int configMinForceAceDriver = GetSettingInt(TEXT("MinForceAceDriver"), 0);
static int configMaxForceAceDriver = GetSettingInt(TEXT("MaxForceAceDriver"), 100);
static int configAlternativeMinForceLeftAceDriver = GetSettingInt(TEXT("AlternativeMinForceLeftAceDriver"), 0);
static int configAlternativeMaxForceLeftAceDriver = GetSettingInt(TEXT("AlternativeMaxForceLeftAceDriver"), 100);
static int configAlternativeMinForceRightAceDriver = GetSettingInt(TEXT("AlternativeMinForceRightAceDriver"), 0);
static int configAlternativeMaxForceRightAceDriver= GetSettingInt(TEXT("AlternativeMaxForceRightAceDriver"), 100);
static int PowerModeAceDriver = GetSettingInt(TEXT("PowerModeAceDriver"), 0);
static int configFeedbackLengthAceDriver = GetSettingInt(TEXT("FeedbackLengthAceDriver"), 120);
static int EnableForceSpringEffectAceDriver = GetSettingInt(TEXT("EnableForceSpringEffectAceDriver"), 0);
static int ForceSpringStrengthAceDriver = GetSettingInt(TEXT("ForceSpringStrengthAceDriver"), 0);
static int FFBDivideAceDriver = GetSettingInt(TEXT("FFBDivideAceDriver"), 0);
static int EnableDamperAceDriver = GetSettingInt(TEXT("EnableDamperAceDriver"), 0);
static int DamperStrengthAceDriver = GetSettingInt(TEXT("DamperStrengthAceDriver"), 100);

static int configMinForceOverRev = GetSettingInt(TEXT("MinForceOverRev"), 0);
static int configMaxForceOverRev = GetSettingInt(TEXT("MaxForceOverRev"), 100);
static int configAlternativeMinForceLeftOverRev = GetSettingInt(TEXT("AlternativeMinForceLeftOverRev"), 0);
static int configAlternativeMaxForceLeftOverRev = GetSettingInt(TEXT("AlternativeMaxForceLeftOverRev"), 100);
static int configAlternativeMinForceRightOverRev = GetSettingInt(TEXT("AlternativeMinForceRightOverRev"), 0);
static int configAlternativeMaxForceRightOverRev = GetSettingInt(TEXT("AlternativeMaxForceRightOverRev"), 100);
static int configFeedbackLengthOverRev = GetSettingInt(TEXT("FeedbackLengthOverRev"), 120);
static int PowerModeOverRev = GetSettingInt(TEXT("PowerModeOverRev"), 0);
static int EnableForceSpringEffectOverRev = GetSettingInt(TEXT("EnableForceSpringEffectOverRev"), 0);
static int ForceSpringStrengthOverRev = GetSettingInt(TEXT("ForceSpringStrengthOverRev"), 0);
static int EnableDamperOverRev = GetSettingInt(TEXT("EnableDamperOverRev"), 0);
static int DamperStrengthOverRev = GetSettingInt(TEXT("DamperStrengthOverRev"), 100);

static int configMinForceCartFury = GetSettingInt(TEXT("MinForceCartFury"), 0);
static int configMaxForceCartFury = GetSettingInt(TEXT("MaxForceCartFury"), 100);
static int configAlternativeMinForceLeftCartFury = GetSettingInt(TEXT("AlternativeMinForceLeftCartFury"), 0);
static int configAlternativeMaxForceLeftCartFury = GetSettingInt(TEXT("AlternativeMaxForceLeftCartFury"), 100);
static int configAlternativeMinForceRightCartFury = GetSettingInt(TEXT("AlternativeMinForceRightCartFury"), 0);
static int configAlternativeMaxForceRightCartFury = GetSettingInt(TEXT("AlternativeMaxForceRightCartFury"), 100);
static int configFeedbackLengthCartFury = GetSettingInt(TEXT("FeedbackLengthCartFury"), 120);
static int PowerModeCartFury = GetSettingInt(TEXT("PowerModeCartFury"), 0);
static int EnableForceSpringEffectCartFury = GetSettingInt(TEXT("EnableForceSpringEffectCartFury"), 0);
static int ForceSpringStrengthCartFury = GetSettingInt(TEXT("ForceSpringStrengthCartFury"), 0);
static int EnableDamperCartFury = GetSettingInt(TEXT("EnableDamperCartFury"), 0);
static int DamperStrengthCartFury = GetSettingInt(TEXT("DamperStrengthCartFury"), 100);

static int configMinForceThrillD = GetSettingInt(TEXT("MinForceThrillD"), 0);
static int configMaxForceThrillD = GetSettingInt(TEXT("MaxForceThrillD"), 100);
static int configAlternativeMinForceLeftThrillD = GetSettingInt(TEXT("AlternativeMinForceLeftThrillD"), 0);
static int configAlternativeMaxForceLeftThrillD = GetSettingInt(TEXT("AlternativeMaxForceLeftThrillD"), 100);
static int configAlternativeMinForceRightThrillD = GetSettingInt(TEXT("AlternativeMinForceRightThrillD"), 0);
static int configAlternativeMaxForceRightThrillD = GetSettingInt(TEXT("AlternativeMaxForceRightThrillD"), 100);
static int configFeedbackLengthThrillD = GetSettingInt(TEXT("FeedbackLengthThrillD"), 120);
static int PowerModeThrillD = GetSettingInt(TEXT("PowerModeThrillD"), 0);
static int EnableForceSpringEffectThrillD = GetSettingInt(TEXT("EnableForceSpringEffectThrillD"), 0);
static int ForceSpringStrengthThrillD = GetSettingInt(TEXT("ForceSpringStrengthThrillD"), 0);
static int EnableDamperThrillD = GetSettingInt(TEXT("EnableDamperThrillD"), 0);
static int DamperStrengthThrillD = GetSettingInt(TEXT("DamperStrengthThrillD"), 100);

static int configMinForceWindheat = GetSettingInt(TEXT("MinForceWindheat"), 0);
static int configMaxForceWindheat = GetSettingInt(TEXT("MaxForceWindheat"), 100);
static int configAlternativeMinForceLeftWindheat = GetSettingInt(TEXT("AlternativeMinForceLeftWindheat"), 0);
static int configAlternativeMaxForceLeftWindheat = GetSettingInt(TEXT("AlternativeMaxForceLeftWindheat"), 100);
static int configAlternativeMinForceRightWindheat = GetSettingInt(TEXT("AlternativeMinForceRightWindheat"), 0);
static int configAlternativeMaxForceRightWindheat = GetSettingInt(TEXT("AlternativeMaxForceRightWindheat"), 100);
static int configFeedbackLengthWindheat = GetSettingInt(TEXT("FeedbackLengthWindheat"), 120);
static int PowerModeWindheat = GetSettingInt(TEXT("PowerModeWindheat"), 0);
static int EnableForceSpringEffectWindheat = GetSettingInt(TEXT("EnableForceSpringEffectWindheat"), 0);
static int ForceSpringStrengthWindheat = GetSettingInt(TEXT("ForceSpringStrengthWindheat"), 0);
static int EnableDamperWindheat = GetSettingInt(TEXT("EnableDamperWindheat"), 0);
static int DamperStrengthWindheat = GetSettingInt(TEXT("DamperStrengthWindheat"), 100);

static int configMinForceMidnRun = GetSettingInt(TEXT("MinForceMidnRun"), 0);
static int configMaxForceMidnRun = GetSettingInt(TEXT("MaxForceMidnRun"), 100);
static int configAlternativeMinForceLeftMidnRun = GetSettingInt(TEXT("AlternativeMinForceLeftMidnRun"), 0);
static int configAlternativeMaxForceLeftMidnRun = GetSettingInt(TEXT("AlternativeMaxForceLeftMidnRun"), 100);
static int configAlternativeMinForceRightMidnRun = GetSettingInt(TEXT("AlternativeMinForceRightMidnRun"), 0);
static int configAlternativeMaxForceRightMidnRun = GetSettingInt(TEXT("AlternativeMaxForceRightMidnRun"), 100);
static int configFeedbackLengthMidnRun = GetSettingInt(TEXT("FeedbackLengthMidnRun"), 120);
static int PowerModeMidnRun = GetSettingInt(TEXT("PowerModeMidnRun"), 0);
static int EnableForceSpringEffectMidnRun = GetSettingInt(TEXT("EnableForceSpringEffectMidnRun"), 0);
static int ForceSpringStrengthMidnRun = GetSettingInt(TEXT("ForceSpringStrengthMidnRun"), 0);
static int EnableDamperMidnRun = GetSettingInt(TEXT("EnableDamperMidnRun"), 0);
static int DamperStrengthMidnRun = GetSettingInt(TEXT("DamperStrengthMidnRun"), 100);

static int configMinForceRacingJ = GetSettingInt(TEXT("MinForceRacingJ"), 0);
static int configMaxForceRacingJ = GetSettingInt(TEXT("MaxForceRacingJ"), 100);
static int configAlternativeMinForceLeftRacingJ = GetSettingInt(TEXT("AlternativeMinForceLeftRacingJ"), 0);
static int configAlternativeMaxForceLeftRacingJ = GetSettingInt(TEXT("AlternativeMaxForceLeftRacingJ"), 100);
static int configAlternativeMinForceRightRacingJ = GetSettingInt(TEXT("AlternativeMinForceRightRacingJ"), 0);
static int configAlternativeMaxForceRightRacingJ = GetSettingInt(TEXT("AlternativeMaxForceRightRacingJ"), 100);
static int configFeedbackLengthRacingJ = GetSettingInt(TEXT("FeedbackLengthRacingJ"), 120);
static int PowerModeRacingJ = GetSettingInt(TEXT("PowerModeRacingJ"), 0);
static int EnableForceSpringEffectRacingJ = GetSettingInt(TEXT("EnableForceSpringEffectRacingJ"), 0);
static int ForceSpringStrengthRacingJ = GetSettingInt(TEXT("ForceSpringStrengthRacingJ"), 0);
static int EnableDamperRacingJ = GetSettingInt(TEXT("EnableDamperRacingJ"), 0);
static int DamperStrengthRacingJ = GetSettingInt(TEXT("DamperStrengthRacingJ"), 100);

static int configMinForceRacingJ2 = GetSettingInt(TEXT("MinForceRacingJ2"), 0);
static int configMaxForceRacingJ2 = GetSettingInt(TEXT("MaxForceRacingJ2"), 100);
static int configAlternativeMinForceLeftRacingJ2 = GetSettingInt(TEXT("AlternativeMinForceLeftRacingJ2"), 0);
static int configAlternativeMaxForceLeftRacingJ2 = GetSettingInt(TEXT("AlternativeMaxForceLeftRacingJ2"), 100);
static int configAlternativeMinForceRightRacingJ2 = GetSettingInt(TEXT("AlternativeMinForceRightRacingJ2"), 0);
static int configAlternativeMaxForceRightRacingJ2 = GetSettingInt(TEXT("AlternativeMaxForceRightRacingJ2"), 100);
static int configFeedbackLengthRacingJ2 = GetSettingInt(TEXT("FeedbackLengthRacingJ2"), 120);
static int PowerModeRacingJ2 = GetSettingInt(TEXT("PowerModeRacingJ2"), 0);
static int EnableForceSpringEffectRacingJ2 = GetSettingInt(TEXT("EnableForceSpringEffectRacingJ2"), 0);
static int ForceSpringStrengthRacingJ2 = GetSettingInt(TEXT("ForceSpringStrengthRacingJ2"), 0);
static int EnableDamperRacingJ2 = GetSettingInt(TEXT("EnableDamperRacingJ2"), 0);
static int DamperStrengthRacingJ2 = GetSettingInt(TEXT("DamperStrengthRacingJ2"), 100);

static int configMinForceGTIClub = GetSettingInt(TEXT("MinForceGTIClub"), 0);
static int configMaxForceGTIClub = GetSettingInt(TEXT("MaxForceGTIClub"), 100);
static int configAlternativeMinForceLeftGTIClub = GetSettingInt(TEXT("AlternativeMinForceLeftGTIClub"), 0);
static int configAlternativeMaxForceLeftGTIClub = GetSettingInt(TEXT("AlternativeMaxForceLeftGTIClub"), 100);
static int configAlternativeMinForceRightGTIClub = GetSettingInt(TEXT("AlternativeMinForceRightGTIClub"), 0);
static int configAlternativeMaxForceRightGTIClub = GetSettingInt(TEXT("AlternativeMaxForceRightGTIClub"), 100);
static int configFeedbackLengthGTIClub = GetSettingInt(TEXT("FeedbackLengthGTIClub"), 120);
static int PowerModeGTIClub = GetSettingInt(TEXT("PowerModeGTIClub"), 0);
static int EnableForceSpringEffectGTIClub = GetSettingInt(TEXT("EnableForceSpringEffectGTIClub"), 0);
static int ForceSpringStrengthGTIClub = GetSettingInt(TEXT("ForceSpringStrengthGTIClub"), 0);
static int EnableDamperGTIClub = GetSettingInt(TEXT("EnableDamperGTIClub"), 0);
static int DamperStrengthGTIClub = GetSettingInt(TEXT("DamperStrengthGTIClub"), 100);

static int configMinForceRoadEdge = GetSettingInt(TEXT("MinForceRoadEdge"), 0);
static int configMaxForceRoadEdge = GetSettingInt(TEXT("MaxForceRoadEdge"), 100);
static int configAlternativeMinForceLeftRoadEdge = GetSettingInt(TEXT("AlternativeMinForceLeftRoadEdge"), 0);
static int configAlternativeMaxForceLeftRoadEdge = GetSettingInt(TEXT("AlternativeMaxForceLeftRoadEdge"), 100);
static int configAlternativeMinForceRightRoadEdge = GetSettingInt(TEXT("AlternativeMinForceRightRoadEdge"), 0);
static int configAlternativeMaxForceRightRoadEdge = GetSettingInt(TEXT("AlternativeMaxForceRightRoadEdge"), 100);
static int PowerModeRoadEdge = GetSettingInt(TEXT("PowerModeRoadEdge"), 0);
static int configFeedbackLengthRoadEdge = GetSettingInt(TEXT("FeedbackLengthRoadEdge"), 120);
static int EnableForceSpringEffectRoadEdge = GetSettingInt(TEXT("EnableForceSpringEffectRoadEdge"), 0);
static int ForceSpringStrengthRoadEdge = GetSettingInt(TEXT("ForceSpringStrengthRoadEdge"), 0);
static int EnableDamperRoadEdge = GetSettingInt(TEXT("EnableDamperRoadEdge"), 0);
static int DamperStrengthRoadEdge = GetSettingInt(TEXT("DamperStrengthRoadEdge"), 100);

static int configMinForceXRally = GetSettingInt(TEXT("MinForceXRally"), 0);
static int configMaxForceXRally = GetSettingInt(TEXT("MaxForceXRally"), 100);
static int configAlternativeMinForceLeftXRally = GetSettingInt(TEXT("AlternativeMinForceLeftXRally"), 0);
static int configAlternativeMaxForceLeftXRally = GetSettingInt(TEXT("AlternativeMaxForceLeftXRally"), 100);
static int configAlternativeMinForceRightXRally = GetSettingInt(TEXT("AlternativeMinForceRightXRally"), 0);
static int configAlternativeMaxForceRightXRally = GetSettingInt(TEXT("AlternativeMaxForceRightXRally"), 100);
static int PowerModeXRally = GetSettingInt(TEXT("PowerModeXRally"), 0);
static int configFeedbackLengthXRally = GetSettingInt(TEXT("FeedbackLengthXRally"), 120);
static int EnableForceSpringEffectXRally = GetSettingInt(TEXT("EnableForceSpringEffectXRally"), 0);
static int ForceSpringStrengthXRally = GetSettingInt(TEXT("ForceSpringStrengthXRally"), 0);
static int EnableDamperXRally = GetSettingInt(TEXT("EnableDamperXRally"), 0);
static int DamperStrengthXRally = GetSettingInt(TEXT("DamperStrengthXRally"), 100);

static int configMinForceSideBS = GetSettingInt(TEXT("MinForceSideBS"), 0);
static int configMaxForceSideBS = GetSettingInt(TEXT("MaxForceSideBS"), 100);
static int configAlternativeMinForceLeftSideBS = GetSettingInt(TEXT("AlternativeMinForceLeftSideBS"), 0);
static int configAlternativeMaxForceLeftSideBS = GetSettingInt(TEXT("AlternativeMaxForceLeftSideBS"), 100);
static int configAlternativeMinForceRightSideBS = GetSettingInt(TEXT("AlternativeMinForceRightSideBS"), 0);
static int configAlternativeMaxForceRightSideBS = GetSettingInt(TEXT("AlternativeMaxForceRightSideBS"), 100);
static int configFeedbackLengthSideBS = GetSettingInt(TEXT("FeedbackLengthSideBS"), 120);
static int PowerModeSideBS = GetSettingInt(TEXT("PowerModeSideBS"), 0);
static int EnableForceSpringEffectSideBS = GetSettingInt(TEXT("EnableForceSpringEffectSideBS"), 0);
static int ForceSpringStrengthSideBS = GetSettingInt(TEXT("ForceSpringStrengthSideBS"), 0);
static int EnableDamperSideBS = GetSettingInt(TEXT("EnableDamperSideBS"), 0);
static int DamperStrengthSideBS = GetSettingInt(TEXT("DamperStrengthSideBS"), 100);

static int configMinForceSideBS2 = GetSettingInt(TEXT("MinForceSideBS2"), 0);
static int configMaxForceSideBS2 = GetSettingInt(TEXT("MaxForceSideBS2"), 100);
static int configAlternativeMinForceLeftSideBS2 = GetSettingInt(TEXT("AlternativeMinForceLeftSideBS2"), 0);
static int configAlternativeMaxForceLeftSideBS2 = GetSettingInt(TEXT("AlternativeMaxForceLeftSideBS2"), 100);
static int configAlternativeMinForceRightSideBS2 = GetSettingInt(TEXT("AlternativeMinForceRightSideBS2"), 0);
static int configAlternativeMaxForceRightSideBS2 = GetSettingInt(TEXT("AlternativeMaxForceRightSideBS2"), 100);
static int configFeedbackLengthSideBS2 = GetSettingInt(TEXT("FeedbackLengthSideBS2"), 120);
static int PowerModeSideBS2 = GetSettingInt(TEXT("PowerModeSideBS2"), 0);
static int EnableForceSpringEffectSideBS2 = GetSettingInt(TEXT("EnableForceSpringEffectSideBS2"), 0);
static int ForceSpringStrengthSideBS2 = GetSettingInt(TEXT("ForceSpringStrengthSideBS2"), 0);
static int EnableDamperSideBS2 = GetSettingInt(TEXT("EnableDamperSideBS2"), 0);
static int DamperStrengthSideBS2 = GetSettingInt(TEXT("DamperStrengthSideBS2"), 100);

static int configMinForceSpeedUp = GetSettingInt(TEXT("MinForceSpeedUp"), 0);
static int configMaxForceSpeedUp = GetSettingInt(TEXT("MaxForceSpeedUp"), 100);
static int configAlternativeMinForceLeftSpeedUp = GetSettingInt(TEXT("AlternativeMinForceLeftSpeedUp"), 0);
static int configAlternativeMaxForceLeftSpeedUp = GetSettingInt(TEXT("AlternativeMaxForceLeftSpeedUp"), 100);
static int configAlternativeMinForceRightSpeedUp = GetSettingInt(TEXT("AlternativeMinForceRightSpeedUp"), 0);
static int configAlternativeMaxForceRightSpeedUp = GetSettingInt(TEXT("AlternativeMaxForceRightSpeedUp"), 100);
static int configFeedbackLengthSpeedUp = GetSettingInt(TEXT("FeedbackLengthSpeedUp"), 120);
static int PowerModeSpeedUp = GetSettingInt(TEXT("PowerModeSpeedUp"), 0);
static int EnableForceSpringEffectSpeedUp = GetSettingInt(TEXT("EnableForceSpringEffectSpeedUp"), 0);
static int ForceSpringStrengthSpeedUp = GetSettingInt(TEXT("ForceSpringStrengthSpeedUp"), 0);
static int EnableDamperSpeedUp = GetSettingInt(TEXT("EnableDamperSpeedUp"), 0);
static int DamperStrengthSpeedUp = GetSettingInt(TEXT("DamperStrengthSpeedUp"), 100);

static int configMinForceDriversEdge = GetSettingInt(TEXT("MinForceDriversEdge"), 0);
static int configMaxForceDriversEdge = GetSettingInt(TEXT("MaxForceDriversEdge"), 100);
static int configAlternativeMinForceLeftDriversEdge = GetSettingInt(TEXT("AlternativeMinForceLeftDriversEdge"), 0);
static int configAlternativeMaxForceLeftDriversEdge = GetSettingInt(TEXT("AlternativeMaxForceLeftDriversEdge"), 100);
static int configAlternativeMinForceRightDriversEdge = GetSettingInt(TEXT("AlternativeMinForceRightDriversEdge"), 0);
static int configAlternativeMaxForceRightDriversEdge = GetSettingInt(TEXT("AlternativeMaxForceRightDriversEdge"), 100);
static int configFeedbackLengthDriversEdge = GetSettingInt(TEXT("FeedbackLengthDriversEdge"), 120);
static int PowerModeDriversEdge = GetSettingInt(TEXT("PowerModeDriversEdge"), 0);
static int EnableForceSpringEffectDriversEdge = GetSettingInt(TEXT("EnableForceSpringEffectDriversEdge"), 0);
static int ForceSpringStrengthDriversEdge = GetSettingInt(TEXT("ForceSpringStrengthDriversEdge"), 0);
static int EnableDamperDriversEdge = GetSettingInt(TEXT("EnableDamperDriversEdge"), 0);
static int DamperStrengthDriversEdge = GetSettingInt(TEXT("DamperStrengthDriversEdge"), 100);

static int configMinForceInitialD = GetSettingInt(TEXT("MinForceInitialD"), 0);
static int configMaxForceInitialD = GetSettingInt(TEXT("MaxForceInitialD"), 100);
static int configAlternativeMinForceLeftInitialD = GetSettingInt(TEXT("AlternativeMinForceLeftInitialD"), 0);
static int configAlternativeMaxForceLeftInitialD = GetSettingInt(TEXT("AlternativeMaxForceLeftInitialD"), 100);
static int configAlternativeMinForceRightInitialD = GetSettingInt(TEXT("AlternativeMinForceRightInitialD"), 0);
static int configAlternativeMaxForceRightInitialD = GetSettingInt(TEXT("AlternativeMaxForceRightInitialD"), 100);
static int configFeedbackLengthInitialD = GetSettingInt(TEXT("FeedbackLengthInitialD"), 120);
static int PowerModeInitialD = GetSettingInt(TEXT("PowerModeInitialD"), 0);
static int EnableForceSpringEffectInitialD = GetSettingInt(TEXT("EnableForceSpringEffectInitialD"), 0);
static int ForceSpringStrengthInitialD = GetSettingInt(TEXT("ForceSpringStrengthInitialD"), 0);
static int EnableDamperInitialD = GetSettingInt(TEXT("EnableDamperInitialD"), 0);
static int DamperStrengthInitialD = GetSettingInt(TEXT("DamperStrengthInitialD"), 100);
static int DoubleSineInitialD = GetSettingInt(TEXT("DoubleSineInitialD"), 0);
static int DoubleConstantInitialD = GetSettingInt(TEXT("DoubleConstantInitialD"), 0);

static int configMinForceF355 = GetSettingInt(TEXT("MinForceF355"), 0);
static int configMaxForceF355 = GetSettingInt(TEXT("MaxForceF355"), 100);
static int configAlternativeMinForceLeftF355 = GetSettingInt(TEXT("AlternativeMinForceLeftF355"), 0);
static int configAlternativeMaxForceLeftF355 = GetSettingInt(TEXT("AlternativeMaxForceLeftF355"), 100);
static int configAlternativeMinForceRightF355 = GetSettingInt(TEXT("AlternativeMinForceRightF355"), 0);
static int configAlternativeMaxForceRightF355 = GetSettingInt(TEXT("AlternativeMaxForceRightF355"), 100);
static int configFeedbackLengthF355 = GetSettingInt(TEXT("FeedbackLengthF355"), 120);
static int PowerModeF355 = GetSettingInt(TEXT("PowerModeF355"), 0);
static int EnableForceSpringEffectF355 = GetSettingInt(TEXT("EnableForceSpringEffectF355"), 0);
static int ForceSpringStrengthF355 = GetSettingInt(TEXT("ForceSpringStrengthF355"), 0);
static int EnableDamperF355 = GetSettingInt(TEXT("EnableDamperF355"), 0);
static int DamperStrengthF355 = GetSettingInt(TEXT("DamperStrengthF355"), 100);

static int configMinForce18Wheeler = GetSettingInt(TEXT("MinForce18Wheeler"), 0);
static int configMaxForce18Wheeler = GetSettingInt(TEXT("MaxForce18Wheeler"), 100);
static int configAlternativeMinForceLeft18Wheeler = GetSettingInt(TEXT("AlternativeMinForceLeft18Wheeler"), 0);
static int configAlternativeMaxForceLeft18Wheeler = GetSettingInt(TEXT("AlternativeMaxForceLeft18Wheeler"), 100);
static int configAlternativeMinForceRight18Wheeler = GetSettingInt(TEXT("AlternativeMinForceRight18Wheeler"), 0);
static int configAlternativeMaxForceRight18Wheeler = GetSettingInt(TEXT("AlternativeMaxForceRight18Wheeler"), 100);
static int configFeedbackLength18Wheeler = GetSettingInt(TEXT("FeedbackLength18Wheeler"), 120);
static int PowerMode18Wheeler = GetSettingInt(TEXT("PowerMode18Wheeler"), 0);
static int EnableForceSpringEffect18Wheeler = GetSettingInt(TEXT("EnableForceSpringEffect18Wheeler"), 0);
static int ForceSpringStrength18Wheeler = GetSettingInt(TEXT("ForceSpringStrength18Wheeler"), 0);
static int EnableDamper18Wheeler = GetSettingInt(TEXT("EnableDamper18Wheeler"), 0);
static int DamperStrength18Wheeler = GetSettingInt(TEXT("DamperStrength18Wheeler"), 100);

static int configMinForceFasterThanSpeed = GetSettingInt(TEXT("MinForceFasterThanSpeed"), 0);
static int configMaxForceFasterThanSpeed = GetSettingInt(TEXT("MaxForceFasterThanSpeed"), 100);
static int configAlternativeMinForceLeftFasterThanSpeed = GetSettingInt(TEXT("AlternativeMinForceLeftFasterThanSpeed"), 0);
static int configAlternativeMaxForceLeftFasterThanSpeed = GetSettingInt(TEXT("AlternativeMaxForceLeftFasterThanSpeed"), 100);
static int configAlternativeMinForceRightFasterThanSpeed = GetSettingInt(TEXT("AlternativeMinForceRightFasterThanSpeed"), 0);
static int configAlternativeMaxForceRightFasterThanSpeed = GetSettingInt(TEXT("AlternativeMaxForceRightFasterThanSpeed"), 100);
static int configFeedbackLengthFasterThanSpeed = GetSettingInt(TEXT("FeedbackLengthFasterThanSpeed"), 120);
static int PowerModeFasterThanSpeed = GetSettingInt(TEXT("PowerModeFasterThanSpeed"), 0);
static int EnableForceSpringEffectFasterThanSpeed = GetSettingInt(TEXT("EnableForceSpringEffectFasterThanSpeed"), 0);
static int ForceSpringStrengthFasterThanSpeed = GetSettingInt(TEXT("ForceSpringStrengthFasterThanSpeed"), 0);
static int EnableDamperFasterThanSpeed = GetSettingInt(TEXT("EnableDamperFasterThanSpeed"), 0);
static int DamperStrengthFasterThanSpeed = GetSettingInt(TEXT("DamperStrengthFasterThanSpeed"), 100);

static int configMinForceMaximumSpeed = GetSettingInt(TEXT("MinForceMaximumSpeed"), 0);
static int configMaxForceMaximumSpeed = GetSettingInt(TEXT("MaxForceMaximumSpeed"), 100);
static int configAlternativeMinForceLeftMaximumSpeed = GetSettingInt(TEXT("AlternativeMinForceLeftMaximumSpeed"), 0);
static int configAlternativeMaxForceLeftMaximumSpeed = GetSettingInt(TEXT("AlternativeMaxForceLeftMaximumSpeed"), 100);
static int configAlternativeMinForceRightMaximumSpeed = GetSettingInt(TEXT("AlternativeMinForceRightMaximumSpeed"), 0);
static int configAlternativeMaxForceRightMaximumSpeed = GetSettingInt(TEXT("AlternativeMaxForceRightMaximumSpeed"), 100);
static int configFeedbackLengthMaximumSpeed = GetSettingInt(TEXT("FeedbackLengthMaximumSpeed"), 120);
static int PowerModeMaximumSpeed = GetSettingInt(TEXT("PowerModeMaximumSpeed"), 0);
static int EnableForceSpringEffectMaximumSpeed = GetSettingInt(TEXT("EnableForceSpringEffectMaximumSpeed"), 0);
static int ForceSpringStrengthMaximumSpeed = GetSettingInt(TEXT("ForceSpringStrengthMaximumSpeed"), 0);
static int EnableDamperMaximumSpeed = GetSettingInt(TEXT("EnableDamperMaximumSpeed"), 0);
static int DamperStrengthMaximumSpeed = GetSettingInt(TEXT("DamperStrengthMaximumSpeed"), 100);

static int configMinForceClubKart = GetSettingInt(TEXT("MinForceClubKart"), 0);
static int configMaxForceClubKart = GetSettingInt(TEXT("MaxForceClubKart"), 100);
static int configAlternativeMinForceLeftClubKart = GetSettingInt(TEXT("AlternativeMinForceLeftClubKart"), 0);
static int configAlternativeMaxForceLeftClubKart = GetSettingInt(TEXT("AlternativeMaxForceLeftClubKart"), 100);
static int configAlternativeMinForceRightClubKart = GetSettingInt(TEXT("AlternativeMinForceRightClubKart"), 0);
static int configAlternativeMaxForceRightClubKart = GetSettingInt(TEXT("AlternativeMaxForceRightClubKart"), 100);
static int configFeedbackLengthClubKart = GetSettingInt(TEXT("FeedbackLengthClubKart"), 120);
static int PowerModeClubKart = GetSettingInt(TEXT("PowerModeClubKart"), 0);
static int EnableForceSpringEffectClubKart = GetSettingInt(TEXT("EnableForceSpringEffectClubKart"), 0);
static int ForceSpringStrengthClubKart = GetSettingInt(TEXT("ForceSpringStrengthClubKart"), 0);
static int EnableDamperClubKart = GetSettingInt(TEXT("EnableDamperClubKart"), 0);
static int DamperStrengthClubKart = GetSettingInt(TEXT("DamperStrengthClubKart"), 100);
static int DoubleSineClubKart = GetSettingInt(TEXT("DoubleSineClubKart"), 0);
static int DoubleConstantClubKart = GetSettingInt(TEXT("DoubleConstantClubKart"), 0);

static int configMinForceKingOfRoute66 = GetSettingInt(TEXT("MinForceKingOfRoute66"), 0);
static int configMaxForceKingOfRoute66 = GetSettingInt(TEXT("MaxForceKingOfRoute66"), 100);
static int configAlternativeMinForceLeftKingOfRoute66 = GetSettingInt(TEXT("AlternativeMinForceLeftKingOfRoute66"), 0);
static int configAlternativeMaxForceLeftKingOfRoute66 = GetSettingInt(TEXT("AlternativeMaxForceLeftKingOfRoute66"), 100);
static int configAlternativeMinForceRightKingOfRoute66 = GetSettingInt(TEXT("AlternativeMinForceRightKingOfRoute66"), 0);
static int configAlternativeMaxForceRightKingOfRoute66 = GetSettingInt(TEXT("AlternativeMaxForceRightKingOfRoute66"), 100);
static int configFeedbackLengthKingOfRoute66 = GetSettingInt(TEXT("FeedbackLengthKingOfRoute66"), 120);
static int PowerModeKingOfRoute66 = GetSettingInt(TEXT("PowerModeKingOfRoute66"), 0);
static int EnableForceSpringEffectKingOfRoute66 = GetSettingInt(TEXT("EnableForceSpringEffectKingOfRoute66"), 0);
static int ForceSpringStrengthKingOfRoute66 = GetSettingInt(TEXT("ForceSpringStrengthKingOfRoute66"), 0);
static int EnableDamperKingOfRoute66 = GetSettingInt(TEXT("EnableDamperKingOfRoute66"), 0);
static int DamperStrengthKingOfRoute66 = GetSettingInt(TEXT("DamperStrengthKingOfRoute66"), 100);
static int DoubleSineKingOfRoute66 = GetSettingInt(TEXT("DoubleSineKingOfRoute66"), 0);
static int DoubleConstantKingOfRoute66 = GetSettingInt(TEXT("DoubleConstantKingOfRoute66"), 0);

static int configMinForceSegaDrivingSimulator = GetSettingInt(TEXT("MinForceSegaDrivingSimulator"), 0);
static int configMaxForceSegaDrivingSimulator = GetSettingInt(TEXT("MaxForceSegaDrivingSimulator"), 100);
static int configAlternativeMinForceLeftSegaDrivingSimulator = GetSettingInt(TEXT("AlternativeMinForceLeftSegaDrivingSimulator"), 0);
static int configAlternativeMaxForceLeftSegaDrivingSimulator = GetSettingInt(TEXT("AlternativeMaxForceLeftSegaDrivingSimulator"), 100);
static int configAlternativeMinForceRightSegaDrivingSimulator = GetSettingInt(TEXT("AlternativeMinForceRightSegaDrivingSimulator"), 0);
static int configAlternativeMaxForceRightSegaDrivingSimulator = GetSettingInt(TEXT("AlternativeMaxForceRightSegaDrivingSimulator"), 100);
static int configFeedbackLengthSegaDrivingSimulator = GetSettingInt(TEXT("FeedbackLengthSegaDrivingSimulator"), 120);
static int PowerModeSegaDrivingSimulator = GetSettingInt(TEXT("PowerModeSegaDrivingSimulator"), 0);
static int EnableForceSpringEffectSegaDrivingSimulator = GetSettingInt(TEXT("EnableForceSpringEffectSegaDrivingSimulator"), 0);
static int ForceSpringStrengthSegaDrivingSimulator = GetSettingInt(TEXT("ForceSpringStrengthSegaDrivingSimulator"), 0);
static int EnableDamperSegaDrivingSimulator = GetSettingInt(TEXT("EnableDamperSegaDrivingSimulator"), 0);
static int DamperStrengthSegaDrivingSimulator = GetSettingInt(TEXT("DamperStrengthSegaDrivingSimulator"), 100);
static int DoubleSineSegaDrivingSimulator = GetSettingInt(TEXT("DoubleSineSegaDrivingSimulator"), 0);
static int DoubleConstantSegaDrivingSimulator = GetSettingInt(TEXT("DoubleConstantSegaDrivingSimulator"), 0);

static bool init = false;
static bool RomGameName = false;
static bool Effect1 = false;
static bool Effect2 = false;
static bool Effect3 = false;
static bool DirtDevilSine = false;
static bool DontSineUntilRaceStart = false;
static bool HardDrivinFrame = false;
static bool NaomiFFBGo = false;
static bool Motion = false;
static bool MotionFalse = false;
static bool PatternFind = false;
static bool Scan = false;
static bool PatternLaunch = false;

HINSTANCE hInstance;
HINSTANCE hPrevInstance;
LPSTR lpCmdLine;
int nCmdShow;

// Name of the last output MAME updated, written by the output thread (mame_updatestate)
// and read by the FFB thread (FFBLoop). It used to be a pointer into a CStringA local to
// mame_updatestate, so it dangled as soon as that returned and FFBLoop read freed memory,
// which crashes when the heap is torn down while MAME exits.
// A Windows lock rather than std::mutex: since VS 2022 17.10, std::mutex crashes when the
// process loads an older msvcp140.dll.
static char nameFFB[256];
static SRWLOCK nameFFBLock = SRWLOCK_INIT;
const char* EmulatorName;
char* name;
char* romname;
char* RunningFFB;
char* Emulator;
int vals[5] = { 0 };
int state4 = 0;
int state3 = 0;
int state2 = 0;
int state1 = 0;
int state0 = 0;
int frame = 0;
int HardDrivinFFB;
int StopConstant;
int newstateFFB;
int oldstateFFB;
int EffectCount;
int stateFFB;
int stateFFB2;
int stateFFB3;
int stateFFBDevice2;
int stateFFBDevice3;
int SineEffectState;
int smgpLamp0 = 0;
int smgpLamp1 = 0;
int smgpLamp2 = 0;
double Divide;
static INT_PTR FFBAddress;
static UINT8 ff;

std::string wheelA("wheel");

static int speedffb(int ffRaw) {
	switch (ffRaw) {
	case 0x1E:
		return 31;
	case 0x1C:
		return 30;
	case 0x1A:
		return 29;
	case 0x18:
		return 28;
	case 0x16:
		return 27;
	case 0x14:
		return 26;
	case 0x12:
		return 25;
	case 0x10:
		return 24;
	case 0x0E:
		return 23;
	case 0x0C:
		return 22;
	case 0x0A:
		return 21;
	case 0x08:
		return 20;
	case 0x06:
		return 19;
	case 0x04:
		return 18;
	case 0x02:
		return 17;

	case 0x1F:
		return 16;
	case 0x1D:
		return 15;
	case 0x1B:
		return 14;
	case 0x19:
		return 13;
	case 0x17:
		return 12;
	case 0x15:
		return 11;
	case 0x13:
		return 10;
	case 0x11:
		return 9;
	case 0x0F:
		return 8;
	case 0x0D:
		return 7;
	case 0x0B:
		return 6;
	case 0x09:
		return 5;
	case 0x07:
		return 4;
	case 0x05:
		return 3;
	case 0x03:
		return 2;
	case 0x01:
		return 1;
	}
	return 0;
}

// Rave Racer wheel_motor output to force (1..123), -1 when the value is not a force
static int raveracer(int ffRaw)
{
	static const int map[256] = {
		/* 0x00 */ -1, -1, 62, -1, 30, -1, 94, -1,
		/* 0x08 */ 46, -1, 78, -1, 14, -1, 110, -1,
		/* 0x10 */ 54, -1, 70, -1, 22, -1, 102, -1,
		/* 0x18 */ 38, -1, 86, -1, 6,  -1, 118, -1,
		/* 0x20 */ 59, -1, 65, -1, 27, -1, 97, -1,
		/* 0x28 */ 43, -1, 81, -1, 11, -1, 113, -1,
		/* 0x30 */ 50, -1, 74, -1, 18, -1, 106, -1,
		/* 0x38 */ 34, -1, 90, -1, 2,  -1, 122, -1,
		/* 0x40 */ 60, -1, 64, -1, 28, -1, 96, -1,
		/* 0x48 */ 44, -1, 80, -1, 12, -1, 112, -1,
		/* 0x50 */ 52, -1, 72, -1, 20, -1, 104, -1,
		/* 0x58 */ 36, -1, 88, -1, 4,  -1, 120, -1,
		/* 0x60 */ 56, -1, 68, -1, 24, -1, 100, -1,
		/* 0x68 */ 40, -1, 84, -1, 8,  -1, 116, -1,
		/* 0x70 */ 48, -1, 76, -1, 16, -1, 108, -1,
		/* 0x78 */ 32, -1, 92, -1, 1,  -1, 123, -1,
		/* 0x80 */ 61, -1, 63, -1, 29, -1, 95, -1,
		/* 0x88 */ 45, -1, 79, -1, 13, -1, 111, -1,
		/* 0x90 */ 53, -1, 71, -1, 21, -1, 103, -1,
		/* 0x98 */ 37, -1, 87, -1, 5,  -1, 119, -1,
		/* 0xA0 */ 57, -1, 67, -1, 25, -1, 99,  -1,
		/* 0xA8 */ 41, -1, 83, -1, 9,  -1, 115, -1,
		/* 0xB0 */ 49, -1, 75, -1, 17, -1, 107, -1,
		/* 0xB8 */ 33, -1, 91, -1, -1, -1, -1, -1,
		/* 0xC0 */ 58, -1, 66, -1, 26, -1, 98,  -1,
		/* 0xC8 */ 42, -1, 82, -1, 10, -1, 114, -1,
		/* 0xD0 */ 51, -1, 73, -1, 19, -1, 105, -1,
		/* 0xD8 */ 35, -1, 89, -1, 3,  -1, 121, -1,
		/* 0xE0 */ 55, -1, 69, -1, 23, -1, 101, -1,
		/* 0xE8 */ 39, -1, 85, -1, 7,  -1, 117, -1,
		/* 0xF0 */ 47, -1, 77, -1, 15, -1, 109, -1,
		/* 0xF8 */ 31, -1, 93, -1, -1, -1, -1, -1
	};

	return map[ffRaw & 0xFF];
}

// Ace Driver wheel_motor output to signed force (-62..62)
static int acedrivertable(int ffRaw)
{
	static const int map[256] = {
		/* 0x00 */  0,   62,  0,  -62,  0,   31,  0,  -31,
		/* 0x08 */  0,   47,  0,  -47,  0,   15,  0,  -15,
		/* 0x10 */  0,   55,  0,  -55,  0,   23,  0,  -23,
		/* 0x18 */  0,   39,  0,  -39,  0,    7,  0,   -7,
		/* 0x20 */  0,   59,  0,  -59,  0,   27,  0,  -27,
		/* 0x28 */  0,   43,  0,  -43,  0,   11,  0,  -11,
		/* 0x30 */  0,   51,  0,  -51,  0,   19,  0,  -19,
		/* 0x38 */  0,   35,  0,  -35,  0,    3,  0,   -3,

		/* 0x40 */  0,   61,  0,  -61,  0,   29,  0,  -29,
		/* 0x48 */  0,   45,  0,  -45,  0,   13,  0,  -13,
		/* 0x50 */  0,   53,  0,  -53,  0,   21,  0,  -21,
		/* 0x58 */  0,   37,  0,  -37,  0,    5,  0,   -5,
		/* 0x60 */  0,   57,  0,  -57,  0,   25,  0,  -25,
		/* 0x68 */  0,   41,  0,  -41,  0,    9,  0,   -9,
		/* 0x70 */  0,   49,  0,  -49,  0,   17,  0,  -17,
		/* 0x78 */  0,   33,  0,  -33,  0,    1,  0,   -1,

		/* 0x80 */  0,   62,  0,  -62,  0,   30,  0,  -30,
		/* 0x88 */  0,   46,  0,  -46,  0,   14,  0,  -14,
		/* 0x90 */  0,   54,  0,  -54,  0,   22,  0,  -22,
		/* 0x98 */  0,   38,  0,  -38,  0,    6,  0,   -6,
		/* 0xA0 */  0,   58,  0,  -58,  0,   26,  0,  -26,
		/* 0xA8 */  0,   42,  0,  -42,  0,   10,  0,  -10,
		/* 0xB0 */  0,   50,  0,  -50,  0,   18,  0,  -18,
		/* 0xB8 */  0,   34,  0,  -34,  0,    2,  0,   -2,

		/* 0xC0 */  0,   60,  0,  -60,  0,   28,  0,  -28,
		/* 0xC8 */  0,   44,  0,  -44,  0,   12,  0,  -12,
		/* 0xD0 */  0,   52,  0,  -52,  0,   20,  0,  -20,
		/* 0xD8 */  0,   36,  0,  -36,  0,    4,  0,   -4,
		/* 0xE0 */  0,   56,  0,  -56,  0,   24,  0,  -24,
		/* 0xE8 */  0,   40,  0,  -40,  0,    8,  0,   -8,
		/* 0xF0 */  0,   48,  0,  -48,  0,   16,  0,  -16,
		/* 0xF8 */  0,   32,  0,  -32,  0,    0,  0,    0  // 0xFF = true neutral
	};
	return map[ffRaw & 0xFF];
}

// Swaps left and right when the ReverseDirection setting is enabled
static int SwapDirection(int direction)
{
	if (!ReverseDirection || !myconstants)
		return direction;

	if (direction == myconstants->DIRECTION_FROM_LEFT)
		return myconstants->DIRECTION_FROM_RIGHT;

	if (direction == myconstants->DIRECTION_FROM_RIGHT)
		return myconstants->DIRECTION_FROM_LEFT;

	return direction;
}

struct DriversEdgeMotor
{
	int direction;
	double strength;
};

static DriversEdgeMotor decode_driversedge_motor(int raw)
{
	// MAME Driver's Edge wheel_motor output is signed -31..+31.
	if (raw > 31)
		raw = 31;
	else if (raw < -31)
		raw = -31;

	DriversEdgeMotor motor{ 0, 0.0 };

	if (raw > 0)
	{
		motor.direction = 1;
		motor.strength = raw / 31.0;
	}
	else if (raw < 0)
	{
		motor.direction = -1;
		motor.strength = (-raw) / 31.0;
	}

	return motor;
}

// Constant force of the games driven by MAME outputs, played as ConstantInf when UseConstantInf is set
static void SendConstant(EffectTriggers* triggers, int direction, double strength)
{
	if (UseConstantInf)
		triggers->ConstantInf(direction, strength);
	else
		triggers->Constant(direction, strength);
}

// Stops the constant force in both directions
static void StopConstantForce(EffectTriggers* triggers, EffectConstants* constants)
{
	SendConstant(triggers, constants->DIRECTION_FROM_LEFT, 0.0);
	SendConstant(triggers, constants->DIRECTION_FROM_RIGHT, 0.0);
}

static int DriveboardSequenceRunning = 0; // 0 = no sequence

static unsigned long DriveboardSequence_starttime_ms = 0;
static unsigned long DriveboardSequence_duration_ms = 0;
static unsigned long CurrentTime_ms = 0;
static double ScudIntensityFactor = 1.0;

// Function called each time we receive a new variable from MAME/Supermodel
static void FFBGameEffects(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers, int stateFFB, LPCSTR name)
{
	if (RunningFFB == Daytona2Active) //Daytona 2
	{
		if (name == RawDrive)
		{
			helpers->log("got value: ");
			std::string ffs = std::to_string(stateFFB);
			helpers->log((char*)ffs.c_str());

			UINT32 length_ms = 100;

			if (stateFFB >= 0x0 && stateFFB <= 0x09) // Sequences
			{
				switch (stateFFB) {
				case 1:
					break;
				}
			}

			if (stateFFB >= 0x0A && stateFFB < 0x20) //Spring
			{
				double percentForce = (stateFFB - 9) / 16.0;
				triggers->Spring(percentForce);
			}

			if (stateFFB >= 0x20 && stateFFB < 0x30) //Clutch
			{
				double percentForce = (stateFFB - 0x1F) / 16.0;
				triggers->Friction(percentForce);
			}

			if (stateFFB >= 0x30 && stateFFB < 0x40) //Centering
			{
				double percentForce = (stateFFB - 0x2F) / 16.0;
				triggers->Rumble(percentForce, percentForce, 100);
				triggers->Sine(40, 0, percentForce, 100);
			}

			if (stateFFB >= 0x40 && stateFFB < 0x50) //Uncentering
			{
				double percentForce = (stateFFB - 0x3F) / 16.0;
				triggers->Spring(0);
			}

			if (stateFFB >= 0x50 && stateFFB < 0x60) //Roll Right
			{
				double percentForce = (stateFFB - 0x4F) / 16.0;
				triggers->Rumble(percentForce, 0, length_ms);
				triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
			}
			else if (stateFFB >= 0x60 && stateFFB < 0x70) //Roll Left
			{
				double percentForce = (stateFFB - 0x5F) / 16.0;
				triggers->Rumble(0, percentForce, length_ms);
				triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
			}

			//Test Menu
			if (stateFFB == 0x80)
			{
				triggers->Rumble(0, 0, 0);
				triggers->Constant(constants->DIRECTION_FROM_LEFT, 0);
				triggers->Constant(constants->DIRECTION_FROM_RIGHT, 0);
			}
			else if (stateFFB == 0x81)
			{
				triggers->Rumble(0.5, 0, 100);
				triggers->Constant(constants->DIRECTION_FROM_LEFT, 0.5);
			}
			else if (stateFFB == 0x82)
			{
				triggers->Rumble(0, 0.5, 100);
				triggers->Constant(constants->DIRECTION_FROM_RIGHT, 0.5);
			}
		}
	}

	
	if (RunningFFB == ScudRaceActive) // Scud Race (should also cover Dirt Devils, ECA and Lemans provided cabinet is set to 'Twin-Lemans')
	{
		if (name == RawDrive)
		{
			helpers->log("got value: ");
			std::string ffs = std::to_string(stateFFB);
			helpers->log((char*)ffs.c_str());

			UINT32 length_ms = 100;
			if (stateFFB > 0x0 && stateFFB <= 0x09) // Sequences
			{
				// Mask: only 3 bits are relevant
				int sequence = stateFFB & 0x7;
				switch (sequence) {
				case 1:
				case 2:
					// Unlimited time ?
					DriveboardSequence_duration_ms = 1000;
					break;
				}
				DriveboardSequenceRunning = sequence;
				if (sequence > 0) {
					DriveboardSequence_starttime_ms = CurrentTime_ms;

					helpers->log("start sequence");
					std::string ffs = std::to_string(sequence);
					helpers->log((char*)ffs.c_str());
				} else {
					helpers->log("sequence stopped");
				}
			}

			if (stateFFB >= 0x10 && stateFFB < 0x20) //Spring
			{
				// Spring Effect. 0x10 to stop the effect. Intensity range from 0x11...0x17(strong). Values are duplicated on 0x18...0x1F
				int spring = stateFFB - 0x10;
				if (spring > 0) {
					double percentForce = (stateFFB & 0x7) / 7.0;
					triggers->Spring(percentForce);

					helpers->log("spring ");
					std::string ffs = std::to_string(percentForce * 100);
					helpers->log((char*)ffs.c_str());
				} else {
					helpers->log("spring stopped");
				}
			}

			if (stateFFB >= 0x20 && stateFFB < 0x30) //Intensity
			{
				// Intensity gain for torque and vibration effects(does not seem to be applied to sequence, spring or power-slide effect).
				// When a torque effect or a vibration effect is played, allows to modulate its intensity. 
				// 0x20(very weak, almost no effect)...0x2F(strong)
				ScudIntensityFactor = (double)(stateFFB - 0x1F) / 16.0; // => 0.066 to 1.0

				helpers->log("intensity ");
				std::string ffs = std::to_string(ScudIntensityFactor * 100);
				helpers->log((char*)ffs.c_str());
			}

			if (stateFFB >= 0x30 && stateFFB < 0x40) //Vibration
			{
				// Vibration effect. 0x30 to stop the effect.
				// Intensity range from 0x31...0x35(strong). Intensity seems to be modulated with 0x2X
				int vibration = stateFFB - 0x30;
				if (vibration > 0) {
					double percentForce = (double)(vibration) / 5.0 * ScudIntensityFactor;

					triggers->Rumble(percentForce, percentForce, 100);
					triggers->Sine(40, 0, percentForce, 100);

					helpers->log("vibration ");
					std::string ffs = std::to_string(percentForce * 100);
					helpers->log((char*)ffs.c_str());
				} else {
					helpers->log("vibration stopped");
				}
			}

			if (stateFFB >= 0x40 && stateFFB < 0x50) // Power slide
			{
				// Power-slide effect. Perform a sine of torque that increase the torque and push the steering wheel in one direction, 
				// then to the other direction, like driving in a hole. 
				// 0x40...0x47(strong) push first right, then left. 
				// 0x48...0x4F(strong) pushes first left, then right. 
				// Intensity is given by the 3bits less significant bits. Duration is about 2-3secondes
				bool direction = stateFFB < 0x48;
				int intensity = stateFFB & 0xF;
				double percentForce = (intensity) / 8.0;
				// Sign it
				if (direction)
					percentForce *= -1.0;
				// Make it 2 seconds, no fade
				triggers->Sine(2000, 0, percentForce, 2000);
				helpers->log("power slide ");
				std::string ffs = std::to_string(percentForce * 100);
				helpers->log((char*)ffs.c_str());
			}

			if (stateFFB >= 0x50 && stateFFB < 0x60) //Constant torque Right
			{
				double percentForce = (double)(stateFFB - 0x4F) / 16.0 * ScudIntensityFactor;
				triggers->Rumble(percentForce, 0, length_ms);
				triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);

				helpers->log("torque Right");
				std::string ffs = std::to_string(percentForce * 100);
				helpers->log((char*)ffs.c_str());
			}
			else if (stateFFB >= 0x60 && stateFFB < 0x70) //Constant torque Left
			{
				double percentForce = (double)(stateFFB - 0x5F) / 16.0 * ScudIntensityFactor;
				triggers->Rumble(0, percentForce, length_ms);
				triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);

				helpers->log("torque Left");
				std::string ffs = std::to_string(percentForce * 100);
				helpers->log((char*)ffs.c_str());
			}

			//Test Menu
			if (stateFFB == 0x80)
			{
				triggers->Rumble(0, 0, 0);
				triggers->Constant(constants->DIRECTION_FROM_LEFT, 0);
				triggers->Constant(constants->DIRECTION_FROM_RIGHT, 0);
			}
			else if (stateFFB == 0x81)
			{
				triggers->Rumble(0.5, 0, 100);
				triggers->Constant(constants->DIRECTION_FROM_LEFT, 0.5);
			}
			else if (stateFFB == 0x82)
			{
				triggers->Rumble(0, 0.5, 100);
				triggers->Constant(constants->DIRECTION_FROM_RIGHT, 0.5);
			}
		}
	}

	if (RunningFFB == DirtDevilsActive) //Dirt Devils, ECA
	{
		if (name == RawDrive)
		{
			helpers->log("got value: ");
			std::string ffs = std::to_string(stateFFB);
			helpers->log((char*)ffs.c_str());

			if (stateFFB == 0x10)
			{
				double percentForce = 0.7;
				triggers->Spring(percentForce);
			}

			if (stateFFB == 0x27 || stateFFB == 0x30)
				DirtDevilSine = false;

			if (stateFFB == 0x2F)
				DirtDevilSine = true;

			if (DirtDevilSine)
			{
				double percentForce = (stateFFB - 31) / 16.0;
				triggers->Sine(60, 0, percentForce, 100);
				triggers->Rumble(percentForce, percentForce, 100);
			}
		}
	}

	if (RunningFFB == Srally2Active) //Sega Rally 2
	{
		if (name == RawDrive)
		{
			helpers->log("got value: ");
			std::string ffs = std::to_string(stateFFB);
			helpers->log((char*)ffs.c_str());

			UINT32 length_ms = 100;
			if (stateFFB >= 0x00 && stateFFB <= 0x3F)
			{
				double percentForce = (stateFFB) / 64.0;
				triggers->Rumble(0, percentForce, length_ms);
				triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
			}
			else if (stateFFB >= 0x40 && stateFFB <= 0x7F)
			{
				double percentForce = (stateFFB - 0x40) / 64.0;
				triggers->Rumble(percentForce, 0, length_ms);
				triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
			}
		}
	}

	if (RunningFFB == VirtuaRacingActive) //Virtua Racing
	{
		if (name == digit0)
		{
			helpers->log("got value: ");
			std::string ffs = std::to_string(stateFFB);
			helpers->log((char*)ffs.c_str());

			UINT32 length_ms = 100;
			if ((stateFFB == 0x03) || (stateFFB == 0x07) || (stateFFB == 0x09) || (stateFFB == 0x10))
			{
				if (stateFFB == 0x07)
					DontSineUntilRaceStart = true;

				if (stateFFB == 0x09)
					DontSineUntilRaceStart = false;

				//Spring
				double percentForce = 0.8;
				triggers->Spring(percentForce);
				SendConstant(triggers, constants->DIRECTION_FROM_LEFT, 0.0);
			}

			if (stateFFB == 0x20 || stateFFB == 0x28) //Clutch
			{
				double percentForce = 0.4;
				triggers->Friction(percentForce);
				SendConstant(triggers, constants->DIRECTION_FROM_LEFT, 0.0);
			}

			if (stateFFB > 0x2F && stateFFB < 0x40) //Centering
			{
				double percentForce = (stateFFB - 47) / 11.0;
				triggers->Spring(percentForce);
				SendConstant(triggers, constants->DIRECTION_FROM_LEFT, 0.0);
			}

			if (stateFFB == 0x40 || stateFFB == 0x46 || stateFFB == 0x4A) //Uncentering
			{
				if (stateFFB == 0x40)
				{
					double percentForce = 0.4;
					triggers->Rumble(percentForce, percentForce, 100);
					triggers->Sine(70, 30, percentForce, 100);
				}
				else
				{
					if (DontSineUntilRaceStart) //Uncentering
					{
						double percentForce = 0.4;
						triggers->Rumble(percentForce, percentForce, 100);
						triggers->Sine(70, 30, percentForce, 100);
					}
				}
				SendConstant(triggers, constants->DIRECTION_FROM_LEFT, 0.0);
			}

			if (stateFFB == 0x50 || stateFFB == 0x5F) //Roll Left
			{
				double percentForce = 0.5;
				triggers->Rumble(0, percentForce, length_ms);
				SendConstant(triggers, constants->DIRECTION_FROM_RIGHT, percentForce);
			}
			else if (stateFFB == 0x60 || stateFFB == 0x6F) //Roll Right
			{
				double percentForce = 0.5;
				triggers->Rumble(percentForce, 0, length_ms);
				SendConstant(triggers, constants->DIRECTION_FROM_LEFT, percentForce);
			}
			else
			{
				SendConstant(triggers, constants->DIRECTION_FROM_LEFT, 0.0);
			}
		}
	}

	if (RunningFFB == SuperMonacoGPActive)
	{
		// MAME output callbacks only update the current 3-bit shaker stage.
		// FFBLoop keeps the periodic effect running continuously.
		if (name == mainpcb_lamp0)
			smgpLamp0 = stateFFB ? 1 : 0;
		else if (name == mainpcb_lamp1)
			smgpLamp1 = stateFFB ? 1 : 0;
		else if (name == mainpcb_lamp2)
			smgpLamp2 = stateFFB ? 1 : 0;
	}

	if (RunningFFB == NamcoFFBNew) //Ace Driver, Ace Driver Victory Lap
	{
		if (name == mcuout3)
		{
			if (newstateFFB == 0)
				StopConstantForce(triggers, constants);
		}

		if (name == wheel_motor)
		{
			int fface = acedrivertable((UINT8)stateFFB);

			if (fface > 0)
			{
				double percentForce = fface / 62.0;
				triggers->Rumble(percentForce, 0.0, 100);
				SendConstant(triggers, constants->DIRECTION_FROM_RIGHT, percentForce);
			}
			else if (fface < 0)
			{
				double percentForce = (-fface) / 62.0;
				triggers->Rumble(0.0, percentForce, 100);
				SendConstant(triggers, constants->DIRECTION_FROM_LEFT, percentForce);
			}
			else
			{
				StopConstantForce(triggers, constants);
			}
		}
	}

	if (RunningFFB == RaveRacerNew) //Rave Racer
	{
		if (name == mcuout3)
		{
			if (newstateFFB == 0)
				StopConstantForce(triggers, constants);
		}

		if (name == wheel_motor)
		{
			UINT8 ffrave = raveracer((UINT8)stateFFB);

			if ((ffrave > 0x3D) && (ffrave < 0x7C))
			{
				double percentForce = (124 - ffrave) / 61.0;
				triggers->Rumble(percentForce, 0.0, 100);
				SendConstant(triggers, SwapDirection(constants->DIRECTION_FROM_LEFT), percentForce);
			}
			else if ((ffrave > 0x00) && (ffrave < 0x3E))
			{
				double percentForce = ffrave / 61.0;
				triggers->Rumble(0.0, percentForce, 100);
				SendConstant(triggers, SwapDirection(constants->DIRECTION_FROM_RIGHT), percentForce);
			}
		}
	}

	if (RunningFFB == DriversEdgeActive) //Driver's Edge
	{
		if (name == wheel_motor)
		{
			const DriversEdgeMotor motor = decode_driversedge_motor(stateFFB);

			helpers->log("Driver's Edge wheel_motor: ");
			std::string ffs = std::to_string(stateFFB);
			helpers->log((char*)ffs.c_str());

			if (motor.direction == 0)
			{
				StopConstantForce(triggers, constants);
				return;
			}

			if (motor.direction > 0)
				SendConstant(triggers, SwapDirection(constants->DIRECTION_FROM_RIGHT), motor.strength);
			else
				SendConstant(triggers, SwapDirection(constants->DIRECTION_FROM_LEFT), motor.strength);
		}
	}

	if (RunningFFB == RacingFullValueActive1) //Mame games using all values
	{
		if (name == wheel || name == wheel_motor || name == m_wheel_motor)
		{
			helpers->log("got value: ");
			std::string ffs = std::to_string(stateFFB);
			helpers->log((char*)ffs.c_str());

			if (stateFFB == 0)
			{
				StopConstantForce(triggers, constants);
				return;
			}

			UINT32 length_ms = 100;
			if (stateFFB >= 0x80 && stateFFB < 0x100)
			{
				double percentForce = (256 - stateFFB) / 128.0;
				if (percentForce > 1.0) percentForce = 1.0;
				triggers->Rumble(percentForce, 0, length_ms);
				SendConstant(triggers, constants->DIRECTION_FROM_LEFT, percentForce);
			}
			else if (stateFFB > 0x00 && stateFFB < 0x80)
			{
				double percentForce = (stateFFB) / 127.0;
				if (percentForce > 1.0) percentForce = 1.0;
				triggers->Rumble(0, percentForce, length_ms);
				SendConstant(triggers, constants->DIRECTION_FROM_RIGHT, percentForce);
			}
			else
			{
				StopConstantForce(triggers, constants);
			}
		}
	}

	if (RunningFFB == RacingFullValueActive2) //Mame games using all values (reverse direction to above)
	{
		if (name == wheel)
		{
			helpers->log("got value: ");
			std::string ffs = std::to_string(stateFFB);
			helpers->log((char*)ffs.c_str());

			if (stateFFB == 0)
			{
				StopConstantForce(triggers, constants);
				return;
			}

			UINT32 length_ms = 100;
			if (stateFFB > 0x80 && stateFFB < 0x100)
			{
				double percentForce = (256 - stateFFB) / 126.0;
				if (percentForce > 1.0) percentForce = 1.0;
				triggers->Rumble(0, percentForce, length_ms);
				SendConstant(triggers, constants->DIRECTION_FROM_RIGHT, percentForce);
			}
			else if (stateFFB > 0x00 && stateFFB < 0x80)
			{
				double percentForce = (stateFFB) / 126.0;
				if (percentForce > 1.0) percentForce = 1.0;
				triggers->Rumble(percentForce, 0, length_ms);
				SendConstant(triggers, constants->DIRECTION_FROM_LEFT, percentForce);
			}
			else
			{
				StopConstantForce(triggers, constants);
			}
		}
	}

	if (RunningFFB == RacingFullValueExotica) //Cruis'n Exotica
	{
		if (name == wheel || name == wheel_motor)
		{
			// Exotica writes the motor command as a signed 8-bit value. 0x80 is
			// neutral, matching the Cruis'n full-value convention used here.
			int signedForce = static_cast<signed char>(static_cast<unsigned char>(stateFFB));
			if (signedForce == -128)
				signedForce = 0;

			int gain = configFFBGainCrusnExotica;
			if (gain < 0) gain = 0;
			if (gain > 800) gain = 800;

			int scaledForce = (signedForce * gain) / 100;
			if (scaledForce > 127) scaledForce = 127;
			if (scaledForce < -127) scaledForce = -127;

			// Exotica with the correct shifter/wheel DIP needs inverted FFB direction.
			scaledForce = -scaledForce;

			char WheelActive[256];
			sprintf(WheelActive, "WheelActiveExotica raw: %i scaled: %i", stateFFB, scaledForce);
			helpers->log(WheelActive);

			if (scaledForce > 0)
			{
				double percentForce = scaledForce / 126.0;
				if (percentForce > 1.0) percentForce = 1.0;
				triggers->Rumble(percentForce, 0, 100);
				SendConstant(triggers, constants->DIRECTION_FROM_LEFT, percentForce);
				return;
			}

			if (scaledForce < 0)
			{
				double percentForce = (-scaledForce) / 126.0;
				if (percentForce > 1.0) percentForce = 1.0;
				triggers->Rumble(0, percentForce, 100);
				SendConstant(triggers, constants->DIRECTION_FROM_RIGHT, percentForce);
				return;
			}
		}
	}

	if (RunningFFB == srallynew) //Sega Rally Championship
	{
		if (name == wheel_motor)
		{
			helpers->log("got value: ");
			std::string ffs = std::to_string(stateFFB);
			helpers->log((char*)ffs.c_str());

			if (stateFFB == 0x00)
			{
				triggers->Rumble(0, 0, 0);
				StopConstantForce(triggers, constants);
				return;
			}

			UINT32 length_ms = 100;
			if (stateFFB >= 0xC0 && stateFFB <= 0xDF) // Force from right, wheel turns left
			{
				double percentForce = (stateFFB - 0xC0) / 31.0;
				triggers->Rumble(0, percentForce, length_ms);
				SendConstant(triggers, constants->DIRECTION_FROM_RIGHT, percentForce);
			}
			else if (stateFFB >= 0x80 && stateFFB <= 0x9F) // Force from left, wheel turns right
			{
				double percentForce = (stateFFB - 0x80) / 31.0;
				triggers->Rumble(percentForce, 0, length_ms);
				SendConstant(triggers, constants->DIRECTION_FROM_LEFT, percentForce);
			}
		}
	}

	if (RunningFFB == Konami) //Thrill Drive, Winding Heat, GTI Club, Midnight Run, Racing Jam 1 and 2
	{
		if (name == wheel || name == wheel_motor || name == pcboutput0 || name == output0 || name == wheel0)
		{
			const uint8_t value = static_cast<uint8_t>(stateFFB);

			helpers->log("got value: ");
			std::string ffs = std::to_string(value);
			helpers->log((char*)ffs.c_str());

			// Bits 0-3: force level 0-15, bit 4: direction.
			const int forceLevel = value & 0x0F;
			const bool directionBit = (value & 0x10) != 0;

			// Any value with a force level of 0 (not only 0 itself, but also 0x80 and 0x90) means no force.
			if (forceLevel == 0)
			{
				StopConstantForce(triggers, constants);
				return;
			}

			const double percentForce = forceLevel / 15.0;

			if (!directionBit)
			{
				triggers->Rumble(0, percentForce, 100);
				SendConstant(triggers, constants->DIRECTION_FROM_RIGHT, percentForce);
			}
			else
			{
				triggers->Rumble(percentForce, 0, 100);
				SendConstant(triggers, constants->DIRECTION_FROM_LEFT, percentForce);
			}
		}
	}

	if (RunningFFB == hng64) //Roads Edge, Xtreme Rally
	{
		if (name == wheel_motor)
		{
			helpers->log("got value: ");
			std::string ffs = std::to_string(stateFFB);
			helpers->log((char*)ffs.c_str());

			if (stateFFB == 0)
			{
				StopConstantForce(triggers, constants);
				return;
			}

			if (stateFFB >= 193 && stateFFB <= 255)
			{
				double percentForce = (stateFFB - 193) / 62.0;
				if (percentForce > 1.0) percentForce = 1.0;
				triggers->Rumble(0, percentForce, 100);
				SendConstant(triggers, constants->DIRECTION_FROM_LEFT, percentForce);
			}
			else if (stateFFB >= 129 && stateFFB <= 191)
			{
				double percentForce = (stateFFB - 129) / 62.0;
				if (percentForce > 1.0) percentForce = 1.0;
				triggers->Rumble(percentForce, 0, 100);
				SendConstant(triggers, constants->DIRECTION_FROM_RIGHT, percentForce);
			}
			else
			{
				StopConstantForce(triggers, constants);
			}
		}
	}

	if (RunningFFB == SideBSActive) //Side by Side 1 and 2
	{
		if (name == wheel_motor)
		{
			helpers->log("got wheel_motor: ");
			std::string ffs = std::to_string(stateFFB);
			helpers->log((char*)ffs.c_str());

			int v = (int)stateFFB & 0x1F; // Extract the last 5 bits (0..31)

			// Values 0 (OFF) and 1E/1F (Attract Mode/Idle) should result in zero force
			if (v == 0 || v == 0x1E || v == 0x1F)
			{
				StopConstantForce(triggers, constants);
				return;
			}

			// Direction logic: Even values = Left, Odd values = Right
			int dir = (v & 1) ? constants->DIRECTION_FROM_RIGHT : constants->DIRECTION_FROM_LEFT;

			// Map raw values to 1..15 range
			int steps = (v + 1) / 2;
			if (steps > 15) steps = 15;

			// Invert logic as per hardware behavior:
			// High MAME values represent low force, low MAME values represent high force
			int stepsInv = 16 - steps;

			double percentForce = (double)stepsInv / 15.0;
			if (percentForce > 1.0) percentForce = 1.0;

			// Prevent stale pull: clear both directions before applying new force
			StopConstantForce(triggers, constants);
			SendConstant(triggers, dir, percentForce);
		}
	}

	if (RunningFFB == m2new) //Model 2 games: Daytona USA, Indy 500, Over Rev, Sega Touring Car Championship, Super GT 24h
	{
		if (name == wheel_motor)
		{
			helpers->log("got value: ");
			std::string ffs = std::to_string(stateFFB);
			helpers->log((char*)ffs.c_str());

			UINT32 length_ms = 100;
			if ((stateFFB > 0x09) && (stateFFB < 0x18))
			{
				//Spring
				double percentForce = (stateFFB - 15) / 8.0;
				triggers->Spring(percentForce);
			}

			if ((stateFFB > 0x1F) && (stateFFB < 0x28))
			{
				//Clutch
				double percentForce = (stateFFB - 31) / 8.0;
				triggers->Friction(percentForce);
			}

			if ((stateFFB > 0x2F) && (stateFFB < 0x3D))
			{
				//Centering
				double percentForce = (stateFFB - 47) / 13.0;
				triggers->Spring(percentForce);
			}

			if ((stateFFB > 0x3F) && (stateFFB < 0x48))
			{
				//Uncentering
				double percentForce = (stateFFB - 63) / 8.0;
				triggers->Sine(40, 0, percentForce, length_ms);
				triggers->Rumble(percentForce, percentForce, length_ms);
			}

			if ((stateFFB > 0x4F) && (stateFFB < 0x58))
			{
				//Roll Left
				double percentForce = (stateFFB - 79) / 8.0;
				triggers->Rumble(0, percentForce, length_ms);
				SendConstant(triggers, constants->DIRECTION_FROM_RIGHT, percentForce);
			}
			else if ((stateFFB > 0x5F) && (stateFFB < 0x68))
			{
				//Roll Right
				double percentForce = (stateFFB - 95) / 8.0;
				triggers->Rumble(percentForce, 0, length_ms);
				SendConstant(triggers, constants->DIRECTION_FROM_LEFT, percentForce);
			}
			else
			{
				SendConstant(triggers, constants->DIRECTION_FROM_LEFT, 0.0);
			}
		}
	}

	if (RunningFFB == MaximumSpeedActive)
	{
		if (name == awffb)
		{
			helpers->log("got value: ");
			std::string ffs = std::to_string(stateFFB);
			helpers->log((char*)ffs.c_str());

			UINT8 FFB = speedffb(stateFFB);
			UINT32 length_ms = 100;

			if (FFB > 0x00 && FFB < 0x11)
			{
				double percentForce = FFB / 16.0;
				triggers->Rumble(percentForce, 0, length_ms);
				triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
			}
			else if (FFB > 0x10 && FFB < 0x20)
			{
				double percentForce = (FFB - 16) / 16.0;
				triggers->Rumble(0, percentForce, length_ms);
				triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
			}
		}
	}

	if (RunningFFB == NaomiFFBActive)
	{
		if (name == m3ffb)
		{
			helpers->log("got value: ");
			std::string ffs = std::to_string(stateFFB);
			helpers->log((char*)ffs.c_str());

			UINT32 length_ms = 100;

			if (stateFFB != 0xD0)
			{
				if (oldstateFFB != stateFFB)
				{
					if ((stateFFB > 0xB0 && stateFFB < 0xC0) || stateFFB == 0x9F)
					{
						SineEffectState = stateFFB;
						Effect1 = true;
					}
				}

				if (stateFFB > 0x10 && stateFFB < 0x20)
				{
					double percentForce = (stateFFB - 10) / 15.0;
					triggers->Spring(percentForce);
				}

				if (stateFFB > 0x20 && stateFFB < 0x30)
				{
					double percentForce = (stateFFB - 32) / 15.0;
					triggers->Friction(percentForce);
				}

				if (stateFFB > 0x30 && stateFFB < 0x40)
				{
					double percentForce = (stateFFB - 48) / 15.0;
					triggers->Rumble(percentForce, percentForce, 100);
					triggers->Sine(40, 0, percentForce, 100);
				}

				if (stateFFB > 0x40 && stateFFB < 0x50)
				{
					double percentForce = (stateFFB - 64) / 15.0;
					triggers->Spring(percentForce);
				}

				if (stateFFB > 0x50 && stateFFB < 0x60)
				{
					double percentForce = (stateFFB - 80) / 15.0;
					triggers->Rumble(percentForce, 0, length_ms);
					triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
				}

				if (stateFFB > 0x60 && stateFFB < 0x70)
				{
					if (stateFFB != 0x68 && stateFFB != 0x69 && stateFFB != 0x6A && stateFFB != 0x6B)
					{
						double percentForce = (stateFFB - 96) / 15.0;
						triggers->Rumble(0, percentForce, length_ms);
						triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
					}
				}

				if (stateFFB > 0x80 && stateFFB < 0x90)
				{
					double percentForce = (stateFFB - 128) / 15.0;
					triggers->Rumble(0, percentForce, length_ms);
					triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
				}

				if (stateFFB > 0x90 && stateFFB < 0xA0)
				{
					double percentForce = (stateFFB - 144) / 15.0;
					triggers->Rumble(percentForce, 0, length_ms);
					triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
				}

				if (stateFFB > 0xA0 && stateFFB < 0xB0)
				{
					double percentForce = (stateFFB - 160) / 15.0;
					triggers->Rumble(percentForce, 0, length_ms);
					triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
				}

				if (stateFFB == 0xBF)
				{
					double percentForce = 1.0;
					triggers->Rumble(0, percentForce, length_ms);
					triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
				}

				if (stateFFB > 0xC0 && stateFFB < 0xD0)
				{
					double percentForce = (stateFFB - 192) / 15.0;
					triggers->Spring(percentForce);
				}

				if (stateFFB > 0xD0 && stateFFB < 0xE0)
				{
					double percentForce = (stateFFB - 208) / 15.0;
					triggers->Friction(percentForce);
				}

				if (stateFFB > 0xE0 && stateFFB < 0xE9)
				{
					double percentForce = (stateFFB - 224) / 8.0;
					triggers->Rumble(percentForce, 0, length_ms);
					triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
				}

				if (stateFFB > 0xE8 && stateFFB < 0xF0)
				{
					double percentForce = (stateFFB - 232) / 8.0;
					triggers->Rumble(percentForce, 0, length_ms);
					triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
				}

				if (Effect1)
				{
					++EffectCount;

					if (EffectCount >= 31)
					{
						Effect1 = false;
						EffectCount = 0;
					}

					double percentForce = (SineEffectState - 176) / 15.0;
					triggers->Rumble(percentForce, percentForce, 100);
					triggers->Sine(60, 0, percentForce, 100);
				}

				if (stateFFB == 0xC0)
				{
					triggers->Rumble(0, 0, 100);
					triggers->Sine(60, 0, 0, 100);
					triggers->Friction(0);
					triggers->Constant(constants->DIRECTION_FROM_RIGHT, 0);
					triggers->Constant(constants->DIRECTION_FROM_LEFT, 0);
				}
				oldstateFFB = stateFFB;
			}
		}
	}

	if (RunningFFB == InitialDActive)
	{
		if (name == midiffb)
		{
			helpers->log("got value: ");
			std::string ffs = std::to_string(stateFFB);
			helpers->log((char*)ffs.c_str());

			BYTE* ffb = reinterpret_cast<BYTE*>(&stateFFB);
			UINT32 length_ms = 100;

			if (ffb[2] == 0x80 && ffb[0] == 0x01)
				triggers->Spring(1.0);

			if (ffb[2] == 0x85 && ffb[1] > 0x00 && ffb[0] > 0x00)
			{
				double percentForce = ffb[0] / 127.0;
				double Period = ffb[1] / 127.0 * 120.0;
				triggers->Rumble(percentForce, percentForce, length_ms);
				triggers->Sine(static_cast<int>(Period), 0, percentForce, length_ms);
			}

			if (ffb[2] == 0x86 && ffb[0] > 0x00 && ffb[0] < 0x80)
			{
				double percentForce = ffb[0] / 127.0;
				triggers->Spring(percentForce);
			}

			if (ffb[2] == 0x84 && ffb[0] > 0x00 && ffb[0] < 0x80)
			{
				if (ffb[1] == 0x00)
				{
					double percentForce = (128 - ffb[0]) / 127.0;
					triggers->Rumble(percentForce, 0, length_ms);
					triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
				}
				else if (ffb[1] == 0x01)
				{
					double percentForce = (ffb[0] / 127.0);
					triggers->Rumble(0, percentForce, length_ms);
					triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
				}
			}
		}
	}

	if (RunningFFB == HardDrivinActive) //Hard Drivin
	{
		if (!HardDrivinFrame)
			HardDrivinFrame = true;

		//if ((frame & 7) == 4)
		if (frame == 4)
		{
			HardDrivinFFB = (vals[0] & 15) + ((vals[3] & 7) << 5);

			if ((vals[1] & 0xF0) == 0xF0)
				HardDrivinFFB |= 0x10;

			if ((vals[2] & 0xF0) == 0xF0)
				HardDrivinFFB = -HardDrivinFFB;

			//helpers->log("got value: ");
			//helpers->log((char*)ffs.c_str());

			//std::string ffs = std::to_string(HardDrivinFFB);
			//std::string val0 = std::to_string(vals[0]);
			//std::string val1 = std::to_string(vals[1]);
			//std::string val2 = std::to_string(vals[2]);
			//std::string val3 = std::to_string(vals[3]);
			//std::string val4 = std::to_string(vals[4]);
			//static char moreTest[256];
			//memset(moreTest, 0, 256);
			//sprintf(moreTest, "vals=%s %s %s %s FFS=%s", val0.c_str(), val1.c_str(), val2.c_str(), val3.c_str(), ffs.c_str());
			//helpers->log((char*)moreTest);

			if (HardDrivinFFB > 100) 
				HardDrivinFFB = 100;

			if (HardDrivinFFB < -100)
				HardDrivinFFB = -100;

			UINT32 length_ms = 100;
			if (HardDrivinFFB >= 0)
			{
				double percentForce = HardDrivinFFB / 100.0;
				triggers->Rumble(percentForce, 0, length_ms);
				SendConstant(triggers, constants->DIRECTION_FROM_LEFT, percentForce);
			}
			else if (HardDrivinFFB < 0)
			{
				HardDrivinFFB = -HardDrivinFFB;
				double percentForce = HardDrivinFFB / 100.0;
				triggers->Rumble(0, percentForce, length_ms);
				SendConstant(triggers, constants->DIRECTION_FROM_RIGHT, percentForce);
			}
		}
	}
}

typedef int(__stdcall* MAME_START)(int hWnd);
typedef int(__stdcall* MAME_STOP)(void);
typedef int(__stdcall* MAME_COPYDATA)(int id, const char* name);
typedef int(__stdcall* MAME_UPDATESTATE)(const char* id, int state);
typedef int(__stdcall* MAME_OUTPUT)(const char* name, int value);
typedef int(__stdcall* INIT_MAME)(int clientid, PWCHAR name, MAME_START start, MAME_STOP stop, MAME_COPYDATA copydata, MAME_UPDATESTATE updatestate, MAME_OUTPUT output);

MAME_START mame_start_ptr;
MAME_STOP mame_stop_ptr;
MAME_COPYDATA mame_copydata_ptr;
MAME_UPDATESTATE mame_updatestate_ptr;
MAME_OUTPUT mame_output_ptr;
INIT_MAME dll_init;

int __stdcall mame_start(int hWnd);
int __stdcall mame_stop(void);
int __stdcall mame_copydata(int id, const char* name);
int __stdcall mame_updatestate(const char* id, int state);
int __stdcall mame_output(const char* name, int value);

void CallTheOutputs()
{
#ifdef _WIN64  
	ProcDLL = LoadLibrary(TEXT("MAME64.dll"));
#else  
	ProcDLL = LoadLibrary(TEXT("MAME32.dll"));
#endif
	if (ProcDLL != NULL)
	{
		dll_init = (INIT_MAME)GetProcAddress(ProcDLL, "init_mame");
	}
	else
	{
#ifdef _WIN64
		MessageBoxA(NULL, "MAME64.dll is missing!", "FFBPluginError", NULL);
#else
		MessageBoxA(NULL, "MAME32.dll is missing!", "FFBPluginError", NULL);
#endif
	}
}
void AppendTextToEditCtrl(HWND hWnd, LPCTSTR lpString);

HWND hEdit;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void AppendTextToEditCtrl(HWND hWnd, LPCTSTR pszText);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CallTheOutputs();
	HWND hWnd;
	MSG msg;
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"MAMEInterop";
	wcex.hIconSm = NULL;

	RegisterClassEx(&wcex);

	hWnd = CreateWindow(L"MAMEInterop", L"MAMEInterop", WS_OVERLAPPEDWINDOW,
		0, 0, 512, 512, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)
		MessageBox(hWnd, L"Could not create window", L"Error", MB_OK | MB_ICONERROR);

	hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
		8, 8, 488, 464, hWnd, NULL, GetModuleHandle(NULL), NULL);

	if (hEdit == NULL)
		MessageBox(hWnd, L"Could not create edit box.", L"Error", MB_OK | MB_ICONERROR);

	if (hWnd != NULL)
	{
		//ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
	}
	
	mame_start_ptr = mame_start;
	mame_stop_ptr = mame_stop;
	mame_copydata_ptr = mame_copydata;
	mame_updatestate_ptr = mame_updatestate;
	mame_output_ptr = mame_output;

	// without MAME64.dll (its message box has been shown), only the network outputs work: calling
	// a null dll_init crashed the game once the message box was closed
	if (dll_init != NULL)
		dll_init(0, L"Test", mame_start_ptr, mame_stop_ptr, mame_copydata_ptr, mame_updatestate_ptr, mame_output_ptr);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void AppendTextToEditCtrl(HWND hWnd, LPCTSTR lpString)
{
	int iPrevLen = GetWindowTextLength(hWnd);
	SendMessage(hWnd, EM_SETSEL, iPrevLen, iPrevLen);
	SendMessage(hWnd, EM_REPLACESEL, 0, (LPARAM)lpString);
}

int __stdcall mame_start(int hWnd)
{
	WCHAR buf[256];

	wsprintf(buf, L"mame_start hwnd = 0x%08x\r\n", hWnd);

	AppendTextToEditCtrl(hEdit, buf);

	return 1;
}

int __stdcall mame_stop(void)
{
	AppendTextToEditCtrl(hEdit, TEXT("mame_stop\r\n"));

	StopConstant = 255;

	return 1;
}

static void removeWord(char* str, char* toRemove)
{
	int i, j, stringLen, toRemoveLen;
	int found;

	stringLen = (int)strlen(str);
	toRemoveLen = (int)strlen(toRemove);


	for (i = 0; i <= stringLen - toRemoveLen; i++)
	{
		found = 1;
		for (j = 0; j < toRemoveLen; j++)
		{
			if (str[i + j] != toRemove[j])
			{
				found = 0;
				break;
			}
		}

		if (str[i + j] != ' ' && str[i + j] != '\t' && str[i + j] != '\n' && str[i + j] != '\0')
		{
			found = 0;
		}


		if (found == 1)
		{
			for (j = i; j <= stringLen - toRemoveLen; j++)
			{
				str[j] = str[j + toRemoveLen];
			}

			stringLen = stringLen - toRemoveLen;

			i--;
		}
	}
}

int __stdcall mame_copydata(int id, const char* name)
{
	// The names are cut to fit buf: the network outputs pass lines of up to 4 KB
	WCHAR buf[256];

	wsprintf(buf, L"id %d = '%.200S'\r\n", id, name);

	if (id == 0)
	{
		sprintf_s(GameName, "%.255s", name);

		removeWord(GameName, "game =");

		UINT whitespace = 0;

		for (UINT i = 0; i < strlen(GameName); i++)
		{
			if (GameName[i] == ' ' || GameName[i] == '\t')
				whitespace++;
			else
				break;
		}

		for (UINT i = 0; i < strlen(GameName); i++)
		{
			if (i + whitespace < strlen(GameName))
				GameName[i] = GameName[i + whitespace];
			else
				GameName[i] = 0;
		}
	}

	AppendTextToEditCtrl(hEdit, buf);

	return 1;
}

int __stdcall mame_updatestate(const char* id, int state)
{
	WCHAR buf[256];

	LPCSTR name = id;

	wsprintf(buf, L"updatestate: id=%ld (%.200S) state=%d\r\n", (INT_PTR)id, IS_INTRESOURCE(name) ? "" : name, state);
	AppendTextToEditCtrl(hEdit, buf);

	AcquireSRWLockExclusive(&nameFFBLock);
	if (IS_INTRESOURCE(name)) // what CStringA did with an id that isn't a string
		nameFFB[0] = '\0';
	else
		strncpy_s(nameFFB, name, _TRUNCATE);
	ReleaseSRWLockExclusive(&nameFFBLock);

	newstateFFB = state;

	if (HardDrivinFrame)
	{
		if (name == wheelA)
		{
			state3 = state2;
			state2 = state1;
			state1 = state0;
			state0 = state;

			//detect 0xE0 and 0x00 
			boolean stop = false;
			if ((state == 0xE0 && state1 == 0x00 && state2 == 0xE0 && state3 == 0x00) || 
				(state == 0x00 && state1 == 0xE0 && state2 == 0x00 && state3 == 0xE0)) 
			{
				stop = true;
				frame = 0;
			}
			if (frame > 4) {
				frame = 0;
			}
			//vals[0] should always be < 200. vals[1] should always be > 200. If not, the state is invalid, start over. 
			if (frame > 2 && (vals[0] > 200 || (vals[1] < 200 && vals[1] != 0))) {
				frame = 0;
				vals[1] = 0;
				vals[2] = 0;
				vals[3] = 0;
			}

			//if (!(((state == 0xE0) || (state == 0)) && ((frame == 0) || (frame == 4))))
			//if ((state != 0xE0) && (state != 0x00))
			if(!stop)
			{
				vals[frame] = state;
				frame++;
			}
		}
	}

	if (RomGameName && RunningFFB > 0)
		FFBGameEffects(myconstants, myhelpers, mytriggers, state, name);
	
	return 1;
}

int __stdcall mame_output(const char* name, int value)
{
	WCHAR buf[256];

	wsprintf(buf, L"output: name=%.200S value=%d\r\n", name, value);
	AppendTextToEditCtrl(hEdit, buf);

	return 1;
}

static DWORD WINAPI ScanThread(LPVOID lpParam)
{
	if (romname == superchs || romname == superchsj || romname == superchsp || romname == superchsu) //Super Chase Criminal Termination
	{
		aAddy2 = PatternScan("\x80\x38\x00\x50\x49\x5A\x05\x70\x11\x01\x4B\x54\x4F", "xxxxxxxxxxxxx");
	}

	if (romname == dirtdash || romname == dirtdasha || romname == dirtdashj || romname == acedrvrw || romname == acedrive || romname == victlapw || romname == victlap) //Dirt Dash, Ace Driver & Ace Driver Victory Lap
	{
		aAddy2 = PatternScan("\xC8\x00\xC8\x00\xC8\x00\xC8\x00\xC8\x00\xC8\x00\xC8\x00\xC8\x00\x14\x00\x14\x00\x14\x00\x14\x00\x14\x00\x14\x00\x14\x00\x14", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	}

	return 0;
}

DWORD WINAPI ThreadForOutputs(LPVOID lpParam)
{
	// also read the network outputs (MAME: "-output network"), unless MAMENetworkOutputs=0
	if (GetSettingInt(TEXT("MAMENetworkOutputs"), 1) != 0)
		StartMameNetworkOutputs(GetSettingInt(TEXT("GameId"), 0));
	WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	return 0;
}

void MAMESupermodel::FFBLoop(EffectConstants* constants, Helpers* helpers, EffectTriggers* triggers) {

	myconstants = constants;
	myhelpers = helpers;
	mytriggers = triggers;

	if (!init)
	{
		OpenHapticDevice2();
		SDL_HapticSetGain(haptic2, 100);

		OpenHapticDevice3();
		SDL_HapticSetGain(haptic3, 100);

		// Flycast is ended with TerminateProcess by the ExitProcess hook of DllMain.cpp
		init = true;
	}

	if (EnableForceSpringEffect)
	{
		triggers->Springi(ForceSpringStrength / 100.0);
	}

	// these used to be new char[256] on every call (every 16 ms) and never freed
	static char romnameBuffer[256];
	static char nameBuffer[256];

	romname = romnameBuffer; //name of rom being played
	sprintf_s(romnameBuffer, "%.255s", GameName);

	name = nameBuffer; //name of FFB currently
	AcquireSRWLockShared(&nameFFBLock);
	strcpy_s(nameBuffer, nameFFB);
	ReleaseSRWLockShared(&nameFFBLock);

	if (!RomGameName)
	{
		if (romname != NULL)
		{
			//Select code to run via rom name
			if (romname == dayto2pe || romname == daytona2)
			{				
				configMinForce = configMinForceDaytona2;
				configMaxForce = configMaxForceDaytona2;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftDaytona2;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftDaytona2;
				configAlternativeMinForceRight = configAlternativeMinForceRightDaytona2;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightDaytona2;
				configFeedbackLength = configFeedbackLengthDaytona2;
				PowerMode = PowerModeDaytona2;
				EnableDamper = EnableDamperDaytona2;
				DamperStrength = DamperStrengthDaytona2;
				EnableForceSpringEffect = EnableForceSpringEffectDaytona2;
				ForceSpringStrength = ForceSpringStrengthDaytona2;

				RunningFFB = "Daytona2Active";
			}

			if (romname == scud || romname == scuda || romname == scudau || romname == scuddx|| romname == scudj || romname == scudplus || romname == scudplusa)
			{
				configMinForce = configMinForceScud;
				configMaxForce = configMaxForceScud;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftScud;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftScud;
				configAlternativeMinForceRight = configAlternativeMinForceRightScud;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightScud;
				configFeedbackLength = configFeedbackLengthScud;
				PowerMode = PowerModeScud;
				EnableDamper = EnableDamperScud;
				DamperStrength = DamperStrengthScud;
				EnableForceSpringEffect = EnableForceSpringEffectScud;
				ForceSpringStrength = ForceSpringStrengthScud;

				RunningFFB = "ScudRaceActive";
			}

			if (romname == lemans24)
			{
				configMinForce = configMinForceLeMans;
				configMaxForce = configMaxForceLeMans;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftLeMans;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftLeMans;
				configAlternativeMinForceRight = configAlternativeMinForceRightLeMans;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightLeMans;
				configFeedbackLength = configFeedbackLengthLeMans;
				PowerMode = PowerModeLeMans;
				EnableDamper = EnableDamperLeMans;
				DamperStrength = DamperStrengthLeMans;
				EnableForceSpringEffect = EnableForceSpringEffectLeMans;
				ForceSpringStrength = ForceSpringStrengthLeMans;

				RunningFFB = "ScudRaceActive";
			}

			if (romname == dirtdvlsa || romname == dirtdvls || romname == dirtdvlsj || romname == dirtdvlsg || romname == dirtdvlsu || romname == dirtdvlsau)
			{
				configMinForce = configMinForceDirtDevils;
				configMaxForce = configMaxForceDirtDevils;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftDirtDevils;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftDirtDevils;
				configAlternativeMinForceRight = configAlternativeMinForceRightDirtDevils;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightDirtDevils;
				configFeedbackLength = configFeedbackLengthDirtDevils;
				PowerMode = PowerModeDirtDevils;
				EnableForceSpringEffect = EnableForceSpringEffectDirtDevils;
				ForceSpringStrength = ForceSpringStrengthDirtDevils;
				EnableDamper = EnableDamperDirtDevils;
				DamperStrength = DamperStrengthDirtDevils;

				RunningFFB = "ScudRaceActive";
			}

			if (romname == ecau || romname == eca || romname == ecap || romname == ecaj)
			{
				configMinForce = configMinForceECA;
				configMaxForce = configMaxForceECA;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftECA;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftECA;
				configAlternativeMinForceRight = configAlternativeMinForceRightECA;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightECA;
				configFeedbackLength = configFeedbackLengthECA;
				PowerMode = PowerModeECA;
				EnableForceSpringEffect = EnableForceSpringEffectECA;
				ForceSpringStrength = ForceSpringStrengthECA;
				EnableDamper = EnableDamperECA;
				DamperStrength = DamperStrengthECA;

				RunningFFB = "ScudRaceActive";
			}
			
			if (romname == srally2 || romname == srally2x || romname == srally2dx || romname == srally2p || romname == srally2pa)
			{
				configMinForce = configMinForceSRally2;
				configMaxForce = configMaxForceSRally2;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftSRally2;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftSRally2;
				configAlternativeMinForceRight = configAlternativeMinForceRightSRally2;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightSRally2;
				configFeedbackLength = configFeedbackLengthSRally2;
				PowerMode = PowerModeSRally2;
				EnableForceSpringEffect = EnableForceSpringEffectSRally2;
				ForceSpringStrength = ForceSpringStrengthSRally2;
				EnableDamper = EnableDamperSRally2;
				DamperStrength = DamperStrengthSRally2;

				RunningFFB = "Srally2Active";
			}

			if (romname == cartfury)
			{
				configMinForce = configMinForceCartFury;
				configMaxForce = configMaxForceCartFury;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftCartFury;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftCartFury;
				configAlternativeMinForceRight = configAlternativeMinForceRightCartFury;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightCartFury;
				configFeedbackLength = configFeedbackLengthCartFury;
				PowerMode = PowerModeCartFury;
				EnableForceSpringEffect = EnableForceSpringEffectCartFury;
				ForceSpringStrength = ForceSpringStrengthCartFury;
				EnableDamper = EnableDamperCartFury;
				DamperStrength = DamperStrengthCartFury;

				RunningFFB = "RacingFullValueActive1";
			}

			if (romname == overrev || romname == overrevba || romname == overrevb)
			{
				configMinForce = configMinForceOverRev;
				configMaxForce = configMaxForceOverRev;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftOverRev;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftOverRev;
				configAlternativeMinForceRight = configAlternativeMinForceRightOverRev;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightOverRev;
				configFeedbackLength = configFeedbackLengthOverRev;
				PowerMode = PowerModeOverRev;
				EnableForceSpringEffect = EnableForceSpringEffectOverRev;
				ForceSpringStrength = ForceSpringStrengthOverRev;
				EnableDamper = EnableDamperOverRev;
				DamperStrength = DamperStrengthOverRev;

				RunningFFB = "m2new";
			}

			if (romname == vr)
			{
				configMinForce = configMinForceVirtuaRacing;
				configMaxForce = configMaxForceVirtuaRacing;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftVirtuaRacing;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftVirtuaRacing;
				configAlternativeMinForceRight = configAlternativeMinForceRightVirtuaRacing;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightVirtuaRacing;
				configFeedbackLength = configFeedbackLengthVirtuaRacing;
				EnableForceSpringEffect = EnableForceSpringEffectVirtuaRacing;
				ForceSpringStrength = ForceSpringStrengthVirtuaRacing;
				EnableDamper = EnableDamperVirtuaRacing;
				DamperStrength = DamperStrengthVirtuaRacing;

				RunningFFB = "VirtuaRacingActive";
			}

			if (romname == smgp || romname == smgp6 || romname == smgp5 ||
				romname == smgpu || romname == smgpu1 || romname == smgpu2 ||
				romname == smgpj || romname == smgpja ||
				romname == smgpd || romname == smgp6d || romname == smgp5d ||
				romname == smgpud || romname == smgpu1d || romname == smgpu2d || romname == smgpjd)
			{
				configMinForce = configMinForceSuperMonacoGP;
				configMaxForce = configMaxForceSuperMonacoGP;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftSuperMonacoGP;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftSuperMonacoGP;
				configAlternativeMinForceRight = configAlternativeMinForceRightSuperMonacoGP;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightSuperMonacoGP;
				configFeedbackLength = configFeedbackLengthSuperMonacoGP;
				EnableForceSpringEffect = EnableForceSpringEffectSuperMonacoGP;
				ForceSpringStrength = ForceSpringStrengthSuperMonacoGP;
				SinePeriod = SinePeriodSuperMonacoGP;
				SineFadePeriod = SineFadePeriodSuperMonacoGP;
				SineStrength = SineStrengthSuperMonacoGP;

				RunningFFB = "SuperMonacoGPActive";
			}

			if (romname == roadedge)
			{
				configMinForce = configMinForceRoadEdge;
				configMaxForce = configMaxForceRoadEdge;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftRoadEdge;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftRoadEdge;
				configAlternativeMinForceRight = configAlternativeMinForceRightRoadEdge;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightRoadEdge;
				configFeedbackLength = configFeedbackLengthRoadEdge;
				PowerMode = PowerModeRoadEdge;
				EnableForceSpringEffect = EnableForceSpringEffectRoadEdge;
				ForceSpringStrength = ForceSpringStrengthRoadEdge;
				EnableDamper = EnableDamperRoadEdge;
				DamperStrength = DamperStrengthRoadEdge;

				RunningFFB = "hng64";
			}

			if (romname == xrally)
			{
				configMinForce = configMinForceXRally;
				configMaxForce = configMaxForceXRally;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftXRally;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftXRally;
				configAlternativeMinForceRight = configAlternativeMinForceRightXRally;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightXRally;
				configFeedbackLength = configFeedbackLengthXRally;
				PowerMode = PowerModeXRally;
				EnableForceSpringEffect = EnableForceSpringEffectXRally;
				ForceSpringStrength = ForceSpringStrengthXRally;
				EnableDamper = EnableDamperXRally;
				DamperStrength = DamperStrengthXRally;

				RunningFFB = "hng64";
			}

			if (romname == thrilld || romname == thrillda || romname == thrilldj || romname == thrillde || romname == thrilldgej || romname == thrilldgeu || romname == thrilldgke || romname == thrilldgkk || romname == thrilldgmj || romname == thrilldgnj || romname == thrilldgpj || romname == thrilldgkee)
			{
				configMinForce = configMinForceThrillD;
				configMaxForce = configMaxForceThrillD;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftThrillD;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftThrillD;
				configAlternativeMinForceRight = configAlternativeMinForceRightThrillD;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightThrillD;
				configFeedbackLength = configFeedbackLengthThrillD;
				PowerMode = PowerModeThrillD;
				EnableForceSpringEffect = EnableForceSpringEffectThrillD;
				ForceSpringStrength = ForceSpringStrengthThrillD;
				EnableDamper = EnableDamperThrillD;
				DamperStrength = DamperStrengthThrillD;

				RunningFFB = "Konami";
			}

			if (romname == windheat || romname == windheata || romname == windheatj || romname == windheatu)
			{
				configMinForce = configMinForceWindheat;
				configMaxForce = configMaxForceWindheat;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftWindheat;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftWindheat;
				configAlternativeMinForceRight = configAlternativeMinForceRightWindheat;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightWindheat;
				configFeedbackLength = configFeedbackLengthWindheat;
				PowerMode = PowerModeWindheat;
				EnableForceSpringEffect = EnableForceSpringEffectWindheat;
				ForceSpringStrength = ForceSpringStrengthWindheat;
				EnableDamper = EnableDamperWindheat;
				DamperStrength = DamperStrengthWindheat;

				RunningFFB = "Konami";
			}

			if (romname == gticlub || romname == gticluba || romname == gticlubj || romname == gticlubu)
			{
				configMinForce = configMinForceGTIClub;
				configMaxForce = configMaxForceGTIClub;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftGTIClub;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftGTIClub;
				configAlternativeMinForceRight = configAlternativeMinForceRightGTIClub;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightGTIClub;
				configFeedbackLength = configFeedbackLengthGTIClub;
				PowerMode = PowerModeGTIClub;
				EnableForceSpringEffect = EnableForceSpringEffectGTIClub;
				ForceSpringStrength = ForceSpringStrengthGTIClub;
				EnableDamper = EnableDamperGTIClub;
				DamperStrength = DamperStrengthGTIClub;

				RunningFFB = "Konami";
			}

			if (romname == midnrun || romname == midnruna || romname == midnrunj || romname == midnruna2)
			{
				configMinForce = configMinForceMidnRun;
				configMaxForce = configMaxForceMidnRun;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftMidnRun;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftMidnRun;
				configAlternativeMinForceRight = configAlternativeMinForceRightMidnRun;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightMidnRun;
				configFeedbackLength = configFeedbackLengthMidnRun;
				PowerMode = PowerModeMidnRun;
				EnableForceSpringEffect = EnableForceSpringEffectMidnRun;
				ForceSpringStrength = ForceSpringStrengthMidnRun;
				EnableDamper = EnableDamperMidnRun;
				DamperStrength = DamperStrengthMidnRun;

				RunningFFB = "Konami";
			}

			if (romname == racingj || romname == racingja || romname == racingje || romname == racingjj || romname == racingju)
			{
				configMinForce = configMinForceRacingJ;
				configMaxForce = configMaxForceRacingJ;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftRacingJ;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftRacingJ;
				configAlternativeMinForceRight = configAlternativeMinForceRightRacingJ;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightRacingJ;
				configFeedbackLength = configFeedbackLengthRacingJ;
				PowerMode = PowerModeRacingJ;
				EnableForceSpringEffect = EnableForceSpringEffectRacingJ;
				ForceSpringStrength = ForceSpringStrengthRacingJ;
				EnableDamper = EnableDamperRacingJ;
				DamperStrength = DamperStrengthRacingJ;

				RunningFFB = "Konami";
			}

			if (romname == racingj2 || romname == racingj2a || romname == racingj2e || romname == racingj2j || romname == racingj2u)
			{
				configMinForce = configMinForceRacingJ2;
				configMaxForce = configMaxForceRacingJ2;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftRacingJ2;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftRacingJ2;
				configAlternativeMinForceRight = configAlternativeMinForceRightRacingJ2;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightRacingJ2;
				configFeedbackLength = configFeedbackLengthRacingJ2;
				PowerMode = PowerModeRacingJ2;
				EnableForceSpringEffect = EnableForceSpringEffectRacingJ2;
				ForceSpringStrength = ForceSpringStrengthRacingJ2;
				EnableDamper = EnableDamperRacingJ2;
				DamperStrength = DamperStrengthRacingJ2;

				RunningFFB = "Konami";
			}

			if (romname == sfrush || romname == sfrusha)
			{
				configMinForce = configMinForceSFRush;
				configMaxForce = configMaxForceSFRush;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftSFRush;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftSFRush;
				configAlternativeMinForceRight = configAlternativeMinForceRightSFRush;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightSFRush;
				configFeedbackLength = configFeedbackLengthSFRush;
				PowerMode = PowerModeSFRush;
				EnableForceSpringEffect = EnableForceSpringEffectSFRush;
				ForceSpringStrength = ForceSpringStrengthSFRush;
				EnableDamper = EnableDamperSFRush;
				DamperStrength = DamperStrengthSFRush;

				RunningFFB = "RacingFullValueActive2";
			}

			if (romname == sfrushrk || romname == sfrushrkwo)
			{
				configMinForce = configMinForceSFRushRock;
				configMaxForce = configMaxForceSFRushRock;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftSFRushRock;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftSFRushRock;
				configAlternativeMinForceRight = configAlternativeMinForceRightSFRushRock;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightSFRushRock;
				configFeedbackLength = configFeedbackLengthSFRushRock;
				PowerMode = PowerModeSFRushRock;
				EnableForceSpringEffect = EnableForceSpringEffectSFRushRock;
				ForceSpringStrength = ForceSpringStrengthSFRushRock;
				EnableDamper = EnableDamperSFRushRock;
				DamperStrength = DamperStrengthSFRushRock;

				RunningFFB = "RacingFullValueActive2";
			}

			if (romname == crusnwld || romname == crusnwld24 || romname == crusnwld23 || romname == crusnwld20 || romname == crusnwld19 || romname == crusnwld17)
			{
				configMinForce = configMinForceCrusnWld;
				configMaxForce = configMaxForceCrusnWld;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftCrusnWld;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftCrusnWld;
				configAlternativeMinForceRight = configAlternativeMinForceRightCrusnWld;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightCrusnWld;
				configFeedbackLength = configFeedbackLengthCrusnWld;
				PowerMode = PowerModeCrusnWld;
				EnableForceSpringEffect = EnableForceSpringEffectCrusnWld;
				ForceSpringStrength = ForceSpringStrengthCrusnWld;
				EnableDamper = EnableDamperCrusnWld;
				DamperStrength = DamperStrengthCrusnWld;

				RunningFFB = "RacingFullValueActive2";
			}

			if (romname == superchs || romname == superchsj || romname == superchsp || romname == superchsu)
			{
				configMinForce = configMinForceSuperChase;
				configMaxForce = configMaxForceSuperChase;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftSuperChase;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftSuperChase;
				configAlternativeMinForceRight = configAlternativeMinForceRightSuperChase;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightSuperChase;
				configFeedbackLength = configFeedbackLengthSuperChase;
				PowerMode = PowerModeSuperChase;
				EnableForceSpringEffect = EnableForceSpringEffectSuperChase;
				ForceSpringStrength = ForceSpringStrengthSuperChase;
				EnableDamper = EnableDamperSuperChase;
				DamperStrength = DamperStrengthSuperChase;

				RunningFFB = "SuperChaseActive";
			}

			if (romname == offroadc || romname == offroadc4 || romname == offroadc3 || romname == offroadc1)
			{
				configMinForce = configMinForceOffRoadC;
				configMaxForce = configMaxForceOffRoadC;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftOffRoadC;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftOffRoadC;
				configAlternativeMinForceRight = configAlternativeMinForceRightOffRoadC;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightOffRoadC;
				configFeedbackLength = configFeedbackLengthOffRoadC;
				PowerMode = PowerModeOffRoadC;
				EnableForceSpringEffect = EnableForceSpringEffectOffRoadC;
				ForceSpringStrength = ForceSpringStrengthOffRoadC;
				EnableDamper = EnableDamperOffRoadC;
				DamperStrength = DamperStrengthOffRoadC;

				RunningFFB = "RacingFullValueActive2";
			}

			if (romname == crusnusa || romname == crusnusa40 || romname == crusnusa21)
			{
				configMinForce = configMinForceCrusnUSA;
				configMaxForce = configMaxForceCrusnUSA;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftCrusnUSA;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftCrusnUSA;
				configAlternativeMinForceRight = configAlternativeMinForceRightCrusnUSA;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightCrusnUSA;
				configFeedbackLength = configFeedbackLengthCrusnUSA;
				PowerMode = PowerModeCrusnUSA;
				EnableForceSpringEffect = EnableForceSpringEffectCrusnUSA;
				ForceSpringStrength = ForceSpringStrengthCrusnUSA;
				EnableDamper = EnableDamperCrusnUSA;
				DamperStrength = DamperStrengthCrusnUSA;

				RunningFFB = "RacingFullValueActive2";
			}

			if (romname == crusnexo)
			{
				configMinForce = configMinForceCrusnExotica;
				configMaxForce = configMaxForceCrusnExotica;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftCrusnExotica;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftCrusnExotica;
				configAlternativeMinForceRight = configAlternativeMinForceRightCrusnExotica;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightCrusnExotica;
				configFeedbackLength = configFeedbackLengthCrusnExotica;
				PowerMode = PowerModeCrusnExotica;
				EnableForceSpringEffect = EnableForceSpringEffectCrusnExotica;
				ForceSpringStrength = ForceSpringStrengthCrusnExotica;
				EnableDamper = EnableDamperCrusnExotica;
				DamperStrength = DamperStrengthCrusnExotica;
				RunningFFB = "RacingFullValueExotica";
			}

			if (romname == calspeed || romname == calspeeda || romname == calspeedb)
			{
				configMinForce = configMinForceCalSpeed;
				configMaxForce = configMaxForceCalSpeed;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftCalSpeed;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftCalSpeed;
				configAlternativeMinForceRight = configAlternativeMinForceRightCalSpeed;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightCalSpeed;
				configFeedbackLength = configFeedbackLengthCalSpeed;
				PowerMode = PowerModeCalSpeed;
				EnableForceSpringEffect = EnableForceSpringEffectCalSpeed;
				ForceSpringStrength = ForceSpringStrengthCalSpeed;
				EnableDamper = EnableDamperCalSpeed;
				DamperStrength = DamperStrengthCalSpeed;
				ConstantInfHoldsAtZero = true; // its raw motor value crosses 0 when the force changes direction

				RunningFFB = "RacingFullValueActive1";
			}

			if (romname == sf2049 || romname == sf2049se || romname == sf2049te)
			{
				configMinForce = configMinForceSFRush2049;
				configMaxForce = configMaxForceSFRush2049;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftSFRush2049;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftSFRush2049;
				configAlternativeMinForceRight = configAlternativeMinForceRightSFRush2049;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightSFRush2049;
				configFeedbackLength = configFeedbackLengthSFRush2049;
				PowerMode = PowerModeSFRush2049;
				EnableForceSpringEffect = EnableForceSpringEffectSFRush2049;
				ForceSpringStrength = ForceSpringStrengthSFRush2049;
				EnableDamper = EnableDamperSFRush2049;
				DamperStrength = DamperStrengthSFRush2049;

				RunningFFB = "RacingFullValueActive1";
			}

			if (romname == harddriv1 || romname == harddrivb6 || romname == harddrivb || romname == harddrivg4 || romname == harddrivg || romname == harddrivj6 || romname == harddrivj ||
				romname == harddriv2 || romname == harddriv3 || romname == harddriv || romname == harddrivcb || romname == harddrivcg || romname == harddrivc1 || romname == harddrivc ||
				romname == hdrivairp || romname == hdrivair || romname == racedrivb1 || romname == racedrivb4 || romname == racedrivb || romname == racedrivg1 || romname == racedrivg4 ||
				romname == racedrivg || romname == racedriv1 || romname == racedriv2 || romname == racedriv3 || romname == racedriv4 || romname == racedriv || romname == racedrivcb4 || 
				romname == racedrivcb || romname == racedrivcg4 || romname == racedrivcg || romname == racedrivc2 || romname == racedrivc4 || romname == racedrivc || romname == racedrivpan)
			{
				configMinForce = configMinForceHardD;
				configMaxForce = configMaxForceHardD;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftHardD;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftHardD;
				configAlternativeMinForceRight = configAlternativeMinForceRightHardD;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightHardD;
				configFeedbackLength = configFeedbackLengthHardD;
				PowerMode = PowerModeHardD;
				EnableForceSpringEffect = EnableForceSpringEffectHardD;
				ForceSpringStrength = ForceSpringStrengthHardD;
				EnableDamper = EnableDamperHardD;
				DamperStrength = DamperStrengthHardD;

				RunningFFB = "HardDrivinActive";
			}

			if (romname == revx || romname == revxp5)
			{
				configMinForce = configMinForceRevX;
				configMaxForce = configMaxForceRevX;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorRevX;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorRevX;
				configMinForceDevice2 = configMinForceRevXDevice2;
				configMaxForceDevice2 = configMaxForceRevXDevice2;
				configMinForceDevice3 = configMinForceRevXDevice3;
				configMaxForceDevice3 = configMaxForceRevXDevice3;

				RunningFFB = "LightGunActive";
			}

			if (romname == zombraid || romname == zombraidpj || romname == zombraidp)
			{
				configMinForce = configMinForceZombRaid;
				configMaxForce = configMaxForceZombRaid;
				configMinForceDevice2 = configMinForceZombRaidDevice2;
				configMaxForceDevice2 = configMaxForceZombRaidDevice2;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorZombRaid;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorZombRaid;

				RunningFFB = "LightGunActive";
			}

			if (romname == bbusters || romname == bbustersu || romname == bbustersua || romname == bbustersj)
			{
				configMinForce = configMinForceBBusters;
				configMaxForce = configMaxForceBBusters;
				configMinForceDevice2 = configMinForceBBustersDevice2;
				configMaxForceDevice2 = configMaxForceBBustersDevice2;
				configMinForceDevice3 = configMinForceBBustersDevice3;
				configMaxForceDevice3 = configMaxForceBBustersDevice3;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorBBusters;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorBBusters;

				RunningFFB = "LightGunActive";
			}

			if (romname == mechatt || romname == mechattu || romname == mechattu1 || romname == mechattj)
			{
				configMinForce = configMinForceMechatt;
				configMaxForce = configMaxForceMechatt;
				configMinForceDevice2 = configMinForceMechattDevice2;
				configMaxForceDevice2 = configMaxForceMechattDevice2;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorMechatt;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorMechatt;

				RunningFFB = "LightGunActive";
			}

			if (romname == tshoot)
			{
				configMinForce = configMinForceTShoot;
				configMaxForce = configMaxForceTShoot;
				configMinForceDevice2 = configMinForceTShootDevice2;
				configMaxForceDevice2 = configMaxForceTShootDevice2;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorTShoot;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorTShoot;

				RunningFFB = "LightGunActive";
			}

			if (romname == gunbustru || romname == gunbustr || romname == gunbustrj)
			{
				configMinForce = configMinForceGunBuster;
				configMaxForce = configMaxForceGunBuster;
				configMinForceDevice2 = configMinForceGunBusterDevice2;
				configMaxForceDevice2 = configMaxForceGunBusterDevice2;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorGunBuster;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorGunBuster;

				RunningFFB = "LightGunActive";
			}

			if (romname == alien3 || romname == alien3u)
			{
				configMinForce = configMinForceAlien3;
				configMaxForce = configMaxForceAlien3;
				configMinForceDevice2 = configMinForceAlien3Device2;
				configMaxForceDevice2 = configMaxForceAlien3Device2;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorAlien3;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorAlien3;

				RunningFFB = "LightGunActive";
			}

			if (romname == ptblank2 || romname == ptblank2ua)
			{
				configMinForce = configMinForcePointBlank2;
				configMaxForce = configMaxForcePointBlank2;
				configMinForceDevice2 = configMinForcePointBlank2Device2;
				configMaxForceDevice2 = configMaxForcePointBlank2Device2;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorPointBlank2;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorPointBlank2;

				RunningFFB = "LightGunActive";
			}

			if (romname == ghlpanic)
			{
				configMinForce = configMinForceGhoulPanic;
				configMaxForce = configMaxForceGhoulPanic;
				configMinForceDevice2 = configMinForceGhoulPanicDevice2;
				configMaxForceDevice2 = configMaxForceGhoulPanicDevice2;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorGhoulPanic;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorGhoulPanic;

				RunningFFB = "LightGunActive";
			}

			if (romname == spacegun || romname == spacegunu || romname == spacegunj)
			{
				configMinForce = configMinForceSpacegun;
				configMaxForce = configMaxForceSpacegun;
				configMinForceDevice2 = configMinForceSpacegunDevice2;
				configMaxForceDevice2 = configMaxForceSpacegunDevice2;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorSpacegun;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorSpacegun;

				RunningFFB = "LightGunActive";
			}

			if (romname == term2 || romname == term2la1 || romname == term2la2 || romname == term2la3 || romname == term2lg1)
			{
				configMinForce = configMinForceTerm2;
				configMaxForce = configMaxForceTerm2;
				configMinForceDevice2 = configMinForceTerm2Device2;
				configMaxForceDevice2 = configMaxForceTerm2Device2;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorTerm2;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorTerm2;

				RunningFFB = "LightGunActive";
			}

			if (romname == rchase || romname == rchasej)
			{
				configMinForce = configMinForceRchase;
				configMaxForce = configMaxForceRchase;
				configMinForceDevice2 = configMinForceRchaseDevice2;
				configMaxForceDevice2 = configMaxForceRchaseDevice2;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorRchase;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorRchase;

				RunningFFB = "LightGunActive";
			}

			if (romname == lghost || romname == lghostd || romname == lghostu || romname == lghostud || romname == lghostj)
			{
				configMinForce = configMinForceLGhost;
				configMaxForce = configMaxForceLGhost;
				configMinForceDevice2 = configMinForceLGhostDevice2;
				configMaxForceDevice2 = configMaxForceLGhostDevice2;
				configMinForceDevice3 = configMinForceLGhostDevice3;
				configMaxForceDevice3 = configMaxForceLGhostDevice3;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorLGhost;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorLGhost;

				RunningFFB = "LightGunActive";
			}

			if (romname == timecris || romname == timecrisa)
			{
				configMinForce = configMinForceTimeCris;
				configMaxForce = configMaxForceTimeCris;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorTimeCris;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorTimeCris;

				RunningFFB = "LightGunActive";
			}

			if (romname == othunder || romname == othundero || romname == othunderuo || romname == othunderu || romname == othunderj)
			{
				configMinForce = configMinForceOThunder;
				configMaxForce = configMaxForceOThunder;
				configMinForceDevice2 = configMinForceOThunderDevice2;
				configMaxForceDevice2 = configMaxForceOThunderDevice2;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorOThunder;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorOThunder;

				RunningFFB = "LightGunActive";
			}

			if (romname == opwolf || romname == opwolfp || romname == opwolfj || romname == opwolfu || romname == opwolfa)
			{
				configMinForce = configMinForceOpWolf;
				configMaxForce = configMaxForceOpWolf;
				configMinForceDevice2 = configMinForceOpWolfDevice2;
				configMaxForceDevice2 = configMaxForceOpWolfDevice2;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorOpWolf;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorOpWolf;

				RunningFFB = "LightGunActive";
			}

			if (romname == undrfire || romname == undrfireu || romname == undrfirej)
			{
				configMinForce = configMinForceUFire;
				configMaxForce = configMaxForceUFire;
				configMinForceDevice2 = configMinForceUFireDevice2;
				configMaxForceDevice2 = configMaxForceUFireDevice2;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorUFire;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorUFire;

				RunningFFB = "LightGunActive";
			}

			if (romname == orunners || romname == orunnersu || romname == orunnersj)
			{
				configMinForce = configMinForceORunners;
				configMaxForce = configMaxForceORunners;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftORunners;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftORunners;
				configAlternativeMinForceRight = configAlternativeMinForceRightORunners;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightORunners;
				configMinForceDevice2 = configMinForceORunnersDevice2;
				configMaxForceDevice2 = configMaxForceORunnersDevice2;
				configAlternativeMinForceLeftDevice2 = configAlternativeMinForceLeftORunnersDevice2;
				configAlternativeMaxForceLeftDevice2 = configAlternativeMaxForceLeftORunnersDevice2;
				configAlternativeMinForceRightDevice2 = configAlternativeMinForceRightORunnersDevice2;
				configAlternativeMaxForceRightDevice2 = configAlternativeMaxForceRightORunnersDevice2;
				configFeedbackLength = configFeedbackLengthORunners;
				EnableForceSpringEffect = EnableForceSpringEffectORunners;
				ForceSpringStrength = ForceSpringStrengthORunners;
				SinePeriod = SinePeriodORunners;
				SineFadePeriod = SineFadePeriodORunners;
				SineStrength = SineStrengthORunners;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorORunners;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorORunners;

				RunningFFB = "RacingActive1";
			}

			if (romname == toutrun || romname == toutrund || romname == toutrunj || romname == toutrunjd)
			{
				configMinForce = configMinForceTOutrun;
				configMaxForce = configMaxForceTOutrun;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftTOutrun;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftTOutrun;
				configAlternativeMinForceRight = configAlternativeMinForceRightTOutrun;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightTOutrun;
				configFeedbackLength = configFeedbackLengthTOutrun;
				EnableForceSpringEffect = EnableForceSpringEffectTOutrun;
				ForceSpringStrength = ForceSpringStrengthTOutrun;
				SinePeriod = SinePeriodTOutrun;
				SineFadePeriod = SineFadePeriodTOutrun;
				SineStrength = SineStrengthTOutrun;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorTOutrun;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorTOutrun;

				RunningFFB = "RacingActive1";
			}

			if (romname == cbombers || romname == cbombersj || romname == cbombersp)
			{
				configMinForce = configMinForceCBombers;
				configMaxForce = configMaxForceCBombers;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftCBombers;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftCBombers;
				configAlternativeMinForceRight = configAlternativeMinForceRightCBombers;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightCBombers;
				configFeedbackLength = configFeedbackLengthCBombers;
				EnableForceSpringEffect = EnableForceSpringEffectCBombers;
				ForceSpringStrength = ForceSpringStrengthCBombers;
				SinePeriod = SinePeriodCBombers;
				SineFadePeriod = SineFadePeriodCBombers;
				SineStrength = SineStrengthCBombers;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorCBombers;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorCBombers;

				RunningFFB = "RacingActive1";
			}

			if (romname == outrunra || romname == outrun || romname == outruneh)
			{
				configMinForce = configMinForceOutrun;
				configMaxForce = configMaxForceOutrun;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftOutrun;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftOutrun;
				configAlternativeMinForceRight = configAlternativeMinForceRightOutrun;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightOutrun;
				configFeedbackLength = configFeedbackLengthOutrun;
				EnableForceSpringEffect = EnableForceSpringEffectOutrun;
				ForceSpringStrength = ForceSpringStrengthOutrun;
				SinePeriod = SinePeriodOutrun;
				SineFadePeriod = SineFadePeriodOutrun;
				SineStrength = SineStrengthOutrun;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorOutrun;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorOutrun;

				RunningFFB = "OutrunActive";
			}

			if (romname == pdrift || romname == pdrifta || romname == pdrifte || romname == pdriftj || romname == pdriftl)
			{
				configMinForce = configMinForcePDrift;
				configMaxForce = configMaxForcePDrift;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftPDrift;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftPDrift;
				configAlternativeMinForceRight = configAlternativeMinForceRightPDrift;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightPDrift;
				configFeedbackLength = configFeedbackLengthPDrift;
				EnableForceSpringEffect = EnableForceSpringEffectPDrift;
				ForceSpringStrength = ForceSpringStrengthPDrift;
				SinePeriod = SinePeriodPDrift;
				SineFadePeriod = SineFadePeriodPDrift;
				SineStrength = SineStrengthPDrift;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorPDrift;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorPDrift;

				RunningFFB = "PDriftActive";
			}

			if (romname == dblaxle || romname == dblaxleu)
			{
				configMinForce = configMinForceDAxle;
				configMaxForce = configMaxForceDAxle;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftDAxle;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftDAxle;
				configAlternativeMinForceRight = configAlternativeMinForceRightDAxle;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightDAxle;
				configFeedbackLength = configFeedbackLengthDAxle;
				EnableForceSpringEffect = EnableForceSpringEffectDAxle;
				ForceSpringStrength = ForceSpringStrengthDAxle;
				SinePeriod = SinePeriodDAxle;
				SineFadePeriod = SineFadePeriodDAxle;
				SineStrength = SineStrengthDAxle;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorDAxle;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorDAxle;

				RunningFFB = "RacingActive1";
			}

			if (romname == aburner2 || romname == aburner2g)
			{
				configMinForce = configMinForceAfterBurner2;
				configMaxForce = configMaxForceAfterBurner2;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorAfterBurner2;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorAfterBurner2;

				RunningFFB = "AfterburnerActive";
			}

			if (romname == cischeat)
			{
				configMinForce = configMinForceCisHeat;
				configMaxForce = configMaxForceCisHeat;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftCisHeat;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftCisHeat;
				configAlternativeMinForceRight = configAlternativeMinForceRightCisHeat;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightCisHeat;
				configFeedbackLength = configFeedbackLengthCisHeat;
				EnableForceSpringEffect = EnableForceSpringEffectCisHeat;
				ForceSpringStrength = ForceSpringStrengthCisHeat;
				SinePeriod = SinePeriodCisHeat;
				SineFadePeriod = SineFadePeriodCisHeat;
				SineStrength = SineStrengthCisHeat;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorCisHeat;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorCisHeat;

				RunningFFB = "RacingActive2";
			}

			if (romname == f1gpstar || romname == f1gpstaro)
			{
				configMinForce = configMinForceF1GpStar;
				configMaxForce = configMaxForceF1GpStar;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftF1GpStar;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftF1GpStar;
				configAlternativeMinForceRight = configAlternativeMinForceRightF1GpStar;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightF1GpStar;
				configFeedbackLength = configFeedbackLengthF1GpStar;
				EnableForceSpringEffect = EnableForceSpringEffectF1GpStar;
				ForceSpringStrength = ForceSpringStrengthF1GpStar;
				SinePeriod = SinePeriodF1GpStar;
				SineFadePeriod = SineFadePeriodF1GpStar;
				SineStrength = SineStrengthF1GpStar;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorF1GpStar;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorF1GpStar;

				RunningFFB = "RacingActive2";
			}

			if (romname == f1gpstr2)
			{
				configMinForce = configMinForceF1GpStar2;
				configMaxForce = configMaxForceF1GpStar2;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftF1GpStar2;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftF1GpStar2;
				configAlternativeMinForceRight = configAlternativeMinForceRightF1GpStar2;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightF1GpStar2;
				configFeedbackLength = configFeedbackLengthF1GpStar2;
				EnableForceSpringEffect = EnableForceSpringEffectF1GpStar2;
				ForceSpringStrength = ForceSpringStrengthF1GpStar2;
				SinePeriod = SinePeriodF1GpStar2;
				SineFadePeriod = SineFadePeriodF1GpStar2;
				SineStrength = SineStrengthF1GpStar2;
				RumbleStrengthLeftMotor = RumbleStrengthLeftMotorF1GpStar2;
				RumbleStrengthRightMotor = RumbleStrengthRightMotorF1GpStar2;

				RunningFFB = "RacingActive2";
			}

			if (romname == hyprdriv)
			{
				configMinForce = configMinForceHyperDrive;
				configMaxForce = configMaxForceHyperDrive;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftHyperDrive;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftHyperDrive;
				configAlternativeMinForceRight = configAlternativeMinForceRightHyperDrive;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightHyperDrive;
				configFeedbackLength = configFeedbackLengthHyperDrive;
				EnableForceSpringEffect = EnableForceSpringEffectHyperDrive;
				ForceSpringStrength = ForceSpringStrengthHyperDrive;
				EnableDamper = EnableDamperHyperDrive;
				DamperStrength = DamperStrengthHyperDrive;
				
				RunningFFB = "RacingFullValueActive1";
			}

			if (romname == vaportrx || romname == vaportrp)
			{
				configMinForce = configMinForceVaporTrx;
				configMaxForce = configMaxForceVaporTrx;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftVaporTrx;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftVaporTrx;
				configAlternativeMinForceRight = configAlternativeMinForceRightVaporTrx;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightVaporTrx;
				configFeedbackLength = configFeedbackLengthVaporTrx;
				EnableForceSpringEffect = EnableForceSpringEffectVaporTrx;
				ForceSpringStrength = ForceSpringStrengthVaporTrx;
				EnableDamper = EnableDamperVaporTrx;
				DamperStrength = DamperStrengthVaporTrx;
				
				RunningFFB = "RacingFullValueActive1";
			}

			if (romname == raveracw || romname == raveracj || romname == raveracja || romname == raveracej || romname == raveraceja || romname == raverace)
			{
				configMinForce = configMinForceRaveRacer;
				configMaxForce = configMaxForceRaveRacer;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftRaveRacer;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftRaveRacer;
				configAlternativeMinForceRight = configAlternativeMinForceRightRaveRacer;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightRaveRacer;
				configFeedbackLength = configFeedbackLengthRaveRacer;
				PowerMode = PowerModeRaveRacer;
				EnableForceSpringEffect = EnableForceSpringEffectRaveRacer;
				ForceSpringStrength = ForceSpringStrengthRaveRacer;
				EnableDamper = EnableDamperRaveRacer;
				DamperStrength = DamperStrengthRaveRacer;

				RunningFFB = "RaveRacerNew";
			}

			if (romname == daytona || romname == daytonas || romname == daytonase)
			{
				configMinForce = configMinForceDaytonaMAME;
				configMaxForce = configMaxForceDaytonaMAME;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftDaytonaMAME;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftDaytonaMAME;
				configAlternativeMinForceRight = configAlternativeMinForceRightDaytonaMAME;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightDaytonaMAME;
				configFeedbackLength = configFeedbackLengthDaytonaMAME;
				PowerMode = PowerModeDaytonaMAME;
				EnableForceSpringEffect = EnableForceSpringEffectDaytonaMAME;
				ForceSpringStrength = ForceSpringStrengthDaytonaMAME;
				EnableDamper = EnableDamperDaytonaMAME;
				DamperStrength = DamperStrengthDaytonaMAME;

				RunningFFB = "m2new";
			}

			if (romname == indy500 || romname == indy500d || romname == indy500to)
			{
				configMinForce = configMinForceIndy500MAME;
				configMaxForce = configMaxForceIndy500MAME;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftIndy500MAME;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftIndy500MAME;
				configAlternativeMinForceRight = configAlternativeMinForceRightIndy500MAME;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightIndy500MAME;
				configFeedbackLength = configFeedbackLengthIndy500MAME;
				PowerMode = PowerModeIndy500MAME;
				EnableForceSpringEffect = EnableForceSpringEffectIndy500MAME;
				ForceSpringStrength = ForceSpringStrengthIndy500MAME;
				EnableDamper = EnableDamperIndy500MAME;
				DamperStrength = DamperStrengthIndy500MAME;

				RunningFFB = "m2new";
			}

			if (romname == sgt24h)
			{
				configMinForce = configMinForceSuperGTMAME;
				configMaxForce = configMaxForceSuperGTMAME;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftSuperGTMAME;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftSuperGTMAME;
				configAlternativeMinForceRight = configAlternativeMinForceRightSuperGTMAME;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightSuperGTMAME;
				configFeedbackLength = configFeedbackLengthSuperGTMAME;
				PowerMode = PowerModeSuperGTMAME;
				EnableForceSpringEffect = EnableForceSpringEffectSuperGTMAME;
				ForceSpringStrength = ForceSpringStrengthSuperGTMAME;
				EnableDamper = EnableDamperSuperGTMAME;
				DamperStrength = DamperStrengthSuperGTMAME;

				RunningFFB = "m2new";
			}

			if (romname == srallyc || romname == srallycb || romname == srallycdx || romname == srallycdxa)
			{
				configMinForce = configMinForceSRallyMAME;
				configMaxForce = configMaxForceSRallyMAME;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftSRallyMAME;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftSRallyMAME;
				configAlternativeMinForceRight = configAlternativeMinForceRightSRallyMAME;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightSRallyMAME;
				configFeedbackLength = configFeedbackLengthSRallyMAME;
				PowerMode = PowerModeSRallyMAME;
				EnableForceSpringEffect = EnableForceSpringEffectSRallyMAME;
				ForceSpringStrength = ForceSpringStrengthSRallyMAME;
				EnableDamper = EnableDamperSRallyMAME;
				DamperStrength = DamperStrengthSRallyMAME;

				RunningFFB = "srallynew";
			}

			if (romname == dirtdash || romname == dirtdasha || romname == dirtdashj)
			{
				configMinForce = configMinForceDirtDash;
				configMaxForce = configMaxForceDirtDash;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftDirtDash;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftDirtDash;
				configAlternativeMinForceRight = configAlternativeMinForceRightDirtDash;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightDirtDash;
				configFeedbackLength = configFeedbackLengthDirtDash;
				PowerMode = PowerModeDirtDash;
				EnableForceSpringEffect = EnableForceSpringEffectDirtDash;
				ForceSpringStrength = ForceSpringStrengthDirtDash;
				Divide = FFBDivideDirtDash * 1.0;
				EnableDamper = EnableDamperDirtDash;
				DamperStrength = DamperStrengthDirtDash;

				RunningFFB = "DirtDashFFB";
			}

			if (romname == stcc || romname == stcca || romname == stccb)
			{
				configMinForce = configMinForceSTCCMAME;
				configMaxForce = configMaxForceSTCCMAME;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftSTCCMAME;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftSTCCMAME;
				configAlternativeMinForceRight = configAlternativeMinForceRightSTCCMAME;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightSTCCMAME;
				configFeedbackLength = configFeedbackLengthSTCCMAME;
				PowerMode = PowerModeSTCCMAME;
				EnableForceSpringEffect = EnableForceSpringEffectSTCCMAME;
				ForceSpringStrength = ForceSpringStrengthSTCCMAME;
				EnableDamper = EnableDamperSTCCMAME;
				DamperStrength = DamperStrengthSTCCMAME;

				RunningFFB = "m2new";
			}

			if (romname == victlapw || romname == victlap)
			{
				configMinForce = configMinForceAceDriverVictory;
				configMaxForce = configMaxForceAceDriverVictory;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftAceDriverVictory;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftAceDriverVictory;
				configAlternativeMinForceRight = configAlternativeMinForceRightAceDriverVictory;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightAceDriverVictory;
				configFeedbackLength = configFeedbackLengthAceDriverVictory;
				PowerMode = PowerModeAceDriverVictory;
				EnableForceSpringEffect = EnableForceSpringEffectAceDriverVictory;
				ForceSpringStrength = ForceSpringStrengthAceDriverVictory;
				Divide = FFBDivideAceDriverVictory * 1.0;
				EnableDamper = EnableDamperAceDriverVictory;
				DamperStrength = DamperStrengthAceDriverVictory;

				RunningFFB = "NamcoFFBNew";
			}

			if (romname == acedrvrw || romname == acedrive)
			{
				configMinForce = configMinForceAceDriver;
				configMaxForce = configMaxForceAceDriver;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftAceDriver;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftAceDriver;
				configAlternativeMinForceRight = configAlternativeMinForceRightAceDriver;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightAceDriver;
				configFeedbackLength = configFeedbackLengthAceDriver;
				PowerMode = PowerModeAceDriver;
				EnableForceSpringEffect = EnableForceSpringEffectAceDriver;
				ForceSpringStrength = ForceSpringStrengthAceDriver;
				Divide = FFBDivideAceDriver * 1.0;
				EnableDamper = EnableDamperAceDriver;
				DamperStrength = DamperStrengthAceDriver;

				RunningFFB = "NamcoFFBNew";
			}

			if (romname == sidebs)
			{
				configMinForce = configMinForceSideBS;
				configMaxForce = configMaxForceSideBS;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftSideBS;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftSideBS;
				configAlternativeMinForceRight = configAlternativeMinForceRightSideBS;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightSideBS;
				configFeedbackLength = configFeedbackLengthSideBS;
				PowerMode = PowerModeSideBS;
				EnableForceSpringEffect = EnableForceSpringEffectSideBS;
				ForceSpringStrength = ForceSpringStrengthSideBS;
				EnableDamper = EnableDamperSideBS;
				DamperStrength = DamperStrengthSideBS;

				RunningFFB = "SideBSActive";
			}

			if (romname == sidebs2 || romname == sidebs2j || romname == sidebs2ja)
			{
				configMinForce = configMinForceSideBS2;
				configMaxForce = configMaxForceSideBS2;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftSideBS2;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftSideBS2;
				configAlternativeMinForceRight = configAlternativeMinForceRightSideBS2;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightSideBS2;
				configFeedbackLength = configFeedbackLengthSideBS2;
				PowerMode = PowerModeSideBS2;
				EnableForceSpringEffect = EnableForceSpringEffectSideBS2;
				ForceSpringStrength = ForceSpringStrengthSideBS2;
				EnableDamper = EnableDamperSideBS2;
				DamperStrength = DamperStrengthSideBS2;

				RunningFFB = "SideBSActive";
			}

			if (romname == drivedge)
			{
				configMinForce = configMinForceDriversEdge;
				configMaxForce = configMaxForceDriversEdge;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftDriversEdge;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftDriversEdge;
				configAlternativeMinForceRight = configAlternativeMinForceRightDriversEdge;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightDriversEdge;
				configFeedbackLength = configFeedbackLengthDriversEdge;
				PowerMode = PowerModeDriversEdge;
				EnableForceSpringEffect = EnableForceSpringEffectDriversEdge;
				ForceSpringStrength = ForceSpringStrengthDriversEdge;
				EnableDamper = EnableDamperDriversEdge;
				DamperStrength = DamperStrengthDriversEdge;

				RunningFFB = "DriversEdgeActive";
			}

			if (romname == speedup || romname == speedup10 || romname == speedup12 || romname == speedup20 || romname == speedup20a || romname == speedup21)
			{
				configMinForce = configMinForceSpeedUp;
				configMaxForce = configMaxForceSpeedUp;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftSpeedUp;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftSpeedUp;
				configAlternativeMinForceRight = configAlternativeMinForceRightSpeedUp;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightSpeedUp;
				configFeedbackLength = configFeedbackLengthSpeedUp;
				PowerMode = PowerModeSpeedUp;
				EnableForceSpringEffect = EnableForceSpringEffectSpeedUp;
				ForceSpringStrength = ForceSpringStrengthSpeedUp;
				EnableDamper = EnableDamperSpeedUp;
				DamperStrength = DamperStrengthSpeedUp;

				RunningFFB = "RacingFullValueActive1";
			}

			if (romname == InitialDVer1 || romname == InitialDVer2 || romname == InitialDVer3)
			{
				configMinForce = configMinForceInitialD;
				configMaxForce = configMaxForceInitialD;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftInitialD;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftInitialD;
				configAlternativeMinForceRight = configAlternativeMinForceRightInitialD;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightInitialD;
				configFeedbackLength = configFeedbackLengthInitialD;
				PowerMode = PowerModeInitialD;
				EnableDamper = EnableDamperInitialD;
				DamperStrength = DamperStrengthInitialD;
				EnableForceSpringEffect = EnableForceSpringEffectInitialD;
				ForceSpringStrength = ForceSpringStrengthInitialD;
				DoubleSine = DoubleSineInitialD;
				DoubleConstant = DoubleConstantInitialD;

				RunningFFB = "InitialDActive";
			}

			if (romname == TheKingOfRoute66)
			{
				configMinForce = configMinForceKingOfRoute66;
				configMaxForce = configMaxForceKingOfRoute66;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftKingOfRoute66;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftKingOfRoute66;
				configAlternativeMinForceRight = configAlternativeMinForceRightKingOfRoute66;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightKingOfRoute66;
				configFeedbackLength = configFeedbackLengthKingOfRoute66;
				PowerMode = PowerModeKingOfRoute66;
				EnableDamper = EnableDamperKingOfRoute66;
				DamperStrength = DamperStrengthKingOfRoute66;
				EnableForceSpringEffect = EnableForceSpringEffectKingOfRoute66;
				ForceSpringStrength = ForceSpringStrengthKingOfRoute66;
				DoubleSine = DoubleSineKingOfRoute66;
				DoubleConstant = DoubleConstantKingOfRoute66;

				RunningFFB = "InitialDActive";
			}

			if (romname == ClubKartInJapan)
			{
				configMinForce = configMinForceClubKart;
				configMaxForce = configMaxForceClubKart;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftClubKart;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftClubKart;
				configAlternativeMinForceRight = configAlternativeMinForceRightClubKart;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightClubKart;
				configFeedbackLength = configFeedbackLengthClubKart;
				PowerMode = PowerModeClubKart;
				EnableDamper = EnableDamperClubKart;
				DamperStrength = DamperStrengthClubKart;
				EnableForceSpringEffect = EnableForceSpringEffectClubKart;
				ForceSpringStrength = ForceSpringStrengthClubKart;
				DoubleSine = DoubleSineClubKart;
				DoubleConstant = DoubleConstantClubKart;

				RunningFFB = "InitialDActive";
			}

			if (romname == SegaDrivingSimulator)
			{
				configMinForce = configMinForceSegaDrivingSimulator;
				configMaxForce = configMaxForceSegaDrivingSimulator;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftSegaDrivingSimulator;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftSegaDrivingSimulator;
				configAlternativeMinForceRight = configAlternativeMinForceRightSegaDrivingSimulator;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightSegaDrivingSimulator;
				configFeedbackLength = configFeedbackLengthSegaDrivingSimulator;
				PowerMode = PowerModeSegaDrivingSimulator;
				EnableDamper = EnableDamperSegaDrivingSimulator;
				DamperStrength = DamperStrengthSegaDrivingSimulator;
				EnableForceSpringEffect = EnableForceSpringEffectSegaDrivingSimulator;
				ForceSpringStrength = ForceSpringStrengthSegaDrivingSimulator;
				DoubleSine = DoubleSineSegaDrivingSimulator;
				DoubleConstant = DoubleConstantSegaDrivingSimulator;

				RunningFFB = "InitialDActive";
			}

			if (romname == MaximumSPEED)
			{
				configMinForce = configMinForceMaximumSpeed;
				configMaxForce = configMaxForceMaximumSpeed;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftMaximumSpeed;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftMaximumSpeed;
				configAlternativeMinForceRight = configAlternativeMinForceRightMaximumSpeed;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightMaximumSpeed;
				configFeedbackLength = configFeedbackLengthMaximumSpeed;
				PowerMode = PowerModeMaximumSpeed;
				EnableDamper = EnableDamperMaximumSpeed;
				DamperStrength = DamperStrengthMaximumSpeed;
				EnableForceSpringEffect = EnableForceSpringEffectMaximumSpeed;
				ForceSpringStrength = ForceSpringStrengthMaximumSpeed;

				RunningFFB = "MaximumSpeedActive";
			}

			if (romname == FasterThanSPEED)
			{
				configMinForce = configMinForceFasterThanSpeed;
				configMaxForce = configMaxForceFasterThanSpeed;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftFasterThanSpeed;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftFasterThanSpeed;
				configAlternativeMinForceRight = configAlternativeMinForceRightFasterThanSpeed;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightFasterThanSpeed;
				configFeedbackLength = configFeedbackLengthFasterThanSpeed;
				PowerMode = PowerModeFasterThanSpeed;
				EnableDamper = EnableDamperFasterThanSpeed;
				DamperStrength = DamperStrengthFasterThanSpeed;
				EnableForceSpringEffect = EnableForceSpringEffectFasterThanSpeed;
				ForceSpringStrength = ForceSpringStrengthFasterThanSpeed;

				RunningFFB = "MaximumSpeedActive";
			}

			if (romname == F355ChallengeJapan)
			{
				configMinForce = configMinForceF355;
				configMaxForce = configMaxForceF355;
				configAlternativeMinForceLeft = configAlternativeMinForceLeftF355;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeftF355;
				configAlternativeMinForceRight = configAlternativeMinForceRightF355;
				configAlternativeMaxForceRight = configAlternativeMaxForceRightF355;
				configFeedbackLength = configFeedbackLengthF355;
				PowerMode = PowerModeF355;
				EnableDamper = EnableDamperF355;
				DamperStrength = DamperStrengthF355;
				EnableForceSpringEffect = EnableForceSpringEffectF355;
				ForceSpringStrength = ForceSpringStrengthF355;

				RunningFFB = "NaomiFFBActive";
				NaomiFFBGo = true;
			}

			if (romname == EighteenWheeler)
			{
				configMinForce = configMinForce18Wheeler;
				configMaxForce = configMaxForce18Wheeler;
				configAlternativeMinForceLeft = configAlternativeMinForceLeft18Wheeler;
				configAlternativeMaxForceLeft = configAlternativeMaxForceLeft18Wheeler;
				configAlternativeMinForceRight = configAlternativeMinForceRight18Wheeler;
				configAlternativeMaxForceRight = configAlternativeMaxForceRight18Wheeler;
				configFeedbackLength = configFeedbackLength18Wheeler;
				PowerMode = PowerMode18Wheeler;
				EnableDamper = EnableDamper18Wheeler;
				DamperStrength = DamperStrength18Wheeler;
				EnableForceSpringEffect = EnableForceSpringEffect18Wheeler;
				ForceSpringStrength = ForceSpringStrength18Wheeler;

				RunningFFB = "NaomiFFBActive";
				NaomiFFBGo = true;
			}

			if (enableLogging)
			{
				char romnametext[256];
				sprintf(romnametext, "RomName = %s", romname);
				helpers->log(romnametext);

				char RunningFFBtext[256];
				sprintf(RunningFFBtext, "RunningFFB = %s", RunningFFB);
				helpers->log(RunningFFBtext);
			}

			if ((RunningFFB != NULL) && (RunningFFB[0] != '\0'))
				RomGameName = true;
		}
	}

	if (RomGameName && RunningFFB > 0)
	{

		if (RunningFFB > 0 && EnableDamper)
			triggers->Damper(DamperStrength / 100.0);

		if (RunningFFB > 0 && DriveboardSequenceRunning > 0) {
			// MAME or Supermodel has an active sequenceplaying
			CurrentTime_ms += 16;

			if ((DriveboardSequence_starttime_ms - CurrentTime_ms) > DriveboardSequence_duration_ms) {
				helpers->log("sequence done ");
				std::string ffs = std::to_string(DriveboardSequenceRunning);
				helpers->log((char*)ffs.c_str());
				
				// Sequence done, stop it
				DriveboardSequenceRunning = 0;
			}

			// Now depending on the game, play a sequence effect each 16ms
			if (RunningFFB == ScudRaceActive) {
				switch (DriveboardSequenceRunning) {
				case 0:
					// No sequence
					break;
				case 1: {
						// Sequence: light random direction shocks (very short impulse of torque) in the steering wheel, like driving on gravel
						double intensity = 0.2;
						const int frequency = 200;
						bool randShock = (rand() % frequency) == 0;
						if (randShock) {
							bool direction = (rand() % 2) == 0;
							if (direction) {
								triggers->Constant(constants->DIRECTION_FROM_LEFT, intensity);
							}
							else {
								triggers->Constant(constants->DIRECTION_FROM_RIGHT, intensity);
							}
						}
					}
					break;
				case 2: {
						// Sequence: light random direction shocks (very short impulse of torque) in the steering wheel, like driving on gravel
						double intensity = 0.5;
						const int frequency = 200;
						bool randShock = (rand() % frequency) == 0;
						if (randShock) {
							bool direction = (rand() % 2) == 0;
							if (direction) {
								triggers->Constant(constants->DIRECTION_FROM_LEFT, intensity);
							}
							else {
								triggers->Constant(constants->DIRECTION_FROM_RIGHT, intensity);
							}
						}
					}
					break;
				}
			}
			
			if (RunningFFB == Daytona2Active) {
				// No yet done
			}

		}

		if (RunningFFB == SuperMonacoGPActive)
		{
			// Upright shaker speed: lamp2 = 1, lamp0 = 2, lamp1 = 4.
			const int rumbleLevel = smgpLamp2 | (smgpLamp0 << 1) | (smgpLamp1 << 2);

			if (rumbleLevel > 0)
			{
				// The original upright has seven motor speed stages. Keep the
				// configured SinePeriod as the stage-1 base and progressively
				// shorten the period as the motor stage rises.
				static const double periodScale[8] =
				{
					0.0, 1.00, 0.80, 0.65, 0.52, 0.42, 0.34, 0.28
				};

				int stagePeriod = static_cast<int>((SinePeriod * periodScale[rumbleLevel]) + 0.5);
				if (stagePeriod < 1)
					stagePeriod = 1;

				const double levelStrength = (SineStrength / 100.0) * (rumbleLevel / 7.0);
				triggers->Sine(static_cast<UINT16>(stagePeriod), SineFadePeriod, levelStrength, stagePeriod);
			}
			else
			{
				triggers->Sine(0, 0, 0.0, 0);
			}
		}

		if (RunningFFB == LightGunActive) //LightGun Games
		{
			if (name == Player1_Recoil_Piston || name == Player1_Gun_Recoil || name == Left_Gun_Recoil || name == P1_Gun_Recoil || name == mcuout1 || name == P1_gun_recoil)
			{
				helpers->log("P1 value: ");
				std::string ffs = std::to_string(newstateFFB);
				helpers->log((char*)ffs.c_str());

				if (newstateFFB == 0)
				{
					Effect1 = false;
				}

				stateFFB = newstateFFB;
			}

			if (name == Player2_Recoil_Piston || name == Player2_Gun_Recoil || name == Right_Gun_Recoil || name == P2_Gun_Recoil || name == P2_gun_recoil)
			{
				helpers->log("P2 value: ");
				std::string ffs = std::to_string(newstateFFB);
				helpers->log((char*)ffs.c_str());

				if (newstateFFB == 0)
				{
					Effect2 = false;
				}

				stateFFBDevice2 = newstateFFB;
			}

			if (name == Player3_Gun_Recoil || name == P3_Gun_Recoil)
			{
				helpers->log("P3 value: ");
				std::string ffs = std::to_string(newstateFFB);
				helpers->log((char*)ffs.c_str());

				if (newstateFFB == 0)
				{
					Effect3 = false;
				}

				stateFFBDevice3 = newstateFFB;
			}

			if (stateFFB == 0x01)
			{
				Effect1 = true;
			}
			else
			{
				Effect1 = false;
			}

			if (stateFFBDevice2 == 0x01)
			{
				Effect2 = true;
			}
			else
			{
				Effect2 = false;
			}

			if (stateFFBDevice3 == 0x01)
			{
				Effect3 = true;
			}
			else
			{
				Effect3 = false;
			}

			if (Effect1)
			{
				triggers->Rumble(RumbleStrengthLeftMotor / 100.0, RumbleStrengthRightMotor / 100.0, 100);
			}

			if (!Effect1)
			{
				triggers->Rumble(0, 0, 0);
			}

			if (Effect2)
			{
				triggers->RumbleDevice2(RumbleStrengthLeftMotor / 100.0, RumbleStrengthRightMotor / 100.0, 100);
			}

			if (!Effect2)
			{
				triggers->RumbleDevice2(0, 0, 0);
			}

			if (Effect3)
			{
				triggers->RumbleDevice3(RumbleStrengthLeftMotor / 100.0, RumbleStrengthRightMotor / 100.0, 100);
			}

			if (!Effect3)
			{
				triggers->RumbleDevice3(0, 0, 0);
			}
		}

		if (RunningFFB == RacingActive1) //Outrunners,Turbo Outrun,CBombers,DAxle
		{
			if (name == MA_Steering_Wheel_motor || name == mainpcb_MA_Steering_Wheel_motor || name == upright_wheel_motor || name == Vibration_motor || name == Wheel_vibration || name == Wheel_Vibration || name == genout2)
			{
				helpers->log("P1 value: ");
				std::string ffs = std::to_string(newstateFFB);
				helpers->log((char*)ffs.c_str());

				if (newstateFFB == 0)
				{
					Effect1 = false;
				}

				stateFFB = newstateFFB;
			}

			if (name == MB_Steering_Wheel_motor || name == mainpcb_MB_Steering_Wheel_motor)
			{
				helpers->log("P2 value: ");
				std::string ffs = std::to_string(newstateFFB);
				helpers->log((char*)ffs.c_str());

				if (newstateFFB == 0)
				{
					Effect2 = false;
				}

				stateFFBDevice2 = newstateFFB;
			}

			if (stateFFB == 0x01)
			{
				Effect1 = true;
			}
			else
			{
				Effect1 = false;
			}

			if (stateFFBDevice2 == 0x01)
			{
				Effect2 = true;
			}
			else
			{
				Effect2 = false;
			}

			if (Effect1)
			{
				triggers->Sine(SinePeriod, SineFadePeriod, SineStrength / 100.0, 100);
				triggers->Rumble(RumbleStrengthLeftMotor / 100.0, RumbleStrengthRightMotor / 100.0, 100);
			}

			if (!Effect1)
			{
				triggers->Sine(0, 0, 0, 0);
				triggers->Rumble(0, 0, 0);
			}

			if (Effect2)
			{
				triggers->SineDevice2(SinePeriod, SineFadePeriod, SineStrength / 100.0, 100);
				triggers->RumbleDevice2(RumbleStrengthLeftMotor / 100.0, RumbleStrengthRightMotor / 100.0, 100);
			}

			if (!Effect2)
			{
				triggers->SineDevice2(0, 0, 0, 0);
				triggers->RumbleDevice2(0, 0, 0);
			}
		}

		if (RunningFFB == RacingActive2) //Cisco Heat,F1 GpStar //Keep seperate as names is led2 and could possibly interfere with other outputs
		{
			if (name == led2)
			{
				helpers->log("P1 value: ");
				std::string ffs = std::to_string(newstateFFB);
				helpers->log((char*)ffs.c_str());

				if (newstateFFB == 0)
				{
					Effect1 = false;
				}

				stateFFB = newstateFFB;
			}

			if (stateFFB == 0x01)
			{
				Effect1 = true;
			}
			else
			{
				Effect1 = false;
			}

			if (Effect1)
			{
				triggers->Sine(SinePeriod, SineFadePeriod, SineStrength / 100.0, 100);
				triggers->Rumble(RumbleStrengthLeftMotor / 100.0, RumbleStrengthRightMotor / 100.0, 100);
			}

			if (!Effect1)
			{
				triggers->Sine(0, 0, 0, 0);
				triggers->Rumble(0, 0, 0);
			}
		}

		if (RunningFFB == AfterburnerActive) //Afterburner
		{
			if (name == lamp1)
			{
				helpers->log("P1 value: ");
				std::string ffs = std::to_string(newstateFFB);
				helpers->log((char*)ffs.c_str());

				if (newstateFFB == 0)
				{
					Effect1 = false;
				}

				stateFFB = newstateFFB;
			}

			if (stateFFB == 0x01)
			{
				Effect1 = true;
			}
			else
			{
				Effect1 = false;
			}

			if (Effect1)
			{
				triggers->Rumble(RumbleStrengthLeftMotor / 100.0, RumbleStrengthRightMotor / 100.0, 100);
			}

			if (!Effect1)
			{
				triggers->Rumble(0, 0, 0);
			}
		}

		if (RunningFFB == OutrunActive)
		{
			if (name == Bank_Motor_Direction)
			{
				helpers->log("got value: ");
				std::string ffs = std::to_string(newstateFFB);
				helpers->log((char*)ffs.c_str());

				stateFFB = newstateFFB;
			}

			if (name == Vibration_motor)
			{
				helpers->log("got value: ");
				std::string ffs = std::to_string(newstateFFB);
				helpers->log((char*)ffs.c_str());
				if (newstateFFB > 0)
				{

				}

				if (newstateFFB == 0)
				{
					Effect1 = false;
				}

				stateFFB2 = newstateFFB;
			}

			if (name == Bank_Motor_Speed)
			{
				stateFFB3 = newstateFFB;
			}

			if (Motion)
			{
				if (stateFFB == 0x01)
				{
					double percentForce = (1 + stateFFB3) / 8.0;
					triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
				}
				if (stateFFB == 0x02)
				{
					double percentForce = (1 + stateFFB3) / 8.0;
					triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
				}
			}

			if (stateFFB == 1)
			{
				Motion = true;
				MotionFalse = false;
			}

			if (stateFFB2 == 1)
			{
				Motion = false;
			}

			if (StopConstant == 255)
			{
				MotionFalse = true;
			}

			if (MotionFalse)
			{
				triggers->Constant(constants->DIRECTION_FROM_LEFT, 0);
				triggers->Constant(constants->DIRECTION_FROM_RIGHT, 0);
				Motion = false;
				StopConstant = 0;
				MotionFalse = false;
			}

			if (!Motion)
			{

				if (stateFFB2 == 0x01)
				{
					Effect1 = true;
				}
				else
				{
					Effect1 = false;
				}

				if (Effect1)
				{
					triggers->Sine(SinePeriod, SineFadePeriod, SineStrength / 100.0, 100);
					triggers->Rumble(RumbleStrengthLeftMotor / 100.0, RumbleStrengthRightMotor / 100.0, 100);
				}

				if (!Effect1)
				{
					triggers->Sine(0, 0, 0, 0);
					triggers->Rumble(0, 0, 0);
				}
			}
		}

		if (RunningFFB == PDriftActive)
		{
			if (name == bank_motor_position)
			{
				helpers->log("got value: ");
				std::string ffs = std::to_string(newstateFFB);
				helpers->log((char*)ffs.c_str());

				stateFFB = newstateFFB;
			}

			if (name == vibration_motor)
			{
				stateFFB2 = newstateFFB;
			}

			if (name == upright_wheel_motor)
			{
				helpers->log("got value: ");
				std::string ffs = std::to_string(newstateFFB);
				helpers->log((char*)ffs.c_str());

				if (newstateFFB == 0)
				{
					Effect1 = false;
				}

				stateFFB3 = newstateFFB;
			}

			if (Motion)
			{
				if ((stateFFB > 0x00) && (stateFFB < 0x04))
				{
					double percentForce = (4 - stateFFB) / 3.0;
					triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
				}
				else if ((stateFFB > 0x04) && (stateFFB < 0x08))
				{
					double percentForce = (stateFFB - 4) / 3.0;
					triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
				}

				if (stateFFB2 >= 0)
				{
					double percentForce = stateFFB2 / 7.0;
					triggers->Sine(60, 0, percentForce / 3.0, 100);
				}
			}

			if (stateFFB == 4)
			{
				Motion = true;
				MotionFalse = false;
			}

			if (stateFFB3 == 1)
			{
				Motion = false;
			}

			if (StopConstant == 255)
			{
				MotionFalse = true;
			}

			if (MotionFalse)
			{
				triggers->Constant(constants->DIRECTION_FROM_LEFT, 0);
				triggers->Constant(constants->DIRECTION_FROM_RIGHT, 0);
				Motion = false;
				StopConstant = 0;
				MotionFalse = false;
			}

			if (!Motion)
			{
				if (stateFFB3 == 0x01)
				{
					Effect1 = true;
				}
				else
				{
					Effect1 = false;
				}

				if (Effect1)
				{
					triggers->Sine(SinePeriod, SineFadePeriod, SineStrength / 100.0, 100);
					triggers->Rumble(RumbleStrengthLeftMotor / 100.0, RumbleStrengthRightMotor / 100.0, 100);
				}

				if (!Effect1)
				{
					triggers->Sine(0, 0, 0, 0);
					triggers->Rumble(0, 0, 0);
				}
			}
		}

		if (RunningFFB == SuperChaseActive)
		{
			if (!PatternFind)
			{
				if (!PatternLaunch)
				{
					Sleep(8000);
					PatternLaunch = true;
				}

				if (!Scan)
				{
					CreateThread(NULL, 0, ScanThread, NULL, 0, NULL);
					Scan = true;
				}

				UINT8 CheckAddy2 = helpers->ReadByte((INT_PTR)aAddy2 + 0x12, false);
				if (CheckAddy2 == 0x5A)
				{
					FFBAddress = (INT_PTR)aAddy2 + 0x6C8;
					PatternFind = true;
				}
			}
			else
			{
				ff = helpers->ReadByte(FFBAddress, false);

				helpers->log("got value: ");
				std::string ffs = std::to_string(ff);
				helpers->log((char*)ffs.c_str());

				UINT32 length_ms = 100;
				if (ff & 0x01)
				{
					double percentForce = 1.0;
					triggers->Rumble(percentForce, 0, length_ms);
					triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
				}

				if (ff & 0x02)
				{
					double percentForce = 1.0;
					triggers->Rumble(0, percentForce, length_ms);
					triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
				}
			}
		}
		
		if (RunningFFB == DirtDashFFB)
		{
			// mcuout1 switches the wheel motor off (0) and on (1). The motor is assumed on
			// until MAME sends the output, so that the wheel is not dead at start.
			static int mcuout1State = 1;

			if (name == mcuout1)
			{
				mcuout1State = newstateFFB;

				std::string debugMsg = "DirtDashFFB: mcuout1 switched to " + std::to_string(mcuout1State);
				helpers->log((char*)debugMsg.c_str());
			}

			if (!PatternFind)
			{
				if (!PatternLaunch)
				{
					if (name == cpuled6)
					{
						if (newstateFFB == 1)
						{
							Sleep(2000);
							PatternLaunch = true;
						}
					}
				}
				else
				{
					if (romname == dirtdash || romname == dirtdasha || romname == dirtdashj)
					{
						if (!Scan)
						{
							CreateThread(NULL, 0, ScanThread, NULL, 0, NULL);
							Scan = true;
						}

						UINT8 CheckAddy2 = helpers->ReadByte((INT_PTR)aAddy2 - 0x3E3, false);
						if (CheckAddy2 == 0x80)
						{
							FFBAddress = (INT_PTR)aAddy2 - 0x2C6;
							PatternFind = true;
						}
					}
				}
			}
			else
			{
				DWORD FFBNamco = helpers->ReadInt32(FFBAddress, false);

				helpers->log("got value: ");
				std::string ffs = std::to_string(FFBNamco);
				helpers->log((char*)ffs.c_str());

				UINT32 length_ms = 100;
				if (mcuout1State == 0) // Wheel motor switched off
				{
					triggers->Rumble(0, 0, 0);
					StopConstantForce(triggers, constants);
				}
				else if ((FFBNamco >= 0x00) && (FFBNamco < 0x77A))
				{
					double percentForce = (FFBNamco / Divide);
					if (percentForce > 1.0)
					{
						percentForce = 1.0;
					}
					triggers->Rumble(0, percentForce, length_ms);
					SendConstant(triggers, constants->DIRECTION_FROM_RIGHT, percentForce);
				}
				else if ((FFBNamco > 0xF886) && (FFBNamco < 0x10000))
				{
					double percentForce = ((65536 - FFBNamco) / Divide);
					if (percentForce > 1.0)
					{
						percentForce = 1.0;
					}
					triggers->Rumble(percentForce, 0, length_ms);
					SendConstant(triggers, constants->DIRECTION_FROM_LEFT, percentForce);
				}
				else
				{
					triggers->Rumble(0, 0, 0);
					StopConstantForce(triggers, constants);
				}
			}
		}
	}


}