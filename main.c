#include "engine/engine.h"
#include "engine/text.h"
#include "engine/renderer2d.h"

#include "stdio.h"
#include "math.h"
#include "string.h"

typedef struct Player{
	Vec2f pos;
	Vec2f size;
	Vec2f velocity;
	Vec2f acceleration;
	float speed;
}Player;

Font font;

Renderer2D renderer;
Renderer2D_Texture texture;

int WIDTH = 480;
int HEIGHT = 270;

Player player;

void Engine_start(){

	Engine_setWindowSize(WIDTH * 2.5, HEIGHT * 2.5);
	Engine_centerWindow();

	Renderer2D_init(&renderer, WIDTH, HEIGHT);

	Renderer2D_Texture_initFromFile(&texture, "assets/textures/6.jpg");

	font = getFont("assets/fonts/times.ttf", 100);

	player.pos = getVec2f(100, 100);
	player.size = getVec2f(40, 40);
	player.velocity = getVec2f(0, 0);
	player.acceleration = getVec2f(0, 0);
	player.speed = 100;

}

void Engine_update(float deltaTime){

	if(ENGINE_KEYS[ENGINE_KEY_Q].down){
		Engine_quit();
	}

	if(ENGINE_KEYS[ENGINE_KEY_A].down){
		player.velocity.x = -player.speed * deltaTime;
	}
	if(ENGINE_KEYS[ENGINE_KEY_D].down){
		player.velocity.x = player.speed * deltaTime;
	}
	if(ENGINE_KEYS[ENGINE_KEY_A].down && ENGINE_KEYS[ENGINE_KEY_D].down
	|| !ENGINE_KEYS[ENGINE_KEY_A].down && !ENGINE_KEYS[ENGINE_KEY_D].down){
		player.velocity.x = 0;
	}

	if(ENGINE_KEYS[ENGINE_KEY_W].down){
		player.velocity.y = -player.speed * deltaTime;
	}
	if(ENGINE_KEYS[ENGINE_KEY_S].down){
		player.velocity.y = player.speed * deltaTime;
	}
	if(ENGINE_KEYS[ENGINE_KEY_W].down && ENGINE_KEYS[ENGINE_KEY_S].down
	|| !ENGINE_KEYS[ENGINE_KEY_W].down && !ENGINE_KEYS[ENGINE_KEY_S].down){
		player.velocity.y = 0;
	}

	Vec2f_add(&player.pos, player.velocity);

	renderer.offsetX = 0;
	renderer.offsetY = 0;

}

void Engine_draw(){

	float alpha;
	Renderer2D_Color color;

	Renderer2D_updateDrawSize(&renderer, windowWidth, windowHeight);

	Renderer2D_clear(&renderer);

	Renderer2D_setShaderProgram(&renderer, renderer.colorShaderProgram);

	Renderer2D_beginRectangle(&renderer, -renderer.offsetX, -renderer.offsetY, renderer.width, renderer.height);

	alpha = 1.0;
	color = Renderer2D_getColor(0.6, 0.3, 0.4);

	Renderer2D_supplyUniform(&renderer, &alpha, "alpha", RENDERER2D_UNIFORM_TYPE_FLOAT);
	Renderer2D_supplyUniform(&renderer, &color, "color", RENDERER2D_UNIFORM_TYPE_COLOR);

	Renderer2D_drawRectangle(&renderer);

	Renderer2D_setShaderProgram(&renderer, renderer.textureShaderProgram);

	Renderer2D_beginRectangle(&renderer, player.pos.x, player.pos.y, player.size.x, player.size.y);

	Renderer2D_setTexture(&renderer, texture);

	alpha = 1.0;

	Renderer2D_supplyUniform(&renderer, &alpha, "alpha", RENDERER2D_UNIFORM_TYPE_FLOAT);

	Renderer2D_drawRectangle(&renderer);

}

void Engine_finnish(){

}
