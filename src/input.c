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

void GetKeyboardInput(struct Input *input, MSG msg)
{
	input->keyboard[msg.wParam].down = true;
	//printf("0x%x pressed\n", msg.wParam);

	bool was_down_last_frame = (msg.lParam >> 30) & 1;

	if(!was_down_last_frame) {
		input->keyboard[msg.wParam].down_previous_frame = true;
		//printf("0x%x pressed this frame\n", msg.wParam);
	} else {
		input->keyboard[msg.wParam].down_previous_frame = false;
	}

	if (input->keyboard[msg.wParam].toggle == false && was_down_last_frame == false) {
		printf("toggle an\n");
		input->keyboard[msg.wParam].toggle = true;
	} else if (input->keyboard[msg.wParam].toggle == true && was_down_last_frame == false) {
		printf("toggle aus\n");
		input->keyboard[msg.wParam].toggle = false;
	}
}
