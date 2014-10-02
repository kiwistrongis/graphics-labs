#version 330 compatibility

uniform vec4 base;

in vec3 normal;

void main() {
	//vec4 base = vec4( 1.0, 0.0, 0.0, 1.0);
	vec3 light = vec3( 1.0, 0.0, 0.0);
	vec3 N = normalize( normal);
	float diffuse = dot( N, light);
	if( diffuse < 0.0)
		diffuse = 0.0;
	gl_FragColor = min( 0.3 * base + 0.9 * diffuse * base, vec4( 1.0));
	gl_FragColor.a = base.a;
}