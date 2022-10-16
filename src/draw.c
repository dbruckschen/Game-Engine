#include "draw.h"

struct Framebuffer *CreateFramebuffer(HWND window) {
    struct Framebuffer *framebuffer = VirtualAlloc(0, sizeof *framebuffer, MEM_COMMIT, PAGE_READWRITE);
    RECT w_rect;

    GetClientRect(window, &w_rect);

    int window_width = w_rect.right - w_rect.left;
    int window_height = w_rect.bottom - w_rect.top;

    framebuffer->width = window_width;
    framebuffer->height = window_height;
    framebuffer->bpp = 4;

    framebuffer->size = framebuffer->height * framebuffer->width * framebuffer->bpp;
    framebuffer->buffer = VirtualAlloc(0, framebuffer->size, MEM_COMMIT, PAGE_READWRITE);

    framebuffer->info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    framebuffer->info.bmiHeader.biWidth = window_width;
    framebuffer->info.bmiHeader.biHeight = -window_height;
    framebuffer->info.bmiHeader.biPlanes = 1;
    framebuffer->info.bmiHeader.biBitCount = 32;
    framebuffer->info.bmiHeader.biCompression = BI_RGB;
    framebuffer->info.bmiHeader.biSizeImage = 0;
    framebuffer->info.bmiHeader.biXPelsPerMeter = 0;
    framebuffer->info.bmiHeader.biYPelsPerMeter = 0;
    framebuffer->info.bmiHeader.biClrUsed = 0;
    framebuffer->info.bmiHeader.biClrImportant = 0;

    framebuffer->bitmap_hdc = CreateCompatibleDC(0);
    framebuffer->bitmap_handle = CreateDIBSection(framebuffer->bitmap_hdc, &framebuffer->info, DIB_RGB_COLORS, &framebuffer->buffer, 0, 0);

    if (framebuffer->bitmap_handle)
        SelectObject(framebuffer->bitmap_hdc, framebuffer->bitmap_handle);

	static bool init = false;
	if(!init) {
		SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)framebuffer);
		init = true;
	}
		
	return framebuffer;
}

void DestroyFramebuffer(struct Framebuffer *fb) {
    VirtualFree(fb->buffer, 0, MEM_RELEASE);
    fb->buffer = 0;
}

void OutputFramebuffer(HWND window, struct Framebuffer fb) {
    HDC window_dc = GetDC(window);
	StretchDIBits(window_dc, 0, 0, fb.width, fb.height, 0, 0, fb.width, fb.height, fb.buffer,
				  &fb.info, DIB_RGB_COLORS, SRCCOPY);
}

u32 RGB_Color(u8 red, u8 green, u8 blue) {
    return (u32)(((u8)0 << 24) + (red << 16) + (green << 8) + blue);
}

u32 RGBA_Color(u8 red, u8 green, u8 blue, u8 alpha) {
	return (u32)((alpha << 24) + (red << 16) + (green << 8) + blue);
}

void FillScreen(struct Framebuffer *framebuffer, u32 color) {
    u32 *pixel = (u32 *)framebuffer->buffer;
    for (int i = 0; i < framebuffer->width * framebuffer->height; ++i)
        *pixel++ = color;
}

void DrawPixel(struct Framebuffer *framebuffer, int x, int y, u32 color) {
    if (x >= 0 && x <= framebuffer->width && y >= 0 && y <= framebuffer->height) {
        u32 *pixel = (u32 *)framebuffer->buffer;

        pixel += x + (y * framebuffer->width);
        *pixel = color;
    }
}

void DrawRectangle(struct Framebuffer *framebuffer, int x0, int y0, int width, int height, u32 color) {
	/* clip the rectangle to the framebuffer dimensions */
	if((x0 > framebuffer->width) || ((x0 + width) < 0) ||
	   (y0 > framebuffer->height) || (y0 + height) < 0) {
		return;
	}

	
	
    u32 *pixel = (u32 *)framebuffer->buffer;
    pixel += x0 + (y0 * framebuffer->width);

    for (int yidx = 0; yidx < height; ++yidx) {
        for (int xidx = 0; xidx < width; ++xidx) {
            *pixel++ = color;
        }
        pixel += framebuffer->width - width;
    }
}

void DrawLine(struct Framebuffer *framebuffer, int x0, int y0, int x1, int y1, u32 color) {
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

void DrawTriangle(struct Framebuffer *framebuffer, int points[6], u32 color) {
    DrawLine(framebuffer, points[0], points[1], points[2], points[3], color);
    DrawLine(framebuffer, points[2], points[3], points[4], points[5], color);
    DrawLine(framebuffer, points[4], points[5], points[0], points[1], color);
}

void *ReadFileContent(char *filename) {
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

struct Bitmap LoadBitmapFile(char *filename) {
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

void HFlipBMP24bpp(struct Bitmap *bitmap) {
    size_t bitmap_size = bitmap->height * bitmap->width * bitmap->bpp;
    u8 *copy_bmp_pixel = malloc(bitmap_size);
    GetPixelFromBMP(bitmap, copy_bmp_pixel);

    u8 *dst = bitmap->pixel;
    u8 *src = copy_bmp_pixel + (bitmap->width * (bitmap->height - 1)) * bitmap->bpp;

    for (int y = 0; y < bitmap->height; y++) {
        for (int x = 0; x < bitmap->width; x++) {
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
        }
        src -= 2 * bitmap->width * bitmap->bpp;
    }

    free(copy_bmp_pixel);
    copy_bmp_pixel = 0;
}

void HFlipBMP32bpp(struct Bitmap *bitmap) {
    size_t bitmap_size = bitmap->height * bitmap->width * bitmap->bpp;
    u8 *copy_bmp_pixel = malloc(bitmap_size);
    GetPixelFromBMP(bitmap, copy_bmp_pixel);

    u32 *dst = (u32 *)bitmap->pixel;
    u32 *src = (u32 *)copy_bmp_pixel + (bitmap->width * (bitmap->height - 1));

    for (int y = 0; y < bitmap->height; y++) {
        for (int x = 0; x < bitmap->width; x++) {
            *dst++ = *src++;
        }
        src -= 2 * bitmap->width;
    }

    free(copy_bmp_pixel);
    copy_bmp_pixel = 0;
}

struct ClippedBmp ClipBitmap(struct Framebuffer *framebuffer,
							 int x, int y, int w, int h, int clip_x, int clip_y, int clip_w, int clip_h) {
	struct ClippedBmp result = {0};

	if((x >= framebuffer->width) || (y >= framebuffer->height) ||
	   ((x + w) <= 0) || (y + h <= 0)) {
		result.x1 = -1;
		result.y1 = -1;
		result.x2 = -1;
		result.y2 = -1;
		result.dx = -1;
		result.dy = -1;
		result.x_off = -1;
		result.y_off = -1;

        return result;
	}
	
	int x1 = x;
	int y1 = y;
	int x2 = x1 + w - 1;
	int y2 = y1 + h - 1;

	if(x1 < clip_x) {
		x1 = clip_x;
	}

	if(y1 < clip_y) {
		y1 = clip_y;
	}

	if(x2 >= (clip_x + clip_w)) {
		x2 = clip_x + clip_w - 1;
	}

	if(y2 >= (clip_y + clip_h)) {
		y2 = clip_y + clip_h - 1;
	}

	int x_off = x1 - x;
	int y_off = y1 - y;

	int dx = x2 - x1 + 1;
	int dy = y2 - y1 + 1;

	result.x1 = x1;
	result.y1 = y1;
	result.x2 = x2;
	result.y2 = y2;
	result.dx = dx;
	result.dy = dy;
	result.x_off = x_off;
	result.y_off = y_off;

	return result;
}

struct ClippedRect ClipRectangle(int x1, int y1, int x2, int y2, int clip_w, int clip_h) {
	struct ClippedRect result = {0};
	
	if((x >= clip_w) || (y >= clip_h) ||
	   ((x + w) <= 0) || (y + h <= 0)) {
		result.x1 = -1;
		result.y1 = -1;
		result.x2 = -1;
		result.y2 = -1;
		result.dx = -1;
		result.dy = -1;
		result.x_off = -1;
		result.y_off = -1;

        return result;
	}

}

void DrawBMP24bpp(struct Framebuffer *framebuffer, struct Bitmap bitmap, int x, int y, u32 color_mask) {

	struct ClippedBmp clippedBmp = ClipBitmap(framebuffer, x, y, bitmap.width, bitmap.height, 0, 0, framebuffer->width, framebuffer->height);
	
    u32 *dst = framebuffer->buffer;
	dst += clippedBmp.x1 + (clippedBmp.y1 * framebuffer->width);
    u8 *src = bitmap.pixel;
	src += (clippedBmp.x_off + (clippedBmp.y_off * bitmap.width)) * bitmap.bpp;

    for(int yidx = 0; yidx < clippedBmp.dy; yidx++) {
        for(int xidx = 0; xidx < clippedBmp.dx; xidx++) {
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
		src += (bitmap.width - clippedBmp.dx) * bitmap.bpp;
        dst += framebuffer->width - clippedBmp.dx;
    }
}

void DrawBMP32bpp(struct Framebuffer *framebuffer, struct Bitmap bitmap, int x, int y, u32 color_mask) {

	struct ClippedBmp clippedBmp = ClipBitmap(framebuffer, x, y, bitmap.width, bitmap.height, 0, 0, framebuffer->width, framebuffer->height);
	
    u32 *dst = (u32 *)framebuffer->buffer;
	dst += clippedBmp.x1 + (clippedBmp.y1 * framebuffer->width);
	u8 *src = (u8 *)bitmap.pixel;
	src += (clippedBmp.x_off + (clippedBmp.y_off * bitmap.width)) * bitmap.bpp;
	
    for(int yidx = 0; yidx < clippedBmp.dy; ++yidx) {
        for(int xidx = 0; xidx < clippedBmp.dx; ++xidx) {
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
		src += (bitmap.width - clippedBmp.dx) * bitmap.bpp;
        dst += framebuffer->width - clippedBmp.dx;
    }
}

void DrawBMP24bppToClipRegion(struct Framebuffer *framebuffer, struct Bitmap bitmap, int x, int y, u32 color_mask, int clip_x, int clip_y, int clip_w, int clip_h) {
	struct ClippedBmp clippedBmp = ClipBitmap(framebuffer, x, y, bitmap.width, bitmap.height, clip_x, clip_y, clip_w, clip_h);

	printf("x: %d, y: %d, w: %d, h: %d, dx: %d, dy: %d \n", clippedBmp.x1, clippedBmp.y1, clippedBmp.x2, clippedBmp.y2, clippedBmp.dx, clippedBmp.dy);
	
	u32 *dst = (u32 *)framebuffer->buffer;
	dst += clippedBmp.x1 + (clippedBmp.y1 * framebuffer->width);
	u8 *src = (u8 *)bitmap.pixel;
	src += (clippedBmp.x_off + (clippedBmp.y_off * bitmap.width)) * bitmap.bpp;
	
    for(int yidx = 0; yidx < clippedBmp.dy; ++yidx) {
        for(int xidx = 0; xidx < clippedBmp.dx; ++xidx) {
            u8 r = *src;
            u8 g = *(src + 1);
            u8 b = *(src + 2);

            u32 src_pixel = (0 << 24) + (r << 16) + (g << 8) + b;

            if(src_pixel != color_mask)
                *dst++ = src_pixel;
            else
                dst++;

            src += bitmap.bpp;
        }
		src += (bitmap.width - clippedBmp.dx) * bitmap.bpp;
        dst += framebuffer->width - clippedBmp.dx;
    }	
}

void GetPixelFromBMP(struct Bitmap *from, u8 *to) {
    for(int i = 0; i < from->height * from->width * from->bpp; i++)
        *(to + i) = *(from->pixel + i);
}

void DrawGlyph(struct Framebuffer *framebuffer, struct Font font, char ch, int x_pos, int y_pos) {
    u32 glyph_offsets[98];

    for(int i = 0; i < font.glyph_count; i++) {
        glyph_offsets[i] = i * (font.glyph_width + 1) * font.bmp.bpp;
    }

    // find glyph offset of the current character to be drawn
    u32 glyph_offset = 0;

	// calculate the offset in the font bitmap for the specific glyph
    for(int c = 0; c < 255; c ++) {
		if(ch == (char)c) {
			int start_of_char_type = 0;
			
			if(c >= 65 && c <= 90) { // A-Z
				start_of_char_type = 65;
				glyph_offset = glyph_offsets[c - start_of_char_type];
			}
			else if(c >= 97 && c <= 122) { // a-z
				start_of_char_type = 71;
				glyph_offset = glyph_offsets[c - start_of_char_type];
			}
			else if(c >= 48 && c <= 57) { // numbers 0-9
				start_of_char_type = 4;
				glyph_offset = glyph_offsets[c + start_of_char_type];
			}
			else if(c == 126) {
				start_of_char_type = 64;
				glyph_offset = glyph_offsets[c + start_of_char_type];
			}
		}
    }
        
    u32 *dst = (u32 *)framebuffer->buffer;
    u8 *src = (u8 *)font.bmp.pixel + glyph_offset;

    dst += x_pos + (y_pos * framebuffer->width);

    for(int y = 0; y < (int)font.bmp.height; ++y) {
        for(int x = 0; x < font.glyph_width; ++x) {
            u8 r = *src;
            u8 g = *(src + 1);
            u8 b = *(src + 2);
            //u8 a = *(src + 3);
			u8 a = 0;

            u32 src_pixel = (a << 24) + (r << 16) + (g << 8) + b;

            if(src_pixel != font.color_mask)
                *dst++ = src_pixel;
            else
                dst++;

            src += font.bmp.bpp;
        }
		src += (font.bmp.width - font.glyph_width) * font.bmp.bpp;
        dst += framebuffer->width - font.glyph_width;
    }
}

void DrawString(struct Framebuffer *buffer, struct Font font, char *string, int x, int y) {
    int new_char_offset = 0;
    
    for(int i = 0; i < StringLen(string)-1; i++) {
		if(string[i] == ' ') {
			new_char_offset += font.glyph_width + font.glyph_spacing;
		} else {
			DrawGlyph(buffer, font, string[i], x + new_char_offset, y);
			new_char_offset += font.glyph_width + font.glyph_spacing;
		}
    }
}

void InitSprite(struct Sprite *s, int x, int y, int frame_count, struct Bitmap *frames, int start_frame, double frame_time) {
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

void UpdateSpriteAnimation(struct Sprite *s) {
    if(s->current_timer >= s->timer_next_frame) {
        s->current_frame++;
        s->current_timer = 0.0;

        if(s->current_frame == s->animation_frame_count)
            s->current_frame = 0;
    }
}
