#version 400
//#version 330 core

uniform mat4 model_view;
uniform mat4 projection;
uniform mat3 normal_mat;
uniform sampler2D tex;

in vec4 vertex;
in vec3 normal_in;
in vec2 texture_in;

out vec3 normal;
out vec4 position;
out vec2 tex_coord;
out vec4 world_pos;
out vec3 world_norm;

void main(){
	vec4 vertex = vertex + 0.1 * texture( tex, texture_in) * vec4( normal_in,1.0);
	gl_Position = projection * model_view * vertex;
	normal = normal_mat * normal_in;
	position = model_view * vertex;
	tex_coord = texture_in;
	world_pos = vertex;
	world_norm = normal_in;
}
