# FFB Arcade Plugin

Brought to you by Boomslangnz, Ducon2016, Spazzy, Adam_, jpb, Rocco & pinkimo, with the MAME racing games of
Endprodukt and the changes of this njz3 fork.

A plugin that gives force feedback and rumble to arcade games, and its configuration GUI.

**How to use it, its settings, how to set up each emulator and the list of supported games, with what each game
needs for force feedback: [FFBPluginReadme.md](FFBArcadePlugin/FFBPluginReadme.md).** That file is shipped
next to the plugin in every release.

What changed in each version: [CHANGES.md](CHANGES.md).

## Repository

- `FFBArcadePlugin/`: the plugin, dinput8.dll (also used under other DLL names for some games), in C++17:
  - `DllMain.cpp`: the wrapped DLLs, the devices and their effects, the FFB loop, the exit.
  - `Core/`: what the games are written with, in namespace `ffb`: `Game` (the base class of the games, updated
    every 16 ms), `Wheel` and `Device` (the effects), `Memory` (the game memory, with typed addresses
    `GameValue`/`PointerChain`), `Hooks` (MinHook), `Settings` and `Config` (FFBPlugin.ini), `Log`,
    `ForceProfile` (the settings of a game of an emulator), `TeknoParrotGame`, `GameFactory` (the game of a GameId).
  - `Hardware/`: the force feedback boards of the emulated games (Sega drive boards, Namco, Midway/Atari, Konami
    wheel motors, gun recoil and vibration motors...), driven by the outputs of the emulators.
  - `GameFiles/`: one class per game (or per family of versions), with its memory addresses as named constants.
    `EmulatorGames.cpp` is the table of the MAME, Supermodel and Flycast games; their outputs come through
    `MAMEOutputs.cpp` (MAME64.dll) and `MAMENetworkOutputs.cpp` (network).
  - [REFACTOR.md](REFACTOR.md): how the code was moved to these classes, and the bugs found on the way.
- `FFBPluginGUI/`: the GUI, FFBPluginGUI.exe (C++/CLI, .NET Framework 4.7.2, MetroFramework).
- `FFBArcadePlugin/Config/FFBPlugin.ini`: the master settings file. The build writes from it one FFBPlugin.ini per
  game profile (`Release.Win32/<profile>/FFBPlugin.ini`); ship the profile's, never the master one (GameId=-1).

## How to build

- Install Visual Studio 2022 Community with the "Desktop development with C++" workload, and its C++/CLI support.
- Install PowerShell 7 (`winget install --id Microsoft.PowerShell --source winget`), used by the post-build step.
- Install 7-Zip (https://www.7-zip.org/download.html) to `C:\Program Files\7-Zip`, used by `BuildAll.cmd`.
- Run `BuildAll.cmd` to build everything and the per-game folders, or build `FFBArcadePlugin/Dinput8Wrapper.sln`
  and `FFBPluginGUI/FFBPluginGUI.sln` in Release, x86 then x64.

## Versions and releases

- `Version.txt` holds MAJOR.MINOR.PATCH: raise PATCH for a bug fix, MINOR for a new feature, and tag the release
  commit `v<version>`.
- Before each build, `Write-Version.ps1` (run by `Version.targets`) writes `Version.h`, not committed: "2.1.0" on the
  tagged commit, "2.1.0.3-g22f99dd" three commits later, "-dirty" with changes that are not committed. It goes in the
  file properties of dinput8.dll and FFBPluginGUI.exe, in the GUI's title and at the top of FFBlog.txt.
- `Make-Release.ps1 -OutDir <folder>` builds both solutions and packs the x64 MAME release
  (`FFBArcadePlugin-<version>-njz3-windows-x64.zip` and its `.sha256`). It refuses changes that are not committed.
