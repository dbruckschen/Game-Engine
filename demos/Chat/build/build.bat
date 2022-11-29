@echo off

cls
cl /Zi /FC /W4 /Fe:chat.exe ../src/main.c /I ../../../include /link ../../../build/engine.lib Ws2_32.lib
