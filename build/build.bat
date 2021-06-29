@echo off
REM /c -> only compile
REM *.c -> compile all .c files
REM /I include header files
REM /Fo -> obj file folder for tmp used files
REM link obj files with other lib files and create rml.dll


cl /c ..\src\*.c /I ..\include /Fo"..\obj\\"
link /DLL /OUT:rml.dll ..\obj\*.obj user32.lib gdi32.lib Ws2_32.lib
cl ..\src\main.c /I ..\include ..\build\rml.lib 

