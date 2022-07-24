@echo off
cls

set dllfiles=../src/common.c ../src/draw.c ../src/mathlib.c ../src/timing.c ../src/window.c ../src/input.c ../src/ui.c ../src/linked_list.c

REM compile dll 
cl /Zi /W4 /c %dllfiles% /I ..\include /Fo"..\obj\\"
link /DEBUG /DLL /OUT:engine.dll ..\obj\*.obj user32.lib gdi32.lib Ws2_32.lib

REM copy the .lib to the test project so it always uses the newest file
copy engine.lib ..\test\build
copy engine.dll ..\test\build


