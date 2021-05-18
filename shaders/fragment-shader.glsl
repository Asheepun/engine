#version 130

in vec2 textureCoord;

out vec4 outColor;

uniform sampler2D tex;

uniform float alpha = 1;

void main(){

	outColor = texture(tex, textureCoord) * vec4(1, 1, 1, alpha);

}
