#include <stdio.h>
#include "window.h"
#include "draw.h"
#include "timing.h"
#include "common.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define TILE_SIZE 64
#define NUM_TILES_X (WINDOW_WIDTH / TILE_SIZE)
#define NUM_TILES_Y (WINDOW_HEIGHT / TILE_SIZE)

#define COIN_ANIM_COUNT 6       
#define COIN_ANIM_FRAME_TIME 0.2

char map[NUM_TILES_Y][NUM_TILES_X] = { {'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
				       {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
				       {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
				       {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
				       {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
				       {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
				       {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
				       {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
				       {'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'}};

void DrawTileMap(Framebuffer *fbuff)
{
    for(int y = 0; y < NUM_TILES_Y; y++) {
	for(int x = 0; x < NUM_TILES_X; x++) {
	    switch(map[y][x]) {
	    case '0':
		DrawRectangle(fbuff, x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE, RGB_Color(0, 255, 0));
		break;
	    case '1':
		DrawRectangle(fbuff, x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE, RGB_Color(100, 100, 100));
		break;
	    }
	}
    }
}

int main(void)
{
    Window window = OpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rudimentary Multimedia Library demo");
    Framebuffer fbuff = CreateFramebuffer(window.wnd_h);
    Input input = {0};
    Timer t = {0};
    InitTimer(&t);
    
    Bitmap coin_animation[COIN_ANIM_COUNT];
    coin_animation[0] = LoadBitmapFile("../assets/coin1.bmp");
    coin_animation[1] = LoadBitmapFile("../assets/coin2.bmp");
    coin_animation[2] = LoadBitmapFile("../assets/coin3.bmp");
    coin_animation[3] = LoadBitmapFile("../assets/coin4.bmp");
    coin_animation[4] = LoadBitmapFile("../assets/coin5.bmp");
    coin_animation[5] = LoadBitmapFile("../assets/coin6.bmp");

    Sprite coin = {0};
    InitSprite(&coin, COIN_ANIM_COUNT, coin_animation, 0, COIN_ANIM_FRAME_TIME);
	
    while(1) {
	StartTimer(&t);
	FillScreen(&fbuff, RGB_Color(50, 0, 0));
		
	if(!MessageLoop(&input))
	    break;

	DrawTileMap(&fbuff);
	UpdateSpriteAnimation(&coin);
	DrawBMP24bpp(&fbuff, coin.frames[coin.current_frame], 100, 100, RGB_Color(255, 0, 255));

	OutputFramebuffer(window.wnd_h, fbuff);

	EndTimer(&t);
	coin.current_timer += t.elapsed_time;
    }

    return 0;
}
