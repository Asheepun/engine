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
	float gas;
	float resistance;
}Player;

typedef struct Block{
	Vec2f pos;
	Vec2f size;
	Vec2f velocity;

	Vec2f originPos;
	Vec2f originSize;
	Vec2f originVelocity;
}Block;

Font font;

Renderer2D renderer;
Renderer2D_Texture texture;

int WIDTH = 480;
int HEIGHT = 270;

float c = 150;

Player player;
Block blocks[255];
int blocksLength = 0;

void addBlock(Vec2f pos, Vec2f size, Vec2f velocity){

	blocks[blocksLength].pos = pos;
	blocks[blocksLength].size = size;
	blocks[blocksLength].velocity = velocity;

	blocks[blocksLength].originPos = pos;
	blocks[blocksLength].originSize = size;
	blocks[blocksLength].originVelocity = velocity;


	blocksLength++;

}

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
	//player.speed = 200;
	player.gas = 600;
	player.resistance = 0.01;

	addBlock(getVec2f(200, 100), getVec2f(50, 40), getVec2f(0, 0));
	addBlock(getVec2f(300, 100), getVec2f(100, 40), getVec2f(0, 0));

}

void Engine_update(float deltaTime){

	if(ENGINE_KEYS[ENGINE_KEY_Q].down){
		Engine_quit();
	}

	if(ENGINE_KEYS[ENGINE_KEY_A].down){
		player.acceleration.x = -player.gas;
	}
	if(ENGINE_KEYS[ENGINE_KEY_D].down){
		player.acceleration.x = player.gas;
	}
	if(ENGINE_KEYS[ENGINE_KEY_A].down && ENGINE_KEYS[ENGINE_KEY_D].down
	|| !ENGINE_KEYS[ENGINE_KEY_A].down && !ENGINE_KEYS[ENGINE_KEY_D].down){
		player.acceleration.x = 0;
	}

	if(ENGINE_KEYS[ENGINE_KEY_W].down){
		player.acceleration.y = -player.gas;
	}
	if(ENGINE_KEYS[ENGINE_KEY_S].down){
		player.acceleration.y = player.gas;
	}
	if(ENGINE_KEYS[ENGINE_KEY_W].down && ENGINE_KEYS[ENGINE_KEY_S].down
	|| !ENGINE_KEYS[ENGINE_KEY_W].down && !ENGINE_KEYS[ENGINE_KEY_S].down){
		player.acceleration.y = 0;
	}

	Vec2f_add(&player.velocity, getMulVec2fFloat(player.acceleration, deltaTime));

	Vec2f_mulByFloat(&player.velocity, pow(player.resistance, deltaTime));

	Vec2f_add(&player.pos, getMulVec2fFloat(player.velocity, deltaTime));

	for(int i = 0; i < blocksLength; i++){

		Block *block_p = &blocks[i];

		float lorentzFactorX = 1 / sqrt(1 - getSquare(player.velocity.x) / getSquare(c));
		float lorentzFactorY = 1 / sqrt(1 - getSquare(player.velocity.y) / getSquare(c));

		block_p->size.x = block_p->originSize.x / lorentzFactorX;
		block_p->size.y = block_p->originSize.y / lorentzFactorY;

		Vec2f playerCenter = getAddVec2f(player.pos, getDivVec2fFloat(player.size, 2));
		Vec2f blockOriginCenter = getAddVec2f(block_p->originPos, getDivVec2fFloat(block_p->originSize, 2));

		Vec2f deltaPos = getSubVec2f(blockOriginCenter, playerCenter);

		block_p->pos.x = playerCenter.x + deltaPos.x / lorentzFactorX - block_p->size.x / 2;
		block_p->pos.y = playerCenter.y + deltaPos.y / lorentzFactorY - block_p->size.y / 2;

	
	}

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

	for(int i = 0; i < blocksLength; i++){

		Block *block_p = &blocks[i];

		Renderer2D_setShaderProgram(&renderer, renderer.colorShaderProgram);

		Renderer2D_beginRectangle(&renderer, block_p->pos.x, block_p->pos.y, block_p->size.x, block_p->size.y);

		alpha = 1.0;
		color = Renderer2D_getColor(0.0, 0.0, 0.0);

		Renderer2D_supplyUniform(&renderer, &alpha, "alpha", RENDERER2D_UNIFORM_TYPE_FLOAT);
		Renderer2D_supplyUniform(&renderer, &color, "color", RENDERER2D_UNIFORM_TYPE_COLOR);

		Renderer2D_drawRectangle(&renderer);
	
	}

}

void Engine_finnish(){

}
