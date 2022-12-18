#include "ui.h"

struct Button InitTextButton(struct Font *font, u32 text_color, int x, int y, int width, int height, char *text, u32 color, int border_thickness, u32 border_color, float delay_time) {
	struct Button btn = {0};

	btn.x = x;
	btn.y = y;
	btn.width = width;
	btn.height = height;
	btn.font = font;
	btn.text_color = text_color;

	size_t str_len = StringLen(text);
	StringCpy(btn.text, text, str_len);
	btn.color = color;
	btn.border_thickness = border_thickness;
	btn.border_color = border_color;
	btn.active = true;
	btn.clicked = false;
	btn.hover = false;
	btn.toggle = false;
	btn.delay_time = delay_time;
	btn.delay_timer = delay_time;

	return btn;
}

void UpdateButtonStatus(struct Button *btn, struct Input input, double dt) {
	int mouse_ptr_w = 1;
	int mouse_ptr_h = 1;
	v2 b1 = {(float)btn->x, (float)btn->y};
	
	bool collision_with_ui_element = BBAA(input.mouse_cursor_pos, mouse_ptr_w, mouse_ptr_h, b1, btn->width, btn->height);
	
	if(!input.left_click_down || !collision_with_ui_element) {
		btn->delay_timer += dt;
	}
	
	/* check if mouse is hovering over button */
	if(btn->active) {
		if(BBAA(input.mouse_cursor_pos, mouse_ptr_w, mouse_ptr_h, b1, btn->width, btn->height)) {
			btn->hover = true;

			if(input.left_click_down && (btn->delay_timer >= btn->delay_time)) {
				btn->clicked = true;
			}
			else {
				btn->clicked = false;
			}
						
			// button toggle & clicked event
			if((input.left_click_down) && (!btn->toggle) && (btn->delay_timer >= btn->delay_time)) {
				btn->toggle = true;
				btn->delay_timer = 0.0;
			}
			else if(input.left_click_down && btn->toggle && (btn->delay_timer >= btn->delay_time)) {
				btn->toggle = false;
				btn->delay_timer = 0.0;
			}
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

    // TODO: pass the string color to the DrawTextButton
	DrawString(fb, *btn->font, btn->text, (int)rec_center.x, (int)rec_center.y, btn->text_color);
}

struct TextField InitTextField(struct Font *font, int x, int y, int width, int height,
							   u32 color, int border_thickness, u32 border_color, float delay,
							   int cursor_width, int cursor_height, float cursor_blink_rate, u32 cursor_color, u32 text_color, bool draw_placeholder_string) {
	
	struct TextField tf = {0};

	tf.x = x;
	tf.y = y;
	tf.width = width;
	tf.height = height;
	tf.font = font;
	tf.text_current_len = 0;
	tf.text[0] = '\0';
	tf.text_color = text_color;
	tf.draw_initial_placeholder_string = draw_placeholder_string;

	tf.color = color;
	tf.border_thickness = border_thickness;
	tf.border_color = border_color;
	tf.active = true;
	tf.write_focus = false;
	tf.inital_state = true;
	tf.delay_time = delay;
	tf.delay_timer = tf.delay_time;

	v2 cursor_rec = GetCenteredCoordinates(x, y, width, height, cursor_width, cursor_height);

	tf.cursor.initial_pos.x = (float)(x + font->glyph_width);
	tf.cursor.initial_pos.y = (float)cursor_rec.y;
	tf.cursor.pos.x = tf.cursor.initial_pos.x;
	tf.cursor.pos.y = tf.cursor.initial_pos.y;
	tf.cursor.width = cursor_width;
	tf.cursor.height = cursor_height;
	tf.cursor.blink_rate = cursor_blink_rate;
	tf.cursor.blink_timer = 0.0;
	tf.cursor.color = cursor_color;
	
	return tf;
}

void DrawTextField(struct Framebuffer *fb, struct TextField *tf) {
	// this is the border rectangle of the text field
	DrawRectangle(fb, tf->x - tf->border_thickness, tf->y - tf->border_thickness,
				  tf->width + (tf->border_thickness*2), tf->height + (tf->border_thickness*2),
				  tf->border_color);
	
	// this is the text field
	DrawRectangle(fb, tf->x, tf->y, tf->width, tf->height, tf->color);

	size_t string_len = StringLen(tf->text);
	int string_width = (int)string_len * tf->font->glyph_width;
	int font_height = tf->font->glyph_height;
	v2 rec_center = GetCenteredCoordinates(tf->x, tf->y, tf->width, tf->height, string_width, font_height);
	
	if(tf->write_focus) {
		// draw a blinking cursor
		if(tf->cursor.blink_timer >= tf->cursor.blink_rate) {
			DrawRectangle(fb, (int)tf->cursor.pos.x, (int)tf->cursor.pos.y, tf->cursor.width, tf->cursor.height, tf->cursor.color);
			
			if(tf->cursor.blink_timer >= (2*tf->cursor.blink_rate)) {
				tf->cursor.blink_timer = 0.0f;
			}
		}
	}
	
    // draw filler text
	if(!tf->write_focus && tf->inital_state && tf->draw_initial_placeholder_string) {
		// TODO: pass the string color to the DrawTextButton
		DrawString(fb, *tf->font, "placeholder", (int)rec_center.x, (int)rec_center.y, tf->text_color);
	}
	else {
		DrawString(fb, *tf->font, tf->text, (int)tf->x + 5, (int)rec_center.y, tf->text_color);
	}
}

void UpdateTextField(struct TextField *tf, struct Input input, double dt) {
	v2 tf_pos = {(float)tf->x, (float)tf->y};	
	bool collision_with_ui_element = BBAA(input.mouse_cursor_pos, 1, 1, tf_pos, tf->width, tf->height);
	
	tf->cursor.blink_timer += dt;

	if(tf->delay_timer <= tf->delay_time) {
		tf->delay_timer += dt;
	}
 
	if(collision_with_ui_element && tf->active &&
	   (tf->delay_timer >= tf->delay_time) && input.left_click_down) {
		tf->delay_timer = 0.0;
		tf->write_focus = true;
		tf->inital_state = false;
	}
	else if(!collision_with_ui_element && tf->write_focus && input.left_click_down) {
		// if mouse clicked somewhere but the text field, remove write focus from text field
		tf->write_focus = false;
		tf->delay_timer = 0.0;
		tf->cursor.blink_timer = tf->cursor.blink_rate;
	}

	// update the text field text 
	if(tf->write_focus) {
		for(int iChar = 0; iChar < MAX_KEYS; iChar++) {
			// move cursor with arrow keys
			if(input.keyboard[iChar].pressed_this_frame && (iChar == (char)arrow_left)) {
				if(tf->cursor.pos.x > tf->cursor.initial_pos.x) {
					tf->cursor.pos.x -= tf->font->glyph_width + tf->font->glyph_spacing;
				}
			}
			else if(input.keyboard[iChar].pressed_this_frame && (iChar == arrow_right)) {
				if(tf->cursor.pos.x <= tf->width) {
					tf->cursor.pos.x += tf->font->glyph_width + tf->font->glyph_spacing;
				}
			}
			
			// delete last character if backspace is pressed
			else if(input.keyboard[iChar].pressed_this_frame && (iChar == bs_key)) {
				if(tf->text_current_len > 0) {
					tf->text[tf->text_current_len-1] = '\0';
					tf->text_current_len--;

					// don't move the cursor back out of the text field
					if(tf->cursor.pos.x > tf->cursor.initial_pos.x) {
						tf->cursor.pos.x -= tf->font->glyph_width + tf->font->glyph_spacing;
					}
				}
			}

			else if(input.keyboard[iChar].pressed_this_frame && (iChar == space_key)) {
				tf->text[tf->text_current_len] = ' ';
				tf->text[tf->text_current_len+1] = '\0';
				tf->text_current_len++;

				if(tf->cursor.pos.x + tf->cursor.width + tf->width > tf->cursor.pos.x) {
					tf->cursor.pos.x += tf->font->glyph_width + tf->font->glyph_spacing;
				}

			}

			// add character to text field
			else if(input.keyboard[iChar].pressed_this_frame && CharBelongsToText((char)iChar)) {
				int lowercase_offset = 0;
				if(!input.keyboard[left_shift_key].down && CharBelongsToAlphabet((char)iChar)) {
					lowercase_offset = 32;
				}

				// calculate cursor position in relationship to char buffer
				//int cursor_dx = (int)(tf->cursor.pos.x - tf->cursor.initial_pos.x);
				//int char_buff_index = cursor_dx / (tf->font->glyph_width + tf->font->glyph_spacing);
				//int len_from_cursor_to_end = tf->text_current_len - char_buff_index;

				// move all chars from position char_buff_index to the right one position
				//char tmp[MAX_TEXTFIELD_LEN] = {0};
				// put iChar + lowercase_offset into buff[char_buff_index]

				tf->text[tf->text_current_len] = (char)(iChar + lowercase_offset);
				tf->text[tf->text_current_len+1] = '\0';
				tf->text_current_len++;

				// TODO: don't move cursor out of text field to the right
				if(tf->cursor.pos.x + tf->cursor.width + tf->width > tf->cursor.pos.x) {
					tf->cursor.pos.x += tf->font->glyph_width + tf->font->glyph_spacing;
				}
			}
		}
	}
}
