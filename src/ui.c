#include "ui.h"

struct Button InitTextButton(struct Font *font, int x, int y, int width, int height, char *text, u32 color, int border_thickness, u32 border_color, float delay_time) {
	struct Button btn = {0};

	btn.x = x;
	btn.y = y;
	btn.width = width;
	btn.height = height;
	btn.font = font;

	size_t str_len = StringLen(text);
	StringCpy(btn.text, text, str_len);
	btn.color = color;
	btn.border_thickness = border_thickness;
	btn.border_color = border_color;
	btn.active = true;
	btn.hover = false;
	btn.toggle = false;
	btn.delay_time = delay_time;
	btn.delay_timer = delay_time;

	return btn;
}

void UpdateButtonStatus(struct Button *btn, struct Input input, double dt) {
	if(!input.left_click_down) {
		btn->delay_timer += dt;
	}
	
	v2 b1 = {(float)btn->x, (float)btn->y};
	int mouse_ptr_w = 1;
	int mouse_ptr_h = 1;

	/* check if mouse is hovering over button */
	if((BBAA(input.mouse_cursor_pos, mouse_ptr_w, mouse_ptr_h, b1, btn->width, btn->height)) &&
	   (btn->active)) {
		btn->hover = true;
		/* check for click event */
		if((input.left_click_down) && (!btn->toggle) && (btn->delay_timer >= btn->delay_time)) {
			btn->toggle = true;
			btn->delay_timer = 0;
		}
		else if(input.left_click_down && btn->toggle && (btn->delay_timer >= btn->delay_time)) {
			btn->toggle = false;
			btn->delay_timer = 0;
		}
	}
	else {
		btn->hover = false;
	}
}

void DrawTextButton(struct Framebuffer *fb, struct Button *btn) {

	// this is the border rectangle of the button
	DrawRectangle(fb,
				  btn->x - btn->border_thickness, btn->y - btn->border_thickness,
				  btn->width + (btn->border_thickness*2), btn->height + (btn->border_thickness*2),
				  btn->border_color);

	// this is the button
	DrawRectangle(fb, btn->x, btn->y, btn->width, btn->height, btn->color);

	size_t string_len = StringLen(btn->text);
	int string_width = (int)string_len * btn->font->glyph_width;
	int font_height = btn->font->glyph_height;
			
	v2 rec_center = GetCenteredCoordinates(btn->x, btn->y, btn->width, btn->height, string_width, font_height);
	
	DrawString(fb, *btn->font, btn->text, (int)rec_center.x, (int)rec_center.y);
}

/* void DrawTextField(struct Framebuffer *fb, struct TextField *tf) { */
/* 	// this is the border rectangle of the text field */
/* 	DrawRectangle(fb, */
/* 				  tf->x - tf->border_thickness, tf->y - tf->border_thickness, */
/* 				  tf->width + (tf->border_thickness*2), tf->height + (tf->border_thickness*2), */
/* 				  tf->border_color); */
	
/* 	// this is the text field */
/* 	DrawRectangle(fb, tf->x, tf->y, tf->width, tf->height, tf->color); */

/* 	// draw filler text */
/* 	DrawString(fb, tf->font, "Text Field", tf->x + tf->font.width); */
/* } */
