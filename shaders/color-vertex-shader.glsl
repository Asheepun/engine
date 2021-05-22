#version 100

attribute vec2 vertexPosition;

uniform float posX;
uniform float posY;

uniform float width;
uniform float height;

void main(){

	gl_Position = vec4(
		posX + width + -1.0 + vertexPosition.x * width,
		-posY + -height + 1.0 + vertexPosition.y * height,
		0.0,
		1.0
	);

}
