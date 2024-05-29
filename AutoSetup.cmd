@echo on
SET current_path="%CD%"
SET releasex86_path=%current_path%\Release.x64
SET releasex64_path=%current_path%\Release.Win32
SET VS=2022\Community
SET BuildMode=Release
SET BUILDER=%ProgramFiles%\Microsoft Visual Studio\%VS%\MSBuild\Current\Bin\MSBuild.exe

set /p Build=<Version.txt

:build86
:: Restore nuget packages
"%BUILDER%" -t:restore Dinput8Wrapper.sln /p:Platform=x86;Configuration=%BuildMode% -p:RestorePackagesConfig=true 
:: Build
::"%BUILDER%"  Dinput8Wrapper.sln  /maxcpucount:1 /t:clean /p:Platform=x86;Configuration=%BuildMode%
"%BUILDER%"  Dinput8Wrapper.sln  /maxcpucount:4  /p:Platform=x86;Configuration=%BuildMode%

:build64
:: Restore nuget packages
"%BUILDER%" -t:restore Dinput8Wrapper.sln /p:Platform=x64;Configuration=%BuildMode% -p:RestorePackagesConfig=true 
:: Build
::"%BUILDER%"  Dinput8Wrapper.sln  /maxcpucount:1 /t:clean /p:Platform=x64;Configuration=%BuildMode%
"%BUILDER%"  Dinput8Wrapper.sln  /maxcpucount:4  /p:Platform=x64;Configuration=%BuildMode%

xcopy ".\Release.x64\dinput8.dll" ".\Release.Win32\Button Rumble 64bit\" /Y
xcopy ".\Release.x64\SDL2.dll" ".\Release.Win32\Button Rumble 64bit\" /Y
xcopy ".\Release.x64\dinput8.dll" ".\Release.Win32\Initial D Zero v131\" /Y
xcopy ".\Release.x64\SDL2.dll" ".\Release.Win32\Initial D Zero v131\" /Y
xcopy ".\Release.x64\dinput8.dll" ".\Release.Win32\Initial D Zero v211\" /Y
xcopy ".\Release.x64\SDL2.dll" ".\Release.Win32\Initial D Zero v211\" /Y
xcopy ".\Release.x64\dinput8.dll" ".\Release.Win32\Initial D Zero v230\" /Y
xcopy ".\Release.x64\SDL2.dll" ".\Release.Win32\Initial D Zero v230\" /Y
xcopy ".\Release.x64\dinput8.dll" ".\Release.Win32\Sega World Drivers Championship 2018\" /Y
xcopy ".\Release.x64\SDL2.dll" ".\Release.Win32\Sega World Drivers Championship 2018\" /Y
xcopy ".\Release.x64\dinput8.dll" ".\Release.Win32\MAME 64bit Outputs\" /Y
xcopy ".\Release.x64\SDL2.dll" ".\Release.Win32\MAME 64bit Outputs\" /Y
xcopy ".\Release.x64\dinput8.dll" ".\Release.Win32\PokkenTournament\" /Y
xcopy ".\Release.x64\SDL2.dll" ".\Release.Win32\PokkenTournament\" /Y
xcopy ".\Release.x64\dinput8.dll" ".\Release.Win32\Supermodel 64bit Outputs\" /Y
xcopy ".\Release.x64\SDL2.dll" ".\Release.Win32\Supermodel 64bit Outputs\" /Y
xcopy ".\Release.x64\dinput8.dll" ".\Release.Win32\Spring Effect 64bit\" /Y
xcopy ".\Release.x64\SDL2.dll" ".\Release.Win32\Spring Effect 64bit\" /Y
xcopy ".\Release.x64\dinput8.dll" ".\Release.Win32\Flycast\" /Y
xcopy ".\Release.x64\SDL2.dll" ".\Release.Win32\Flycast\" /Y
xcopy ".\Release.x64\dinput8.dll" ".\Release.Win32\WMMT5\" /Y
xcopy ".\Release.x64\SDL2.dll" ".\Release.Win32\WMMT5\" /Y
xcopy ".\Release.x64\dinput8.dll" ".\Release.Win32\WMMT5DX\" /Y
xcopy ".\Release.x64\SDL2.dll" ".\Release.Win32\WMMT5DX\" /Y
xcopy ".\Release.x64\dinput8.dll" ".\Release.Win32\WMMT5DX+\" /Y
xcopy ".\Release.x64\SDL2.dll" ".\Release.Win32\WMMT5DX+\" /Y
xcopy ".\Release.x64\dinput8.dll" ".\Release.Win32\WMMT6\" /Y
xcopy ".\Release.x64\SDL2.dll" ".\Release.Win32\WMMT6\" /Y
xcopy ".\Release.x64\dinput8.dll" ".\Release.Win32\WMMT6R\" /Y
xcopy ".\Release.x64\SDL2.dll" ".\Release.Win32\WMMT6R\" /Y
xcopy ".\MAME32.dll" ".\Release.Win32\MAME 32bit Outputs\" /Y
xcopy ".\MAME64.dll" ".\Release.Win32\MAME 64bit Outputs\" /Y
xcopy ".\MAME32.dll" ".\Release.Win32\Supermodel 32bit Outputs\" /Y
xcopy ".\MAME64.dll" ".\Release.Win32\Supermodel 64bit Outputs\" /Y
xcopy ".\MAME64.dll" ".\Release.Win32\Flycast\" /Y

cd %releasex86_path%
cd "Afterburner Climax"
rename dinput8.dll opengl32.dll

CD "%releasex86_path%"
cd "Aliens Extermination"
rename dinput8.dll opengl32.dll

CD "%current_path%"
cd "Arctic Thunder"
rename dinput8.dll d3d9.dll
CD "%current_path%"

cd "Batman"
rename dinput8.dll d3d11.dll
CD "%current_path%"

cd "Battle Gear 4 Tuned"
rename dinput8.dll d3d9.dll
CD "%current_path%"

cd Cruisn Blast
rename dinput8.dll winmm.dll
CD "%current_path%"

cd Crazy Taxi Steam
rename dinput8.dll xinput1_3.dll
CD "%current_path%"

cd Chase HQ 2
rename dinput8.dll d3d9.dll
CD "%current_path%"

cd Dead Heat
rename dinput8.dll opengl32.dll
CD "%current_path%"

cd Dead Heat Riders
rename dinput8.dll opengl32.dll
CD "%current_path%"

cd Gaelco Tuning Race
rename dinput8.dll opengl32.dll
CD "%current_path%"

cd Tokyo Cop
rename dinput8.dll opengl32.dll
CD "%current_path%"

cd Ring Riders
rename dinput8.dll opengl32.dll
CD "%current_path%"

cd D1GP Arcade
rename dinput8.dll winmm.dll
CD "%current_path%"

cd Demul
rename dinput8.dll winmm.dll
CD "%current_path%"

cd Dirty Drivin'
rename dinput8.dll d3d9.dll
CD "%current_path%"

cd FnF
rename dinput8.dll dinput8.dll
CD "%current_path%"

cd FnFDrift
rename dinput8.dll dinput8.dll
CD "%current_path%"

cd FnFSuperCars
rename dinput8.dll dinput8.dll
CD "%current_path%"

cd H2Overdrive
rename dinput8.dll d3d9.dll
CD "%current_path%"

cd HOTD4
rename dinput8.dll opengl32.dll
CD "%current_path%"

cd Flycast
rename dinput8.dll winmm.dll
CD "%current_path%"

cd Initial D 4
rename dinput8.dll opengl32.dll
CD "%current_path%"

cd Initial D 4 Japan
rename dinput8.dll opengl32.dll
CD "%current_path%"

cd Initial D 5
rename dinput8.dll opengl32.dll
CD "%current_path%"

cd KODrive
rename dinput8.dll d3d9.dll
CD "%current_path%"

cd LGI
rename dinput8.dll d3d9.dll
CD "%current_path%"

cd LGI3D
rename dinput8.dll d3d9.dll
CD "%current_path%"

cd Machstorm
rename dinput8.dll xinput1_3.dll
CD "%current_path%"

cd Outrun 2 Special Tours Deluxe Custom
rename dinput8.dll opengl32.dll
CD "%current_path%"

cd Outrun 2 Special Tours Deluxe Real
rename dinput8.dll opengl32.dll
CD "%current_path%"

cd Rambo
rename dinput8.dll opengl32.dll
CD "%current_path%"

cd R-Tuned
rename dinput8.dll opengl32.dll
CD "%current_path%"

cd "Sega World Drivers Championship 2018"
rename dinput8.dll d3d11.dll
CD "%current_path%"

cd "Sonic & Sega All Stars Racing"
rename dinput8.dll d3d9.dll
CD "%current_path%"

cd "Storm Racer G"
rename dinput8.dll xinput1_3.dll
CD "%current_path%"

cd "Supermodel 32bit Outputs"
rename dinput8.dll opengl32.dll
CD "%current_path%"

cd "Supermodel 64bit Outputs"
rename dinput8.dll opengl32.dll
CD "%current_path%"

cd "Wacky Races"
rename dinput8.dll d3d9.dll
CD "%current_path%"

cd WMMT3
rename dinput8.dll opengl32.dll
CD "%current_path%"

cd "WMMT3DX+"
rename dinput8.dll opengl32.dll
CD "%current_path%"

cd WMMT5
rename dinput8.dll d3d11.dll
CD "%current_path%"

cd WMMT5DX
rename dinput8.dll d3d11.dll
CD "%current_path%"

cd "WMMT5DX+"
rename dinput8.dll d3d11.dll
CD "%current_path%"

cd WMMT6
rename dinput8.dll d3d11.dll
CD "%current_path%"

cd WMMT6R
rename dinput8.dll d3d11.dll
CD "%current_path%"

del dinput8.exp
del dinput8.lib
del SDL2.dll
rd /s /q ".\DInput8Wrapper\"
"C:\Program Files\7-Zip\7z.exe" a ".\FFB Arcade Plugin %Build%.zip" ".\*"
move ".\FFB Arcade Plugin %Build%.zip" "..\"
"C:\Program Files\7-Zip\7z.exe" a ".\FFB Arcade Plugin %Build%.7z" ".\*"
move ".\FFB Arcade Plugin %Build%.7z" "..\"
EXIT

