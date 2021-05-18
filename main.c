#include "engine.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glad/glad.h"

char *readFile_mustFree(char *filePath){

	char *buffer = malloc(sizeof(char) * 1024);
	FILE *fd = NULL;
	long fileLength = 0;

	memset(buffer, 0, 1024);

	fd = fopen(filePath, "r");

	char c;
	while((c = fgetc(fd)) != EOF){
		buffer[fileLength] = c;
		fileLength++;
	}
	
	fclose(fd);

	return buffer;

}

unsigned int getCompiledShader(char *shaderSourcePath, GLenum type){
	char *shaderSource = readFile_mustFree(shaderSourcePath);

	unsigned int shader;
	shader = glCreateShader(type);

	glShaderSource(shader, 1, (const GLchar * const *)&shaderSource, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("FAILED TO COMPILE SHADER: %s\n%s\n", shaderSourcePath, infoLog);
	}

	free(shaderSource);

	return shader;
}

enum Renderer2D_ShaderTypeEnum{
	RENDERER2D_VERTEX_SHADER,
	RENDERER2D_FRAGMENT_SHADER,
};

typedef struct Renderer2D_Texture{
	char *name;
	unsigned int ID;
}Renderer2D_Texture;

typedef struct Renderer2D_ShaderProgram{
	char *name;
	unsigned int ID;
}Renderer2D_ShaderProgram;

typedef struct Renderer2D{
	int width;
	int height;
	unsigned int rectangleTextureBufferID;
}Renderer2D;

typedef struct Renderer2D_ShaderPathTypePair{
	char *path;
	enum Renderer2D_ShaderTypeEnum type;
}Renderer2D_ShaderPathTypePair;

void Renderer2D_init(Renderer2D *renderer_p, int width, int height){

	renderer_p->width = width;
	renderer_p->height = height;

	static float rectangleVertices[] = {
		1, 1, 1, 0,
		-1, 1, 0, 0,
		-1, -1, 0, 1,

		1, 1, 1, 0,
		1, -1, 1, 1,
		-1, -1, 0, 1,
	};

	glGenBuffers(1, &renderer_p->rectangleTextureBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, renderer_p->rectangleTextureBufferID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Renderer2D_drawTexture(Renderer2D *renderer_p, float x, float y, float width, float height, Renderer2D_Texture texture, Renderer2D_ShaderProgram shaderProgram){

	glBindBuffer(GL_ARRAY_BUFFER, renderer_p->rectangleTextureBufferID);

	glBindTexture(GL_TEXTURE_2D, texture.ID);
	
	glUseProgram(shaderProgram.ID);

	unsigned int vertexPositionAttributeLocation = glGetAttribLocation(shaderProgram.ID, "vertexPosition");
	unsigned int textureVertexAttributeLocation = glGetAttribLocation(shaderProgram.ID, "textureVertex");

	glVertexAttribPointer(vertexPositionAttributeLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glVertexAttribPointer(textureVertexAttributeLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

	glEnableVertexAttribArray(vertexPositionAttributeLocation);
	glEnableVertexAttribArray(textureVertexAttributeLocation);

	unsigned int posXUniformLocation = glGetUniformLocation(shaderProgram.ID, "posX");
	unsigned int posYUniformLocation = glGetUniformLocation(shaderProgram.ID, "posY");
	unsigned int widthUniformLocation = glGetUniformLocation(shaderProgram.ID, "width");
	unsigned int heightUniformLocation = glGetUniformLocation(shaderProgram.ID, "height");

	glUniform1f(posXUniformLocation, (float)(x * 2) / (float)renderer_p->width);
	glUniform1f(posYUniformLocation, (float)(y * 2) / (float)renderer_p->height);
	glUniform1f(widthUniformLocation, (float)width / (float)renderer_p->width);
	glUniform1f(heightUniformLocation, (float)height / (float)renderer_p->height);

	glDrawArrays(GL_TRIANGLES, 0, 6);

}

void Renderer2D_ShaderProgram_init(Renderer2D_ShaderProgram *shaderProgram_p, char *name, Renderer2D_ShaderPathTypePair *shaders, unsigned int shadersLength){

	shaderProgram_p->name = name;

	shaderProgram_p->ID = glCreateProgram();

	for(int i = 0; i < shadersLength; i++){

		unsigned int shaderID;
		
		if(shaders[i].type == RENDERER2D_VERTEX_SHADER){
			shaderID = getCompiledShader(shaders[i].path, GL_VERTEX_SHADER);
		}
		if(shaders[i].type == RENDERER2D_FRAGMENT_SHADER){
			shaderID = getCompiledShader(shaders[i].path, GL_FRAGMENT_SHADER);
		}

		glAttachShader(shaderProgram_p->ID, shaderID);

	}

	glBindFragDataLocation(shaderProgram_p->ID, 0, "outColor");

	glLinkProgram(shaderProgram_p->ID);

}

void Renderer2D_Texture_initFromFile(Renderer2D_Texture *texture_p, char *path){
	
	texture_p->name = path;

	int width, height, channels;
	char *data = stbi_load(path, &width, &height, &channels, 4);

	glGenTextures(1, &texture_p->ID);

	glBindTexture(GL_TEXTURE_2D, texture_p->ID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

void Renderer2D_Texture_initFromText(Renderer2D_Texture *texture_p, char *text){

	
};

Renderer2D renderer;
Renderer2D_ShaderProgram shaderProgram;
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

int scale = 2;

void Engine_init(){

	Engine_setWindowSize(WIDTH * 2.5, HEIGHT * 2.5);
	Engine_centerWindow();

	printf("%s\n", glGetString(GL_VERSION));

	Renderer2D_init(&renderer, WIDTH, HEIGHT);

	Renderer2D_ShaderPathTypePair shaders[] = {
		"shaders/vertex-shader.glsl", RENDERER2D_VERTEX_SHADER,
		"shaders/fragment-shader.glsl", RENDERER2D_FRAGMENT_SHADER,
	};

	int shadersLength = sizeof(shaders) / sizeof(Renderer2D_ShaderPathTypePair);

	Renderer2D_ShaderProgram_init(&shaderProgram, "texture-shader", shaders, shadersLength);

	Renderer2D_Texture_initFromFile(&texture, "6.jpg");

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

}

void Engine_draw(){

	glViewport(0, 0, screenWidth, screenHeight);

	glClearColor(0.5, 0.5, 0.5, 1.0);

	glClear(GL_COLOR_BUFFER_BIT);

	Renderer2D_drawTexture(&renderer, posX, posY, 100, 100, texture, shaderProgram);

}

void Engine_finnish(){

}
