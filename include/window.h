#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include <stdbool.h>
#include <stdlib.h> // maybe already in <window.h> ?

typedef struct 
{
    HWND wnd_h;
    WNDCLASSEX wc;
	
} Window;

typedef struct
{
    int i;

} Keyboard;

__declspec(dllexport) LRESULT CALLBACK WindowProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);

// Returns false if application should stop running
__declspec(dllexport) bool MessageLoop(void);
__declspec(dllexport) Window OpenWindow(int w, int h, char *title);

#endif //WINDOW_H