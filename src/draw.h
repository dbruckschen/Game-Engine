#ifndef DRAW_H
#define DRAW_H

#include "common.h"
#include "mathlib.h"
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
    u32 bbp;
    u32 width;
    u32 height;
	
} Bitmap;

typedef struct
{
    float verts[6];
    v2 world_coords;
    u32 color;
    
} Triangle;

// Framebuffer Setup
//-------------------

Framebuffer CreateFramebuffer(HWND window);
void DestroyFramebuffer(Framebuffer *fb);
void OutputFramebuffer(HWND window, Framebuffer fb);

// Drawing Functions	
//-------------------

u32 RGB_Color(u8 red, u8 green, u8 blue);
void FillScreen(Framebuffer *framebuffer, u32 color);
void DrawPixel(Framebuffer *framebuffer, u32 x, u32 y, u32 color);
void DrawRectangle(Framebuffer *framebuffer, u32 x0, u32 y0, u32 width, u32 height, u32 color);
void DrawLine(Framebuffer* framebuffer, int x0, int y0, int x1, int y1, u32 color);
void DrawTriangle(Framebuffer *framebuffer, u32 points[6], u32 color);

// Bitmap Loading and Drawing
//----------------------------

void *ReadFileContent(char *filename);
Bitmap LoadBitmapFile(char *filename); // Bitmap return value is a empty Bitmap if loading didn't work
void DrawBMP24bbp(Framebuffer *framebuffer, Bitmap bitmap, u32 x_pos, u32 y_pos, u32 color_mask);
void DrawBMP32bbp(Framebuffer *framebuffer, Bitmap bitmap, u32 x_pos, u32 y_pos, u32 color_mask);

// Utility Functions
void Flip_Vertices(float *v, int c);

// Converts coordinates from range [-1,1] to range [0, window_width/window_height]
void NDC_TO_DC(v2 *v, v2 width_height);
// Converts coordinates from range [0, window_width/window_height] to range [-1,1]
void DC_TO_NDC(v2 *v, v2 width_height);

// Bounding Box Axis Align Collsion Detection
//--------------------------------------------

bool BBAA(v2 b1, int width1, int height1, v2 b2, int width2, int height2);

// Demo Functions
//--------------------------------------------

void DrawCircleWithLines(Framebuffer *framebuffer, int x0, int y0, u32 color);

#endif // DRAW_H
