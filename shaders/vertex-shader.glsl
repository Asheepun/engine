#version 130

in vec2 vertexPosition;
in vec2 textureVertex;

out vec2 textureCoord;

void main(){

	gl_Position = vec4(vertexPosition.x, vertexPosition.y, 0.0, 1.0);

	textureCoord = textureVertex;

}
