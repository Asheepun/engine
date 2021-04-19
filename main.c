#include "engine.h"
#include "stdio.h"

int WIDTH = 1200;
int HEIGHT = 675;

void Engine_init(){
	
	printf("hello from init function!\n");
	
	Engine_setWindowSize(WIDTH, HEIGHT);
	
	for(int x = 0; x < WIDTH; x++){
		for(int y = 0; y < HEIGHT; y++){
		
			Engine_Pixel *pixel_p = Engine_getScreenPixelPointer(x, y);
		
			pixel_p->g = 255;
		
		}
	}
	
}

void Engine_update(){
	
	
}

void Engine_draw(){
	
	/*
	for(int i = 0; i < 100; i++){
		
		Engine_Pixel *pixel_p = Engine_getScreenPixelPointer(i, 100);
		pixel_p->r = 255;
		pixel_p->g = 255;
		pixel_p->b = 255;
		
		*(unsigned char *)(pixel_p + 3 * sizeof(unsigned char)) = 255;
		
	}*/
	
}