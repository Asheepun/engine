#include "engine.h"
#include "stdio.h"
#include "math.h"
#include "string.h"

int WIDTH = 800;
int HEIGHT = 450;

float fov = M_PI / 4;

Vec3f triangleVerts[] = {
	-1, -1, 5,
	1, -1, 5,
	-1, 1, 5,

	-1, 1, 5,
	1, 1, 5,
	1, -1, 5,

};

unsigned int triangleVertsLength = 2;

void Engine_init(){

	Engine_setWindowSize(WIDTH, HEIGHT);
	
	Engine_setWindowTitle("My cool program");


}

void Engine_update(){


}

float time = 0;

void Engine_draw(){

	printf("---\n");

	Engine_fillRect(0, 0, screenWidth, screenHeight, COLOR_BLACK);

	for(int i = 0; i < triangleVertsLength; i++){

		Vec3f verts3d[] = {
			triangleVerts[i * 3],
			triangleVerts[i * 3 + 1],
			triangleVerts[i * 3 + 2],
		};

		Vec2f verts2d[3];

		for(int i = 0; i < 3; i++){
			
			Vec3f v3 = verts3d[i];
			
			float w = v3.z * tan(fov / 2);

			verts2d[i].x = (WIDTH + v3.x * WIDTH / w) / 2;
			verts2d[i].y = (HEIGHT + v3.y * HEIGHT / w) / 2;

		}

		Vec2f sortedVerts2d[3];
		memcpy(sortedVerts2d, verts2d, sizeof(Vec2f) * 3);

		Vec2f tmpVert;
		if(sortedVerts2d[1].y > sortedVerts2d[0].y){
			tmpVert = sortedVerts2d[0];
			sortedVerts2d[0] = sortedVerts2d[1];
			sortedVerts2d[1] = tmpVert;
		}
		if(sortedVerts2d[2].y > sortedVerts2d[1].y){
			tmpVert = sortedVerts2d[1];
			sortedVerts2d[1] = sortedVerts2d[2];
			sortedVerts2d[2] = tmpVert;
		}
		if(sortedVerts2d[1].y > sortedVerts2d[0].y){
			tmpVert = sortedVerts2d[0];
			sortedVerts2d[0] = sortedVerts2d[1];
			sortedVerts2d[1] = tmpVert;
		}

		Vec2f yMax = sortedVerts2d[0];
		Vec2f yMid = sortedVerts2d[1];
		Vec2f yMin = sortedVerts2d[2];

		for(int i = 0; i < yMid.y - yMin.y; i++){

			int y = yMin.y + i;

			int x1 = yMin.x	+ i * (yMin.x - yMax.x) / (yMin.y - yMax.y);
			int x2 = yMin.x	+ i * (yMin.x - yMid.x) / (yMin.y - yMid.y);

			int xLeft = (x1 < x2) * x1 + (x2 < x1) * x2;
			int xRight = (x1 > x2) * x1 + (x2 > x1) * x2;

			int width = xRight - xLeft;

			for(int j = 0; j < width; j++){

				unsigned int index = Engine_getScreenPixelIndex(xLeft + j, y);
				
				screenPixels[index] = ENGINE_COLORS[COLOR_BLUE];
			
			}
			
		}

		for(int i = 0; i < yMax.y - yMid.y; i++){

			int y = yMax.y - i;

			int x1 = yMax.x - i * (yMin.x - yMax.x) / (yMin.y - yMax.y);
			int x2 = yMax.x - i * (yMid.x - yMax.x) / (yMid.y - yMax.y);

			int xLeft = (x1 < x2) * x1 + (x2 < x1) * x2;
			int xRight = (x1 > x2) * x1 + (x2 > x1) * x2;

			int width = xRight - xLeft;

			for(int j = 0; j < width; j++){

				unsigned int index = Engine_getScreenPixelIndex(xLeft + j, y);
				
				screenPixels[index] = ENGINE_COLORS[COLOR_BLUE];
			
			}
		
		}
	
	}

}

void Engine_finnish(){

}
