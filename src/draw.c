#include "draw.h"

Framebuffer CreateFramebuffer(HWND window)
{
    Framebuffer framebuffer;
    RECT w_rect;
	
    GetClientRect(window, &w_rect);
    int window_width = w_rect.right - w_rect.left;
    int window_height = w_rect.bottom - w_rect.top;
	
    framebuffer.width = window_width;
    framebuffer.height = window_height;
    framebuffer.bbp = 4;
	
    framebuffer.size = framebuffer.height * framebuffer.width * framebuffer.bbp;
    framebuffer.buffer = VirtualAlloc(0, framebuffer.size, MEM_COMMIT, PAGE_READWRITE);
	
    framebuffer.info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    framebuffer.info.bmiHeader.biWidth = window_width;
    framebuffer.info.bmiHeader.biHeight = -window_height;
    framebuffer.info.bmiHeader.biPlanes = 1;
    framebuffer.info.bmiHeader.biBitCount = 32;
    framebuffer.info.bmiHeader.biCompression = BI_RGB;
    framebuffer.info.bmiHeader.biSizeImage = 0;
    framebuffer.info.bmiHeader.biXPelsPerMeter = 0;
    framebuffer.info.bmiHeader.biYPelsPerMeter = 0;
    framebuffer.info.bmiHeader.biClrUsed = 0;
    framebuffer.info.bmiHeader.biClrImportant = 0;
	
    framebuffer.bitmap_hdc = CreateCompatibleDC(0);
    framebuffer.bitmap_handle = CreateDIBSection(framebuffer.bitmap_hdc, &framebuffer.info, DIB_RGB_COLORS, &framebuffer.buffer, 0, 0);
	
    if (framebuffer.bitmap_handle)
    {
        // TODO: error handling
        SelectObject(framebuffer.bitmap_hdc, framebuffer.bitmap_handle);
    }
    return framebuffer;
}

void DestroyFramebuffer(Framebuffer *fb)
{
    VirtualFree(&fb->buffer, 0, MEM_RELEASE);
    fb->buffer = 0;
}

void OutputFramebuffer(HWND window, Framebuffer fb)
{
    HDC window_dc = GetDC(window);
    BitBlt(window_dc, 0, 0, fb.width, fb.height, fb.bitmap_hdc, 0, 0, SRCCOPY);
}

u32 RGB_Color(u8 red, u8 green, u8 blue)
{
    // alpha red green bluen
    u32 color = 0;
    return color = (red << 16) + (green << 8) + blue;
}

void FillScreen(Framebuffer *framebuffer, u32 color)
{
    u32 *pixel = (u32 *)framebuffer->buffer;
    for(u32 i = 0; i < framebuffer->width * framebuffer->height; ++i)
    {
        *pixel++ = color;
    }
} 

void DrawPixel(Framebuffer* framebuffer, u32 x, u32 y, u32 color)
{
    u32* pixel = (u32*)framebuffer->buffer;

    if (x < framebuffer->width && y < framebuffer->height)
    {
        pixel += x + (y * framebuffer->width);
        *pixel = color;
    }	
}

void DrawRectangle(Framebuffer *framebuffer, u32 x0, u32 y0, u32 width, u32 height, u32 color)
{
    u32 *pixel = (u32 *)framebuffer->buffer;
    pixel += x0 + (y0 * framebuffer->width);

    if (width > framebuffer->width)
    {
        width = framebuffer->width;
    }
		
    if (height > framebuffer->height)
    {
        height = framebuffer->height;
    }
	
    for(u32 y = 0; y < height; ++y)
    {
        for(u32 x = 0; x < width; ++x)
        {
            *pixel++ = color;
        }
        pixel += framebuffer->width - width;
    }
}

// copied from: https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
void DrawLine(Framebuffer *framebuffer, int x0, int y0, int x1, int y1, u32 color)
{
    assert(x0 > 0);
    assert(x0 < framebuffer->width);
    assert(y0 > 0);
    assert(y0 < framebuffer->height);
    
    int dx = abs(x1-x0);
    int dy = -abs(y1-y0);
    int sx = x0<x1 ? 1 : -1;
    int sy = y0<y1 ? 1 : -1;
    int err = dx+dy;
    int e2;

    while (1)
    {
        DrawPixel(framebuffer, x0, y0,color);
	
        if (x0==x1 && y0==y1)
        {
            break;
        }

        e2 = err * 2;
	
        if (e2 > dy)
        {
            err += dy;
            x0 += sx;
        } 

        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void DrawTriangle(Framebuffer *framebuffer, u32 points[6], u32 color)
{
    DrawLine(framebuffer, points[0], points[1], points[2], points[3], color);
    DrawLine(framebuffer, points[2], points[3], points[4], points[5], color);
    DrawLine(framebuffer, points[4], points[5], points[0], points[1], color);
}

void* ReadFileContent(char* filename)
{
    HANDLE file_handle = CreateFileA(filename, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    DWORD error = GetLastError();

    // TODO: handle errors
    DWORD file_size;
    file_size = GetFileSize(file_handle, 0);

    void* file_data;
    file_data = malloc(file_size);
    DWORD bytes_read;

    if (!ReadFile(file_handle, file_data, file_size, &bytes_read, 0))
    {
        // TODO: handle error
    }
	
    CloseHandle(file_handle);
	
    return file_data;
}

Bitmap LoadBitmapFile(char *filename)
{
    Bitmap bitmap = {0};
	
    void *file;
    file = ReadFileContent(filename);
	
    if (file != 0)
    {
        u8* file_ptr = (u8*)file;

        BITMAPFILEHEADER* bmp_file_header = (BITMAPFILEHEADER*)file_ptr;
        BITMAPINFOHEADER* bmp_info_header = (BITMAPINFOHEADER*)(file_ptr + sizeof(BITMAPFILEHEADER));

        bitmap.pixel = file_ptr + bmp_file_header->bfOffBits;
        bitmap.width = bmp_info_header->biWidth;
        bitmap.height = bmp_info_header->biHeight;
        bitmap.bbp = bmp_info_header->biBitCount / 8;

        // TODO: flip the bitmap horizontally
    }	
    return bitmap;
}

void DrawBMP24bbp(Framebuffer *framebuffer, Bitmap bitmap, u32 x_pos, u32 y_pos, u32 color_mask)
{
    u32 *dst = (u32 *)framebuffer->buffer;
    u8 *src = bitmap.pixel;
	
    dst += x_pos + (y_pos * framebuffer->width);
	
    for(u32 y = 0; y < bitmap.height; ++y) 
    {
        for(u32 x = 0; x < bitmap.width; ++x)
        {
            u8 r = *src;
            u8 g = *(src + 1);
            u8 b = *(src + 2);
			
            u32 src_pixel = (0 << 24) + (b << 16) + (g << 8) + r;
			
            if(src_pixel != color_mask)
            {
                *dst++ = src_pixel;
            }
            else
            {
                dst++;
            }
            src += bitmap.bbp;
        }
		
        dst += (framebuffer->width - bitmap.width);
    }
}

void DrawBMP32bbp(Framebuffer *framebuffer, Bitmap bitmap, u32 x_pos, u32 y_pos, u32 color_mask)
{
    u8 *dst = (u8 *)framebuffer->buffer;
    u8 *src = bitmap.pixel;
	
    dst += x_pos + (y_pos * framebuffer->width);
	
    for(u32 y = 0; y < bitmap.height; ++y)
    {
        for(u32 x = 0; x < bitmap.width; ++x)
        {
            u8 r = *src;
            u8 g = *(src + 1);
            u8 b = *(src + 2);
            u8 a = *(src + 3);
			
            u32 src_pixel = (a << 24) + (r << 16) + (g << 8) + b;
			
            if(src_pixel != color_mask)
            {
                *dst++ = src_pixel;
            }
            else
            {
                dst++;
            }
            src += bitmap.bbp;
        }
		
        dst += (framebuffer->width - bitmap.width);
    }
}

// converts device coordinates to normalized device coordinates
// ([window_width, window_height] to [-1. 1]
void DC_TO_NDC(v2 *v, v2 width_height)
{
    //  2x/w - 1
    // -2y/h + 1 <- check if the sign is needed (bootom up/top down etc...)
    //

    v->x = 2*v->x/width_height.x - 1;
    v->y = -2*v->y/width_height.y + 1;
    
}

// converts normalized device coordinates to device coordinates
// ([-1,1] to [window_width, window_height].
void NDC_TO_DC(v2 *v, v2 width_height)
{
    v->x = fabs((v->x/2)*width_height.x);
    v->y = fabs((v->y/2)*width_height.y);

    assert(v->x > 0);
    assert(v->y > 0);
    assert(v->x < width_height.x);
    assert(v->y < width_height.y);
	
}

bool BBAA(v2 b1, int width1, int height1, v2 b2, int width2, int height2)
{
    if(b1.x < b2.x + width2 &&
       b1.x + width1 > b2.x &&
       b1.y < b2.y + height2 &&
       b1.y + width1 > b2.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void DrawCircleWithLines(Framebuffer *framebuffer, int x0, int y0, u32 color)
{
    v2 center = {x0, y0};
    int x = 0;
    int y = 0;
    int radius = 100;
    float angle = 1;
    
    for(int i = 0; i < 360; i++)
    {
        angle++;
        x = radius * cos(DegToRad(angle));
        y = radius * sin(DegToRad(angle));

        x += center.x;
        y += center.y;
	   	
        DrawLine(framebuffer, center.x, center.y, x, y, color);
    }
}
