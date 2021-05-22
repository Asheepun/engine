#include "engine/engine.h"
#include "engine/text.h"
#include "engine/renderer2d.h"

#include "stdio.h"
#include "math.h"
#include "string.h"

Font font;

Renderer2D renderer;
Renderer2D_Texture texture;

int WIDTH = 480;
int HEIGHT = 270;

int width = 100;
int height = 100;

float posX = 100;
float posY = 100;

float velocityX = 0;
float velocityY = 0;

float speedX = 2;

float fontSize = 50;

int scale = 2;

void Engine_start(){

	Engine_setWindowSize(WIDTH * 2.5, HEIGHT * 2.5);
	Engine_centerWindow();

	Renderer2D_init(&renderer, WIDTH, HEIGHT);

	Renderer2D_Texture_initFromFile(&texture, "assets/textures/6.jpg");

	font = getFont("assets/fonts/times.ttf", 100);

}

void Engine_update(){

	if(ENGINE_KEYS[ENGINE_KEY_D].down){
		velocityX = speedX;
	}
	if(ENGINE_KEYS[ENGINE_KEY_A].down){
		velocityX = -speedX;
	}
	if(ENGINE_KEYS[ENGINE_KEY_A].down && ENGINE_KEYS[ENGINE_KEY_D].down
	|| !ENGINE_KEYS[ENGINE_KEY_A].down && !ENGINE_KEYS[ENGINE_KEY_D].down){
		velocityX = 0;
	}

	posX += velocityX;
	posY += velocityY;

	fontSize = 50 + sin(elapsedFrames * 0.05) * 40;

}

void Engine_draw(){

	Renderer2D_updateDrawSize(&renderer, screenWidth, screenHeight);

	Renderer2D_clearBackground(0.5, 0.3, 0.4, 1.0);

	Renderer2D_setShaderProgram(&renderer, renderer.colorShaderProgram);

	Renderer2D_beginRectangle(&renderer, posX, posY, 100, 100);

	Renderer2D_setTexture(&renderer, texture);

	float alpha = 1.0;

	Renderer2D_Color color = { 0.5, 1.0, 0.7 };

	Renderer2D_supplyUniform(&renderer, &alpha, "alpha", RENDERER2D_UNIFORM_TYPE_FLOAT);

	Renderer2D_supplyUniform(&renderer, &color, "color", RENDERER2D_UNIFORM_TYPE_COLOR);

	Renderer2D_drawRectangle(&renderer);

	Renderer2D_setShaderProgram(&renderer, renderer.textureShaderProgram);
	
	Renderer2D_beginText(&renderer, "hello", 50, 50, fontSize, font);

	alpha = 1.0;

	Renderer2D_supplyUniform(&renderer, &alpha, "alpha", RENDERER2D_UNIFORM_TYPE_FLOAT);

	Renderer2D_drawRectangle(&renderer);

}

void Engine_finnish(){

}
