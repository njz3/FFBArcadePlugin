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

#pragma once
#include <Windows.h>
#include "SDL.h"

// The state of the plugin some games use, owned by DllMain.cpp

// The devices: device 1 (DeviceGUID), opened by Initialize; devices 2 and 3 (Device2GUID, Device3GUID),
// opened by the games that use them (Devices.h)
extern int joystick_index1;   // SDL instance id of device 1
extern int joystick1Index;    // its SDL device index, -1 if none
extern int joystick_index2;   // SDL instance id of device 2, -1 if none
extern int joystick_index3;   // SDL instance id of device 3, -1 if none
extern SDL_Joystick* GameController2;
extern SDL_Joystick* GameController3;
extern SDL_Haptic* ControllerHaptic2;
extern SDL_Haptic* ControllerHaptic3;
extern SDL_Haptic* haptic2;
extern SDL_Haptic* haptic3;

// The last SDL event, read by the games that run their own event loop and by the FFB strength buttons
extern SDL_Event e;

// Set once the game is running: the max force can then be changed with the buttons
extern bool WaitForGame;

// The constant force of California Speed is held at a level of 0 instead of stopped (ForceProfile)
extern bool ConstantInfHoldsAtZero;

// DLLs loaded by DllMain.cpp for some games
extern HINSTANCE gl_cgGLDll;  // House of the Dead 4, Rambo
extern HINSTANCE gl_hjgtDll;  // Road Fighters 3D
extern HINSTANCE gl_hlibavs;  // Road Fighters 3D

// Rom or game name of the emulators, for the keys of the persistent max force (PersistentValues.cpp)
extern char* romname;      // MAME, Supermodel, Flycast
extern char* romnameM2;    // M2Emulator
extern char* romnameDemul; // Demul

// Keys of FFBPlugin.ini the max force changed in game is saved to (PersistentValues.cpp)
extern LPCSTR CustomMaxForce;
extern LPCSTR CustomAlternativeMaxForceLeft;
extern LPCSTR CustomAlternativeMaxForceRight;
