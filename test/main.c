#include <stdio.h>
#include "window.h"
#include "draw.h"
#include "timing.h"
#include "common.h"

#define COIN_ANIM_COUNT 6
#define COIN_ANIM_FRAME_TIME 0.2

typedef struct
{
    char glyph;
    u8 *pixel;
    u32 x;
    u32 y;
    u32 w;
    u32 h;

} Glyphs;

int main(int argc, char **argv)
{
    // unreferenced arguments
    argc, argv;
    
    Window window = OpenWindow(900, 600, "Rudimentary Multimedia Library");
    Framebuffer fbuff = CreateFramebuffer(window.wnd_h);
    Input input = {0};
    Timer t = {0};

    // Example of loading different bitmaps for an animation
    Bitmap coin_animation[COIN_ANIM_COUNT];
    coin_animation[0] = LoadBitmapFile("../assets/coin1.bmp");
    coin_animation[1] = LoadBitmapFile("../assets/coin2.bmp");
    coin_animation[2] = LoadBitmapFile("../assets/coin3.bmp");
    coin_animation[3] = LoadBitmapFile("../assets/coin4.bmp");
    coin_animation[4] = LoadBitmapFile("../assets/coin5.bmp");
    coin_animation[5] = LoadBitmapFile("../assets/coin6.bmp");
    // Creating Sprites. The same bitmap can be used for mutliple sprites.
    Sprite coin = {0};
    Sprite coin2 = {0};
    InitSprite(&coin, COIN_ANIM_COUNT, coin_animation, 0, COIN_ANIM_FRAME_TIME);
    InitSprite(&coin2, COIN_ANIM_COUNT, coin_animation, 2, COIN_ANIM_FRAME_TIME);

    // Creating an Array of sub rectangles which represent glyphs.
    Bitmap glyphs_bmp = LoadBitmapFile("../assets/font.bmp");
    Glyphs glyphs[256] = {0};
/*    
    u32 x = 0;
    u32 y = 0;
    for(char i = 32; i <= '~'; i++)
    {
	glyphs[i].glyph = i;
	glyphs[i].x = x;
	glyphs[i].y = y;
	glyphs[i].w = glyphs_bmp.width/18;
	glyphs[i].h = glyphs_bmp.height/7;
	glyphs[i].pixel = (u8*)malloc(glyphs[i].w * glyphs[i].h * glyphs_bmp.bpp);
	//GetSubRecPixel(glyphs_bmp, glyphs[i].x, glyphs[i].y, glyphs[i].w, glyphs[i].h, glyphs[i].pixel);

	
	x += glyphs[i].w;

	if(x > 128)
	{
	    x = 0;
	    y += glyphs[i].h;
	if(y > 64)
	{
	    y = 0;
	}
    }
*/  
    glyphs['3'].glyph = '3';
    glyphs['3'].x = 128;
    glyphs['3'].y = 9;
    glyphs['3'].w = 7;
    glyphs['3'].h = 9;
    glyphs['3'].pixel = malloc(glyphs['3'].w * glyphs['3'].h * glyphs_bmp.bpp);
    GetSubRecPixel(glyphs_bmp,
    glyphs['3'].x,
    glyphs['3'].y,
    glyphs['3'].w,
    glyphs['3'].h,
    glyphs['3'].pixel);

/*    GetSubRecPixel(glyphs_bmp, glyphs['3'].x,
		   glyphs['3'].y,
		   glyphs['3'].w,
		   glyphs['3'].h,
		   glyphs['3'].pixel);
*/

    InitTimer(&t);
    double timer = 0;
    u8 green = 0;

    while(1)
    {
	StartTimer(&t);
	FillScreen(&fbuff, RGB_Color(50, 0, 0));
	
	if(!MessageLoop(&input))
	{
	    break;
	}
	
	if(input.keyboard[a_key].toggle)
	{
	    FillScreen(&fbuff, RGB(0, green, 0));
		       
	    if(green < 255)
		green++;
	}
	else
	{
	    if(green > 0)
		FillScreen(&fbuff, RGB(0, green--,  0));
	}

	if(input.keyboard[tilde_key].toggle)
	{
	    DrawRectangle(&fbuff, 0, 0, fbuff.width, 100, RGB_Color(0, 255, 0));
	}
	
	UpdateSpriteAnimation(&coin);
	UpdateSpriteAnimation(&coin2);
	
	DrawBMP24bpp(&fbuff, coin.frames[coin.current_frame], 100, 100, RGB_Color(255, 0, 255));
	DrawBMP24bpp(&fbuff, coin2.frames[coin2.current_frame], 200, 200, RGB_Color(255, 0, 255));
	//DrawBMP24bpp(&fbuff, glyphs, 0, 0, RGB_Color(0, 0, 0));
	//DrawBMPSubRec24bpp(&fbuff, glyphs_bmp, 0, 0, RGB_Color(0, 0, 0), 128, 9, 7, 9);
	DrawArray24bpp(&fbuff, glyphs['3'].pixel, 100, 100, glyphs['3'].w, glyphs['3'].h, RGB_Color(0, 0, 0));

	OutputFramebuffer(window.wnd_h, fbuff);

	EndTimer(&t);
	timer += t.elapsed_time;
	
	coin.current_timer += t.elapsed_time;
	coin2.current_timer += t.elapsed_time;
	
	//printf("%f ms/frame\n", t.elapsed_time);
    }

    return 0;
}
