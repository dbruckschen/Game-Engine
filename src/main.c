#include <stdio.h>
#include "window.h"
#include "draw.h"
#include "timing.h"
#include "common.h"

#define COIN_ANIM_COUNT 6

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

    Sprite Coin = {0};
    Coin.animation_frame_count = 6
    Coin.frames = malloc(Coin.animation_frame_count * sizeof(Bitmap));
    
    coin_animation[0] = LoadBitmapFile("coin1.bmp");
    coin_animation[1] = LoadBitmapFile("coin2.bmp");
    coin_animation[2] = LoadBitmapFile("coin3.bmp");
    coin_animation[3] = LoadBitmapFile("coin4.bmp");
    coin_animation[4] = LoadBitmapFile("coin5.bmp");
    coin_animation[5] = LoadBitmapFile("coin6.bmp");
  
    InitTimer(&t);
    double timer = 0;
    u8 green = 0;

    int anim_counter = 0;
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
	    if(green < 255)
		FillScreen(&fbuff, RGB(0, green++, 0));
	}
	else
	{
	    if(green > 0)
		FillScreen(&fbuff, RGB(0, green--,  0));
	}
  
	if(timer >= .2f)
	{
	    anim_counter++;
	    timer = 0.0f;
	}
	   
	if(anim_counter > 5)
	    anim_counter = 0;
	
	DrawBMP24bpp(&fbuff, coin_animation[anim_counter], 100, 100, RGB_Color(255, 0, 255));
	
	OutputFramebuffer(window.wnd_h, fbuff);

	EndTimer(&t);
	timer += t.elapsed_time;
	printf("%f\n", t.elapsed_time);
    }

    return 0;
}
