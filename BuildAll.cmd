@echo on
SET root_path="%CD%"

echo Build plugin
cd FFBArcadePlugin
call AutoSetup.cmd
cd %root_path%

pause

echo Build GUI
cd FFBPluginGUI
call AutoSetup.cmd
cd %root_path%

echo Done
