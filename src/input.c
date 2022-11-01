#include "input.h"

v2 GetMousePosition(HWND window) {
    POINT mouse_pos = {0};
    v2 position = {0};

    GetCursorPos(&mouse_pos);

    ScreenToClient(window, &mouse_pos);
    position.x = (float)mouse_pos.x;
    position.y = (float)mouse_pos.y;

    return position;
}

void GetKeyboardInput(struct Input *input, MSG msg) {
	input->keyboard[msg.wParam].down = true;
	bool was_down_last_frame = (msg.lParam >> 30) & 1;

	// the high order bit of a 16bit value is 1 if the button is down, otherwise zero
	int16_t lshift_status = GetAsyncKeyState(VK_LSHIFT);
	bool lshift_pressed = lshift_status & 0x8000;
	input->keyboard[left_shift_key].down = lshift_pressed;
	
	if(was_down_last_frame) {
		input->keyboard[msg.wParam].pressed_this_frame = false;
		input->keyboard[msg.wParam].down_previous_frame = false;
	}
	else {
		input->keyboard[msg.wParam].down_previous_frame = true;
		input->keyboard[msg.wParam].pressed_this_frame = true;
	}

	if (input->keyboard[msg.wParam].toggle == false && was_down_last_frame == false) {
		input->keyboard[msg.wParam].toggle = true;
	}
	else if (input->keyboard[msg.wParam].toggle == true && was_down_last_frame == false) {
		input->keyboard[msg.wParam].toggle = false;
	}
}

void ResetInput(struct Input *input) {
	input->mouse_cursor_pos.x = 0;
	input->mouse_cursor_pos.y = 0;
	input->left_click_down  = false;
	input->right_click_down = false;

	for(int i = 0; i < MAX_KEYS; i++) {
		input->keyboard[i].down = false;
		input->keyboard[i].down_previous_frame = false;
		input->keyboard[i].pressed_this_frame = false;
		input->keyboard[i].toggle = false;
	}
}
