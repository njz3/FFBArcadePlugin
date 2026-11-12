# FFB Arcade Plugin

Brought to you by Boomslangnz, Ducon2016, Spazzy, Adam_, jpb, Rocco & pinkimo and a bit of Endprodukt.

This is a plugin to provide Force Feedback and Rumble to various arcade games.


## Requirements

- Requires Visual Runtime Files https://www.techpowerup.com/download/visual-c-redistributable-runtime-package-all-in-one/

- GUI Requires Visual Runtime 2022 and has to be in same folder with metroframework.dll & SDL2.dll

## How to Build

- Install VS 2022 Community edition with C++ desktop development package

- Install Powershell (from command line: ```winget install --id Microsoft.PowerShell --source winget```)

- Install 7zip (https://www.7-zip.org/download.html) to C:\Program Files\7-Zip

- Run the BuildAll.cmd script

## How to Use

Launch FFBPluginGUI.exe & Go to Setup Help Section. 

For more advanced information read below


## Advanced information

Alternative FFB is for certain wheels such as Thrustmaster wheels or PWM2M2 etc where rotation always goes only 1 direction, enable this to fix if your wheel does this

MinForce is the lowest force the wheel strength will be, if AlternativeFFB is enabled, use AlternativeFFB MinForce instead.

MaxForce is the maximum force the wheel strength will be, if AlternativeFFB is enabled, use AlternativeFFB MaxForce instead.

Feedback length is how long the force feedback effect will last for

Enable Rumble is to enable or disable Rumble for controllers

Reverse Rumble is to swap the motors around if it rumbles opposite to how you want

PowerMode is to raise the strength of low values to increase strength feel on certain games

Some games such as Road Fighters 3D have input support. Go to Input section and enable Input Support and set up axis/buttons

Enable ConstantInf is an alternative way of providing effects to the wheel. Test it enabled and see if FFB becomes less erratic. 

## Output Support 

### For MAME

Create a ini file on root of MAME folder called Mame.ini and place inside it and save

```
#
# OSD OUTPUT OPTIONS
#
output                    windows
```

Ensure either MAME32.dll or MAME64.dll is with FFB Plugin files in MAME folder depending on 32bit or 64bit MAME

The plugin also reads the network outputs (MAME's `output network`, Supermodel's `-outputs=net`, Flycast's network output): a TCP server on port 8000, so you can use them when another program needs them. An emulator has only one output mode at a time. Like BackForceFeeder, the plugin listens on UDP port 8001 for the broadcast that announces another TCP port (Supermodel njz3); it opens that port shared (SO_REUSEADDR), so every program that does the same also gets the broadcast, BackForceFeeder included since its commit 1ae6eca; an older BackForceFeeder opens it alone and keeps the others out while it has it: then keep the TCP port at 8000, or set `NetworkOutputsUdpPort=0`. Set `MAMENetworkOutputs=0` in FFBPlugin.ini to turn the network outputs off.

## For Supermodel Emulator

If you wish to use FFB Plugin over Supermodel's FFB then disable Supermodel's FFB.

When you launch game, ensure you have command -outputs=win for FFB Plugin to recieve Output Values etc

Ensure either MAME32.dll or MAME64.dll is with FFB Plugin files in Supermodel folder depending on 32bit or 64bit Supermodel

## For Lindbergh games

1) Ensure visual studio c++ runtime files are up to date. If un-sure then download & install all in one visual runtime pack.

2) Copy opengl32.dll & SDL2.dll to TeknoParrot folder inside main TeknoParrot folder eg C:\TeknoParrot\TeknoParrot\

3) Copy remaining files (including SDL2.dll) in game folder from FFB Arcade Plugin to same folder as elf file


## CREDITS

- Reaver from Teknoparrot . Huge thanks to Reaver for supplying code necessary for some games & general force feedback,
extremely generous.

- Jackchen for his Daytona Championship USA FFB work.

- seam for always testing and helping

- Howard Castro for help on game FFB code. Always helpful and a big reason this plugin was ever made

- Mame team

- SailorSat for finding the offsets etc required for Daytona USA Multiplayer AI hack

- Nuexzz for finding offset required for Daytona Panoramic Force Hack

- POOTERMAN for making logo on GUI

- headkaze for making MAMEInterOp SDK

- Rocco for WMMT6RR FFB.

- Everyone who helps and gives back to this awesome scene. Thanks for everything!


## SUPPORTED GAMES

### MAME:

- Ace Driver
- Ace Driver: Victory Lap
- Alien 3
- Afterburner II
- Beast Busters
- California Speed
- Cart Fury
- Chase Bombers
- Cisco Heat
- Cruis'n Exotica
- Crusn'USA
- Crusn' World
- Daytona USA
- Dirt Dash
- Driver's Edge
- F-1 Grand Prix Star
- F-1 Grand Prix Star II
- Ghoul Panic
- GTI Club
- Gun Buster
- Hard Drivin'
- Hyperdrive
- Indy 500
- Laser Ghost
- Midnight Run
- Mechanized Attack
- OffRoad Challenge
- Operation Thunderbolt
- Operation Wolf
- Outrun
- Outrunners
- Overrev (settings: see below)
- Point Blank 2
- Power Drift
- Race Drivin'
- Rail Chase
- Racing Jam
- Racing Jam 2
- Rave Racer (settings: see below)
- Roads Edge
- Revolution X
- San Francisco Rush
- San Francisco Rush The Rock
- San Francisco Rush 2049
- Sega Rally Championship
- Sega Touring Car Championship
- Side by Side
- Side by Side 2
- Super Chase - Criminal Termination
- Super GT 24h (see below)
- Super Monaco GP
- Space gun
- Speed Up
- Terminator 2
- Thrill Drive
- Turbo Outrun
- Turkey Shoot
- Under Fire
- Vapor TRX
- Virtua Racing (settings: see below)
- Winding Heat
- Xtreme Rally

The Model 2 games (Daytona USA, Indy 500, Over Rev, Sega Rally Championship, Sega Touring Car Championship,
Super GT 24h), Driver's Edge, Speed Up and Cruis'n Exotica use `wheel_motor` outputs that the official MAME 0.289
does not have: they need a MAME that adds them.
Most MAME racing games play their constant force with the infinite constant effect (`UseConstantInf=1`, the default).

#### Settings each MAME game needs for force feedback

Some games only drive their wheel motor when their own settings say the cabinet has one: with the
wrong setting, MAME sends no motor output and the plugin has nothing to play, whatever FFBPlugin.ini
says. Set them once, in the game's test menu (MAME's service key, F2 by default) or in MAME's
Machine Configuration / DIP Switches menu (Tab), as said for each game.

For every game:
- `output windows` (or `output network`) in mame.ini, and dinput8.dll, SDL2.dll, MAME64.dll and
  FFBPlugin.ini next to mame.exe.
- The wheel chosen in the GUI (`DeviceGUID`). A GUID saved with another SDL version still matches
  (since 2.1.0).

Game by game:
- **Virtua Racing**: in the test menu, Game Assignments, set the CABINET TYPE to UPRIGHT, or anything
  but STANDARD. With STANDARD, the game sends no command to its Motor PCB and there is no force
  feedback.
- **Rave Racer**: set FEEDBACK STEERING to ON: turn the "Test Mode" DIP switch on in MAME's DIP Switches
  menu, then TEST MODE > GAME OPTIONS > NEXT PAGE (page 2/2). It is OFF by default and stored in the EEPROM,
  so once per NVRAM: in a link, in the NVRAM of every cabinet (byte 0x2D1 of nvram/raverace/eeprom, 00 = ON,
  01 = OFF, then the checksum at 0x2DE-0x2DF, big-endian, so that the bytes 0x200-0x2DD plus it make 0x10EE). Checked on two cabinets, alone and in a link: about 900 FFB
  packets per race with it ON, 66 (the motor's initialization only) with it OFF. With it off, the game writes no drive command at all during the race, whatever the
  Cabinet setting of MAME's Machine Configuration (Standard, Twin, Deluxe and 50 Inch all measured): the
  plugin only gets the motor's initialization. The GUI can invert the effect, which
  differs between the standard and deluxe cabinets.
- **Cruis'n Exotica**: in the test menu, set Wheel Invert to On, the game type to Dedicated and the
  cabinet to Sitdown.
- **Over Rev**: change the output mode in the service menu.
- **Super Monaco GP**: set the game to Sitdown or Upright, not Deluxe. Then tune the sine length in
  the GUI until the 7 values of the test menu's motor test give a steady rise of strength.
- **Daytona USA, Sega Rally Championship, Indy 500, Sega Touring Car Championship, Over Rev, Super
  GT 24h, Driver's Edge, Speed Up, Cruis'n Exotica**: need a MAME with their `wheel_motor` outputs
  (MAME njz3), see above. Super GT 24h: the outputs work but its analog inputs do not.
- **Side by Side, Side by Side 2**: they need the CPU overclocked (MAME's sliders, with cheats on) to be
  playable; even then, they are not considered working.
- **Hard Drivin', Race Drivin', Afterburner II, Alien 3**: MAME 0.289 names their outputs with the
  `mainpcb:` prefix, which the plugin does not read yet: no force feedback with it.

### MODEL 2 EMULATOR:

- Daytona USA
- Indy 500
- Over Rev
- Sega Rally Championship
- Sega Touring Car Championship
- Super GT 24h


### SUPERMODEL3:

-Daytona USA 2 - Power Edition
-Daytona USA 2 - Battle on the Edge
-Dirt Devils
-Emergency Call Ambulance
-LeMans 24
-Scud Race
-Scud Race Plus
-Sega Rally 2

### DEMUL:

- ATV Track
- Initial D Arcade Stage
- Initial D Arcade Stage 2
- Initial D Arcade Stage 3
- Faster Than Speed
- Maximum Speed
- Smashing Drive
- Nascar Racing

### FLYCAST:

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

### PC STEAM:

-Crazy Taxi (Steam)

### TEKNOPARROT:

- Alien Extermination
- Afterburner Climax
- Batman (2013)
- Battle Gear 4 Tuned (Japan version v2.07)
- ChaseHQ 2
- Daytona Championship USA
- Daytona Championship USA New Season Edition
- Dead Heat
- Dead Heat Riders
- Dirty Drivin
- Ford Racing
- Gaelco Tuning Race
- Golden Gun
- GRID
- GTI Club Supermini Festa
- House of the dead 4
- Initial D Arcade Stage 4
- Initial D Arcade Stage 4 Japan
- Initial D Arcade Stage 5
- Initial D Arcade Stage 6
- Initial D Arcade Stage 7
- Initial D Arcade Stage 8
- Initial D Arcade Stage Zero
- KO Drive
- Let's Go Island
- Let's Go Island 3D
- Machstorm
- Mario Kart GP DX
- Outrun 2 Special Tours Deluxe
- Pokken Tournament
- Ring Riders
- Rambo
- Road Fighters 3D
- R-Tuned : Ultimate Street Racing
- Sega Racing Classic
- Sega Rally 3
- Sega World Drivers Championship 2018
- Showdown
- SnoCross
- Sonic & Sega All Stars Racing
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