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

// Force feedback hardware of the Sega games emulated by MAME, Supermodel and Flycast
#pragma once
#include "OutputBoard.h"

namespace ffb
{
	// Model 1 drive board of Virtua Racing: a command on the digit0 output
	class VirtuaRacingDriveBoard : public OutputBoard
	{
	public:
		const char* Name() const override { return "VirtuaRacingActive"; }
		void OnOutput(const char* name, int value) override;
	private:
		bool uncenteringVibrates = false; // 0x46 and 0x4A vibrate from command 0x07 until command 0x09
	};

	// Model 2 drive board (Daytona USA, Indy 500, Over Rev, Sega Touring Car, Super GT 24h):
	// a command on wheel_motor, 0x10 spring, 0x20 clutch, 0x30 centering, 0x40 uncentering, 0x50/0x60 roll
	class Model2DriveBoard : public OutputBoard
	{
	public:
		const char* Name() const override { return "m2new"; }
		void OnOutput(const char* name, int value) override;
	};

	// Drive board of Sega Rally Championship (Model 2A): 0x80-0x9F force from the left, 0xC0-0xDF from the right
	class SegaRallyDriveBoard : public OutputBoard
	{
	public:
		const char* Name() const override { return "srallynew"; }
		void OnOutput(const char* name, int value) override;
	};

	// Model 3 drive board of Daytona USA 2 (Supermodel RawDrive output)
	class Daytona2DriveBoard : public OutputBoard
	{
	public:
		const char* Name() const override { return "Daytona2Active"; }
		void OnOutput(const char* name, int value) override;
	};

	// Model 3 drive board of Scud Race, Le Mans 24, Dirt Devils and Emergency Call Ambulance (Supermodel
	// RawDrive output): sequences, spring, intensity, vibration, power slide and torque commands
	class ScudRaceDriveBoard : public OutputBoard
	{
	public:
		const char* Name() const override { return "ScudRaceActive"; }
		void OnOutput(const char* name, int value) override;
		void Update(const char* lastName, int lastValue) override;
	private:
		int sequence = 0; // 0 = none
		unsigned long sequenceStart_ms = 0;
		unsigned long sequenceDuration_ms = 0;
		unsigned long now_ms = 0;
		double intensity = 1.0; // gain of the torque and vibration commands
	};

	// Model 3 drive board of Sega Rally 2 (Supermodel RawDrive output): 0x00-0x3F force from the right, 0x40-0x7F from the left
	class SegaRally2DriveBoard : public OutputBoard
	{
	public:
		const char* Name() const override { return "Srally2Active"; }
		void OnOutput(const char* name, int value) override;
	};

	// Drive board of F355 Challenge and 18 Wheeler (Flycast m3ffb output)
	class NaomiDriveBoard : public OutputBoard
	{
	public:
		const char* Name() const override { return "NaomiFFBActive"; }
		void OnOutput(const char* name, int value) override;
	private:
		int lastCommand = 0;
		int vibration = 0;      // command of the vibration being played
		bool vibrating = false;
		int vibrationCount = 0; // commands received since it started
	};

	// Shaker of the Super Monaco GP upright: 3 lamp outputs give the speed of its motor
	class SuperMonacoGPShaker : public OutputBoard
	{
	public:
		const char* Name() const override { return "SuperMonacoGPActive"; }
		void OnOutput(const char* name, int value) override;
		void Update(const char* lastName, int lastValue) override;
	private:
		int lamp0 = 0;
		int lamp1 = 0;
		int lamp2 = 0;
	};

	// Motors of the Out Run deluxe cabinet: bank motor (direction and speed) and wheel vibration
	class OutRunMotors : public OutputBoard
	{
	public:
		const char* Name() const override { return "OutrunActive"; }
		void Update(const char* lastName, int lastValue) override;
		void OnStop() override { stopped = true; }
	private:
		int bankDirection = 0;
		int vibration = 0;
		int bankSpeed = 0;
		bool moving = false;
		bool stopping = false;
		volatile bool stopped = false;
	};

	// Motors of the Power Drift deluxe cabinet: bank motor position, vibration and wheel motor
	class PowerDriftMotors : public OutputBoard
	{
	public:
		const char* Name() const override { return "PDriftActive"; }
		void Update(const char* lastName, int lastValue) override;
		void OnStop() override { stopped = true; }
	private:
		int bankPosition = 0;
		int vibration = 0;
		int wheelMotor = 0;
		bool moving = false;
		bool stopping = false;
		volatile bool stopped = false;
	};
}
