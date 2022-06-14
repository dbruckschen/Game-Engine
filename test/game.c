#include "game.h"

void GameInit(struct GameState *game_state) {
	InitRandomNumberGen();
	GenerateShapeOrder(game_state->random_shape_queue);
		
	game_state->running = true;
	game_state->previous_lock_down = true;

	/* create window */
	const char *window_title = "Rudimentary Multimedia Library demo";
	game_state->window = OpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, window_title);

	/* create framebuffer */
	game_state->fbuff = CreateFramebuffer(game_state->window.wnd_h);

	/* init timer */
	InitTimer(&game_state->timer);

	/* load font bitmap */
	game_state->font = LoadBitmapFile("../assets/font.bmp");
	
	game_state->debug = LoadBitmapFile("../assets/test.bmp");
	InitSprite(&game_state->debug_sprite, 100, 100, 1, &game_state->debug, 1, RGB_Color(255, 0, 255));
}

void GameStart() {
	struct GameState game_state;
	memset(&game_state, 0, sizeof game_state);
	GameInit(&game_state);
		
	while(game_state.running) {
		StartTimer(&game_state.timer);
		
		if(!MessageLoop(&game_state.input)) {
			break;
		}

		GameUpdate(&game_state);
		GameRender(&game_state);
	
		EndTimer(&game_state.timer);
	}
}

void GameUpdate(struct GameState *gs) {
	/* char performance_values[256]; */
	/* sprintf(performance_values, "ms frame: %f", gs->timer.elapsed_time); */
	/* DrawString(gs->fbuff, gs->font, performance_values, 5, 10, RGB_Color(255, 255, 255)); */

	/* sprintf(performance_values, "frames: %d", frames); */
	/* DrawString(&fbuff, font, performance_values, 5, 20, RGB_Color(255, 255, 255)); */

	/* fps_timer += gs->timer.elapsed_time; */
	/* if(fps_timer >= 1.0) { */
	/* 	fps = (int)(frames/fps_timer); */
	/* 	frames = 0; */
	/* 	fps_timer = 0; */
	/* } */
	/* sprintf(performance_values, "fps: %d", fps); */
	/* DrawString(&fbuff, font, performance_values, 5, 30, RGB_Color(255, 255, 255)); */

	/* update timer */
	if(gs->previous_lock_down) {
		gs->spawn_timer += gs->timer.elapsed_time;
	}
	
	gs->move_timer += gs->timer.elapsed_time;

	/* spawn shape after certain time, timer starts only if previous shape locked */
	if(gs->spawn_timer >= GENERATION_TIME) {
		gs->previous_lock_down = false;
		gs->spawn_timer = 0.0;
		
		for(int i = 0; i < MAX_SHAPES; i++) {
			/* find unused shape */
			if(!gs->shapes[i].alive) {
				/* get next shape out of shape queue */
				gs->shapes[i] = InitShape(J_SHAPE);
				break;
			}
		}
	}

	/* lock shape if it reached the ground or had collusion with other shapes*/
	for(int i_shape = 0; i_shape < MAX_SHAPES; i_shape++) {
		for(int i_block = 0; i_block < SHAPE_BLOCK_COUNT; i_block++) {
			if(gs->shapes[i_shape].p[i_block].y == MATRIX_HEIGHT-1 &&
			   !gs->shapes[i_shape].locked) {
				gs->shapes[i_shape].locked = true;
				gs->previous_lock_down = true;
				break;
			}
		}
	}

	/* move shape every .5 sec for now */
	if(gs->move_timer >= FALL_SPEED) {
		for(int i = 0; i < MAX_SHAPES; i++) {
			if(gs->shapes[i].alive && !gs->shapes[i].locked) {
				/* check if matrix square is already occupied before moving shape */
				for(int i_block = 0; i_block < SHAPE_BLOCK_COUNT; i_block++) {
					gs->shapes[i].p[i_block].y++;
				}
			}
		}
		gs->move_timer = 0.0;
	}
	
	//DrawString(&gs->fbuff, gs->font, "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz 0123456789 ", 10, 50, RGB_Color(255, 255, 255));
	//~!@#$%^&*()-_+={}[];:'\",<.>/?
	/* frames++; */
}

void GameRender(struct GameState *gs) {
	FillScreen(&gs->fbuff, RGB_Color(186, 188, 190));
	DrawMatrix(&gs->fbuff);

	for(int i = 0; i < MAX_SHAPES; i++) {
		if(gs->shapes[i].alive) {
			DrawShape(&gs->fbuff, &gs->shapes[i]);
		}
	}

	if(gs->input.left_click_down) {
		v2 mouse_pos = GetMousePosition(gs->window.wnd_h);
		int center_x = mouse_pos.x - 32;
		int center_y = mouse_pos.y - 32;
			
		v2 bmp_pos = {gs->debug_sprite.x, gs->debug_sprite.y};
		if(BBAA(mouse_pos, 32, 32, bmp_pos, gs->debug_sprite.frames[0].width, gs->debug_sprite.frames[0].height)) {
			printf("mouse & bitmap collision\n");
			/* gs->debug_sprite.x = mouse_pos.x; */
			/* gs->debug_sprite.y = mouse_pos.y; */
		}
	}
	
	DrawColorPicker(&gs->fbuff, 200, 200);
	DrawBMP24bpp(&gs->fbuff, gs->debug, (int)gs->debug_sprite.x, (int)gs->debug_sprite.y, RGB_Color(255, 0, 255));

	DrawString(&gs->fbuff, gs->font, "Hello World 1234567890", 300, 300, RGB_Color(255, 255, 255));
	
	OutputFramebuffer(gs->window.wnd_h, gs->fbuff);
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

static void FillMatrixTile(struct Framebuffer *fbuff, int x, int y, u32 color) {
	int offset_matrix_x = WINDOW_WIDTH/2 - (MATRIX_WIDTH*TILE_SIZE)/2;
	int offset_matrix_y = WINDOW_HEIGHT/2 - (MATRIX_HEIGHT*TILE_SIZE)/2;

	int x_rec = (x * TILE_SIZE) + offset_matrix_x + 1;
	int y_rec = (y * TILE_SIZE) + offset_matrix_y + 1;
	
	DrawRectangle(fbuff, x_rec, y_rec, TILE_SIZE-1, TILE_SIZE-1, color);
}

static void DrawShape(struct Framebuffer *fbuff, struct Shape *shape) {
	switch(shape->type) {
	case O_SHAPE:
		for(int i = 0; i < SHAPE_BLOCK_COUNT; i++) {
			FillMatrixTile(fbuff, shape->p[i].x, shape->p[i].y, shape->color);
		}
		break;

	case I_SHAPE:
		for(int i = 0; i < SHAPE_BLOCK_COUNT; i++) {
			FillMatrixTile(fbuff, shape->p[i].x, shape->p[i].y, shape->color);
		}
		break;

	case T_SHAPE:
		for(int i = 0; i < SHAPE_BLOCK_COUNT; i++) {
			FillMatrixTile(fbuff, shape->p[i].x, shape->p[i].y, shape->color);
		}
		break;
		
	case L_SHAPE:
		for(int i = 0; i < SHAPE_BLOCK_COUNT; i++) {
			FillMatrixTile(fbuff, shape->p[i].x, shape->p[i].y, shape->color);
		}
		break;

	case J_SHAPE:
		for(int i = 0; i < SHAPE_BLOCK_COUNT; i++) {
			FillMatrixTile(fbuff, shape->p[i].x, shape->p[i].y, shape->color);
		}
		break;

	case S_SHAPE:
		for(int i = 0; i < SHAPE_BLOCK_COUNT; i++) {
			FillMatrixTile(fbuff, shape->p[i].x, shape->p[i].y, shape->color);
		}
		break;

	case Z_SHAPE:
		for(int i = 0; i < SHAPE_BLOCK_COUNT; i++) {
			FillMatrixTile(fbuff, shape->p[i].x, shape->p[i].y, shape->color);
		}
		break;
	};
}

static void SpawnShape(struct GameState *gs, enum ShapeType type) {
	gs->shapes[gs->num_shapes].type = type;
	gs->num_shapes++;
}

static struct Point InitPoint(int x, int y) {
	struct Point p = {x, y};
	return p;
}

static struct Shape InitShape(enum ShapeType type) {
	struct Shape s = {0};
	s.alive = true;
	s.locked = false;

	/* starting spawn points*/
	int x = 4;
	int y = 0;
	
	switch(type) {
	case O_SHAPE:
		s.color = YELLOW;	
		s.p[0] = InitPoint(x, y);
		s.p[1] = InitPoint(x+1, y);
		s.p[2] = InitPoint(x+1, y+1);
		s.p[3] = InitPoint(x, y+1);
		break;

	case I_SHAPE:
		s.color = LIGHT_BLUE;	
		s.p[0] = InitPoint(x, y);
		s.p[1] = InitPoint(x+1, y);
		s.p[2] = InitPoint(x+2, y);
		s.p[3] = InitPoint(x+3, y);
		break;

	case T_SHAPE:
		s.color = PURPLE;	
		s.p[0] = InitPoint(x, y);
		s.p[1] = InitPoint(x-1, y);
		s.p[2] = InitPoint(x+1, y);
		s.p[3] = InitPoint(x, y+1);
		break;

	case L_SHAPE:
		s.color = ORANGE;	
		s.p[0] = InitPoint(x, y+1);
		s.p[1] = InitPoint(x, y+2);
		s.p[2] = InitPoint(x, y+3);
		s.p[3] = InitPoint(x+1, y+3);
		break;

	case J_SHAPE:
		s.color = DARK_BLUE;	
		s.p[0] = InitPoint(x, y+1);
		s.p[1] = InitPoint(x, y+2);
		s.p[2] = InitPoint(x, y+3);
		s.p[3] = InitPoint(x-1, y+3);
		break;
		
	case S_SHAPE:
		s.color = GREEN;	
		s.p[0] = InitPoint(x, y);
		s.p[0] = InitPoint(x+1, y);
		s.p[0] = InitPoint(x+1, y-1);
		s.p[0] = InitPoint(x+2, y-1);
		break;

	case Z_SHAPE:
		s.color = RED;	
		s.p[0] = InitPoint(x, y);
		s.p[0] = InitPoint(x+1, y);
		s.p[0] = InitPoint(x+1, y+1);
		s.p[0] = InitPoint(x+2, y+1);
		break;
	};

	return s;
}

static void	GenerateShapeOrder(enum ShapeType *shape_queue) {
	int lower = 1;
	int upper = NUM_SHAPE_TYPES;
	bool shapes_drawn[7] = {0};

	for(int i = 0; i < NUM_SHAPE_TYPES; i++) {
		int next_type = GenRandomInt(lower, upper);
		shape_queue[i] = next_type;
		shapes_drawn[next_type] = true;
	}
}
