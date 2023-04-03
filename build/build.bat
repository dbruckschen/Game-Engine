@echo off
cls

set dllfiles=../src/common.c ../src/draw.c ../src/mathlib.c ../src/timing.c ../src/window.c ../src/input.c ../src/ui.c ../src/network.c ../src/linked_list.c

REM compile dll 
cl /Zi /FC /W4 /c %dllfiles% /I ../include /Fo"..\obj\\"
link /DEBUG /DLL /OUT:engine.dll ../obj/*.obj user32.lib gdi32.lib Ws2_32.lib

REM copy the .lib to the demo projects so it always uses the newest file
copy engine.lib ..\demos\TetrisClone\build
copy engine.dll ..\demos\TetrisClone\build

copy engine.lib ..\demos\Chat\build
copy engine.dll ..\demos\Chat\build

copy engine.lib ..\demos\UI\build
copy engine.dll ..\demos\UI\build

copy engine.lib ..\demos\DemoParser\build
copy engine.dll ..\demos\DemoParser\build
