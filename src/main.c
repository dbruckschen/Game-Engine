#include <stdio.h>
#include "window.h"
#include "draw.h"
#include "timing.h"

int main(int argc, char **argv)
{
    // unreferenced arguments
    argc, argv;
    
    printf("Hello World\n");

    Window window = OpenWindow(800, 600, "Rudimentary Multimedia Library");
    Framebuffer fbuff = CreateFramebuffer(window.wnd_h);
    Input input = {0};
    Timer t = {0};
    Bitmap coin1 = LoadBitmapFile("coin1.bmp");

    // TODO: FLIP BITMAP
    u32 *dst = (u32*)coin1.pixel;
    u32 *src = ((u32*)coin1.pixel) + (coin1.width * coin1.height)-coin1.width;

    for(u32 y = 0; y < coin1.height; y++)
    {
	for(u32 x = 0; x < coin1.width; x++)
	{
	    *dst++ = *src++;
	}
	src = src - coin1.width;
    }
        
    InitTimer(&t);
    u8 green = 0;
    
    while(1)
    {
	StartTimer(&t);
		
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
	
	printf("%d\n", input.keyboard[a_key].toggle);

	DrawBMP24bpp(&fbuff, coin1, 50, 50, RGB_Color(255, 255, 255));
	OutputFramebuffer(window.wnd_h, fbuff);

	EndTimer(&t);
	printf("time: %f\n", t.elapsed_time); 
    }

    return 0;
}
