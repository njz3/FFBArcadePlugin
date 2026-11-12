# FFB Arcade Plugin

Brought to you by Boomslangnz, MightyMike, Ducon2016, Spazzy, Adam_, jpb, Rocco & pinkimo, with the MAME racing
games of Endprodukt and the njz3 fork's changes.

A plugin that gives force feedback and rumble to arcade games: it reads what the game or the emulator sends to its
wheel motor, lamps or recoil, and plays it on your wheel or controller through SDL.

The version is in the title of FFBPluginGUI.exe, in the file properties of dinput8.dll and FFBPluginGUI.exe, and at
the top of FFBlog.txt when `Logging=1`. What changed: CHANGES.md in the source repository
(https://github.com/njz3/FFBArcadePlugin, branch `dev/mame-racer`).

## Contents

- [Requirements](#requirements)
- [How to use](#how-to-use)
- [Settings of FFBPlugin.ini](#settings-of-ffbpluginini)
- [Emulator setup](#emulator-setup)
- [Supported games](#supported-games)
- [Credits](#credits)

## Requirements

- The plugin (dinput8.dll) needs only SDL2.dll and Windows DLLs.
- The GUI (FFBPluginGUI.exe) needs the .NET Framework 4.7.2 and the Visual C++ 2015-2022 redistributable
  (https://www.techpowerup.com/download/visual-c-redistributable-runtime-package-all-in-one/). It must be in the
  same folder as MetroFramework*.dll and SDL2.dll.

## How to use

1. Copy the plugin's files to the game's or the emulator's folder (for MAME, next to mame.exe).
2. Launch FFBPluginGUI.exe from that folder. Choose your wheel at the top of the game's page, then set the
   strengths. The GUI writes them to FFBPlugin.ini, which the plugin reads when the game starts.
3. The Setup Help section of the GUI says which files go where for each game.

The wheel is found by its GUID (`DeviceGUID`). A GUID saved with another version of SDL2 still matches (since 2.1.0).

## Settings of FFBPlugin.ini

- **AlternativeFFB**: for wheels whose rotation always goes one way only (some Thrustmaster wheels, PWM2M2...). With
  it on, use the AlternativeFFB MinForce/MaxForce settings instead of MinForce/MaxForce.
- **MinForce / MaxForce**: the lowest and the highest strength of the wheel.
- **FeedbackLength**: how long each force feedback effect lasts.
- **EnableRumble / ReverseRumble**: rumble for controllers, and its two motors swapped.
- **PowerMode**: raises the low values, so that weak forces are felt.
- **UseConstantInf** (MAME racing games, 1 by default): plays the constant force as one infinite effect updated in
  place. Try it on and off if the force feels erratic on your wheel.
- **Input support**: some games, such as Road Fighters 3D, can read the wheel through the plugin: enable it in the
  GUI's Input section and set the axes and buttons.
- **Logging**: 1 writes FFBlog.txt next to the game, the first thing to look at when there is no force feedback.

## Emulator setup

### MAME

- In mame.ini, set the outputs to Windows messages:

  ```
  #
  # OSD OUTPUT OPTIONS
  #
  output                    windows
  ```

- Put MAME64.dll (64-bit MAME) or MAME32.dll (32-bit MAME) next to the plugin's files.
- The plugin also reads the network outputs (`output network`, a TCP server on port 8000), for when another program
  needs them: MAME has only one output mode at a time. `MAMENetworkOutputs=0` in FFBPlugin.ini turns this off.
- Some games also need a setting of their own: see [MAME](#mame-1) in the list of games.

### Supermodel

- To use the plugin rather than Supermodel's own force feedback, turn Supermodel's off.
- Launch the games with `-outputs=win`, and put MAME64.dll or MAME32.dll next to the plugin's files.
- The network outputs (`-outputs=net`) work too. Like BackForceFeeder, the plugin listens on UDP port 8001 for the
  broadcast of Supermodel njz3 that announces its TCP port, so a TCP port other than 8000 works. It opens that port
  shared, as BackForceFeeder does since its commit 1ae6eca; an older BackForceFeeder keeps it to itself, then keep
  the TCP port at 8000 or set `NetworkOutputsUdpPort=0`.

### Flycast

- Its network output (TCP port 8000) is read like MAME's.

### Lindbergh games (TeknoParrot)

1. Update the Visual C++ runtime files (the all-in-one pack above).
2. Copy opengl32.dll and SDL2.dll to the TeknoParrot folder inside the main TeknoParrot folder, e.g.
   `C:\TeknoParrot\TeknoParrot\`.
3. Copy the other files of the game's folder (SDL2.dll included) next to the game's elf file.

## Supported games

### MAME

What every MAME game needs: the outputs on (see [MAME](#mame) above) and the wheel chosen in the GUI. The games
marked *MAME njz3* use `wheel_motor` outputs that the official MAME 0.289 does not have: they need MAME njz3
(FFB edition or mame3d).

#### Games that need nothing else

- Ace Driver
- Ace Driver: Victory Lap
- Beast Busters
- California Speed
- Cart Fury
- Chase Bombers
- Cisco Heat
- Cruis'n USA
- Cruis'n World
- Daytona USA (*MAME njz3*)
- Dirt Dash
- Driver's Edge (*MAME njz3*)
- F-1 Grand Prix Star
- F-1 Grand Prix Star II
- Ghoul Panic
- GTI Club
- Gun Buster
- Hyperdrive
- Indy 500 (*MAME njz3*)
- Laser Ghost
- Mechanized Attack
- Midnight Run
- Off Road Challenge
- Operation Thunderbolt
- Operation Wolf
- Outrun
- Outrunners
- Point Blank 2
- Power Drift
- Racing Jam
- Racing Jam 2
- Rail Chase
- Revolution X
- Road's Edge
- San Francisco Rush
- San Francisco Rush The Rock
- San Francisco Rush 2049
- Sega Rally Championship (*MAME njz3*)
- Sega Touring Car Championship (*MAME njz3*)
- Space Gun
- Speed Up (*MAME njz3*)
- Super Chase - Criminal Termination
- Terminator 2
- Thrill Drive
- Turbo Outrun
- Turkey Shoot
- Under Fire
- Vapor TRX
- Winding Heat
- Xtreme Rally

#### Games that need a setting

Without it, the game itself does not drive its wheel motor: MAME sends no motor output and the plugin has nothing to
play, whatever FFBPlugin.ini says. The setting is made once, in the game's test menu (MAME's service key, F2 by
default, or a "Test Mode" DIP switch in MAME's menu, Tab) and kept in the game's NVRAM.

##### Virtua Racing

In the test menu, Game Assignments, set CABINET TYPE to UPRIGHT, or to anything but STANDARD. With STANDARD, the game
sends no command to its Motor PCB.

##### Rave Racer

Set FEEDBACK STEERING to ON: turn the "Test Mode" DIP switch on in MAME's DIP Switches menu, then TEST MODE >
GAME OPTIONS > NEXT PAGE (page 2/2). It is OFF by default, so in a link it must be set in the NVRAM of every cabinet.
With it OFF, the game sends only the motor's initialization (66 packets), whatever MAME's Cabinet setting; with it ON,
about 900 per race (checked on two cabinets, alone and in a link). The GUI can invert the effect, which differs
between the standard and deluxe cabinets.

In the NVRAM (`nvram\raverace\eeprom`), FEEDBACK STEERING is byte 0x2D1 (00 = ON, 01 = OFF); the checksum is the
big-endian word at 0x2DE-0x2DF, so that the bytes 0x200-0x2DD plus it make 0x10EE.

##### Cruis'n Exotica (*MAME njz3*)

In the test menu, set Wheel Invert to On, the game type to Dedicated and the cabinet to Sitdown.

##### Over Rev (*MAME njz3*)

Change the output mode in the service menu.

##### Super Monaco GP

Set the game to Sitdown or Upright, not Deluxe. Then tune the sine length in the GUI until the 7 values of the test
menu's motor test give a steady rise of strength.

##### Super GT 24h (*MAME njz3*)

The force feedback works, but the game's analog inputs do not.

#### Games that do not work yet

- **Hard Drivin', Race Drivin', Afterburner II, Alien 3**: MAME 0.289 names their outputs with the `mainpcb:` prefix,
  which the plugin does not read yet.
- **Side by Side, Side by Side 2**: they need the CPU overclocked (MAME's sliders, cheats on) to be playable, and even
  then are not considered working.

### Model 2 Emulator

- Daytona USA
- Indy 500
- Over Rev
- Sega Rally Championship
- Sega Touring Car Championship
- Super GT 24h

### Supermodel

- Daytona USA 2 - Battle on the Edge
- Daytona USA 2 - Power Edition
- Dirt Devils
- Emergency Call Ambulance
- Le Mans 24
- Scud Race
- Scud Race Plus
- Sega Rally 2

### Demul

- ATV Track
- Faster Than Speed
- Initial D Arcade Stage
- Initial D Arcade Stage 2
- Initial D Arcade Stage 3
- Maximum Speed
- Nascar Racing
- Smashing Drive

### Flycast

- 18 Wheeler
- Club Kart
- F355 Challenge
- F355 Challenge 2
- Faster Than Speed
- Initial D Arcade Stage
- Initial D Arcade Stage 2
- Initial D Arcade Stage 3
- King Of Route 66
- Maximum Speed
- Sega Driving Simulator

### PC (Steam)

- Crazy Taxi

### TeknoParrot

- Afterburner Climax
- Aliens Extermination
- Batman (2013)
- Battle Gear 4 Tuned (Japan version v2.07)
- Chase HQ 2
- Cruis'n Blast
- Daytona Championship USA
- Daytona Championship USA New Season Edition
- Dead Heat
- Dead Heat Riders
- Dirty Drivin'
- FNF
- FNF Drift
- FNF Supercars
- Ford Racing
- Gaelco Tuning Race
- Golden Gun
- GRID
- GTI Club Supermini Festa
- House of the Dead 4
- Hummer
- Hummer Extreme
- Initial D Arcade Stage 4
- Initial D Arcade Stage 4 Japan
- Initial D Arcade Stage 5
- Initial D Arcade Stage 6
- Initial D Arcade Stage 7
- Initial D Arcade Stage 8
- Initial D Arcade Stage Zero
- Initial D The Arcade v2.31
- KO Drive
- Let's Go Island
- Let's Go Island 3D
- Machstorm
- Mario Kart Arcade GP DX
- Outrun 2 Special Tours Deluxe
- Pokken Tournament
- R-Tuned: Ultimate Street Racing
- Rambo
- Ring Riders
- Road Fighters 3D
- Sega Racing Classic
- Sega Rally 3
- Sega World Drivers Championship 2018
- Showdown
- SnoCross
- Sonic & Sega All-Stars Racing
- Storm Racer G
- Tokyo Cop
- Transformers: Human Alliance
- Wacky Races
- Wangan Midnight Maximum Tune 3
- Wangan Midnight Maximum Tune 3DX+
- Wangan Midnight Maximum Tune 5
- Wangan Midnight Maximum Tune 5DX
- Wangan Midnight Maximum Tune 5DX+
- Wangan Midnight Maximum Tune 6
- Wangan Midnight Maximum Tune 6R
- Wangan Midnight Maximum Tune 6RR
- Wasteland Racers 2071

## Credits

- Reaver from TeknoParrot, for the code of some games and of the force feedback in general: extremely generous.
- Jackchen for his Daytona Championship USA force feedback work.
- seam for always testing and helping.
- Howard Castro for his help on the games' force feedback code: always helpful, and a big reason this plugin was made.
- The MAME team.
- SailorSat for the offsets of the Daytona USA multiplayer AI hack, and for MAME's System 22 outputs.
- Nuexzz for the offset of the Daytona Panoramic Force hack.
- POOTERMAN for the GUI's logo.
- headkaze for the MAMEInterOp SDK.
- Rocco for the WMMT6RR force feedback.
- Endprodukt for the MAME racing games (FFBPluginRacerMAME) and their outputs in MAME.
- Everyone who helps and gives back to this awesome scene. Thanks for everything!
