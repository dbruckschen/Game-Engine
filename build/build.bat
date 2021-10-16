@echo off
cls

set dllfiles=..\src\common.c ..\src\draw.c ..\src\mathlib.c ..\src\network.c ..\src\timing.c ..\src\window.c

REM compile dll 
cl /Zi /W4 /c %dllfiles% /I ..\include /Fo"..\obj\\"
link /DEBUG /DLL /OUT:rml.dll ..\obj\*.obj user32.lib gdi32.lib Ws2_32.lib

REM copy the .lib to the test project so it always uses the newest file
copy rml.lib ..\test\build
copy rml.dll ..\test\build

