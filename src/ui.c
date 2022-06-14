#include "ui.h"

void DrawColorPicker(struct Framebuffer *fb, int x, int y) {
	DrawRectangle(fb, x, y, 384, 384, RGB_Color(255, 0, 255));
}
