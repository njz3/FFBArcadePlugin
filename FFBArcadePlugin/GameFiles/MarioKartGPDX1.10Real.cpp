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

#include "MarioKartGPDX1.10Real.h"

using namespace ffb;

MarioKartGPDXReal::MarioKartGPDXReal(const Addresses& addresses)
	: addresses(addresses)
{
}

void MarioKartGPDXReal::Update()
{
	ApplyDamper();

	if (springEnable)
		wheel.KeptSpring(springStrength / 100.0);

	DWORD pointer = Memory::Read<int>(Memory::Address(Base::Module, addresses.forcePointer));
	DWORD structure = Memory::Read<int>(pointer + ForceStructure);
	float force = Memory::Read<float>(structure + addresses.forceOffset);

	if (force > 0)
		wheel.ConstantWithRumble(Direction::FromLeft, force);
	else if (force < 0)
		wheel.ConstantWithRumble(Direction::FromRight, -force);
}
