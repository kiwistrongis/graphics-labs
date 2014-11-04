#version 330 core

uniform vec3 attenuation;
uniform vec4 base;
uniform vec3 light;
uniform vec4 material;
uniform sampler2D tex;

in vec3 normal;
in vec4 position;
in vec2 tex_coord;

void main() {
	gl_FragColor = texture( tex, tex_coord);
}
