#include "draw.h"

struct Framebuffer CreateFramebuffer(HWND window)
{
    struct Framebuffer framebuffer;
    RECT w_rect;

    GetClientRect(window, &w_rect);

    int window_width = w_rect.right - w_rect.left;
    int window_height = w_rect.bottom - w_rect.top;

    framebuffer.width = window_width;
    framebuffer.height = window_height;
    framebuffer.bpp = 4;

    framebuffer.size = framebuffer.height * framebuffer.width * framebuffer.bpp;
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
        SelectObject(framebuffer.bitmap_hdc, framebuffer.bitmap_handle);

    return framebuffer;
}

void DestroyFramebuffer(struct Framebuffer *fb)
{
    VirtualFree(&fb->buffer, 0, MEM_RELEASE);
    fb->buffer = 0;
}

void OutputFramebuffer(HWND window, struct Framebuffer fb)
{
    HDC window_dc = GetDC(window);
    BitBlt(window_dc, 0, 0, fb.width, fb.height, fb.bitmap_hdc, 0, 0, SRCCOPY);
}

u32 RGB_Color(u8 red, u8 green, u8 blue)
{
    return (unsigned int)(((u8)0 << 24) + (red << 16) + (green << 8) + blue);
}

void FillScreen(struct Framebuffer *framebuffer, u32 color)
{
    u32 *pixel = (u32 *)framebuffer->buffer;
    for (u32 i = 0; i < framebuffer->width * framebuffer->height; ++i)
        *pixel++ = color;
}

void DrawPixel(struct Framebuffer *framebuffer, u32 x, u32 y, u32 color)
{
    if (x >= 0 && x <= framebuffer->width && y >= 0 && y <= framebuffer->height) {
        u32 *pixel = (u32 *)framebuffer->buffer;

        pixel += x + (y * framebuffer->width);
        *pixel = color;
    }
}

void DrawRectangle(struct Framebuffer *framebuffer, u32 x0, u32 y0, u32 width, u32 height, u32 color)
{
    u32 *pixel = (u32 *)framebuffer->buffer;
    pixel += x0 + (y0 * framebuffer->width);

    for (u32 y = 0; y < height; ++y) {
        for (u32 x = 0; x < width; ++x) {
            *pixel++ = color;
        }
        pixel += framebuffer->width - width;
    }
}

void DrawLine(struct Framebuffer *framebuffer, int x0, int y0, int x1, int y1, u32 color)
{
    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    int e2;

    while (1) {
        DrawPixel(framebuffer, x0, y0, color);

        if (x0 == x1 && y0 == y1)
            break;

        e2 = err * 2;

        if (e2 > dy) {
            err += dy;
            x0 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void DrawTriangle(struct Framebuffer *framebuffer, u32 points[6], u32 color)
{
    DrawLine(framebuffer, points[0], points[1], points[2], points[3], color);
    DrawLine(framebuffer, points[2], points[3], points[4], points[5], color);
    DrawLine(framebuffer, points[4], points[5], points[0], points[1], color);
}

void *ReadFileContent(char *filename)
{
    HANDLE file_handle = CreateFileA(filename, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    DWORD file_size;
    file_size = GetFileSize(file_handle, 0);

    void *file_data;
    file_data = malloc(file_size);
    DWORD bytes_read;

    ReadFile(file_handle, file_data, file_size, &bytes_read, 0);

    CloseHandle(file_handle);

    return file_data;
}

struct Bitmap LoadBitmapFile(char *filename)
{
    struct Bitmap bitmap = {0};

    void *file;
    file = ReadFileContent(filename);

    if (file != 0) {
        u8 *file_ptr = (u8 *)file;

        BITMAPFILEHEADER *bmp_file_header = (BITMAPFILEHEADER *)file_ptr;
        BITMAPINFOHEADER *bmp_info_header = (BITMAPINFOHEADER *)(file_ptr + sizeof(BITMAPFILEHEADER));

        bitmap.pixel = file_ptr + bmp_file_header->bfOffBits;
        bitmap.width = bmp_info_header->biWidth;
        bitmap.height = bmp_info_header->biHeight;
        bitmap.bpp = bmp_info_header->biBitCount / 8;

        if (bitmap.bpp == 3)
            HFlipBMP24bpp(&bitmap);

        else if (bitmap.bpp == 4)
            HFlipBMP32bpp(&bitmap);

    }
    return bitmap;
}

void HFlipBMP24bpp(struct Bitmap *bitmap)
{
    size_t bitmap_size = bitmap->height * bitmap->width * bitmap->bpp;
    u8 *copy_bmp_pixel = malloc(bitmap_size);
    GetPixelFromBMP(bitmap, copy_bmp_pixel);

    u8 *dst = bitmap->pixel;
    u8 *src = copy_bmp_pixel + (bitmap->width * (bitmap->height - 1)) * bitmap->bpp;

    for (u32 y = 0; y < bitmap->height; y++) {
        for (u32 x = 0; x < bitmap->width; x++) {
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
        }
        src -= 2 * bitmap->width * bitmap->bpp;
    }

    free(copy_bmp_pixel);
    copy_bmp_pixel = 0;
}

void HFlipBMP32bpp(struct Bitmap *bitmap)
{
    size_t bitmap_size = bitmap->height * bitmap->width * bitmap->bpp;
    u8 *copy_bmp_pixel = malloc(bitmap_size);
    GetPixelFromBMP(bitmap, copy_bmp_pixel);

    u32 *dst = (u32 *)bitmap->pixel;
    u32 *src = (u32 *)copy_bmp_pixel + (bitmap->width * (bitmap->height - 1));

    for (u32 y = 0; y < bitmap->height; y++) {
        for (u32 x = 0; x < bitmap->width; x++) {
            *dst++ = *src++;
        }
        src -= 2 * bitmap->width;
    }

    free(copy_bmp_pixel);
    copy_bmp_pixel = 0;
}

void DrawBMP24bpp(struct Framebuffer *framebuffer, struct Bitmap bitmap, u32 x, u32 y, u32 color_mask)
{
    if(x >= framebuffer->width || y >= framebuffer->height)
        return;

    if(y + bitmap.height - 1 > framebuffer->height) {
        bitmap.height = framebuffer->height - bitmap.height - 1;
    }


    u32 *dst = framebuffer->buffer;
    u8 *src = bitmap.pixel;

    dst += x + y * framebuffer->width;

    for(u32 yidx = 0; yidx < bitmap.height; yidx++) {
        for(u32 xidx = 0; xidx < bitmap.width; xidx++) {
            u8 r = *src;
            u8 g = *(src + 1);
            u8 b = *(src + 2);

            u32 src_pixel = (0 << 24) + (b << 16) + (g << 8) + r;

            if(src_pixel != color_mask)
                *dst++ = src_pixel;
            else
                dst++;

            src += bitmap.bpp;
        }
        dst += (framebuffer->width - bitmap.width);
    }
}

void DrawBMP32bpp(struct Framebuffer *framebuffer, struct Bitmap bitmap, u32 x_pos, u32 y_pos, u32 color_mask)
{
    u32 *dst = (u32 *)framebuffer->buffer;
    u8 *src = (u8 *)bitmap.pixel;

    dst += x_pos + (y_pos * framebuffer->width);

    for(u32 y = 0; y < bitmap.height; ++y) {
        for(u32 x = 0; x < bitmap.width; ++x) {
            u8 r = *src;
            u8 g = *(src + 1);
            u8 b = *(src + 2);
            u8 a = *(src + 3);

            u32 src_pixel = (a << 24) + (r << 16) + (g << 8) + b;

            if(src_pixel != color_mask)
                *dst++ = src_pixel;
            else
                dst++;

            src += bitmap.bpp;
        }
        dst += (framebuffer->width - bitmap.width);
    }
}

void GetPixelFromBMP(struct Bitmap *from, u8 *to)
{
    for(u32 i = 0; i < from->height * from->width * from->bpp; i++)
        *(to + i) = *(from->pixel + i);
}

void DrawGlyph(struct Framebuffer *framebuffer, struct Bitmap font, char ch, u32 x_pos, u32 y_pos, u32 color_mask)
{
    u32 glyph_width = 5;
    u32 num_glyphs = 98;
    u32 glyph_offsets[98];

    for(int i = 0; i < num_glyphs; i ++) {
        glyph_offsets[i] = i * (glyph_width + 1) * font.bpp;
    }

    // find glyph offset of the current character to be drawn
    u32 glyph_offset = 0;

    for(int c = 0; c < 255; c ++) {
	if(ch == (char)c) {
	    int start_of_char_type = 0;

	    if(c >= 65 && c <= 90) { 
		start_of_char_type = 65;
		glyph_offset = glyph_offsets[c - start_of_char_type];
	    } else if(c >= 97 && c <= 122) {
		start_of_char_type = 71;
		glyph_offset = glyph_offsets[c - start_of_char_type];
	    } else if(c >= 48 && c <= 57) {
		start_of_char_type = 4;
		glyph_offset = glyph_offsets[c + start_of_char_type];
	    } else if(c == '`') {
		glyph_offset = glyph_offsets[62];
	    } else if(c == '~') {
		glyph_offset = glyph_offsets[63];
	    } else if(c == '!') {
		glyph_offset = glyph_offsets[64];
	    } else if(c == 196) { // copyright sign
		glyph_offset = glyph_offsets[65];
	    } else if(c == '#') {
		glyph_offset = glyph_offsets[66];
	    } else if(c == '$') {
		glyph_offset = glyph_offsets[67];
	    } else if(c == '%') {
		glyph_offset = glyph_offsets[68];
	    } else if(c == '^') {
		glyph_offset = glyph_offsets[69];
	    } else if(c == '&') {
		glyph_offset = glyph_offsets[70];
	    } else if(c == '*') {
		glyph_offset = glyph_offsets[71];
	    } else if(c == '(') {
		glyph_offset = glyph_offsets[72];
	    } else if(c == ')') {
		glyph_offset = glyph_offsets[73];
	    } else if(c == '-') {
		glyph_offset = glyph_offsets[74];
	    } else if(c == '=') {
		glyph_offset = glyph_offsets[75];
	    } else if(c == '_') {
		glyph_offset = glyph_offsets[76];
	    } else if(c == '+') {
		glyph_offset = glyph_offsets[77];
	    } else if(c == '\\') {
		glyph_offset = glyph_offsets[78];
	    } else if(c == '|') {
		glyph_offset = glyph_offsets[79];
	    } else if(c == '[') {
		glyph_offset = glyph_offsets[80];
	    } else if(c == ']') {
		glyph_offset = glyph_offsets[81];
	    } else if(c == '{') {
		glyph_offset = glyph_offsets[82];
	    } else if(c == '}') {
		glyph_offset = glyph_offsets[83];
	    } else if(c == ';') {
		glyph_offset = glyph_offsets[84];
	    } else if(c == '\'') {
		glyph_offset = glyph_offsets[85];
	    } else if(c == ':') {
		glyph_offset = glyph_offsets[86];
	    } else if(c == '"') {
		glyph_offset = glyph_offsets[87];
	    }else if(c == ',') {
		glyph_offset = glyph_offsets[88];
	    }else if(c == '<') {
		glyph_offset = glyph_offsets[89];
	    }else if(c == '>') {
		glyph_offset = glyph_offsets[90];
	    } else if(c == '.') {
		glyph_offset = glyph_offsets[91];
	    } else if(c == '/') {
		glyph_offset = glyph_offsets[92];
	    }else if(c == '?') {
		glyph_offset = glyph_offsets[93];
	    }/*else if(c == '') {
		glyph_offset = glyph_offsets[94];
	    }else if(c == '') {
		glyph_offset = glyph_offsets[95];
	    }else if(c == '') {
		glyph_offset = glyph_offsets[96];
		} */
	    else {
		glyph_offset = glyph_offsets[97];
	    }

	    // H - 72 ascii - index 7  (+65)
	    // a - 97 ascii - index 26 (+71)
	    // 0 - 48 ascii - index 52 (+3)
	    // 
	}
    }
        
    u32 *dst = (u32 *)framebuffer->buffer;
    u8 *src = (u8 *)font.pixel + glyph_offset;

    dst += x_pos + (y_pos * framebuffer->width);

    for(u32 y = 0; y < font.height; ++y) {
        for(u32 x = 0; x < glyph_width; ++x) {
            u8 r = *src;
            u8 g = *(src + 1);
            u8 b = *(src + 2);
            u8 a = *(src + 3);

            u32 src_pixel = (a << 24) + (r << 16) + (g << 8) + b;

            if(src_pixel != color_mask)
                *dst++ = src_pixel;
            else
                dst++;

            src += font.bpp;
        }
	src += (font.width - glyph_width) * font.bpp;
        dst += framebuffer->width - glyph_width;
    }
}

void DrawString(struct Framebuffer *buffer, struct Bitmap font, char *string, u32 x, u32 y, u32 color_mask)
{
    int new_char_offset = 0;
    int glyph_width = 5;
    int glyph_spacing = 2;
    
    for(int i = 0; i < StringLen(string)-1; i++) {
	if(string[i] == ' ') {
	    new_char_offset += glyph_width + glyph_spacing;
	} else {
	    DrawGlyph(buffer, font, string[i], x + new_char_offset, y, color_mask);
	    new_char_offset += glyph_width + glyph_spacing;
	}
    }
}

void InitSprite(struct Sprite *s, float x, float y, int frame_count, struct Bitmap *frames, int start_frame, double frame_time)
{
    s->x = x;
    s->y = y;
    s->animation_frame_count = frame_count;
    s->frames = malloc(frame_count * sizeof(struct Bitmap));

    for(int i = 0; i < frame_count; i++)
        *(s->frames + i) = *(frames + i);

    s->current_frame = start_frame;
    s->timer_next_frame = frame_time;
    s->current_timer = 0.0f;
}

void UpdateSpriteAnimation(struct Sprite *s)
{
    if(s->current_timer >= s->timer_next_frame) {
        s->current_frame++;
        s->current_timer = 0.0;

        if(s->current_frame == s->animation_frame_count)
            s->current_frame = 0;
    }
}

float DC_TO_NDC(float v, int width_height)
{
    return (v * 2) / width_height - 1;
}

float NDC_TO_DC(float v, int width_height)
{
    return (float)fabs((v / 2) * width_height);
}

bool BBAA(v2 b1, int width1, int height1, v2 b2, int width2, int height2)
{
    if(b1.x < b2.x + width2 && b1.x + width1 > b2.x &&
            b1.y < b2.y + height2 && b1.y + height1 > b2.y)
        return true;
    else
        return false;
}

