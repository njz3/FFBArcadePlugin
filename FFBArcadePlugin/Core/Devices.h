#pragma once
#include <Windows.h>
#include "SDL.h"

// True when the two GUIDs are the same device. Bytes 2-3 are left out: SDL 2.26 and later put a
// CRC of the device's name there, older SDL versions 0, so a GUID saved in FFBPlugin.ini with one
// SDL version still matches the device when the plugin ships another.
bool SameJoystickGUID(const SDL_JoystickGUID& a, const SDL_JoystickGUID& b);

// Opens the joystick whose GUID is saved in the given setting of FFBPlugin.ini (e.g. Device2GUID),
// skipping the joystick at ignoredIndex. Returns NULL when no joystick matches.
SDL_Joystick* OpenJoystickFromSetting(LPCWSTR guidSetting, int ignoredIndex = -1);

// Opens the second device (Device2GUID) to read its inputs: sets GameController2 and joystick_index2.
void OpenInputDevice2();

// Opens the second/third device (Device2GUID/Device3GUID) to send rumble or FFB to it:
// sets GameControllerN, joystick_indexN, ControllerHapticN and hapticN.
void OpenHapticDevice2();
void OpenHapticDevice3();
