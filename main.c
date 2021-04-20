#include "engine.h"

int WIDTH = 800;
int HEIGHT = 450;

void Engine_init(){

	Engine_setWindowSize(WIDTH, HEIGHT);

}

void Engine_update(){

}

void Engine_draw(){

	Engine_fillRect(0, 0, screenWidth, screenHeight, COLOR_BLACK);

	Vec2f verts[] = {
		400, 200,
		500, 300,
		300, 300,
	};

	Engine_drawTriangle(verts, ENGINE_COLORS[COLOR_BLUE]);

}

void Engine_finnish(){

}
