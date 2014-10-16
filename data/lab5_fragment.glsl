#version 330 core

uniform vec4 material;
uniform vec3 light;
uniform vec4 base;
uniform vec3 attenuation;

in vec3 normal;
in vec4 position;

void main() {
	vec4 white = vec4( 1.0, 1.0, 1.0, 1.0);
	vec3 N = normalize( normal);
	vec3 L = normalize( light - vec3( position));
	vec3 H = normalize( L + vec3( 0.0, 0.0, 1.0));

	float diffuse = dot( N, L);
	float specular;

	if( diffuse < 0.0){
		diffuse = 0.0;
		specular = 0.0;}
	else
		specular = pow( max( 0.0, dot( N, H)), material[3]);

	float dist = length( light - vec3( position));
	float atten =
		attenuation[0] +
		attenuation[1] * dist +
		attenuation[2] * dist * dist;

	gl_FragColor = min( vec4(1.0),
		material[0] * base +
		material[1] * diffuse * base +
		material[2] * white * specular);
	gl_FragColor /= atten;
	gl_FragColor.a = base.a;
}
