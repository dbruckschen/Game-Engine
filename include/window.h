#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> // maybe already in <window.h> ?
#include "input.h"

typedef struct 
{
    HWND wnd_h;
    WNDCLASSEX wc;
    int width;
    int height;
	
} Window;

__declspec(dllexport) LRESULT CALLBACK WindowProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);

// Returns false if application should stop running.
// MessageLoop function is not supposed to poll the input in the future. It's the easiest
// solution right now.
__declspec(dllexport) bool MessageLoop(Input *input);
__declspec(dllexport) Window OpenWindow(int w, int h, char *title);

#endif //WINDOW_H
