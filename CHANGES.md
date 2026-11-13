# Changes

Versions of the njz3 fork (branch `dev/mame-racer`), numbered MAJOR.MINOR.PATCH in `Version.txt`:
PATCH for a bug fix, MINOR for a new feature. Each release commit is tagged `v<version>`; a build
between two tags is named `<version>.<commits since the tag>-g<commit>` (see `Write-Version.ps1`).
`Make-Release.ps1` builds the release zip.

## Unreleased

- The user guide shipped with the plugin is now Markdown, FFBPluginReadme.md (was FFBPluginReadme.txt). Its list of
  MAME games is split into the games that need nothing else, the games that need a setting (one paragraph each:
  Virtua Racing, Rave Racer, Cruis'n Exotica, Over Rev, Super Monaco GP, Super GT 24h) and the games that do not
  work yet. README.md keeps the build, version and release notes and points to it.
- BuildAll.cmd names its archives of all the games with the full version of the build, e.g.
  "FFB Arcade Plugin 2.1.0.6-g09c2657.zip" (was the MAJOR.MINOR.PATCH of Version.txt only), and starts them empty.
- Exit: the forces are stopped and the devices closed when the game calls ExitProcess (hooked), while its
  threads still run, instead of in DLL_PROCESS_DETACH, where the other threads have been killed, maybe holding
  an SDL lock, which could hang the exit. DLL_PROCESS_DETACH only stops the forces if that did not happen.
  Flycast is still ended with TerminateProcess, now after the forces are stopped and its ProcessID cleared.
- The plugin can no longer be unloaded before the process exits (FreeLibrary would have left its threads,
  window and hooks running code that is gone), and FFBPluginGUI.exe no longer starts the outputs threads.
- Leaks fixed: a thread for each gear change in Wangan Midnight Maximum Tune 5 to 6RR (the effect is now
  played by the FFB loop), a thread at each loop while Demul's error popup is open, a voice object each time
  the force is spoken (and its release after CoUninitialize when the plugin was unloaded), the memory region
  copied by the pattern scan that finds a match, the DirectInput8Create wrapper, the joysticks opened while
  looking for the one with haptics.
- MAME outputs: names cut to fit the log buffer (a network line can be 4 KB); dead id cache removed; no
  crash once the "MAME64.dll is missing!" message is closed (the network outputs still work).

## 2.1.0 (2026-11-12)

First numbered version of the fork, after Boomslangnz's 2.0.0.54.

- MAME racing games of Endprodukt's FFBPluginRacerMAME and of its GUI, ported on top of the refactored plugin:
  Model 2 (Daytona USA, Sega Rally, Indy 500, Sega Touring Car, Over Rev, Super GT 24h), Driver's Edge, Speed Up,
  Cruis'n Exotica, Side by Side 1 and 2, Super Monaco GP and more; the support of the other games is kept.
- ConstantInf effect (one infinite constant effect, updated in place) and `UseConstantInf`.
- No crash when the wheel has no haptic support or lacks some effects (PR #108).
- MAME no longer crashes on exit: use-after-free of the name of the last output; leaks of 256 bytes every 16 ms
  fixed in the MAME and M2Emulator code.
- The C/C++ runtime is linked statically: the plugin needs only SDL2.dll and Windows DLLs.
- FFB strength adjusted with keyboard keys too.
- Network outputs: besides `-output windows` (MAME64.dll), the plugin reads MAME's `-output network`,
  Supermodel's `-outputs=net` and Flycast's network output (TCP port 8000), and listens, shared, on UDP port
  8001 for the broadcast that announces another TCP port, as BackForceFeeder does. `MAMENetworkOutputs` and
  `NetworkOutputsUdpPort` in FFBPlugin.ini.
- The wheel is found again when its GUID was saved in FFBPlugin.ini with another SDL version: SDL 2.26 and later
  put a CRC in bytes 2-3 of the GUID, which made the cabinets' DeviceGUID match no device.
- GUI: the MAME, Demul and FFB test pages list the wheels again; the MAME game list has all the pages,
  in 4 columns, with its "Go Back To Previous Menu" link.
- Version: dinput8.dll and FFBPluginGUI.exe carry the version in their file properties, the GUI shows it in
  its title (with the dinput8.dll's when it differs), and the plugin writes it, with SDL's, at the top of
  FFBlog.txt.
