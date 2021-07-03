#include <stdio.h>
#include "window.h"
#include "draw.h"
#include "timing.h"
#include "common.h"

#define COIN_ANIM_COUNT 6
#define COIN_ANIM_FRAME_TIME 0.2

int main(int argc, char **argv)
{
    // unreferenced arguments
    argc, argv;
    
    printf("Hello World\n");

    Window window = OpenWindow(800, 600, "Rudimentary Multimedia Library");
    Framebuffer fbuff = CreateFramebuffer(window.wnd_h);
    Input input = {0};
    Timer t = {0};

    Bitmap coin_animation[COIN_ANIM_COUNT];
    coin_animation[0] = LoadBitmapFile("coin1.bmp");
    coin_animation[1] = LoadBitmapFile("coin2.bmp");
    coin_animation[2] = LoadBitmapFile("coin3.bmp");
    coin_animation[3] = LoadBitmapFile("coin4.bmp");
    coin_animation[4] = LoadBitmapFile("coin5.bmp");
    coin_animation[5] = LoadBitmapFile("coin6.bmp");
    
    Sprite coin = {0};
    InitSprite(&coin, COIN_ANIM_COUNT, coin_animation, COIN_ANIM_FRAME_TIME);
    
    InitTimer(&t);
    double timer = 0;
    u8 green = 0;

    while(1)
    {
	StartTimer(&t);

	FillScreen(&fbuff, RGB_Color(0, 0, 0));
	
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
  
	if(coin.current_timer >= .2f)
	{
	    coin.current_frame++;
	    coin.current_timer = 0.0;
	}
	   
	if(coin.current_frame == coin.animation_frame_count)
	    coin.current_frame = 0;
	
	DrawBMP24bpp(&fbuff, coin.frames[coin.current_frame], 100, 100, RGB_Color(255, 0, 255));
	
	OutputFramebuffer(window.wnd_h, fbuff);

	EndTimer(&t);
	timer += t.elapsed_time;
	coin.current_timer += t.elapsed_time;
	
	//printf("%f ms/frame\n", t.elapsed_time);
    }

    return 0;
}
