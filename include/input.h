#ifndef INPUT_H
#define INPUT_H

#include <windows.h>
#include "..\include\mathlib.h"

typedef struct
{
    bool keyboard[256];
    
} Keyboard;

typedef struct 
{
    int mouse_x;
    int mouse_y;
	
    bool left_click;
    bool left_click_locked;
    bool right_click;
    
} Input;

__declspec(dllexport) v2 GetMousePosition(HWND window);
__declspec(dllexport) void GetInput(Input *input);

#endif // INPUT_H
