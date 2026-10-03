#include "InitialDFFB.h"

void PlayInitialDFFB(BYTE command, BYTE param, UINT32 strength, const InitialDFFBSettings& settings, EffectConstants* constants, EffectTriggers* triggers)
{
	UINT32 length_ms = 100;

	if (command == 0x80 && strength == 0x01)
	{
		triggers->Spring(1.0);
	}

	if (command == 0x85 && param > 0x00 && strength > 0x00)
	{
		double percentForce = strength / settings.strengthScale;
		double Period = param / settings.periodScale * 120.0;
		triggers->Rumble(percentForce, percentForce, length_ms);
		triggers->Sine(static_cast<int>(Period), 0, percentForce, settings.sineLength_ms);
	}

	if (command == 0x86 && strength > 0x00 && strength <= settings.maxStrength)
	{
		double percentForce = strength / settings.strengthScale;
		triggers->Spring(percentForce);
	}

	if (command == 0x84 && strength > 0x00 && strength <= settings.maxStrength)
	{
		if (param == 0x00)
		{
			double percentForce = (settings.leftStrengthBase - (int)strength) / settings.strengthScale;
			triggers->Rumble(percentForce, 0, length_ms);
			triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
		}
		else if (param == 0x01)
		{
			double percentForce = (strength / settings.strengthScale);
			triggers->Rumble(0, percentForce, length_ms);
			triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
		}
	}
}
