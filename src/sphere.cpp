
void init( float radius) {
	GLuint vbuffer;
	GLuint ibuffer;
	GLint vPosition;
	GLint vTex;
	GLint vNormal;
	glGenVertexArrays( 1, &sphere_vao);
	glBindVertexArray( sphere_vao);
	int i, j;
	float theta, phi;
	float dtheta, dphi;
	int k = 0;
	float x, y, z;
	int start;
	struct Texture* texture;
	GLuint tBuffer;
	int asdf = 2 * ( spokes + 1) + slices * spokes

	dtheta = 2.0 * M_PI / spokes;
	dphi = 2 * M_PI_2 / slices;

	GLfloat vertices[ asdf][4];
	GLfloat normals[ asdf][3];
	GLfloat tex[ asdf][2];
	GLushort indexes[ index_count];

	vertices[k][0] = 0.0;
	vertices[k][1] = 0.0;
	vertices[k][2] = radius;
	vertices[k][3] = 1.0;
	normals[k][0] = 0.0;
	normals[k][1] = 0.0;
	normals[k][2] = 1.0;
	tex[k][0] = 0.0;
	tex[k][1] = 1.0;
	indexes[v] = k;
	k++;
	v++;

	phi = M_PI_2 - dphi;
	theta = 0.0;
	start = k;
	for(i=0; i<spokes; i++) {
		x = radius * cos( phi) * cos( theta);
		y = radius * cos( phi) * sin( theta);
		z = radius * sin( phi);
		vertices[k][0] = x;
		vertices[k][1] = y;
		vertices[k][2] = z;
		vertices[k][3] = 1.0;
		normals[k][0] = x;
		normals[k][1] = y;
		normals[k][2] = z;
		tex[k][0] = theta/(2.0*M_PI);
		tex[k][1] = phi/M_PI + 0.5;
		indexes[v] = k;
		k++;
		v++;
		theta += dtheta;}
	indexes[v] = start;
	v++;

	phi = M_PI_2 - 2*dphi;
	for(j=0; j<slices; j++) {
		theta = 0.0;
		start = k;
		for(i=0; i<spokes; i++) {
			x = radius * cos( phi) * cos( theta);
			y = radius * cos( phi) * sin( theta);
			z = radius * sin( phi);
			vertices[k][0] = x;
			vertices[k][1] = y;
			vertices[k][2] = z;
			vertices[k][3] = 1.0;
			normals[k][0] = x;
			normals[k][1] = y;
			normals[k][2] = z;
			tex[k][0] = theta / ( 2.0 * M_PI);
			tex[k][1] = phi/M_PI + 0.5;
			indexes[v] = k;
			if(i > 0) {
				indexes[v++] = k - 1;
				indexes[v++] = k - 1-spokes;
				indexes[v++] = k - 1-spokes+1;
				indexes[v++] = k - 1;
				indexes[v++] = k - 1-spokes+1;
				indexes[v++] = k;}
			k++;
			theta += dtheta;}
		indexes[v++] = k-1;
		indexes[v++] = start;
		indexes[v++] = start+1;
		indexes[v++] = k-1;
		indexes[v++] = k-2;
		indexes[v++] = start+1;
		phi -= dphi;}

	vertices[k][0] = 0.0;
	vertices[k][1] = 0.0;
	vertices[k][2] = -radius;
	vertices[k][3] = 1.0;
	normals[k][0] = 0.0;
	normals[k][1] = 0.0;
	normals[k][2] = -1.0;
	tex[k][0] = 0.0;
	tex[k][1] = 0.0;
	indexes[v] = k;
	k++;
	v++;

	phi = -M_PI_2 + dphi;
	theta = 0.0;
	start = k;
	for(i=0; i<spokes; i++) {
		x = radius*cos(phi)*cos(theta);
		y = radius*cos(phi)*sin(theta);
		z = radius*sin(phi);
		vertices[k][0] = x;
		vertices[k][1] = y;
		vertices[k][2] = z;
		vertices[k][3] = 1.0;
		normals[k][0] = x;
		normals[k][1] = y;
		normals[k][2] = z;
		tex[k][0] = theta/(2.0*M_PI);
		tex[k][1] = phi/M_PI + 0.5;
		indexes[v] = k;
		k++;
		v++;
		theta += dtheta;}
	indexes[v] = start;
	v++;

	printf("%d %d\n", k, v);

	glGenBuffers( 1, &vbuffer);
	glBindBuffer( GL_ARRAY_BUFFER, vbuffer);
	glBufferData( GL_ARRAY_BUFFER,
		sizeof( vertices) + sizeof( normals) + sizeof( tex),
		NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER,
		0, sizeof( vertices), vertices);
	glBufferSubData( GL_ARRAY_BUFFER,
		sizeof( vertices), sizeof( normals), normals);
	glBufferSubData( GL_ARRAY_BUFFER,
		sizeof( vertices) + sizeof( normals), sizeof( tex), tex);

	glGenBuffers( 1, &ibuffer);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibuffer);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER,
		sizeof( indexes), indexes, GL_STATIC_DRAW);

	glUseProgram( program);
	vPosition = glGetAttribLocation(program,"vPosition");
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);
	vNormal = glGetAttribLocation(program, "vNormal");
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, (void*) sizeof(vertices));
	glEnableVertexAttribArray(vNormal);
	vTex = glGetAttribLocation(program, "vTex");
	glVertexAttribPointer(vTex, 2, GL_FLOAT, GL_FALSE, 0, (void*) (sizeof(vertices)+sizeof(normals)));
	glEnableVertexAttribArray(vTex);

	texture = loadTexture("data/checkerboard.jpg");
	glGenTextures( 1, &tBuffer);
	glActiveTexture( GL_TEXTURE0);
	glBindTexture( GL_TEXTURE_2D, tBuffer);
	glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGBA8, texture->width, texture->height);
	glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, texture->width, texture->height, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}