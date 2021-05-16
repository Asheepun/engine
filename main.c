#include "engine.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glad/glad.h"

unsigned int shaderProgram;

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

	for(int i = fileLength; i < 1024; i++){
		//buffer[i] = "\o";
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
	1, 1,
	-1, 1,
	-1, -1,

	1, 1,
	1, -1,
	-1, -1,
};

unsigned int VAO;
unsigned int VBO;

int WIDTH = 480;
int HEIGHT = 270;

int width = 200;
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

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

	unsigned int vertexShader = getCompiledShader("shaders/vertex-shader.glsl", GL_VERTEX_SHADER);

	unsigned int fragmentShader = getCompiledShader("shaders/fragment-shader.glsl", GL_FRAGMENT_SHADER);

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindFragDataLocation(shaderProgram, 0, "outColor");

	glLinkProgram(shaderProgram);

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

	glClearColor(0, 0, 0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);

	unsigned int aPosAttributeLocation = glGetAttribLocation(shaderProgram, "aPos");
	
	glVertexAttribPointer(aPosAttributeLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(aPosAttributeLocation);

	glDrawArrays(GL_TRIANGLES, 0, 6);

}

void Engine_finnish(){

}
