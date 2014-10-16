#version 330 compatibility

uniform mat4 model_view;
uniform mat4 projection;

in vec4 vertex;
in vec3 normal_in;

out vec3 normal;

void main(){
	gl_Position = projection * model_view * vertex;
	normal = ( model_view * vec4( normal_in, 1.0)).xyz;
}