/*This file is part of FFB Arcade Plugin GUI.
FFB Arcade Plugin GUI is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
FFB Arcade Plugin GUI is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with FFB Arcade Plugin GUI.If not, see < https://www.gnu.org/licenses/>.
*/

#pragma once
#include <Windows.h>
#include "Helper.h"

namespace FFBPluginGUI {

	using namespace System;
	using namespace System::Windows::Forms;
	using namespace MetroFramework::Controls;
	using namespace MetroFramework::Forms;

	public ref class DriversEdge : Helper
	{
	public:
		DriversEdge(MetroForm^ obj1)
		{
			this->obj = obj1;
			this->InitializeComponent();
		}

	private:
		void InitializeComponent(void)
		{
			this->SuspendLayout();

			this->AutoAddLongTextBox(L"Options", L"");
			this->AutoAddShortCheckBox("PowerModeDriversEdge", L"Power Mode", L"Enable to raise strength of lower values");
			this->AutoAddShortCheckBox("EnableDamperDriversEdge", L"Enable Damper", L"Enable damper to lower wheel oscillation");
			this->AutoAddLongTrackBarBlock("DamperStrengthDriversEdge", L"Damper Strength", 0, 100, L"Strength of damper effect to lower oscillation");
			this->AutoAddLongTrackBarBlock("FeedbackLengthDriversEdge", L"Feedback Length", 16, 10000, L"Length of a feedback command");

			this->AutoAddLongTextBox(L"Force Spring Effect", L"Force Spring effect over using default game spring");
			this->AutoAddShortCheckBox("EnableForceSpringEffectDriversEdge", L"Force Spring Effect", L"Forces spring effect to work all the time");
			this->AutoAddShortTrackBarBlock("ForceSpringStrengthDriversEdge", L"Forced Spring Strength", 0, 100, L"How strong the spring effect is when forced on");

			this->New2ColsPage();

			this->AutoAddComponent("GlobalForceHeader");
			this->AutoAddShortTrackBarBlock("MinForceDriversEdge", L"Min Force", 0, 100, L"Minimum overall force strength that will be applied to device");
			this->AutoAddShortTrackBarBlock("MaxForceDriversEdge", L"Max Force", 0, 100, L"Maximum overall force strength that will be applied to device");

			this->AutoAddComponent("AltGlobalForceHeader");
			this->AutoAddShortTrackBarBlock("AlternativeMinForceLeftDriversEdge", L"Min Force Left", -100, 100, L"Minimum overall left force strength that will be applied to device");
			this->AutoAddShortTrackBarBlock("AlternativeMaxForceLeftDriversEdge", L"Max Force Left", -100, 100, L"Maximum overall left force strength that will be applied to device");
			this->AutoAddShortTrackBarBlock("AlternativeMinForceRightDriversEdge", L"Min Force Right", -100, 100, L"Minimum overall right force strength that will be applied to device");
			this->AutoAddShortTrackBarBlock("AlternativeMaxForceRightDriversEdge", L"Max Force Right", -100, 100, L"Maximum overall right force strength that will be applied to device");

			this->Init();
		}
	};
}
