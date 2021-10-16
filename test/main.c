#include <stdio.h>
#include "window.h"
#include "draw.h"
#include "timing.h"
#include "common.h"

#define COIN_ANIM_COUNT 6
#define COIN_ANIM_FRAME_TIME 0.2

typedef struct {
    char glyph;
    u8 *pixel;
    u32 x;
    u32 y;
    u32 w;
    u32 h;

} Glyphs;

int main(int argc, char **argv) {
    // unreferenced arguments
    argc, argv;

    Window window = OpenWindow(840, 650, "Rudimentary Multimedia Library demo");
    Framebuffer fbuff = CreateFramebuffer(window.wnd_h);
    Input input = {0};
    Timer t = {0};

    Bitmap coin_animation[COIN_ANIM_COUNT];
    coin_animation[0] = LoadBitmapFile("../assets/coin1.bmp");
    coin_animation[1] = LoadBitmapFile("../assets/coin2.bmp");
    coin_animation[2] = LoadBitmapFile("../assets/coin3.bmp");
    coin_animation[3] = LoadBitmapFile("../assets/coin4.bmp");
    coin_animation[4] = LoadBitmapFile("../assets/coin5.bmp");
    coin_animation[5] = LoadBitmapFile("../assets/coin6.bmp");

    Sprite coin = {0};
    Sprite coin2 = {0};
    InitSprite(&coin, COIN_ANIM_COUNT, coin_animation, 0, COIN_ANIM_FRAME_TIME);
    InitSprite(&coin2, COIN_ANIM_COUNT, coin_animation, 2, COIN_ANIM_FRAME_TIME);

    Bitmap background = LoadBitmapFile("../assets/background.bmp");
    Bitmap background2 = {0};
    background2.pixel = malloc(background.height * background.width * background.bpp);
    background2.bpp = background.bpp;
    background2.width = background.width;
    background2.height = background.height;
    GetPixelFromBMP(&background, background2.pixel);

    Bitmap glyphs_bmp = LoadBitmapFile("../assets/font.bmp");
    Glyphs glyphs[256] = {0};

    Bitmap debug_bmp = LoadBitmapFile("../assets/test.bmp");

    glyphs['3'].glyph = '3';
    glyphs['3'].x = 0;
    glyphs['3'].y = 0;
    glyphs['3'].w = 126;
    glyphs['3'].h = 54;
    glyphs['3'].pixel = malloc(glyphs['3'].w * glyphs['3'].h * glyphs_bmp.bpp);
    GetSubRecPixel24bpp(glyphs_bmp,
    glyphs['3'].x,
    glyphs['3'].y,
    glyphs['3'].w,
    glyphs['3'].h,
    glyphs['3'].pixel);

    InitTimer(&t);
    double timer = 0;
    u8 green = 0;

    int debug_x = 0;
    int debug_y = 0;
    float speed = 500.0f;
    
    while(1) {
	StartTimer(&t);
	FillScreen(&fbuff, RGB_Color(50, 0, 0));
	
	if(!MessageLoop(&input))
	    break;
	
	
	if(input.keyboard[a_key].toggle) {
	    FillScreen(&fbuff, RGB(0, green, 0));
	    if(green < 255)
		green++;
	} else {
	    if(green > 0) {
		FillScreen(&fbuff, RGB(0, green--,  0));
	    }
	}

	if(input.keyboard[w_key].down) {
	    debug_y -= speed * t.elapsed_time;
	    if(debug_y < 0)
		debug_y = 0;
	}
	if(input.keyboard[a_key].down) 
	    debug_x -= speed * t.elapsed_time;
	    	
	if(input.keyboard[d_key].down) {
	    debug_x += speed * t.elapsed_time;
	    if(debug_y < 0)
		debug_y = 0;
	}
	if(input.keyboard[s_key].down)
	    debug_y += speed * t.elapsed_time;
	    
	UpdateSpriteAnimation(&coin);
	UpdateSpriteAnimation(&coin2);

	//DrawBMP24bpp(&fbuff, background2, 0, 0, RGB_Color(0, 0, 0));
	DrawBMP24bpp(&fbuff, background, 0, 0, RGB_Color(0, 0, 0));
	
	DrawBMP24bpp(&fbuff, coin.frames[coin.current_frame], 100, 100, RGB_Color(255, 0, 255));
	DrawBMP24bpp(&fbuff, coin2.frames[coin2.current_frame], 200, 200, RGB_Color(255, 0, 255));
	DrawBMPSubRec24bpp(&fbuff, glyphs_bmp, 0, 0, RGB_Color(0, 0, 0), 128, 9, 7, 9);
	DrawBuffer24bpp(&fbuff, glyphs['3'].pixel, 400, 200, glyphs['3'].w, glyphs['3'].h, RGB_Color(0, 0, 0));
	DrawRectangle(&fbuff, 350, 100, 50, 50, RGB_Color(100, 12, 253));
	DrawBMP24bpp(&fbuff, debug_bmp, debug_x, debug_y, RGB_Color(0, 0, 0));

	if(input.keyboard[tilde_key].toggle)
	    DrawRectangle(&fbuff, 0, 0, fbuff.width, 100, RGB_Color(0, 255, 0));
	

	Triangle tr = {0};
	tr.verts[0] = NDC_TO_DC(-0.5f, fbuff.width);
	tr.verts[1] = NDC_TO_DC(-0.5f, fbuff.height);
	tr.verts[2] = NDC_TO_DC(0.5f, fbuff.width); 
	tr.verts[3] = NDC_TO_DC(-0.5f, fbuff.height);
	tr.verts[4] = NDC_TO_DC(0.0f, fbuff.width);
	tr.verts[5] = NDC_TO_DC(0.5f, fbuff.height);
	tr.color = RGB_Color(255, 0, 0);

	//DrawTriangle(&fbuff, (u32*)tr.verts, RGB_Color(10, 12, 32));
	OutputFramebuffer(window.wnd_h, fbuff);

	EndTimer(&t);
	timer += t.elapsed_time;
	
	coin.current_timer += t.elapsed_time;
	coin2.current_timer += t.elapsed_time;
	
	//printf("%f ms/frame\n", t.elapsed_time);
    }

    return 0;
}
