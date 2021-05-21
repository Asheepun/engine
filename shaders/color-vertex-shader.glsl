#version 130

in vec2 vertexPosition;

uniform float posX = 0;
uniform float posY = 0;

uniform float width = 1;
uniform float height = 1;

void main(){

	gl_Position = vec4(
		posX + width + -1 + vertexPosition.x * width,
		-posY + -height + 1 + vertexPosition.y * height,
		0.0,
		1.0
	);

}
