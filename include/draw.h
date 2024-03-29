#ifndef DRAW_H
#define DRAW_H

#include <windows.h>
#include "common.h"

enum glyph_idx {
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    a,
    b,
    c,
    d,
    e,
    f,
    g,
    h,
    i,
    j,
    k,
    l,
    m,
    n,
    o,
    p,
    q,
    r,
    s,
    t,
    u,
    v,
    w,
    x,
    y,
    z,
    nill,
    one,
    two,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
};

// The framebuffer drawable area is from (0, 0) to (width-1, height-1).
struct Framebuffer {
    void *buffer;
    int bpp;
    int width;
    int height;
    int size;
    HBITMAP bitmap_handle;
    BITMAPINFO info;
    HDC bitmap_hdc;
};

struct Bitmap {
    u8 *pixel;
    int bpp;
    int width;
	int height;
};

struct Font {
	struct Bitmap bmp;
	int glyph_width;
	int glyph_height;
	int glyph_count;
	int glyph_spacing;
	u32 color_mask;
};

struct Sprite {
    int x;
    int y;

    struct Bitmap *frames;

    int animation_frame_count;
    int current_frame;

    double timer_next_frame;
    double current_timer;
};

struct Rectangle {
	int x;
	int y;
	int w;
	int h;
	u32 color;
};

struct Triangles {
    float verts[6];
    u32 color;
};

struct ClippedBmp {
	int x1, y1, x2 , y2;

	// drawable part of bitmap from (x, y)
	int dx;
	int dy;

	// offsets into bitmap pixel buffer
	int x_off;
	int y_off;
};

// Create only one framebuffer at a time with the win32 window handle
__declspec(dllexport) struct Framebuffer *CreateFramebuffer(HWND window);

/*
 * This function is mainly used when recreating the Framebuffer.
 * It doens't need to be called otherwise really,
 * since a framebuffer needs to be up at all times to be able to render stuff.
 */
__declspec(dllexport) void DestroyFramebuffer(struct Framebuffer *fb);


 /* After everything got rendered to the framebuffer output the buffer to the the screen. */

__declspec(dllexport)void OutputFramebuffer(HWND window, struct Framebuffer fb);

/*
 * u32 RGB_Color(u8 red, u8 green, u8 blue);
 * Packs three 8bit values in a unsigned 32bit value in the order the framebuffer uses it and returns the value.
 * The Format: 0RGB.
 */
__declspec(dllexport) u32 RGB_Color(u8 red, u8 green, u8 blue);

/*
 * u32 RGB_Color(u8 red, u8 green, u8 blue);
 * Packs three 8bit values in a unsigned 32bit value in the order the framebuffer uses it and returns the value.
 * The Format: ARGB.
 */
__declspec(dllexport) u32 RGBA_Color(u8 red, u8 green, u8 blue, u8 alpha);

 /* Fill the screen with a solid color. The Color format is 0RGB. */
__declspec(dllexport) void FillScreen(struct Framebuffer *framebuffer, u32 color);
__declspec(dllexport) void DrawPixel(struct Framebuffer *framebuffer, int x, int y, u32 color);

/* Draws a rectangle and clips it to the window dimensions */
__declspec(dllexport) void DrawRectangle(struct Framebuffer *framebuffer, int x0, int y0, int width, int height, u32 color);

/* Draws a rectangle and clips it to a specific region */
__declspec(dllexport) void DrawRectangleToClipRegion(struct Framebuffer *framebuffer, int x0, int y0, int width, int height, u32 color, int clip_x, int clip_y, int clip_w, int clip_h);

 /* I copied this function from: https://en.wikipedia.org/wiki/Bresenham's_line_algorithm. 
  * I should make an effort to fully understand this algorithm :P. 
  * Lines get clipped through the DrawPixel function */
__declspec(dllexport) void DrawLine(struct Framebuffer *framebuffer, int x0, int y0, int x1, int y1, u32 color);

/* Triangles get clipped thorugh the DrawPixel function inside the DrawLine function. */
__declspec(dllexport) void DrawTriangle(struct Framebuffer *framebuffer, int points[6], u32 color);


 /* NOTE: This function gets only used in LoadBitmapFile(). */
__declspec(dllexport) void *ReadFileContent(char *filename);
__declspec(dllexport) struct Bitmap LoadBitmapFile(char *filename);
__declspec(dllexport) struct Font LoadBitmapFont(char *filename);

/*  The win32 ReadFile() function loads .bmp (or all formats?) file formats horizontally flipped.
 *  Flip the bmp again to draw bitmaps with right orientation.
 */
__declspec(dllexport) void HFlipBMP24bpp(struct Bitmap *bitmap);
__declspec(dllexport) void HFlipBMP32bpp(struct Bitmap *bitmap);

/*
 * Clip a rectangle to the window dimensions. Return the clipped rectangle. 
 * If coordinates are invalid return a clipped rectangle with all values set to -1.
 */
__declspec(dllexport) struct  ClippedBmp ClipBitmap(int window_w, int window_h, int x, int y, int w, int h, int clip_x, int clip_y, int clip_w, int clip_h);

// DrawBMP24bpp doesn't work properly at the moment
__declspec(dllexport) void DrawBMP24bpp(struct Framebuffer *framebuffer, struct Bitmap bitmap, int x, int y, u32 color_mask);
__declspec(dllexport) void DrawBMP32bpp(struct Framebuffer *framebuffer, struct Bitmap bitmap, int x, int y, u32 color_mask);


 /* Draws a Bitmap and clips it to a specified region */
__declspec(dllexport) void DrawBMP24bppToClipRegion(struct Framebuffer *framebuffer, struct Bitmap bitmap, int x, int y, u32 color_mask, int clip_x, int clip_y, int clip_w, int clip_h);

/*
 * Initalizes a sprite with a collection of bitmaps (animation frames).
 * Sprites are just bitmaps with animation variables at the moment.
 * To update sprite animations call UpdateSpriteAnimation().
 * Bitmap *frames: an array with all the possible frames for an animation cycle.
 */
__declspec(dllexport) void InitSprite(struct Sprite *s, int x, int y, int frame_count, struct Bitmap *frames, int start_frame, double frame_time);
__declspec(dllexport) void UpdateSpriteAnimation(struct Sprite *s);
__declspec(dllexport) void GetPixelFromBMP(struct Bitmap *from, u8 *to);

/*
 * For the glyph_color to be drawn, the font bitmap should be a black text (rgb(0, 0, 0) on a white background (rgb(255, 255, 255).
 */
__declspec(dllexport) void DrawGlyph(struct Framebuffer *buffer, struct Font font, char ch, int x, int y, u32 glyph_color, int clip_x, int clip_y, int clip_w, int clip_h);

__declspec(dllexport) void DrawString(struct Framebuffer *buffer, struct Font font, char *string, int x, int y, u32 glyph_color, int clip_x, int clip_y, int clip_w, int clip_h);
 
#endif // DRAW_H
