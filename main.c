#include "engine.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct Fold{
	float pos;
	float angle;
}Fold;

int WIDTH = 800;
int HEIGHT = 450;

float fov = M_PI / 2;

Vec3f triangleVerts[6];
Vec2f triangleTextureVerts[6];

int triangleIndices[] = {
	
	0, 1, 2,
	3, 2, 1,

	2, 3, 4,
	5, 4, 3,

};

int triangleIndicesLength = 4;

int textureWidth = 0;
int textureHeight = 0;

int paperWidth = 50;
int paperHeight = 70;

Vec3f rotation = { 0, 0, 0};
Vec3f pos = { 0, 0, 5 };
float scale = 2;

Vec3f lightPos = { 0, 0, -10 };

float screenZ = 2;
float screenSize = 2;

float ambient = 0.2;
float diffuse = 0.6;

float time = 0;

Engine_Pixel *textureData;

unsigned int getTextureIndex(int x, int y){
	return y * textureWidth + x;
}

void shader(int screenX, int screenY, int screenPixelIndex, int texturePosX, int texturePosY, int textureStartIndex, Vec3f intersectionPoint, Vec3f normal){

	Engine_Pixel color = textureData[texturePosY * textureWidth + texturePosX + textureStartIndex];
	//Engine_Pixel color = ENGINE_COLORS[COLOR_BLUE];

	Vec3f L = getSubVec3f(lightPos, intersectionPoint);
	Vec3f_normalize(&L);

	float dot = getDotVec3f(L, normal);
	dot = (dot < 0) * -dot + (dot > 0) * dot;

	Engine_Pixel pixel = { 
		ambient * color.r + dot * diffuse * color.r,
		ambient * color.g + dot * diffuse * color.g,
		ambient * color.b + dot * diffuse * color.b,
	};

	screenPixels[screenPixelIndex] = pixel;

}

void Engine_init(){

	Engine_setWindowSize(WIDTH, HEIGHT);
	
	Engine_setWindowTitle("My cool program");

	int channels;
	textureData = (Engine_Pixel *)stbi_load("level-1.png", &textureWidth, &textureHeight, &channels, 3);


}

void Engine_update(){

	time += 1;

	//lightPos.x = cos(time * 0.05) * 1.5;
	//lightPos.y = sin(time * 0.05) * 1.5;

	//triangleVerts[2].z = 2 + sin(time * 0.05) * 2;

	//pos.z = 7 + cos(time * 0.05) * 4;

	//pos.y = sin(time * 0.01) * 3;

	//rotation.z += 0.02;
	//rotation.x += 0.02;
	//rotation.y += 0.02;
	rotation.y = M_PI / 4;

	//generate verts
	float foldPos = 0;
	float afterFoldWidth = 0.7 - foldPos;
	float foldAngle = M_PI / 2 + M_PI / 1.5 * sin(time * 0.05);

	triangleVerts[0] = getVec3f(-0.5, -0.7, 0);
	triangleVerts[1] = getVec3f(0.5, -0.7, 0);
	triangleVerts[2] = getVec3f(-0.5, foldPos, 0);
	triangleVerts[3] = getVec3f(0.5, foldPos, 0);
	triangleVerts[4] = getVec3f(-0.5, foldPos + afterFoldWidth * sin(foldAngle), afterFoldWidth * cos(foldAngle));
	triangleVerts[5] = getVec3f(0.5, foldPos + afterFoldWidth * sin(foldAngle), afterFoldWidth * cos(foldAngle));

	triangleTextureVerts[0] = getVec2f(0.0, 0.0);
	triangleTextureVerts[1] = getVec2f(1.0, 0.0);
	triangleTextureVerts[2] = getVec2f(0.0, (foldPos + 0.7) / 1.4);
	triangleTextureVerts[3] = getVec2f(1.0, (foldPos + 0.7) / 1.4);
	triangleTextureVerts[4] = getVec2f(0.0, 1.0);
	triangleTextureVerts[5] = getVec2f(1.0, 1.0);

}

void Engine_draw(){

	Engine_fillRect(0, 0, screenWidth, screenHeight, COLOR_BLACK);

	for(int i = 0; i < triangleIndicesLength; i++){

		Vec3f verts3d[] = {
			triangleVerts[triangleIndices[i * 3 + 0]],
			triangleVerts[triangleIndices[i * 3 + 1]],
			triangleVerts[triangleIndices[i * 3 + 2]],
		};

		Vec2f textureVerts[] = {
			triangleTextureVerts[triangleIndices[i * 3 + 0]],
			triangleTextureVerts[triangleIndices[i * 3 + 1]],
			triangleTextureVerts[triangleIndices[i * 3 + 2]],
		};

		bool clip = false;

		for(int j = 0; j < 3; j++){

			Vec3f_mulByFloat(&verts3d[j], scale);

			Vec3f_rotate(&verts3d[j], rotation.x, rotation.y, rotation.z);

			Vec3f_add(&verts3d[j], pos);

			verts3d[j].x *= (float)HEIGHT / (float)WIDTH;

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

		Vec3f O = getVec3f(0, 0, 0);
		Vec3f TP = verts3d[0];

		Vec3f N = getNormalFromTriangleVec3f(verts3d[0], verts3d[1], verts3d[2]);
		Vec3f normal = N;

		if(N.z < 0){
			Vec3f_inverse(&N);
		}

		float area = getAreaFromTriangleVec3f(verts3d[0], verts3d[1], verts3d[2]);

		int textureStartIndex = 0 + (normal.z < 0) * paperWidth * paperHeight;

		int height = ceil(yMid.y - yMin.y);

		for(int i = 0; i < height; i++){

			int y = floor(yMin.y) + i;

			float x1 = yMin.x + i * (yMin.x - yMax.x) / (yMin.y - yMax.y);
			float x2 = yMin.x + i * (yMin.x - yMid.x) / (yMin.y - yMid.y);

			int xLeft = floor((x1 < x2) * x1 + (x2 < x1) * x2);
			int xRight = ceil((x1 > x2) * x1 + (x2 > x1) * x2);

			xLeft = !(xLeft < 0) * xLeft;
			xRight = !(xRight > WIDTH) * xRight + (xRight > WIDTH) * WIDTH;

			if(y < 0
			|| y >= HEIGHT){
				continue;
			}

			int width = xRight - xLeft;

			int index = Engine_getScreenPixelIndex(xLeft, y);

			for(int j = 0; j < width; j++){

				Vec3f SP = getVec3f(2 * (xLeft + j) / (float)WIDTH - 1, 2 * y / (float)HEIGHT - 1, screenZ);

				Vec3f_mulByFloat(&SP, getDotVec3f(N, TP) / getDotVec3f(N, SP));

				float area0 = getAreaFromTriangleVec3f(SP, verts3d[1], verts3d[2]);
				float area1 = getAreaFromTriangleVec3f(SP, verts3d[0], verts3d[2]);
				float area2 = getAreaFromTriangleVec3f(SP, verts3d[0], verts3d[1]);

				float areaSum = area0 + area1 + area2;

				area0 = area0 / areaSum;
				area1 = area1 / areaSum;
				area2 = area2 / areaSum;

				int texturePosX = paperWidth * (textureVerts[0].x * area0 + textureVerts[1].x * area1 + textureVerts[2].x * area2);
				int texturePosY = paperHeight * (textureVerts[0].y * area0 + textureVerts[1].y * area1 + textureVerts[2].y * area2);

				shader(xLeft + j, y, index + j, texturePosX, texturePosY, textureStartIndex, SP, normal);
			
			}
			
		}

		height = ceil(yMax.y - yMid.y);

		for(int i = 0; i < height; i++){

			int y = floor(yMax.y) - i;

			float x1 = yMax.x - i * (yMin.x - yMax.x) / (yMin.y - yMax.y);
			float x2 = yMax.x - i * (yMid.x - yMax.x) / (yMid.y - yMax.y);

			int xLeft = floor((x1 < x2) * x1 + (x2 < x1) * x2);
			int xRight = ceil((x1 > x2) * x1 + (x2 > x1) * x2);

			xLeft = !(xLeft < 0) * xLeft;
			xRight = !(xRight > WIDTH) * xRight + (xRight > WIDTH) * WIDTH;

			if(y < 0
			|| y >= HEIGHT){
				continue;
			}

			int width = xRight - xLeft;

			unsigned int index = Engine_getScreenPixelIndex(xLeft, y);

			for(int j = 0; j < width; j++){

				Vec3f SP = getVec3f(2 * (xLeft + j) / (float)WIDTH - 1, 2 * y / (float)HEIGHT - 1, screenZ);

				Vec3f_mulByFloat(&SP, getDotVec3f(N, TP) / getDotVec3f(N, SP));

				float area0 = getAreaFromTriangleVec3f(SP, verts3d[1], verts3d[2]);
				float area1 = getAreaFromTriangleVec3f(SP, verts3d[0], verts3d[2]);
				float area2 = getAreaFromTriangleVec3f(SP, verts3d[0], verts3d[1]);

				float areaSum = area0 + area1 + area2;

				area0 = area0 / areaSum;
				area1 = area1 / areaSum;
				area2 = area2 / areaSum;

				int texturePosX = paperWidth * (textureVerts[0].x * area0 + textureVerts[1].x * area1 + textureVerts[2].x * area2);
				int texturePosY = paperHeight * (textureVerts[0].y * area0 + textureVerts[1].y * area1 + textureVerts[2].y * area2);

				shader(xLeft + j, y, index + j, texturePosX, texturePosY, textureStartIndex, SP, normal);

			}
		
		}
	
	}

}

void Engine_finnish(){

}
