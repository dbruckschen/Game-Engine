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
	
	struct Font font = {0};
	font.bmp = LoadBitmapFile("font.bmp");
	font.glyph_width = 5;
	font.glyph_height = 7;
	font.glyph_count = 98;
	font.glyph_spacing = 2;
	font.color_mask = RGB_Color(255, 255, 255); // i don't need a color mask, but 0 will mask off white
		
	u32 background_color = RGB_Color(12, 12, 12);
	u32 input_bar_color = RGB_Color(155, 55, 55);
	u32 text_color = RGB_Color(255, 255, 244);
	u32 cursor_color = RGB_Color(145, 237, 145);
	
	struct TextField chat_input_field = {0};
	chat_input_field = InitTextField(&font, 0, window_height - 25, window_width, 25, background_color, 0, input_bar_color, .3f, 5, 17, .5, cursor_color, text_color, false);
	chat_input_field.write_focus = true;

	char chat_buff[CHAT_MAX_BUFF_ROWS][CHAT_MAX_BUFF_COLS];
	int current_buff_line = 0;
	int chat_text_x = 25;
	int chat_text_y = 50;
	
	bool running = true;
	while(running) {
		StartTimer(&main_timer);

		ResetInput(&input);
		
		input.mouse_cursor_pos = GetMousePosition(window.wnd_h);
		
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
			printf("hello\n");
			chat_input_field.text[0] = '\0';
			chat_input_field.text_current_len = 0;
			chat_input_field.cursor.pos = chat_input_field.cursor.initial_pos;
		}

		for(int i = 0; i < current_buff_line; i++) {
			DrawString(framebuffer, font, "Admin", chat_text_x, chat_text_y+(i*12), text_color);
			DrawString(framebuffer, font, chat_buff[i], chat_text_x + 50, chat_text_y+(i * 12), text_color);
		}
		
		DrawTextField(framebuffer, &chat_input_field);
		
		OutputFramebuffer(window.wnd_h, *framebuffer);

		EndTimer(&main_timer);
	}
	
    return 0;
}
