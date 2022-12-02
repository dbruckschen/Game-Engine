#ifndef UI_H
#define UI_H

#include "draw.h"
#include "mathlib.h"
#include "input.h"

#define MAX_TEXTFIELD_LEN 128

struct Button {
	int x;
	int y;
	int width;
	int height;
	char text[128];
	u32 text_color;

	u32 color;
	u32 border_color;
	u32 border_thickness;

	struct Font *font;

	bool active;
	bool hover;
	bool toggle;

	double delay_time;
	double delay_timer;
};

struct Cursor {
	v2 pos;
	v2 initial_pos;
	int width;
	int height;
	float blink_rate;
	double blink_timer;
	u32 color;
};

struct TextField {
	int x;
	int y;
	int width;
	int height;

	u32 color;
	u32 border_color;
	u32 border_thickness;

	struct Font *font;
	bool active;
	bool write_focus;
	bool inital_state; // still in it's inital state aka no text input into text field

	float delay_time;
	double delay_timer;

	char text[MAX_TEXTFIELD_LEN];
	int text_current_len;
	bool draw_initial_placeholder_string;
	
	u32 text_color;

	struct Cursor cursor;
};

__declspec(dllexport) struct Button InitTextButton(struct Font *font, u32 text_color, int x, int y, int width, int height, char *text, u32 color,
												   int border_thickness, u32 border_color, float delay);

__declspec(dllexport) void UpdateButtonStatus(struct Button *btn, struct Input input, double dt);

// TODO: pass the string color to the DrawTextButton
__declspec(dllexport) void DrawTextButton(struct Framebuffer *fb, struct Button *btn);

__declspec(dllexport) struct TextField InitTextField(struct Font *font, int x, int y, int width, int height, u32 color, int border_thickness, u32 border_color,
													 float delay, int cursor_width, int cursor_height, float cursor_blink_rate, u32 cursor_color, u32 text_color, bool draw_placeholder_string);

__declspec(dllexport) void UpdateTextField(struct TextField *tf, struct Input input, double dt); 
__declspec(dllexport) void DrawTextField(struct Framebuffer *fb, struct TextField *tf);
#endif 
