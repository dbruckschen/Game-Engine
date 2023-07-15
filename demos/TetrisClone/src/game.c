#include "game.h"

void GameInit(struct GameState *gs) {
	InitRandomNumberGen();
	GenerateShapeOrder(&gs->shape_queue);
	
	gs->running = true;
	gs->previous_lock_down = true;

	/* create window */
	char *window_title = "Game Engine";
	gs->window = OpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, window_title);

	/* create framebuffer */
	gs->fbuff = CreateFramebuffer(gs->window.wnd_h);
	
	/* init timer */
	InitTimer(&gs->timer);

	/* load font bitmap */
	gs->font.bmp = LoadBitmapFile("../assets/font.bmp");
	gs->font.glyph_width = 5;
	gs->font.glyph_height = 7;
	gs->font.glyph_count = 98;
	gs->font.glyph_spacing = 2;
	gs->font.color_mask = RGB_Color(255, 255, 255);
}

void GameStart() {
	struct GameState gs;
	memset(&gs, 0, sizeof gs);
	GameInit(&gs);
		
	while(gs.running) {
		StartTimer(&gs.timer);

		ResetInput(&gs.input);
		
		if(!MessageLoop(&gs.input)) {
			break;
		}
		
		GameUpdate(&gs);
		GameRender(&gs);
	
		EndTimer(&gs.timer);
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
	
	/* update the mouse position */
	gs->input.mouse_cursor_pos = GetMousePosition(gs->window.wnd_h);
		
	/* update timer */
	if(gs->previous_lock_down) {
		gs->spawn_timer += gs->timer.elapsed_time;
	}
	
	gs->move_timer += gs->timer.elapsed_time;

	/* If shape queue is empty generate new  */
	if(gs->shape_queue.current_index == gs->shape_queue.length) {
		GenerateShapeOrder(&gs->shape_queue);
	}
	
	/* spawn shape after certain time, timer starts only if previous shape locked */
	if(gs->spawn_timer >= GENERATION_TIME) {
		gs->previous_lock_down = false;
		gs->spawn_timer = 0.0;
		
		for(int i = 0; i < MAX_SHAPES; i++) {
			/* find unused shape */
			if(!gs->shapes[i].alive) {
				/* get next shape out of shape queue */
				enum ShapeType next_type = GetNextShapeFromQueue(&gs->shape_queue);
				SpawnShape(gs->shapes, &gs->num_shapes, next_type);
				break;
			}
		}
	}

	/* move shape if arrow keys are pressed */
	if(gs->input.keyboard[arrow_up].pressed_this_frame) {
		printf("up\n");
	}
	if(gs->input.keyboard[arrow_down].pressed_this_frame) {
		printf("down\n");
	}
	if(gs->input.keyboard[arrow_left].pressed_this_frame) {
		printf("left\n");
	}
	if(gs->input.keyboard[arrow_right].pressed_this_frame) {
		printf("right\n");
	}

	// every shape consists of exactly 4 blocks/ rectangles.
	int block_count_of_shapes = SHAPE_BLOCK_COUNT;
	struct Shape current_shape = {0};
	struct Block block[SHAPE_BLOCK_COUNT] = {0};
	int current_shape_index = 0;
	
	for(int i_shape = 0; i_shape < gs->num_shapes; i_shape++) {
		// There can only be one locked shape at a time.
		// So we need to find this shape and check for collision with
		// every other shape.
		if(!gs->shapes[i_shape].locked) {
			current_shape = gs->shapes[i_shape];
			current_shape_index = i_shape;
			
			for(int i_block = 0; i_block < block_count_of_shapes; i_block++) {
				int current_shape_block_x = current_shape.p[i_block].x;
				int current_shape_block_y = current_shape.p[i_block].y;
			
				block[i_block] = GetShapeBlockDimensions(current_shape_block_x, current_shape_block_y);
			}
		}          
	}
	// Now we have all blocks of the current shape which is locked
	// (The current moving one which the player controls).
	// The next step is to check for collision of the current shape block (4 total)
	// with all other block of shapes.
	for(int i_shape = 0; i_shape < gs->num_shapes - 1; i_shape++) {
		if(i_shape == current_shape_index) {
			continue;
		}
			
		for(int i_current_shape_block = 0;
			i_current_shape_block < block_count_of_shapes;
			i_current_shape_block++) {

			v2i current_block_vec = V2I(block[i_current_shape_block].x, block[i_current_shape_block].y+TILE_SIZE);
			int current_block_w = TILE_SIZE;
			int current_block_h = TILE_SIZE;
			
			for(int i_block = 0; i_block < block_count_of_shapes; i_block++) {
				int current_locked_shape_x = gs->shapes[i_shape].p[i_block].x;
				int current_locked_shape_y = gs->shapes[i_shape].p[i_block].y;

				struct Block locked_block = GetShapeBlockDimensions(current_locked_shape_x,
																  current_locked_shape_y);
				v2i locked_block_vec = V2I(locked_block.x, locked_block.y);
										   
				if(BBAAI(current_block_vec, current_block_w, current_block_h,
						 locked_block_vec, locked_block.w, locked_block.h)) {
					gs->shapes[current_shape_index].locked = true;
					gs->previous_lock_down = true;
				}
			}
		}
	}
	
	/* lock shape if it reached the ground */
	for(int i_shape = 0; i_shape < gs->num_shapes; i_shape++) {
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
	if(gs->move_timer >= FALL_RATE) {
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
}

void GameRender(struct GameState *gs) {
	FillScreen(gs->fbuff, RGB_Color(186, 188, 190));
	DrawMatrix(gs->fbuff);

	for(int i = 0; i < MAX_SHAPES; i++) {
		if(gs->shapes[i].alive) {
			DrawShape(gs->fbuff, &gs->shapes[i]);
		}
	}

	OutputFramebuffer(gs->window.wnd_h, *gs->fbuff);
}

void DrawMatrix(struct Framebuffer *fbuff) {
	struct Point offset_start_of_matrix = MatrixStartCoords();

	/* draw matrix tiles */
    for(int y = 0; y < MATRIX_HEIGHT; y++) {
		for(int x = 0; x < MATRIX_WIDTH; x++) {
			int offset_x = (x*TILE_SIZE) + offset_start_of_matrix.x;
			int offset_y = (y*TILE_SIZE) + offset_start_of_matrix.y;

			DrawRectangle(fbuff, offset_x, offset_y, TILE_SIZE, TILE_SIZE, RGB_Color(255, 255, 255));
		}
    }

	/* draw vertical grid lines */
	for(int i = 1; i < MATRIX_WIDTH; i++) {
		DrawLine(fbuff,
				 offset_start_of_matrix.x+(i*TILE_SIZE),
				 offset_start_of_matrix.y,
				 offset_start_of_matrix.x+(TILE_SIZE*i),
				 offset_start_of_matrix.y+(MATRIX_HEIGHT*TILE_SIZE),
				 RGB_Color(155, 156, 159));
	}

	u32 border_color = RGB_Color(155, 156, 159);
	/* draw horizontal grid lines */
	for(int i = 1; i < MATRIX_HEIGHT; i++) {
		DrawLine(fbuff,
				 offset_start_of_matrix.x,
				 offset_start_of_matrix.y + (TILE_SIZE*i),
				 offset_start_of_matrix.x + (MATRIX_WIDTH*TILE_SIZE),
				 offset_start_of_matrix.y + TILE_SIZE * i,
				 border_color);
	}

	/* draw top  matrix border */
	int border_thickness_pixel = 8;
	DrawRectangle(fbuff,
				  offset_start_of_matrix.x - border_thickness_pixel,
				  offset_start_of_matrix.y - border_thickness_pixel,
				  MATRIX_WIDTH * TILE_SIZE + (border_thickness_pixel*2),
				  border_thickness_pixel,
				  border_color);

	/* draw bottom matrix border */
	DrawRectangle(fbuff,
				  offset_start_of_matrix.x - border_thickness_pixel,
				  (MATRIX_HEIGHT * TILE_SIZE) + offset_start_of_matrix.y,
				  MATRIX_WIDTH * TILE_SIZE + (border_thickness_pixel*2),
				  border_thickness_pixel,
				  border_color);

	/* draw left matrix border */
	DrawRectangle(fbuff,
				  offset_start_of_matrix.x - border_thickness_pixel,
				  offset_start_of_matrix.y,
				  border_thickness_pixel,
				  MATRIX_HEIGHT * TILE_SIZE,
				  border_color);

	/* draw right matrix border */
	DrawRectangle(fbuff,
				  offset_start_of_matrix.x + (MATRIX_WIDTH * TILE_SIZE),
				  offset_start_of_matrix.y,
				  border_thickness_pixel,
				  MATRIX_HEIGHT * TILE_SIZE,
				  border_color);
}

static void FillMatrixTile(struct Framebuffer *fbuff, int x, int y, u32 color) {
	int offset_matrix_x = WINDOW_WIDTH/2 - (MATRIX_WIDTH*TILE_SIZE)/2;
	int offset_matrix_y = WINDOW_HEIGHT/2 - (MATRIX_HEIGHT*TILE_SIZE)/2;

	int x_rec = (x * TILE_SIZE) + offset_matrix_x + 1;
	int y_rec = (y * TILE_SIZE) + offset_matrix_y + 1;

	struct Point p = MatrixStartCoords();
	struct Point matrix_dim = MatrixWidthHeight();
	
	DrawRectangleToClipRegion(fbuff, x_rec, y_rec, TILE_SIZE-1, TILE_SIZE-1, color, p.x, p.y, matrix_dim.x, matrix_dim.y);
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

static struct Point InitPoint(int x, int y) {
	struct Point p = {x, y};
	return p;
}

static struct Shape InitShape(enum ShapeType type) {
	struct Shape s = {0};
	s.alive = true;
	s.locked = false;

	// inital position of shapes 
	int x = 4;
	int y = -4;
	
	switch(type) {
	case O_SHAPE:
		s.color = YELLOW;	
		s.p[0] = InitPoint(x, y);
		s.p[1] = InitPoint(x+1, y);
		s.p[2] = InitPoint(x, y+1);
		s.p[3] = InitPoint(x+1, y+1);
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
		s.p[1] = InitPoint(x+1, y);
		s.p[2] = InitPoint(x+2, y);
		s.p[3] = InitPoint(x+1, y+1);
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
		s.p[0] = InitPoint(x, y+1);
		s.p[1] = InitPoint(x+1, y+1);
		s.p[2] = InitPoint(x+1, y);
		s.p[3] = InitPoint(x+2, y);
		break;

	case Z_SHAPE:
		s.color = RED;	
		s.p[0] = InitPoint(x, y);
		s.p[1] = InitPoint(x+1, y);
		s.p[2] = InitPoint(x+1, y+1);
		s.p[3] = InitPoint(x+2, y+1);
		break;
	};

	return s;
}

static void SpawnShape(struct Shape *shapes, int *num_shapes, enum ShapeType type) {
	bool valid_shape = false;
	for(int i = 0; i < NUM_SHAPE_TYPES; i++) {
		if(type == i) {
			valid_shape = true;
		}
	}
	assert(valid_shape);
	
	struct Shape s = InitShape(type);

	shapes[*num_shapes] = s;
	(*num_shapes)++;
}

static void GenerateShapeOrder(struct ShapeQueue *queue) {
	int lower = 0;
	int upper = NUM_SHAPE_TYPES-1;
	
   	queue->length = NUM_SHAPE_TYPES;
		
	for(int i = 0; i < queue->length; i++) {
		int next_type = GenRandomInt(lower, upper);
		queue->queue[i] = next_type;
	}

	queue->current_index = 0;
}

static enum ShapeType GetNextShapeFromQueue(struct ShapeQueue *queue) {
	assert(queue->current_index < queue->length);
	
	enum ShapeType type = queue->queue[queue->current_index];
	queue->current_index++;

	return type;
}

static struct Point MatrixStartCoords() {
	int offset_start_of_matrix_x = WINDOW_WIDTH/2 - (MATRIX_WIDTH*TILE_SIZE)/2;
	int offset_start_of_matrix_y = WINDOW_HEIGHT/2 - (MATRIX_HEIGHT*TILE_SIZE)/2;

	struct Point result = {offset_start_of_matrix_x, offset_start_of_matrix_y};

	return result;
}

static struct Point MatrixWidthHeight() {
	struct Point result = {0};
	result.x = MATRIX_WIDTH * TILE_SIZE;
	result.y = MATRIX_HEIGHT * TILE_SIZE;

	return result;
}

static struct Point GridCoordsToScreenCoords(int x, int y) {
	int offset_matrix_x = WINDOW_WIDTH/2 - (MATRIX_WIDTH*TILE_SIZE)/2;
	int offset_matrix_y = WINDOW_HEIGHT/2 - (MATRIX_HEIGHT*TILE_SIZE)/2;

	int x_rec = (x * TILE_SIZE) + offset_matrix_x + 1;
	int y_rec = (y * TILE_SIZE) + offset_matrix_y + 1;
	
	struct Point p = {0};
	p.x = x_rec;
	p.y = y_rec;

	return p;
}

static struct Block GetShapeBlockDimensions(int x, int y) {
	struct Block b;

	struct Point p = GridCoordsToScreenCoords(x, y);
	
	b.x = p.x;
	b.y = p.y;
	b.w = x + TILE_SIZE;
	b.h = y + TILE_SIZE;

	return b;
}
