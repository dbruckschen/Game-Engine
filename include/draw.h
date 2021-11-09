#ifndef DRAW_H
#define DRAW_H

#include "..\include\common.h"
#include "..\include\mathlib.h"
#include <assert.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

// Framebuffer drawable area is from 0,0 to width-1, height-1
// Clipping happens inside drawing frunctions to protect from crashes
// Negative x,y,width,height numbers wrap to a positive number because parameter are u32

typedef struct
{
    void *buffer;
    u32 bpp;
    u32 width;
    u32 height;
    u32 size;
    HBITMAP bitmap_handle;
    BITMAPINFO info;
    HDC bitmap_hdc;
	
} Framebuffer;

typedef struct 
{
    u8 *pixel;
    u32 bpp;
    u32 width;
    u32 height;
	
} Bitmap;

typedef struct
{
    float x;
    float y;

    Bitmap *frames;

    int animation_frame_count;
    int current_frame;

    double timer_next_frame;
    double current_timer;

} Sprite;

typedef struct
{
    float verts[6];
    u32 color;
    
} Triangle;

__declspec(dllexport) Framebuffer CreateFramebuffer(HWND window);
__declspec(dllexport) void DestroyFramebuffer(Framebuffer *fb);
__declspec(dllexport) void OutputFramebuffer(HWND window, Framebuffer fb);
// Packs three 8bit values in a unsigned 32bit value. Format: 0RGB
__declspec(dllexport) u32 RGB_Color(u8 red, u8 green, u8 blue);
__declspec(dllexport) void FillScreen(Framebuffer *framebuffer, u32 color);
__declspec(dllexport) void DrawPixel(Framebuffer *framebuffer, u32 x, u32 y, u32 color);
__declspec(dllexport) void DrawRectangle(Framebuffer *framebuffer, u32 x0, u32 y0, u32 width, u32 height, u32 color);
__declspec(dllexport) void DrawLine(Framebuffer* framebuffer, int x0, int y0, int x1, int y1, u32 color);
__declspec(dllexport) void DrawTriangle(Framebuffer *framebuffer, u32 points[6], u32 color);
// Used in LoadBitmapFile()
__declspec(dllexport) void *ReadFileContent(char *filename);
__declspec(dllexport) Bitmap LoadBitmapFile(char *filename);
// Window's ReadFile() loads .bmp files flipped. Flip again to draw bitmaps with right direction.
__declspec(dllexport) void HFlipBMP24bpp(Bitmap *bitmap);
__declspec(dllexport) void HFlipBMP32bpp(Bitmap *bitmap);
__declspec(dllexport) void DrawBMP24bpp(Framebuffer *framebuffer, Bitmap bitmap, u32 x_pos, u32 y_pos, u32 color_mask);
__declspec(dllexport) void DrawBuffer24bpp(Framebuffer *framebuffer, u8 *pixel, u32 x_pos, u32 y_pos, u32 w, u32 h, u32 color_mask);
__declspec(dllexport) void DrawBMPSubRec24bpp(Framebuffer *framebuffer, Bitmap bitmap, u32 x_pos, u32 y_pos, u32 color_mask, u32 rec_x, u32 rec_y, u32 rec_w, u32 rec_h);
__declspec(dllexport) void DrawBMP32bpp(Framebuffer *framebuffer, Bitmap bitmap, u32 x_pos, u32 y_pos, u32 color_mask);
// Creates a Sprite from a Bitmap. Sprites are just Bitmaps with Animation atm.
__declspec(dllexport) void InitSprite(Sprite *s, float x, float y, int frame_count, Bitmap *frames, int start_frame, double frame_time);
__declspec(dllexport) void UpdateSpriteAnimation(Sprite *s);
__declspec(dllexport) void GetSubRecPixel24bpp(Bitmap b, u32 x, u32 y, u32 w, u32 h, u8 *sub_rec);
__declspec(dllexport) void GetPixelFromBMP(Bitmap *from, u8 *to);
__declspec(dllexport) void DrawString(Framebuffer *buffer, Bitmap font, char *string, u32 x, u32 y);
// NDC range: [-1,1] DC range: [0,window width] / [0, window height]
__declspec(dllexport) float NDC_TO_DC(float v, int width_height);
__declspec(dllexport) float DC_TO_NDC(float v, int width_height);
// Bounding Box Axis Aligned
__declspec(dllexport) bool BBAA(v2 b1, int width1, int height1, v2 b2, int width2, int height2);

#endif // DRAW_H
