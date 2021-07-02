#include <stdio.h>
#include "window.h"
#include "draw.h"
#include "timing.h"
#include "common.h"

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
	
	//printf("%d\n", input.keyboard[a_key].toggle);
	DrawBMP24bpp(&fbuff, coin1, 250, 250, RGB_Color(255, 255, 255));
	
    	OutputFramebuffer(window.wnd_h, fbuff);

	EndTimer(&t);
	//printf("time: %f\n", t.elapsed_time); 
    }

    return 0;
}
