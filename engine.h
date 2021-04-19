#ifndef ENGINE_H_
#define ENGINE_H_

typedef struct Engine_Pixel{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}Engine_Pixel;

void Engine_init();

void Engine_update();

void Engine_draw();

void Engine_setWindowSize(int, int);

Engine_Pixel *Engine_getScreenPixelPointer(int, int);

#endif