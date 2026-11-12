#include <stdio.h>
#include <string.h>
#include "Devices.h"

extern int joystick1Index;
extern int joystick_index2;
extern int joystick_index3;
extern SDL_Joystick* GameController2;
extern SDL_Joystick* GameController3;
extern SDL_Haptic* ControllerHaptic2;
extern SDL_Haptic* ControllerHaptic3;
extern SDL_Haptic* haptic2;
extern SDL_Haptic* haptic3;

bool SameJoystickGUID(const SDL_JoystickGUID& a, const SDL_JoystickGUID& b)
{
	return memcmp(a.data, b.data, 2) == 0 && memcmp(a.data + 4, b.data + 4, sizeof(a.data) - 4) == 0;
}

SDL_Joystick* OpenJoystickFromSetting(LPCWSTR guidSetting, int ignoredIndex)
{
	wchar_t guidSettingValue[256];
	GetPrivateProfileString(TEXT("Settings"), guidSetting, NULL, guidSettingValue, 256, TEXT(".\\FFBPlugin.ini"));
	char joystick_guid[256];
	sprintf(joystick_guid, "%S", guidSettingValue);
	SDL_JoystickGUID guid = SDL_JoystickGetGUIDFromString(joystick_guid);

	for (int i = 0; i < SDL_NumJoysticks(); i++)
	{
		if (i == ignoredIndex)
			continue;

		SDL_Joystick* js = SDL_JoystickOpen(i);
		SDL_JoystickGUID dev_guid = SDL_JoystickGetGUID(js);
		SDL_JoystickClose(js);

		if (SameJoystickGUID(guid, dev_guid))
			return SDL_JoystickOpen(i);
	}
	return NULL;
}

void OpenInputDevice2()
{
	SDL_Joystick* joystick = OpenJoystickFromSetting(TEXT("Device2GUID"));
	if (joystick != NULL)
	{
		GameController2 = joystick;
		joystick_index2 = SDL_JoystickInstanceID(joystick);
	}
}

static void OpenHapticDevice(LPCWSTR guidSetting, SDL_Joystick*& controller, int& joystickIndex, SDL_Haptic*& controllerHaptic, SDL_Haptic*& haptic)
{
	SDL_Joystick* joystick = OpenJoystickFromSetting(guidSetting, joystick1Index);
	if (joystick != NULL)
	{
		controller = joystick;
		joystickIndex = SDL_JoystickInstanceID(joystick);
		controllerHaptic = SDL_HapticOpenFromJoystick(joystick);
	}

	haptic = controllerHaptic;
	if (SDL_HapticRumbleSupported(haptic) == SDL_TRUE)
		SDL_HapticRumbleInit(controllerHaptic);
}

void OpenHapticDevice2()
{
	OpenHapticDevice(TEXT("Device2GUID"), GameController2, joystick_index2, ControllerHaptic2, haptic2);
}

void OpenHapticDevice3()
{
	OpenHapticDevice(TEXT("Device3GUID"), GameController3, joystick_index3, ControllerHaptic3, haptic3);
}
