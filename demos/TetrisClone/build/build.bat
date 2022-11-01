@echo off

cls

cl /Zi /FC /W4 /Fe:TetrisClone.exe ../src/main.c ../src/game.c /I ../../../include /link ../../../build/engine.lib 

