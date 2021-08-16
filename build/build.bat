@echo off
cls

set dllfiles=..\src\common.c ..\src\draw.c ..\src\mathlib.c ..\src\network.c ..\src\timing.c ..\src\window.c

REM compile dll 
cl -fsanitize=address /Zi /W4 /c %dllfiles% /I ..\include /Fo"..\obj\\"
link /DEBUG /DLL /OUT:rml.dll ..\obj\*.obj user32.lib gdi32.lib Ws2_32.lib

REM compile test program  
cl -fsanitize=address /Zi /W4 ..\test\main.c /I ..\include ..\build\rml.lib 

