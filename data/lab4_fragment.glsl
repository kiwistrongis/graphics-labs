#version 330 compatibility

uniform vec4 base;

in vec3 normal;

void main() {
	vec4 white = vec4( 1.0, 1.0, 1.0, 1.0);
	vec3 L = vec3( 1.0, 0.0, 0.0);
	vec3 N = normalize( normal);
	vec3 H = normalize( L + vec3( 0.0, 0.0, 1.0));
	float diffuse = dot( N, L);
	float specular;

	if( diffuse < 0.0) {
		diffuse = 0.0;
		specular = 0.0;}
	else
		specular = pow( max( 0.0, dot( N, H)), 100.0);

	gl_FragColor = min(
		0.3 * base +
		0.7 * diffuse * base +
		0.7 * white * specular, vec4(1.0));
	gl_FragColor.a = base.a;
}