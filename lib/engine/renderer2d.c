#include "engine/text.h"
#include "engine/renderer2d.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "glad/glad.h"

#include "stdio.h"
#include "math.h"
#include "string.h"

//PRIVATE FUNCTIONS

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

//INIT FUNCTIONS

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

	Font font = getFont("assets/fonts/times.ttf", 100);

	Renderer2D_Texture_initFromText(&renderer_p->textTexture, "", font);

}

void Renderer2D_Texture_init(Renderer2D_Texture *texture_p, char *name, char *data, int width, int height){

	texture_p->name = name;
	texture_p->width = width;
	texture_p->height = height;

	glGenTextures(1, &texture_p->ID);

	glBindTexture(GL_TEXTURE_2D, texture_p->ID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_p->width, texture_p->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

void Renderer2D_Texture_initFromFile(Renderer2D_Texture *texture_p, char *path){

	int width, height, channels;
	char *data = stbi_load(path, &width, &height, &channels, 4);

	Renderer2D_Texture_init(texture_p, path, data, width, height);

	free(data);

}

void Renderer2D_Texture_initFromText(Renderer2D_Texture *texture_p, char *text, Font font){

	int width, height;
	char *data = getImageDataFromFontAndString_mustFree(font, text, &width, &height);

	Renderer2D_Texture_init(texture_p, text, data, width, height);

	free(data);
	
};

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

//DRAWING FUNCTIONS

void Renderer2D_setRenderSize(int width, int height){

	glViewport(0, 0, width, height);

}

void Renderer2D_clearBackground(float r, float g, float b, float a){

	glClearColor(r, g, b, a);

	glClear(GL_COLOR_BUFFER_BIT);

}

void Renderer2D_drawTexture(Renderer2D *renderer_p, float x, float y, float width, float height, float alpha, Renderer2D_Texture texture, Renderer2D_ShaderProgram shaderProgram){

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
	unsigned int alphaUniformLocation = glGetUniformLocation(shaderProgram.ID, "alpha");

	glUniform1f(posXUniformLocation, (float)(x * 2) / (float)renderer_p->width);
	glUniform1f(posYUniformLocation, (float)(y * 2) / (float)renderer_p->height);
	glUniform1f(widthUniformLocation, (float)width / (float)renderer_p->width);
	glUniform1f(heightUniformLocation, (float)height / (float)renderer_p->height);
	glUniform1f(alphaUniformLocation, alpha);

	glDrawArrays(GL_TRIANGLES, 0, 6);

}

void Renderer2D_drawText(Renderer2D *renderer_p, char *text, int x, int y, int fontSize, Font font, float alpha, Renderer2D_ShaderProgram shaderProgram){

	glDeleteTextures(1, &renderer_p->textTexture.ID);

	Renderer2D_Texture_initFromText(&renderer_p->textTexture, text, font);

	int height = fontSize;
	int width = renderer_p->textTexture.width * fontSize / renderer_p->textTexture.height;

	Renderer2D_drawTexture(renderer_p, x, y, width, height, alpha, renderer_p->textTexture, shaderProgram);

}
