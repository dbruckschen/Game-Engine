#ifndef GAME_H
#define GAME_H

#include "window.h"
#include "draw.h"
#include "timing.h"
#include "input.h"
#include "common.h"
//#include "allocator.h"

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

#define FALL_SPEED 1
#define SOFT_DROP_SPEED (FALL_SPEED/20.0)
#define HARD_DROP_SPEED 0.0001
#define AUTO_REPEAD_TIME 0.3

enum ShapeType {
	O_SHAPE,
	I_SHAPE,
	T_SHAPE,
	L_SHAPE,
	J_SHAPE,
	S_SHAPE,
	Z_SHAPE,
};

struct GameState {
	bool running;
	
	struct Shape shapes[512];
	int num_shapes;

	struct Window window;
	struct Framebuffer fbuff;
	struct Input input;
	struct Timer timer;
	struct Bitmap font;
};

struct Point {
	int x;
	int y;
};

struct Shape {
	struct Point p[4];
	enum ShapeType type;
	u32 color;
	bool locked;
};

static int matrix[MATRIX_HEIGHT][MATRIX_WIDTH];

static void GameInit(struct GameState *game_state);
void GameStart();
static void GameUpdate(struct GameState *game_state);
static void GameRender();

static void DrawMatrix(struct Framebuffer *fbuff);
static void FillMatrixTile(struct GameState *game_state, int x, int y, u32 color);
static void DrawShape(struct GameState *game_state, int x, int y, enum ShapeType type);

static void SpawnShape(enum ShapeType tpye);

#endif
