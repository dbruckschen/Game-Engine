#ifndef INPUT_H
#define INPUT_H

#include <windows.h>
#include "mathlib.h"

struct Input
{
    int mouse_x;
    int mouse_y;
	
    bool left_click;
    bool left_click_locked;
    bool right_click;
};

v2 GetMousePosition(HWND window);

#endif // INPUT_H
