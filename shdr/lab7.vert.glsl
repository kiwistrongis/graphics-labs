#version 330 core

uniform mat4 model_view;
uniform mat4 projection;

in vec4 vertex;
out vec3 tex_coord;

void main(){
	gl_Position = projection * model_view * vertex;
	tex_coord = vertex.xyz;
}
