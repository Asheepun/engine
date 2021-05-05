#include "engine.h"
#include "stdio.h"
#include "math.h"
#include "string.h"

int WIDTH = 800;
int HEIGHT = 450;

float fov = M_PI / 2;

Vec3f triangleVerts[] = {

	0, -1, 0,
	-1, 1, 0,
	1, 1, 0,

	1, -1, 0,
	-1, -1, 0,
	-1, 1, 0,

	-1, 1, 0,
	1, 1, 0,
	1, -1, 0,

};

Vec2f textureVerts[] = {
	
	0.5, 1,
	0.0, 0,
	1.0, 0,

};

unsigned int triangleVertsLength = 1;
unsigned int textureVertsLength = 1;

int textureWidth = 100;
int textureHeight = 100;

int paperWidth = 400;
int paperHeight = 560;

Vec3f rotation = { 0, 0, 0};
Vec3f pos = { 0, 0, 4 };

Vec3f lightPos = { 2, 2, 4 };

float screenZ = 2;
float screenSize = 2;

float time = 0;

void Engine_init(){

	Engine_setWindowSize(WIDTH, HEIGHT);
	
	Engine_setWindowTitle("My cool program");

}

void Engine_update(){

	time += 1;

	lightPos.x = cos(time * 0.05) * 1.5;
	lightPos.y = sin(time * 0.05) * 1.5;

	//triangleVerts[2].z = 2 + sin(time * 0.05) * 2;

	//pos.z = 7 + cos(time * 0.05) * 4;

	//pos.y = sin(time * 0.05) * 4;

	//rotation.z += 0.1;
	//rotation.x += 0.01;
	//rotation.y += 0.02;

}

void Engine_draw(){

	Engine_fillRect(0, 0, screenWidth, screenHeight, COLOR_GREY);

	for(int i = 0; i < triangleVertsLength; i++){

		Vec3f verts3d[] = {
			triangleVerts[i * 3],
			triangleVerts[i * 3 + 1],
			triangleVerts[i * 3 + 2],
		};

		bool clip = false;

		for(int j = 0; j < 3; j++){

			//verts3d[j].y *= (float)paperHeight / (float)paperWidth;

			//Vec3f_mulByFloat(&verts3d[j], 1.1);

			Vec3f_rotate(&verts3d[j], rotation.x, rotation.y, rotation.z);

			Vec3f_add(&verts3d[j], pos);

			//verts3d[j].x *= (float)HEIGHT / (float)WIDTH;

			if(verts3d[j].z <= screenZ){
				clip = true;
				break;
			}

		}

		if(clip){
			continue;
		}

		Vec2f verts2d[3];

		for(int i = 0; i < 3; i++){
			
			Vec3f v3 = verts3d[i];

			verts2d[i].x = (WIDTH / 2 + (v3.x / v3.z) * (WIDTH / 2) * screenZ);
			verts2d[i].y = (HEIGHT / 2 + (v3.y / v3.z) * (HEIGHT / 2) * screenZ);

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

		Vec3f P1 = getVec3f(0, 0, 0);

		float area = getAreaFromTriangleVec3f(verts3d[0], verts3d[1], verts3d[2]);

		for(int i = 0; i < yMid.y - yMin.y; i++){

			int y = yMin.y + i;

			int x1 = yMin.x	+ i * (yMin.x - yMax.x) / (yMin.y - yMax.y);
			int x2 = yMin.x	+ i * (yMin.x - yMid.x) / (yMin.y - yMid.y);

			int xLeft = (x1 < x2) * x1 + (x2 < x1) * x2;
			int xRight = (x1 > x2) * x1 + (x2 > x1) * x2;

			xLeft = !(xLeft < 0) * xLeft;
			xRight = !(xRight > WIDTH) * xRight + (xRight > WIDTH) * WIDTH;

			if(y < 0
			|| y >= HEIGHT){
				continue;
			}

			int width = xRight - xLeft;

			for(int j = 0; j < width; j++){

				Vec3f P2 = getVec3f(2 * (xLeft + j) / (float)WIDTH - 1, 2 * y / (float)HEIGHT - 1, screenZ);

				Vec3f I = getLineToTriangleIntersectionVec3f(P1, P2, verts3d[0], verts3d[1], verts3d[2]);

				float area0 = getAreaFromTriangleVec3f(I, verts3d[1], verts3d[2]) / area;
				float area1 = getAreaFromTriangleVec3f(I, verts3d[0], verts3d[2]) / area;
				float area2 = getAreaFromTriangleVec3f(I, verts3d[0], verts3d[1]) / area;

				Vec2f texturePos = textureVerts[0] * area0 + textureVerts[1] * area1 + textureVerts[2] * area2;

				unsigned int index = Engine_getScreenPixelIndex(xLeft + j, y);
				
				screenPixels[index] = ENGINE_COLORS[COLOR_WHITE];

				Vec3f diff = getSubVec3f(I, lightPos);

				screenPixels[index].r = 255 * pow(0.50, getMagVec3f(diff));
				screenPixels[index].g = 255 * pow(0.50, getMagVec3f(diff));
				screenPixels[index].b = 255 * pow(0.50, getMagVec3f(diff));
			
			}
			
		}

		for(int i = 0; i < yMax.y - yMid.y; i++){

			int y = yMax.y - i;

			int x1 = yMax.x - i * (yMin.x - yMax.x) / (yMin.y - yMax.y);
			int x2 = yMax.x - i * (yMid.x - yMax.x) / (yMid.y - yMax.y);

			int xLeft = (x1 < x2) * x1 + (x2 < x1) * x2;
			int xRight = (x1 > x2) * x1 + (x2 > x1) * x2;

			xLeft = !(xLeft < 0) * xLeft;
			xRight = !(xRight > WIDTH) * xRight + (xRight > WIDTH) * WIDTH;

			if(y < 0
			|| y >= HEIGHT){
				continue;
			}

			int width = xRight - xLeft;

			for(int j = 0; j < width; j++){

				Vec3f P2 = getVec3f(2 * (xLeft + j) / (float)WIDTH - 1, 2 * y / (float)HEIGHT - 1, screenZ);

				Vec3f I = getLineToTriangleIntersectionVec3f(P1, P2, verts3d[0], verts3d[1], verts3d[2]);

				unsigned int index = Engine_getScreenPixelIndex(xLeft + j, y);
				
				screenPixels[index] = ENGINE_COLORS[COLOR_WHITE];

				Vec3f diff = getSubVec3f(I, lightPos);

				screenPixels[index].r = 255 * pow(0.50, getMagVec3f(diff));
				screenPixels[index].g = 255 * pow(0.50, getMagVec3f(diff));
				screenPixels[index].b = 255 * pow(0.50, getMagVec3f(diff));
			
			
			}
		
		}

		//Engine_drawLine(verts2d[0], verts2d[1], ENGINE_COLORS[COLOR_BLACK]);
		//Engine_drawLine(verts2d[1], verts2d[2], ENGINE_COLORS[COLOR_BLACK]);
	
	}

}

void Engine_finnish(){

}
