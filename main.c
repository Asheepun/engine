#include "engine.h"
#include "stdio.h"

int WIDTH = 800;
int HEIGHT = 450;

int x = 300;

void Engine_init(){

	Engine_setWindowSize(WIDTH, HEIGHT);
	
	Engine_setWindowTitle("My cool program");

}

void Engine_update(){

	x += 1;

}

void Engine_draw(){

	Engine_fillRect(0, 0, screenWidth, screenHeight, COLOR_BLACK);

	Vec2f verts[] = {
		400, 200,
		500, 300,
		x, 300,
	};

	Engine_drawTriangle(verts, ENGINE_COLORS[COLOR_BLUE]);

}

void Engine_finnish(){

}
