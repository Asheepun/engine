#include "engine.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int WIDTH = 480;
int HEIGHT = 270;

int textureWidth, textureHeight, channels;

unsigned char *textureData;

int width = 200;
int height = 100;

int posX = 100;
int posY = 100;

int scale = 2;

void Engine_init(){

	Engine_setWindowSize(WIDTH * 2.5, HEIGHT * 2.5);
	Engine_centerWindow();

	textureData = stbi_load("testtexture.png", &textureWidth, &textureHeight, &channels, 4);

}

void Engine_update(){


	//posX += 2;
	//posY += 2;

}

void Engine_draw(){

	Engine_fillRect(0, 0, WIDTH * scale, HEIGHT * scale, COLOR_BLACK);

	float textureScaleX = (float)textureWidth / (float)width;
	float textureScaleY = (float)textureHeight / (float)height;

	for(int i = 0; i < height; i++){
		for(int j = 0; j < width * scale; j++){

			unsigned int index = Engine_getScreenPixelIndex(posX * scale + j, posY * scale + i * scale);

			unsigned int textureIndex = 4 * (int)((int)(i * textureScaleY) * textureWidth + (int)(j / scale * textureScaleX)) + 3;

			if(textureData[textureIndex] > 0){
				screenPixels[index] = ENGINE_COLORS[COLOR_GREEN];
			}
		
		}
	}

	for(int i = 0; i < HEIGHT; i++){
		for(int j = 1; j < scale; j++){

			unsigned int index = Engine_getScreenPixelIndex(0, i * scale);

			memcpy(screenPixels + index + j * screenWidth, screenPixels + index, sizeof(Engine_Pixel) * screenWidth);
			
		}
	}

}

void Engine_finnish(){

}
