echo on
:: Change directory to the first argument 
cd %1
:: Scan all sub-directories and perform xcopy of dll files in (.) to them
for /D /r %%d in (.) do @xcopy dinput8.dll "%%d\" /y
for /D /r %%d in (.) do @xcopy SDL2.dll "%%d\" /y
:: Scan all sub-directories and perform xcopy of FFBPluginReadme.txt file in (.) to them
for /D /r %%d in (.) do @xcopy FFBPluginReadme.txt "%%d\" /y

:: Remove build files that have been copied
::del %1\*.dll
del %1\dinput8.dll
del %1\SDL2.dll
del %1\FFBPluginReadme.txt
