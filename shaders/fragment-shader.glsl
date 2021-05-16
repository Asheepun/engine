#version 130

in vec2 textureCoord;

out vec4 outColor;

uniform sampler2D tex;

void main(){

	outColor = texture(tex, textureCoord);

}
