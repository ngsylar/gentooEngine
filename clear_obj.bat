echo Script to delete binaries

del "GAME.exe" /f /q
del "bin\*.o" /f /q
del "dep\*.d" /f /q
del "*.dll" /f /q

echo Done!