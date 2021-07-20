#include "input.h"

v2 GetMousePosition(HWND window)
{
    POINT mouse_pos = {0};
    v2 position = {0};
    
    GetCursorPos(&mouse_pos);

    ScreenToClient(window, &mouse_pos);
    position.x = (float)mouse_pos.x;
    position.y = (float)mouse_pos.y;

    return position;
}
