#version 330 core

uniform mat4 model_view;
uniform mat4 projection;
uniform mat3 normal_mat;

in vec4 vertex;
in vec3 normal_in;

out vec3 normal;
out vec4 position;

void main(){
	gl_Position = projection * model_view * vertex;
	normal = normal_mat * normal_in;
	position = model_view * vertex;
}
