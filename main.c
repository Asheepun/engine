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

	if(ENGINE_KEYS[ENGINE_KEY_G].downed){
		printf("G is downed!\n");
	}
	if(ENGINE_KEYS[ENGINE_KEY_G].down){
		printf("G is down!\n");
	}
	if(ENGINE_KEYS[ENGINE_KEY_G].upped){
		printf("G is upped!\n");
	}

	x += 1;

}

void Engine_draw(){

	Engine_fillRect(0, 0, screenWidth, screenHeight, COLOR_RED);

	Vec2f verts[] = {
		400, 200,
		500, 300,
		x, 300,
	};

	Engine_drawTriangle(verts, ENGINE_COLORS[COLOR_BLUE]);

}

void Engine_finnish(){

}
