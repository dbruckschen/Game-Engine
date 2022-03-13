#include <stdio.h>
#include "window.h"
#include "draw.h"
#include "timing.h"
#include "common.h"

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080
#define TILE_SIZE 64
#define NUM_TILES_X (WINDOW_WIDTH / TILE_SIZE)
#define NUM_TILES_Y (WINDOW_HEIGHT / TILE_SIZE)

#define COIN_ANIM_COUNT 6       
#define COIN_ANIM_FRAME_TIME 0.2

char map[NUM_TILES_Y][NUM_TILES_X] = {
    {'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
    {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
    {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '1'},
    {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '1'},
    {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '1'},
    {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '1'},
    {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '1'},
    {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '1'},
    {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '1'},
    {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '1'},
    {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '1'},
    {'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '0', '0', '0', '1'}
};

void DrawTileMap(struct Framebuffer *fbuff)
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
			
			default:
				DrawRectangle(fbuff, x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE, RGB_Color(50, 58, 69));
			}
		}
    }
}

int main(void)
{
    struct Window window = OpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rudimentary Multimedia Library demo");
    struct Framebuffer fbuff = CreateFramebuffer(window.wnd_h);
    struct Input input = {0};
    struct Timer t = {0};
    InitTimer(&t);

    struct Bitmap font = LoadBitmapFile("../assets/font.bmp");

    struct Bitmap coin_animation[COIN_ANIM_COUNT];
    coin_animation[0] = LoadBitmapFile("../assets/coin1.bmp");
    coin_animation[1] = LoadBitmapFile("../assets/coin2.bmp");
    coin_animation[2] = LoadBitmapFile("../assets/coin3.bmp");
    coin_animation[3] = LoadBitmapFile("../assets/coin4.bmp");
    coin_animation[4] = LoadBitmapFile("../assets/coin5.bmp");
    coin_animation[5] = LoadBitmapFile("../assets/coin6.bmp");

    struct Sprite coin = {0};
    InitSprite(&coin, 100, 100, COIN_ANIM_COUNT, coin_animation, 0, COIN_ANIM_FRAME_TIME);

    printf("window width: %d window height: %d\n", window.width, window.height);

    int frames = 0;
    int fps = 0;
    double fps_timer = 0.0;
    
    while(1) {
		StartTimer(&t);
		
		if(!MessageLoop(&input))
			break;

		if(input.keyboard[d_key].down)
			coin.x += (float)t.elapsed_time * 500;
	
		else if(input.keyboard[a_key].down)
			coin.x -= (float)t.elapsed_time * 500;
	
		else if(input.keyboard[s_key].down)
			coin.y += (float)t.elapsed_time * 500;
	
		else if(input.keyboard[w_key].down)
			coin.y -= (float)t.elapsed_time * 500;
	
    	
		FillScreen(&fbuff, RGB_Color(100, 100, 0));
	
		DrawTileMap(&fbuff);
		UpdateSpriteAnimation(&coin);
		DrawBMP24bpp(&fbuff, coin.frames[coin.current_frame], (u32)coin.x, (u32)coin.y, RGB_Color(255, 0, 255));

		char performance_values[256];
		sprintf(performance_values, "ms/frame: %f", t.elapsed_time);
		DrawString(&fbuff, font, performance_values, 5, 10, RGB_Color(255, 255, 255));

		sprintf(performance_values, "#frames: %d", frames);
		DrawString(&fbuff, font, performance_values, 5, 20, RGB_Color(255, 255, 255));

		fps_timer += t.elapsed_time;
		if(fps_timer >= 1.0) {
			fps = (int)(frames/fps_timer);
			frames = 0;
			fps_timer = 0;
		}
		sprintf(performance_values, "fps: %d", fps);
		DrawString(&fbuff, font, performance_values, 5, 30, RGB_Color(255, 255, 255));

		DrawString(&fbuff, font, "HELLO", 100, 100, RGB_Color(255, 255, 255));
	
		OutputFramebuffer(window.wnd_h, fbuff);

		frames++;
	
		EndTimer(&t);
		coin.current_timer += t.elapsed_time;
    }

    return 0;
}
