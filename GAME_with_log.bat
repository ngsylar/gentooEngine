@echo off

xcopy "extlib\*.dll"
GAME.exe
del "*.dll" /f /q
cmd /k