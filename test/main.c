#include <stdio.h>
#include "window.h"
#include "draw.h"
#include "timing.h"
#include "common.h"

#define WINDOW_WIDTH  1600
#define WINDOW_HEIGHT 900
#define TILE_SIZE 64
#define NUM_TILES_X (WINDOW_WIDTH / TILE_SIZE)
#define NUM_TILES_Y (WINDOW_HEIGHT / TILE_SIZE)

#define COIN_ANIM_COUNT 6       
#define COIN_ANIM_FRAME_TIME 0.2

char map[NUM_TILES_Y][NUM_TILES_X] = {
    {'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
    {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
    {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
    {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
    {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
    {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
    {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
    {'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
    {'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'}
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
	    }
	}
    }
}

enum glyph_idx
{
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    a,
    b,
    c,
    d,
    e,
    f,
    g,
    h,
    i,
    j,
    k,
    l,
    m,
    n,
    o,
    p,
    q,
    r,
    s,
    t,
    u,
    v,
    w,
    x,
    y,
    z,
    nill,
    one,
    two,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
};

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
	DrawBMP32bpp(&fbuff, font, 0, 0, RGB_Color(0, 0, 0));

	// test drawing string
	DrawString(&fbuff, font, "Hallo", 300, 300, RGB_Color(0, 0, 0));
		
	OutputFramebuffer(window.wnd_h, fbuff);

	EndTimer(&t);
	coin.current_timer += t.elapsed_time;
    }

    return 0;
}
