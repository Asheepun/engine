#ifndef ENGINE_H_
#define ENGINE_H_

#include "stdlib.h"
#include "stdbool.h"
#include "geometry.h"

enum Engine_ColorEnum{
	COLOR_WHITE,
	COLOR_BLACK,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,
	ENGINE_COLORS_LENGTH,
};

typedef struct Engine_Pixel{
	float r;
	float g;
	float b;
}Engine_Pixel;

static Engine_Pixel ENGINE_COLORS[ENGINE_COLORS_LENGTH] = {
	1, 1, 1,
	0, 0, 0,
	1, 0, 0,
	0, 1, 0,
	0, 0, 1,
};

#define COLOR_BUFFER_SIZE 1920
static Engine_Pixel colorBuffers[ENGINE_COLORS_LENGTH][COLOR_BUFFER_SIZE];

static int screenWidth = 800;
static int screenHeight = 450;

static Engine_Pixel *screenPixels = NULL;

//ENGINE FUNCTIONS

//WINDOW FUNCTIONS

void Engine_setWindowTitle(char *);

void Engine_setWindowSize(int, int);

//DRAWING FUNCTIONS

Engine_Pixel *Engine_getScreenPixelPointer(int, int);

void Engine_fillRect(int, int, int, int, enum Engine_ColorEnum);

void Engine_drawLine(Vec2f, Vec2f, Engine_Pixel);

void Engine_drawTriangle(Vec2f *, Engine_Pixel);

void Engine_fillTriangle(Vec2f *, Engine_Pixel);

void Engine_init();

void Engine_update();

void Engine_draw();

void Engine_finnish();

#endif
