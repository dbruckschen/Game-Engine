#include "game.h"

void GameInit(struct GameState *game_state) {
	game_state->running = true;

	/* create window */
	const char *window_title = "Rudimentary Multimedia Library demo";
	game_state->window = OpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, window_title);

	/* create framebuffer */
	game_state->fbuff = CreateFramebuffer(game_state->window.wnd_h);

	/* init timer */
	InitTimer(&game_state->timer);

	/* load font bitmap */
	game_state->font = LoadBitmapFile("../assets/font.bmp");
}

void GameStart() {
	struct GameState game_state;
	GameInit(&game_state);
		
	while(game_state.running) {
		StartTimer(&game_state.timer);
		
		if(!MessageLoop(&game_state.input)) {
			break;
		}

		GameUpdate(&game_state);
		GameRender(&game_state.fbuff, &game_state.window);
	
		EndTimer(&game_state.timer);
	}
}

void GameUpdate(struct GameState *game_state) {
	/* char performance_values[256]; */
	/* sprintf(performance_values, "ms frame: %f", game_state->timer.elapsed_time); */
	/* DrawString(game_state->fbuff, game_state->font, performance_values, 5, 10, RGB_Color(255, 255, 255)); */

	/* sprintf(performance_values, "frames: %d", frames); */
	/* DrawString(&fbuff, font, performance_values, 5, 20, RGB_Color(255, 255, 255)); */

	/* fps_timer += game_state->timer.elapsed_time; */
	/* if(fps_timer >= 1.0) { */
	/* 	fps = (int)(frames/fps_timer); */
	/* 	frames = 0; */
	/* 	fps_timer = 0; */
	/* } */
	/* sprintf(performance_values, "fps: %d", fps); */
	/* DrawString(&fbuff, font, performance_values, 5, 30, RGB_Color(255, 255, 255)); */
	
	FillScreen(&game_state->fbuff, RGB_Color(186, 188, 190));
	DrawMatrix(&game_state->fbuff);
	/* DrawShape(game_state, 3, 3, O_SHAPE); */
	/* DrawShape(game_state, 5, 5, I_SHAPE); */
	/* DrawShape(game_state, 6, 8, T_SHAPE); */
	/* DrawShape(game_state, 2, 8, L_SHAPE); */
	/* DrawShape(game_state, 4, 11, J_SHAPE); */
	/* DrawShape(game_state, 2, 14, S_SHAPE); */
	/* DrawShape(game_state, 2, 17, Z_SHAPE); */
		
	DrawString(&game_state->fbuff, game_state->font, "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz 0123456789 ", 10, 50, RGB_Color(255, 255, 255));
	//~!@#$%^&*()-_+={}[];:'\",<.>/?
	/* frames++; */
}

void GameRender(struct Framebuffer *fbuff, struct Window *window) {
	OutputFramebuffer(window->wnd_h, *fbuff);
}

void DrawMatrix(struct Framebuffer *fbuff) {
	int offset_start_of_matrix_x = WINDOW_WIDTH/2 - (MATRIX_WIDTH*TILE_SIZE)/2;
	int offset_start_of_matrix_y = WINDOW_HEIGHT/2 - (MATRIX_HEIGHT*TILE_SIZE)/2;

	/* draw matrix tiles */
    for(int y = 0; y < MATRIX_HEIGHT; y++) {
		for(int x = 0; x < MATRIX_WIDTH; x++) {
			int offset_x = (x*TILE_SIZE) + offset_start_of_matrix_x;
			int offset_y = (y*TILE_SIZE) + offset_start_of_matrix_y;

			DrawRectangle(fbuff, offset_x, offset_y, TILE_SIZE, TILE_SIZE, RGB_Color(255, 255, 255));
		}
    }

	/* draw vertical grid lines */
	for(int i = 1; i < MATRIX_WIDTH; i++) {
		DrawLine(fbuff,
				 offset_start_of_matrix_x+(i*TILE_SIZE),
				 offset_start_of_matrix_y,
				 offset_start_of_matrix_x+(TILE_SIZE*i),
				 offset_start_of_matrix_y+(MATRIX_HEIGHT*TILE_SIZE),
				 RGB_Color(155, 156, 159));
	}

	u32 border_color = RGB_Color(155, 156, 159);
	/* draw horizontal grid lines */
	for(int i = 1; i < MATRIX_HEIGHT; i++) {
		DrawLine(fbuff,
				 offset_start_of_matrix_x,
				 offset_start_of_matrix_y + (TILE_SIZE*i),
				 offset_start_of_matrix_x + (MATRIX_WIDTH*TILE_SIZE),
				 offset_start_of_matrix_y + TILE_SIZE * i,
				 border_color);
	}

	/* draw top  matrix border */
	int border_thickness_pixel = 8;
	DrawRectangle(fbuff,
				  offset_start_of_matrix_x - border_thickness_pixel,
				  offset_start_of_matrix_y - border_thickness_pixel,
				  MATRIX_WIDTH * TILE_SIZE + (border_thickness_pixel*2),
				  border_thickness_pixel,
				  border_color);

	/* draw bottom matrix border */
	DrawRectangle(fbuff,
				  offset_start_of_matrix_x - border_thickness_pixel,
				  (MATRIX_HEIGHT * TILE_SIZE) + offset_start_of_matrix_y,
				  MATRIX_WIDTH * TILE_SIZE + (border_thickness_pixel*2),
				  border_thickness_pixel,
				  border_color);

	/* draw left matrix border */
	DrawRectangle(fbuff,
				  offset_start_of_matrix_x - border_thickness_pixel,
				  offset_start_of_matrix_y,
				  border_thickness_pixel,
				  MATRIX_HEIGHT * TILE_SIZE,
				  border_color);

	/* draw right matrix border */
	DrawRectangle(fbuff,
				  offset_start_of_matrix_x + (MATRIX_WIDTH * TILE_SIZE),
				  offset_start_of_matrix_y,
				  border_thickness_pixel,
				  MATRIX_HEIGHT * TILE_SIZE,
				  border_color);
}

static void FillMatrixTile(struct GameState *game_state, int x, int y, u32 color) {
	int offset_matrix_x = WINDOW_WIDTH/2 - (MATRIX_WIDTH*TILE_SIZE)/2;
	int offset_matrix_y = WINDOW_HEIGHT/2 - (MATRIX_HEIGHT*TILE_SIZE)/2;

	int x_rec = (x * TILE_SIZE) + offset_matrix_x + 1;
	int y_rec = (y * TILE_SIZE) + offset_matrix_y + 1;
	
	DrawRectangle(&game_state->fbuff, x_rec, y_rec, TILE_SIZE-1, TILE_SIZE-1, color);
}

static void DrawShape(struct GameState *game_state, int x, int y, enum ShapeType type) {
	switch(type) {
	case O_SHAPE:
		FillMatrixTile(game_state, x, y, YELLOW);
		FillMatrixTile(game_state, x+1, y, YELLOW);
		FillMatrixTile(game_state, x+1, y+1, YELLOW);
		FillMatrixTile(game_state, x, y+1, YELLOW);
		break;

	case I_SHAPE:
		FillMatrixTile(game_state, x, y, LIGHT_BLUE);
		FillMatrixTile(game_state, x+1, y, LIGHT_BLUE);
		FillMatrixTile(game_state, x+2, y, LIGHT_BLUE);
		FillMatrixTile(game_state, x+3, y, LIGHT_BLUE);
		break;

	case T_SHAPE:
		FillMatrixTile(game_state, x, y, PURPLE);
		FillMatrixTile(game_state, x-1, y+1, PURPLE);
		FillMatrixTile(game_state, x, y+1, PURPLE);
		FillMatrixTile(game_state, x+1, y+1, PURPLE);
		break;
		
	case L_SHAPE:
		FillMatrixTile(game_state, x, y, ORANGE);
		FillMatrixTile(game_state, x+1, y, ORANGE);
		FillMatrixTile(game_state, x+2, y, ORANGE);
		FillMatrixTile(game_state, x+2, y-1, ORANGE);
		break;

	case J_SHAPE:
		FillMatrixTile(game_state, x, y, DARK_BLUE);
		FillMatrixTile(game_state, x, y+1, DARK_BLUE);
		FillMatrixTile(game_state, x+1, y+1, DARK_BLUE);
		FillMatrixTile(game_state, x+2, y+1, DARK_BLUE);
		break;

	case S_SHAPE:
		FillMatrixTile(game_state, x, y, GREEN);
		FillMatrixTile(game_state, x+1, y, GREEN);
		FillMatrixTile(game_state, x+1, y-1, GREEN);
		FillMatrixTile(game_state, x+2, y-1, GREEN);
		break;

	case Z_SHAPE:
		FillMatrixTile(game_state, x, y, RED);
		FillMatrixTile(game_state, x+1, y, RED);
		FillMatrixTile(game_state, x+1, y+1, RED);
		FillMatrixTile(game_state, x+2, y+1, RED);
		break;
	};
}

static void SpawnShape(struct GameState *game_sate, enum ShapeType tpye) {
	game_state->shapes[num_shapes].type = type;
	game_state->num_shapes++;
}
