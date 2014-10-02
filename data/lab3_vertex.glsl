#version 330 core

uniform mat4 model_view;
uniform mat4 projection;

in vec4 vertex;

out vec3 normal;

void main(){
	gl_Position = projection * model_view * vertex;
	normal = ( model_view * vec4( gl_Normal, 1.0)).xyz;
}