#include <stdio.h>
#include "window.h"
#include "draw.h"

int main(int argc, char **argv)
{
    printf("Hello World\n");
    Window window = OpenWindow(800, 600, "Rudimentary Multimedia Library");
    Framebuffer fbuff = CreateFramebuffer(window.wnd_h);
    Input input = {0};

    u8 green = 0;
    
    while(1)
    {
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

	OutputFramebuffer(window.wnd_h, fbuff);
    }

    return 0;
}
