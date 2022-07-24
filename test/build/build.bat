REM compile test program  
cls
cl /Zi /W4 /Fe:test.exe ../src/main.c ../src/game.c /I ../../include /link rml.lib

