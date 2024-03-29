#ifndef GAME_H
#define GAME_H

#include "window.h"
#include "draw.h"
#include "timing.h"
#include "input.h"
#include "common.h"
#include "mathlib.h"
#include "ui.h"
#include "linked_list.h"
//#include "allocator.h"

#include <stdio.h>
#include <assert.h>


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define MATRIX_WIDTH 10
#define MATRIX_HEIGHT 20

#define TILE_SIZE 32

#define MATRIX_TILE '0'
#define WALL '1'

#define YELLOW 0xFFD702
#define LIGHT_BLUE 0x24BEF4
#define PURPLE 0x861F84
#define ORANGE 0xF89B39
#define DARK_BLUE 0x1388CA
#define GREEN 0x8EC85A
#define RED 0xE11A21

#define FALL_RATE .05
#define GENERATION_TIME .2
#define LOCK_DOWN_TIMER .5
#define SOFT_DROP_SPEED (FALL_RATE/20.0)
#define HARD_DROP_SPEED 0.0001
#define AUTO_REPEAD_TIME 0.3

#define NUM_SHAPE_TYPES 7
#define MAX_SHAPES 512
#define SHAPE_BLOCK_COUNT 4
#define NUM_QUEUE_SHAPES 7

enum ShapeType {
	O_SHAPE,
	I_SHAPE,
	T_SHAPE,
	L_SHAPE,
	J_SHAPE,
	S_SHAPE,
	Z_SHAPE,
};

struct Point {
	int x;
	int y;
};

struct Block {
	int x;
	int y;
	int w;
	int h;
};

struct Shape {
	bool alive;
	// Grid/Matrixs position not screen coordinates
	struct Point p[SHAPE_BLOCK_COUNT];
	enum ShapeType type;
	u32 color;
	bool locked;
};

struct ShapeQueue {
	enum ShapeType queue[NUM_QUEUE_SHAPES];
	int length;
	int current_index;
};

struct GameState {
	bool running;
	bool first_frame;
	bool previous_lock_down;

	double spawn_timer;
	double move_timer;

	struct Shape shapes[MAX_SHAPES];
	int num_shapes;

	struct ShapeQueue shape_queue;

	struct Window window;
	struct Framebuffer *fbuff;
	struct Input input;
	struct Timer timer;
	struct Font font;
};

static int matrix[MATRIX_HEIGHT][MATRIX_WIDTH];

static void GameInit(struct GameState *game_state);
void GameStart();
static void GameUpdate(struct GameState *game_state);
static void GameRender(struct GameState *game_state);

static void DrawMatrix(struct Framebuffer *fbuff);
static void FillMatrixTile(struct Framebuffer *fbuff, int x, int y, u32 color);
static void DrawShape(struct Framebuffer *fbuff, struct Shape *shape);

static struct Point InitPoint(int x, int y);
static struct Shape InitShape(enum ShapeType type);
static void SpawnShape(struct Shape *shapes, int *num_shapes, enum ShapeType type);
static void GenerateShapeOrder(struct ShapeQueue *queue);

static struct Point MatrixStartCoords();
static struct Point MatrixWidthHeight();
static struct Point GridCoordsToScreenCoords(int x, int y);
static struct Block GetShapeBlockDimensions(int x, int y);
static enum ShapeType GetNextShapeFromQueue(struct ShapeQueue *queue);

#endif
