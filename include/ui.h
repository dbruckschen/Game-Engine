#ifndef UI_H
#define UI_H

#include "draw.h"
#include "mathlib.h"
#include "input.h"

struct Button {
	int x;
	int y;
	int width;
	int height;
	char text[128];

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

struct TextField {
	int x;
	int y;
	int width;
	int height;

	u32 color;
	u32 border_color;
	u32 border_thickness;

	struct Bitmap *font;
	bool active;
	bool clicked;

	v2 cursor_pos;
	char text[256];
};

__declspec(dllexport) struct Button
InitTextButton(struct Font *font, int x, int y,
		   int width, int height,
			   char *text, u32 color, int border_thickness, u32 border_color, float delay);

__declspec(dllexport) void UpdateButtonStatus(struct Button *btn, struct Input input, double dt);
__declspec(dllexport) void DrawTextButton(struct Framebuffer *fb, struct Button *btn);


#endif 
