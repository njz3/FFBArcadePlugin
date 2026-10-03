#pragma once
#include <Windows.h>
#include "SDL.h"

// Opens the joystick whose GUID is saved in the given setting of FFBPlugin.ini (e.g. Device2GUID),
// skipping the joystick at ignoredIndex. Returns NULL when no joystick matches.
SDL_Joystick* OpenJoystickFromSetting(LPCWSTR guidSetting, int ignoredIndex = -1);

// Opens the second device (Device2GUID) to read its inputs: sets GameController2 and joystick_index2.
void OpenInputDevice2();

// Opens the second/third device (Device2GUID/Device3GUID) to send rumble or FFB to it:
// sets GameControllerN, joystick_indexN, ControllerHapticN and hapticN.
void OpenHapticDevice2();
void OpenHapticDevice3();
