#version 130

out vec4 outColor;

uniform vec3 color;

uniform float alpha = 1;

void main(){

	gl_FragColor = vec4(color.x, color.y, color.z, alpha);

}
