@echo on
SET start_path="%CD%"

cd FFBArcadePlugin
call AutoSetup.cmd
cd %start_path%

cd FFBPluginGUI
call AutoSetup.cmd
cd %start_path%

echo Done
