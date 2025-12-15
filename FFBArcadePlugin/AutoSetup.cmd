@echo on
SET start_path="%CD%"
SET releasex64_path=%start_path%\Release.x64
SET releasex86_path=%start_path%\Release.Win32
SET VS=2022\Community
SET BuildMode=Release
SET BUILDER=%ProgramFiles%\Microsoft Visual Studio\%VS%\MSBuild\Current\Bin\MSBuild.exe

set /p Build=<..\Version.txt

:build64
:: Restore nuget packages
"%BUILDER%" -t:restore Dinput8Wrapper.sln /p:Platform=x64;Configuration=%BuildMode% -p:RestorePackagesConfig=true 
:: Build
::"%BUILDER%"  Dinput8Wrapper.sln  /maxcpucount:1 /t:clean /p:Platform=x64;Configuration=%BuildMode%
"%BUILDER%"  Dinput8Wrapper.sln  /maxcpucount:4  /p:Platform=x64;Configuration=%BuildMode%

:build86
:: Restore nuget packages
"%BUILDER%" -t:restore Dinput8Wrapper.sln /p:Platform=x86;Configuration=%BuildMode% -p:RestorePackagesConfig=true 
:: Build
::"%BUILDER%"  Dinput8Wrapper.sln  /maxcpucount:1 /t:clean /p:Platform=x86;Configuration=%BuildMode%
"%BUILDER%"  Dinput8Wrapper.sln  /maxcpucount:4  /p:Platform=x86;Configuration=%BuildMode%


:: Copy x64 files for specific games to releasex86_path
xcopy "%releasex64_path%\dinput8.dll" "%releasex86_path%\Button Rumble 64bit\" /Y
xcopy "%releasex64_path%\SDL2.dll" "%releasex86_path%\Button Rumble 64bit\" /Y
xcopy "%releasex64_path%\dinput8.dll" "%releasex86_path%\Initial D Zero v131\" /Y
xcopy "%releasex64_path%\SDL2.dll" "%releasex86_path%\Initial D Zero v131\" /Y
xcopy "%releasex64_path%\dinput8.dll" "%releasex86_path%\Initial D Zero v211\" /Y
xcopy "%releasex64_path%\SDL2.dll" "%releasex86_path%\Initial D Zero v211\" /Y
xcopy "%releasex64_path%\dinput8.dll" "%releasex86_path%\Initial D Zero v230\" /Y
xcopy "%releasex64_path%\SDL2.dll" "%releasex86_path%\Initial D Zero v230\" /Y
xcopy "%releasex64_path%\dinput8.dll" "%releasex86_path%\Sega World Drivers Championship 2018\" /Y
xcopy "%releasex64_path%\SDL2.dll" "%releasex86_path%\Sega World Drivers Championship 2018\" /Y
xcopy "%releasex64_path%\dinput8.dll" "%releasex86_path%\MAME 64bit Outputs\" /Y
xcopy "%releasex64_path%\SDL2.dll" "%releasex86_path%\MAME 64bit Outputs\" /Y
xcopy "%releasex64_path%\dinput8.dll" "%releasex86_path%\PokkenTournament\" /Y
xcopy "%releasex64_path%\SDL2.dll" "%releasex86_path%\PokkenTournament\" /Y
xcopy "%releasex64_path%\dinput8.dll" "%releasex86_path%\Supermodel 64bit Outputs\" /Y
xcopy "%releasex64_path%\SDL2.dll" "%releasex86_path%\Supermodel 64bit Outputs\" /Y
xcopy "%releasex64_path%\dinput8.dll" "%releasex86_path%\Spring Effect 64bit\" /Y
xcopy "%releasex64_path%\SDL2.dll" "%releasex86_path%\Spring Effect 64bit\" /Y
xcopy "%releasex64_path%\dinput8.dll" "%releasex86_path%\Flycast\" /Y
xcopy "%releasex64_path%\SDL2.dll" "%releasex86_path%\Flycast\" /Y
xcopy "%releasex64_path%\dinput8.dll" "%releasex86_path%\WMMT5\" /Y
xcopy "%releasex64_path%\SDL2.dll" "%releasex86_path%\WMMT5\" /Y
xcopy "%releasex64_path%\dinput8.dll" "%releasex86_path%\WMMT5DX\" /Y
xcopy "%releasex64_path%\SDL2.dll" "%releasex86_path%\WMMT5DX\" /Y
xcopy "%releasex64_path%\dinput8.dll" "%releasex86_path%\WMMT5DX+\" /Y
xcopy "%releasex64_path%\SDL2.dll" "%releasex86_path%\WMMT5DX+\" /Y
xcopy "%releasex64_path%\dinput8.dll" "%releasex86_path%\WMMT6\" /Y
xcopy "%releasex64_path%\SDL2.dll" "%releasex86_path%\WMMT6\" /Y
xcopy "%releasex64_path%\dinput8.dll" "%releasex86_path%\WMMT6R\" /Y
xcopy "%releasex64_path%\SDL2.dll" "%releasex86_path%\WMMT6R\" /Y
xcopy "%releasex64_path%\dinput8.dll" "%releasex86_path%\WMMT6RR\" /Y
xcopy "%releasex64_path%\SDL2.dll" "%releasex86_path%\WMMT6RR\" /Y
xcopy ".\MAME32.dll" "%releasex86_path%\MAME 32bit Outputs\" /Y
xcopy ".\MAME64.dll" "%releasex86_path%\MAME 64bit Outputs\" /Y
xcopy ".\MAME32.dll" "%releasex86_path%\Supermodel 32bit Outputs\" /Y
xcopy ".\MAME64.dll" "%releasex86_path%\Supermodel 64bit Outputs\" /Y
xcopy ".\MAME64.dll" "%releasex86_path%\Flycast\" /Y

:: Go back to root path
cd %releasex86_path%

:: Now copy all files to release x86 directory
cd "Afterburner Climax"
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd "Aliens Extermination"
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd "Arctic Thunder"
rename dinput8.dll d3d9.dll
cd %releasex86_path%

cd "Batman"
rename dinput8.dll d3d11.dll
cd %releasex86_path%

cd "Battle Gear 4 Tuned"
rename dinput8.dll d3d9.dll
cd %releasex86_path%

cd Cruisn Blast
rename dinput8.dll winmm.dll
cd %releasex86_path%

cd Crazy Taxi Steam
rename dinput8.dll xinput1_3.dll
cd %releasex86_path%

cd Chase HQ 2
rename dinput8.dll d3d9.dll
cd %releasex86_path%

cd Dead Heat
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd Dead Heat Riders
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd Gaelco Tuning Race
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd Tokyo Cop
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd Ring Riders
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd D1GP Arcade
rename dinput8.dll winmm.dll
cd %releasex86_path%

cd Demul
rename dinput8.dll winmm.dll
cd %releasex86_path%

cd Dirty Drivin'
rename dinput8.dll d3d9.dll
cd %releasex86_path%

cd FnF
rename dinput8.dll dinput8.dll
cd %releasex86_path%

cd FnFDrift
rename dinput8.dll dinput8.dll
cd %releasex86_path%

cd FnFSuperCars
rename dinput8.dll dinput8.dll
cd %releasex86_path%

cd H2Overdrive
rename dinput8.dll d3d9.dll
cd %releasex86_path%

cd "Hummer Extreme"
rename dinput8.dll winmm.dll
cd %releasex86_path%

cd HOTD4
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd Flycast
rename dinput8.dll winmm.dll
cd %releasex86_path%

cd "Initial D 4"
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd "Initial D 4 Japan"
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd "Initial D 5"
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd "Initial D The Arcade v231"
rename dinput8.dll d3d11.dll
cd %releasex86_path%

cd KODrive
rename dinput8.dll d3d9.dll
cd %releasex86_path%

cd LGI
rename dinput8.dll d3d9.dll
cd %releasex86_path%

cd LGI3D
rename dinput8.dll d3d9.dll
cd %releasex86_path%

cd Machstorm
rename dinput8.dll xinput1_3.dll
cd %releasex86_path%

cd "Outrun 2 Special Tours Deluxe Custom"
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd "Outrun 2 Special Tours Deluxe Real"
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd Rambo
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd R-Tuned
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd "Sega Race TV"
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd "Sega World Drivers Championship 2018"
rename dinput8.dll d3d11.dll
cd %releasex86_path%

cd "Sonic & Sega All Stars Racing"
rename dinput8.dll d3d9.dll
cd %releasex86_path%

cd "Storm Racer G"
rename dinput8.dll xinput1_3.dll
cd %releasex86_path%

cd "Supermodel 32bit Outputs"
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd "Supermodel 64bit Outputs"
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd "Wacky Races"
rename dinput8.dll d3d9.dll
cd %releasex86_path%

cd WMMT3
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd "WMMT3DX+"
rename dinput8.dll opengl32.dll
cd %releasex86_path%

cd WMMT5
rename dinput8.dll d3d11.dll
cd %releasex86_path%

cd WMMT5DX
rename dinput8.dll d3d11.dll
cd %releasex86_path%

cd "WMMT5DX+"
rename dinput8.dll d3d11.dll
cd %releasex86_path%

cd WMMT6
rename dinput8.dll d3d11.dll
cd %releasex86_path%

cd WMMT6R
rename dinput8.dll d3d11.dll
cd %releasex86_path%

cd WMMT6RR
rename dinput8.dll d3d11.dll
cd %releasex86_path%


del dinput8.exp
del dinput8.lib
del SDL2.dll
rd /s /q ".\DInput8Wrapper\"

cd %start_path%
