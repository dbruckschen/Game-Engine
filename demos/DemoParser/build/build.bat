@echo off

cls

cl /Zi /FC /W4 /Fe:DemoParser.exe ../src/main.c /I ../../../include /link ../../../build/engine.lib 

