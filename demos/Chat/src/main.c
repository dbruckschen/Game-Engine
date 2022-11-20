#include "network.h"
#include "window.h"
#include "draw.h"
#include "timing.h"
#include "input.h"
#include "ui.h"
#include "common.h"
#include "mathlib.h"

#include <assert.h>

#define CHAT_MAX_BUFF_ROWS 1000
#define CHAT_MAX_BUFF_COLS MAX_TEXTFIELD_LEN

void copy_input_to_chat_buff(char *src, int src_len, char buff[CHAT_MAX_BUFF_ROWS][CHAT_MAX_BUFF_COLS], int buff_index) {
	if(src[0] != '\0') {
		assert(src_len <= CHAT_MAX_BUFF_COLS);
	
		for(int i = 0; i < src_len; i++) {
			buff[buff_index][i] = src[i];
		}
		buff[buff_index][src_len] = '\0';
	}
}

int main(void) {
	int window_width = 800;
	int window_height = 600;
	
	struct Window window = OpenWindow(window_width, window_height, "Chat Demo");
	struct Input input = {0};
	struct Framebuffer *framebuffer = CreateFramebuffer(window.wnd_h);
	struct Timer main_timer = {0};
	InitTimer(&main_timer);
	SYSTEMTIME system_time = {0};
	
	struct Font font = {0};
	font.bmp = LoadBitmapFile("font.bmp");
	font.glyph_width = 5;
	font.glyph_height = 7;
	font.glyph_count = 98;
	font.glyph_spacing = 2;
	font.color_mask = RGB_Color(255, 255, 255); // i don't need a color mask, but 0 will mask off white

	char ip[] = "127.0.0.1";
	bool socket_init_success = InitializeWinsock(ip);
	if(socket_init_success) {
		printf("successfuly initalized winsock\n");
	}
	
	u32 background_color = RGB_Color(245, 245, 245);
	u32 input_bar_color = RGB_Color(255, 255, 255);
	u32 text_color = RGB_Color(0, 0, 0);
	u32 cursor_color = RGB_Color(20, 20, 20);
	int border_thickness = 4;
	u32 border_color = RGB_Color(204, 204, 206);
	int input_bar_w = window_width - (border_thickness*2);
	int input_bar_h = 25;
	int input_bar_x = border_thickness;
	int input_bar_y = window_height - (border_thickness+input_bar_h);
	float input_bar_delay = .3f;
	int input_bar_cursor_width = 4;
	int input_bar_cursor_height = 12;
	float input_bar_cursor_blinkrate = .5f;
	bool draw_placeholder_string = false;
		
	struct TextField chat_input_field = {0};
	chat_input_field = InitTextField(&font, input_bar_x, input_bar_y, input_bar_w, input_bar_h, input_bar_color, border_thickness, border_color, input_bar_delay,
									 input_bar_cursor_width, input_bar_cursor_height, input_bar_cursor_blinkrate, cursor_color, text_color, draw_placeholder_string);
	chat_input_field.write_focus = true;

	char chat_buff[CHAT_MAX_BUFF_ROWS][CHAT_MAX_BUFF_COLS];
	int current_buff_line = 0;
	int chat_text_x = 25;
	int chat_text_y = 50;

	u32 user_color = RGB_Color(255, 0, 0);

	bool running = true;
	while(running) {
		StartTimer(&main_timer);
		
		ResetInput(&input);
		
		input.mouse_cursor_pos = GetMousePosition(window.wnd_h);

		GetSystemTime(&system_time);
			
		if(!MessageLoop(&input)) {
			running = false;
		}

		FillScreen(framebuffer, background_color);

		UpdateTextField(&chat_input_field, input, main_timer.elapsed_time);
		
		if(input.keyboard[enter_key].down) {
			// copy input to chat
			assert(current_buff_line <= CHAT_MAX_BUFF_ROWS);
			copy_input_to_chat_buff(chat_input_field.text, chat_input_field.text_current_len, chat_buff, current_buff_line);

			current_buff_line++;
			
			// clear text field buff
			chat_input_field.text[0] = '\0';
			chat_input_field.text_current_len = 0;
			chat_input_field.cursor.pos = chat_input_field.cursor.initial_pos;
		}

		for(int i = 0; i < current_buff_line; i++) {
			DrawString(framebuffer, font, "Admin:", chat_text_x, chat_text_y+(i*12), user_color);
			DrawString(framebuffer, font, chat_buff[i], chat_text_x + 50, chat_text_y+(i * 12), text_color);
		}

		char time[64];
		IntToString(system_time.wHour, time, 64);
		time[2] = ':';
		IntToString(system_time.wMinute, time+3, 64);
		time[5] = ':';
		IntToString(system_time.wSecond, time+6, 64);
		
		DrawString(framebuffer, font, time, 400, 400, user_color);
		//DrawString(framebuffer, font, ":", 400, 400, user_color);
		
		DrawTextField(framebuffer, &chat_input_field);
		
		OutputFramebuffer(window.wnd_h, *framebuffer);

		EndTimer(&main_timer);
	}
	
    return 0;
}
