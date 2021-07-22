@echo off

REM /c -> only compile
REM *.c -> compile all .c files
REM /I include header files
REM /Fo -> obj file folder for tmp used files
REM link obj files with other lib files and create rml.dll

set dllfiles=..\src\common.c ..\src\draw.c ..\src\mathlib.c ..\src\network.c ..\src\timing.c ..\src\window.c

REM dll compiling
cl -fsanitize=address /Zi /W4 /c %dllfiles% /I ..\include /Fo"..\obj\\"
link /DEBUG /DLL /OUT:rml.dll ..\obj\*.obj user32.lib gdi32.lib Ws2_32.lib

REM test project 
cl -fsanitize=address /Zi /W4 ..\test\main.c /I ..\include ..\build\rml.lib 

