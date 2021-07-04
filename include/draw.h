#ifndef DRAW_H
#define DRAW_H

#include "..\include\common.h"
#include "..\include\mathlib.h"
#include <assert.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Framebuffer drawable area is from 0,0 to width-1, height-1
 * Clipping happens inside drawing frunctions to protect from crashes
 * 
 * Negative x,y,width,height numbers wrap to a positive number because parameter are u32
 */

typedef struct
{
    void *buffer;
    u32 bbp;
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
    Bitmap *frames;

    int animation_frame_count;
    int current_frame;
    
    double timer_next_frame;
    double current_timer;

} Sprite;

typedef struct
{
    float verts[6];
    v2 world_coords;
    u32 color;
    
} Triangle;

// Framebuffer Setup
//-------------------

__declspec(dllexport) Framebuffer CreateFramebuffer(HWND window);
__declspec(dllexport) void DestroyFramebuffer(Framebuffer *fb);
__declspec(dllexport) void OutputFramebuffer(HWND window, Framebuffer fb);

// Drawing Functions	
//-------------------

__declspec(dllexport) u32 RGB_Color(u8 red, u8 green, u8 blue);
__declspec(dllexport) void FillScreen(Framebuffer *framebuffer, u32 color);
__declspec(dllexport) void DrawPixel(Framebuffer *framebuffer, u32 x, u32 y, u32 color);
__declspec(dllexport) void DrawRectangle(Framebuffer *framebuffer, u32 x0, u32 y0, u32 width, u32 height, u32 color);
__declspec(dllexport) void DrawLine(Framebuffer* framebuffer, int x0, int y0, int x1, int y1, u32 color);
__declspec(dllexport) void DrawTriangle(Framebuffer *framebuffer, u32 points[6], u32 color);

// Bitmap anad Sprite Loading and Drawing
//----------------------------

__declspec(dllexport) void *ReadFileContent(char *filename);
__declspec(dllexport) Bitmap LoadBitmapFile(char *filename); // Bitmap return value is a empty Bitmap if loading didn't work
__declspec(dllexport) void FlipBMP24bpp(Bitmap *bitmap);
__declspec(dllexport) void DrawBMP24bpp(Framebuffer *framebuffer, Bitmap bitmap, u32 x_pos, u32 y_pos, u32 color_mask);
__declspec(dllexport) void DrawBMP32bpp(Framebuffer *framebuffer, Bitmap bitmap, u32 x_pos, u32 y_pos, u32 color_mask);
__declspec(dllexport) void InitSprite(Sprite *s, int frame_count, Bitmap *frames, int start_frame, double frame_time);
__declspec(dllexport) void UpdateSpriteAnimation(Sprite *s);
// Utility Functions
//----------------------------

// Assumes that the Bitmap and the Array have the same size
__declspec(dllexport) void CopyBitmapIntoArray(Bitmap *from, u8 *to);
    
// Converts coordinates from range [-1,1] to range [0, window_width/window_height]
__declspec(dllexport) void NDC_TO_DC(float *v, int width_height);
// Converts coordinates from range [0, window_width/window_height] to range [-1,1]
__declspec(dllexport) void DC_TO_NDC(float *v, int width_height);

// Bounding Box Axis Align Collsion Detection
//--------------------------------------------

__declspec(dllexport) bool BBAA(v2 b1, int width1, int height1, v2 b2, int width2, int height2);

// Demo Functions
//--------------------------------------------

__declspec(dllexport) void DrawCircleWithLines(Framebuffer *framebuffer, int x0, int y0, u32 color);

#endif // DRAW_H
