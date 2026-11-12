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
