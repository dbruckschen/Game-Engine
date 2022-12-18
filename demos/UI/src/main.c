#include <stdio.h>

#include "network.h"
#include "window.h"
#include "draw.h"
#include "timing.h"
#include "input.h"
#include "ui.h"
#include "common.h"
#include "mathlib.h"

bool MouseRectangleCollision(struct Input *input, struct Rectangle rec) {
	return BBAA(input->mouse_cursor_pos, 1, 1, V2((float)rec.x, (float)rec.y), rec.w, rec.h);
}

int main(void) {
	struct Window window = OpenWindow(800, 600, "UI demo");
	struct Input input = {0};
	struct Framebuffer *framebuffer = NULL;
	struct Font font = {0};
	
	framebuffer = CreateFramebuffer(window.wnd_h);
	font.bmp = LoadBitmapFile("font.bmp");

	// panel and scrollbar 
	int panel_x = 100;
	int panel_y = 100;
	int panel_w = 200;
	int panel_h = 300;

	int bar_w = 30;
	int bar_h = panel_h;
	int bar_x = panel_x + panel_w - bar_w;
	int bar_y = panel_y;

	int scroll_w = bar_w;
	int scroll_h = 30;
	int scroll_x = bar_x;
	int scroll_y = bar_y;
	int scroll_min_y = bar_y;
	int scroll_max_y = bar_y + bar_h - scroll_h;

	int old_y = (int)input.mouse_cursor_pos.y;
	int new_y = 0;
	int dy = 0;

	bool running = true;
	while(running) {
		running = MessageLoop(&input);

		input.mouse_cursor_pos = GetMousePosition(window.wnd_h);
		new_y = (int)input.mouse_cursor_pos.y;

		dy = new_y - old_y;

		FillScreen(framebuffer, RGB_Color(255, 0, 0));
		
		// Draw UI element with scrollbar
		struct Rectangle scroll_rec = {scroll_x, scroll_y, scroll_w, scroll_h, RGB_Color(0, 0, 0)};
		bool collision = MouseRectangleCollision(&input, scroll_rec);

		if(collision && input.left_click_down) {
			scroll_y += dy;

			if(scroll_y >= scroll_max_y) {
				scroll_y = scroll_max_y;
			}
			else if(scroll_y <= scroll_min_y) {
				scroll_y = scroll_min_y;
			}
		}
		
		DrawRectangle(framebuffer, panel_x, panel_y, panel_w, panel_h, RGB_Color(0, 255, 0));
		DrawRectangle(framebuffer, bar_x, bar_y, bar_w, bar_h, RGB_Color(0, 0, 255));
		DrawRectangle(framebuffer, scroll_x, scroll_y, scroll_w, scroll_h, RGB_Color(0, 100, 100));

		DrawString(framebuffer, font, "Hello World\n", 100, 100, RGB_Color(0, 255, 0));

		OutputFramebuffer(window.wnd_h, *framebuffer);

		old_y = new_y;
	}

	DestroyFramebuffer(framebuffer);

	return 0;
}
