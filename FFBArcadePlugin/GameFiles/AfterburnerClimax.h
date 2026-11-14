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
#include "../Core/Game.h"

// After Burner Climax (Sega Lindbergh): the stick rumbles, with the strength of the RumbleStrength
// setting, while a bit of an output byte of the game is set. The two versions of the game are told
// apart by a value (0x1320221) found at one address or the other.
class AfterburnerClimax : public ffb::Game
{
public:
	void Update() override;

private:
	static const DWORD VersionMark = 0x1320221;
	// 32 bits: VersionMark in the original version
	static constexpr ffb::GameValue<int> OldVersionMark{ ffb::Base::Absolute, 0x8347A5A };
	// 32 bits: VersionMark in the new version
	static constexpr ffb::GameValue<int> NewVersionMark{ ffb::Base::Absolute, 0x834A51A };
	// Outputs of the game, bit 0x40: the stick rumbles
	static constexpr ffb::GameValue<UINT8> OldVersionOutputs{ ffb::Base::Absolute, 0x8347A5E };
	static constexpr ffb::GameValue<UINT8> NewVersionOutputs{ ffb::Base::Absolute, 0x834A51E };

	int rumbleStrength = ffb::Settings::Int(L"RumbleStrength", 0);

	bool versionFound = false;
	bool newVersion = false;
	bool oldVersion = false;
	UINT8 outputs = 0; // kept when no version is found
};
