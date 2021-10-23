REM compile test program  
cls
cl /Zi /W4 /Fe:test.exe ..\main.c /I ..\..\include /link rml.lib

