#include "engine.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glad/glad.h"

unsigned int shaderProgram;
unsigned int texture;

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

float rectangleVertices[] = {
	1, 1, 1, 0,
	-1, 1, 0, 0,
	-1, -1, 0, 1,

	1, 1, 1, 0,
	1, -1, 1, 1,
	-1, -1, 0, 1,
};

float textureVertices[] = {
	1, 1,
	0, 1,
	0, 0,

	1, 1,
	1, 0,
	0, 0,
};

unsigned int VBO;

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

	//generate buffer

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

	//compile and link shaders
	unsigned int vertexShader = getCompiledShader("shaders/vertex-shader.glsl", GL_VERTEX_SHADER);

	unsigned int fragmentShader = getCompiledShader("shaders/fragment-shader.glsl", GL_FRAGMENT_SHADER);

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindFragDataLocation(shaderProgram, 0, "outColor");

	glLinkProgram(shaderProgram);

	//load texture

	int textureWidth, textureHeight, channels;
	unsigned char *textureData = stbi_load("6.jpg", &textureWidth, &textureHeight, &channels, 4);

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	//velocityY = 1;

	posX += velocityX;
	posY += velocityY;

}

void Engine_draw(){

	glViewport(0, 0, screenWidth, screenHeight);

	glClearColor(0.5, 0.5, 0.5, 1.0);

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);

	glBindTexture(GL_TEXTURE_2D, texture);

	unsigned int vertexPositionAttributeLocation = glGetAttribLocation(shaderProgram, "vertexPosition");
	unsigned int textureVertexAttributeLocation = glGetAttribLocation(shaderProgram, "textureVertex");
	
	glVertexAttribPointer(vertexPositionAttributeLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	glVertexAttribPointer(textureVertexAttributeLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

	glEnableVertexAttribArray(vertexPositionAttributeLocation);
	glEnableVertexAttribArray(textureVertexAttributeLocation);

	unsigned int posXUniformLocation = glGetUniformLocation(shaderProgram, "posX");
	unsigned int posYUniformLocation = glGetUniformLocation(shaderProgram, "posY");
	unsigned int widthUniformLocation = glGetUniformLocation(shaderProgram, "width");
	unsigned int heightUniformLocation = glGetUniformLocation(shaderProgram, "height");

	//printf("%f\n", (float)(posX - WIDTH / 2) / (float)WIDTH);

	glUniform1f(posXUniformLocation, (float)(posX * 2) / (float)WIDTH);
	glUniform1f(posYUniformLocation, (float)(posY * 2) / (float)HEIGHT);
	glUniform1f(widthUniformLocation, (float)width / (float)WIDTH);
	glUniform1f(heightUniformLocation, (float)height / (float)HEIGHT);


	glDrawArrays(GL_TRIANGLES, 0, 6);

}

void Engine_finnish(){

}
