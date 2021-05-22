#ifndef RENDERER_2D_H_
#define RENDERER_2D_H_

#include "engine/text.h"

enum Renderer2D_ShaderTypeEnum{
	RENDERER2D_VERTEX_SHADER,
	RENDERER2D_FRAGMENT_SHADER,
};

enum Renderer2D_UniformTypeEnum{
	RENDERER2D_UNIFORM_TYPE_FLOAT,
	RENDERER2D_UNIFORM_TYPE_COLOR,
};

typedef struct Renderer2D_Color{
	float r;
	float g;
	float b;
}Renderer2D_Color;

typedef struct Renderer2D_ShaderPathTypePair{
	char *path;
	enum Renderer2D_ShaderTypeEnum type;
}Renderer2D_ShaderPathTypePair;

typedef struct Renderer2D_Texture{
	char *name;
	unsigned int ID;
	int width;
	int height;
}Renderer2D_Texture;

typedef struct Renderer2D_ShaderProgram{
	char *name;
	unsigned int ID;
}Renderer2D_ShaderProgram;

typedef struct Renderer2D{
	int width;
	int height;
	unsigned int rectangleTextureBufferID;
	Renderer2D_Texture textTexture;

	Renderer2D_ShaderProgram textureShaderProgram;
	Renderer2D_ShaderProgram colorShaderProgram;

	Renderer2D_ShaderProgram currentShaderProgram;
}Renderer2D;

//INIT FUNCTIONS

void Renderer2D_init(Renderer2D *, int, int);

void Renderer2D_Texture_init(Renderer2D_Texture *, char *, char *, int, int);

void Renderer2D_Texture_initFromFile(Renderer2D_Texture *, char *);

void Renderer2D_Texture_initFromText(Renderer2D_Texture *, char *, Font);

void Renderer2D_ShaderProgram_init(Renderer2D_ShaderProgram *, char *, Renderer2D_ShaderPathTypePair *, unsigned int);

//SETTINGS FUNCTIONS

void Renderer2D_updateDrawSize(Renderer2D *, int, int);

//DRAWING FUNCTIONS

void Renderer2D_clearBackground(float, float, float, float);

void Renderer2D_setShaderProgram(Renderer2D *, Renderer2D_ShaderProgram);

void Renderer2D_setTexture(Renderer2D *, Renderer2D_Texture);

void Renderer2D_beginRectangle(Renderer2D *, float, float, float, float);

void Renderer2D_beginText(Renderer2D *, char *, int, int, int, Font);

void Renderer2D_supplyUniform(Renderer2D *, void *, char *, enum Renderer2D_UniformTypeEnum);

void Renderer2D_drawRectangle(Renderer2D *);

void Renderer2D_drawTexture(Renderer2D *, float, float, float, float, float, Renderer2D_Texture, Renderer2D_ShaderProgram);

void Renderer2D_drawText(Renderer2D *, char *, int, int, int, Font, float, Renderer2D_ShaderProgram);

#endif
