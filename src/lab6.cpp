//standard libary includes
#include <algorithm>
#include <cmath>
#include <cstdio>
//core opengl includes
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
//glm includes
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//freeimage includes
#include <FreeImage.h>
//local includes
#include "shaders.h"
#include "texture.h"

//window vars
int window;
char window_name[] = "lab6";
int window_width = 800;
int window_height = 600;
int display_width = 1600;
int display_height = 900;
//camera stuff
const double pi = M_PI;
const double half_pi = M_PI / 2;
const double tau = 2 * M_PI;
double camera_r = 8.0;
double theta = 0.0;
double phi = 0.0;
//shaders
int shader_program = 0;
char vertshdr_file[] = "shdr/lab6.vert.glsl";
char fragshdr_file[] = "shdr/lab6.frag.glsl";
char texture_file[] = "data/checkerboard.jpg";
//matices and stuff
glm::vec3 camera_eye;
glm::vec3 camera_center;
glm::vec3 camera_up;
glm::mat4 projection;
//consts
const int slices = 100;
const int spokes = 100;
const int vert_count = 2 * ( spokes + 1) + slices * spokes;
const int inner = 6 * slices * spokes + 6 * slices;
const int index_count = 2 * ( spokes + 2) + inner;
const float radius = 2.0f;
//other
GLuint sphere_vao;
struct Texture* texture;
glm::vec4 sphere_colour;
glm::vec3 atten = glm::vec3( 1.0, 0.001, 0.0001);
glm::vec3 light = glm::vec3( 0.0, 0.0, 2.0);
int vertex_count = 0;

//init stuff
void vars_init();
void window_init( int argc, char** argv);
void shaders_init();
void sphere_init();
//destructors
void vars_destroy();
//event functions
void display();
void resize( int width_new, int height_new);
void keydown( unsigned char key, int x, int y);
//other
void camera_recalc();

int main( int argc, char** argv){
	//init
	vars_init();
	window_init( argc, argv);
	shaders_init();
	sphere_init();

	//start main loop
	glutMainLoop();}

void vars_init(){
	sphere_colour = glm::vec4( 0.2, 0.4, 0.6, 1.0);
	camera_eye = glm::vec3( 0.0f, 0.0f, 0.0f);
	camera_center = glm::vec3( 0.0f, 0.0f, 0.0f);
	camera_up = glm::vec3( 0.0f, 0.0f, 1.0f);
	camera_recalc();}

void sphere_init(){
	GLuint vertex_buffer;
	GLuint index_buffer;
	GLint vertex;
	GLint texture_in;
	GLint normal_in;
	glGenVertexArrays( 1, &sphere_vao);
	glBindVertexArray( sphere_vao);
	float theta, phi;
	float dtheta, dphi;
	int face_count = 0;
	float x, y, z;
	int start;
	GLuint texture_buffer;

	dtheta = tau / spokes;
	dphi = pi / slices;

	//geometry
	GLfloat vertices[ vert_count][4];
	GLfloat normals[ vert_count][3];
	GLfloat tex[ vert_count][2];
	GLushort indexes[ index_count];

	vertices[face_count][0] = 0.0;
	vertices[face_count][1] = 0.0;
	vertices[face_count][2] = radius;
	vertices[face_count][3] = 1.0;
	normals[face_count][0] = 0.0;
	normals[face_count][1] = 0.0;
	normals[face_count][2] = 1.0;
	tex[face_count][0] = 0.0;
	tex[face_count][1] = 1.0;
	indexes[vertex_count] = face_count;
	face_count++;
	vertex_count++;

	phi = M_PI_2 - dphi;
	theta = 0.0;
	start = face_count;
	for( int i = 0; i < spokes; i++){
		x = radius * cos( phi) * cos( theta);
		y = radius * cos( phi) * sin( theta);
		z = radius * sin( phi);
		vertices[face_count][0] = x;
		vertices[face_count][1] = y;
		vertices[face_count][2] = z;
		vertices[face_count][3] = 1.0;
		normals[face_count][0] = x;
		normals[face_count][1] = y;
		normals[face_count][2] = z;
		tex[face_count][0] = theta / tau;
		tex[face_count][1] = phi / M_PI + 0.5;
		indexes[vertex_count] = face_count;
		face_count++;
		vertex_count++;
		theta += dtheta;}
	indexes[vertex_count] = start;
	vertex_count++;

	phi = half_pi - 2 * dphi;
	for( int j = 0; j < slices; j++){
		theta = 0.0;
		start = face_count;
		for( int i = 0; i < spokes; i++){
			x = radius * cos( phi) * cos( theta);
			y = radius * cos( phi) * sin( theta);
			z = radius * sin( phi);
			vertices[face_count][0] = x;
			vertices[face_count][1] = y;
			vertices[face_count][2] = z;
			vertices[face_count][3] = 1.0;
			normals[face_count][0] = x;
			normals[face_count][1] = y;
			normals[face_count][2] = z;
			tex[face_count][0] = theta / tau;
			tex[face_count][1] = phi / M_PI + 0.5;
			indexes[vertex_count] = face_count;
			if(i > 0) {
				indexes[vertex_count++] = face_count-1;
				indexes[vertex_count++] = face_count-1-spokes;
				indexes[vertex_count++] = face_count-1-spokes+1;
				indexes[vertex_count++] = face_count-1;
				indexes[vertex_count++] = face_count-1-spokes+1;
				indexes[vertex_count++] = face_count;}
			face_count++;
			theta += dtheta;}
		indexes[vertex_count++] = face_count-1;
		indexes[vertex_count++] = start;
		indexes[vertex_count++] = start+1;
		indexes[vertex_count++] = face_count-1;
		indexes[vertex_count++] = face_count-2;
		indexes[vertex_count++] = start+1;
		phi -= dphi;}

	vertices[face_count][0] = 0.0;
	vertices[face_count][1] = 0.0;
	vertices[face_count][2] = -radius;
	vertices[face_count][3] = 1.0;
	normals[face_count][0] = 0.0;
	normals[face_count][1] = 0.0;
	normals[face_count][2] = -1.0;
	tex[face_count][0] = 0.0;
	tex[face_count][1] = 0.0;
	indexes[vertex_count] = face_count;
	face_count++;
	vertex_count++;

	phi = - half_pi + dphi;
	theta = 0.0;
	start = face_count;
	for( int i = 0; i < spokes; i++){
		x = radius * cos( phi) * cos( theta);
		y = radius * cos( phi) * sin( theta);
		z = radius * sin( phi);
		vertices[face_count][0] = x;
		vertices[face_count][1] = y;
		vertices[face_count][2] = z;
		vertices[face_count][3] = 1.0;
		normals[face_count][0] = x;
		normals[face_count][1] = y;
		normals[face_count][2] = z;
		tex[face_count][0] = theta / tau;
		tex[face_count][1] = phi / pi + 0.5;
		indexes[vertex_count] = face_count;
		face_count++;
		vertex_count++;
		theta += dtheta;}
	indexes[vertex_count] = start;
	vertex_count++;

	printf( "face_count: %d, vertex_count: %d\n",
		face_count, vertex_count);

	glGenBuffers( 1, &vertex_buffer);
	glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData( GL_ARRAY_BUFFER,
		sizeof( vertices) + sizeof( normals) + sizeof( tex),
		NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER,
		0, sizeof( vertices), vertices);
	glBufferSubData( GL_ARRAY_BUFFER,
		sizeof( vertices), sizeof( normals), normals);
	glBufferSubData( GL_ARRAY_BUFFER,
		sizeof( vertices) + sizeof( normals), sizeof( tex), tex);

	glGenBuffers( 1, &index_buffer);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER,
		sizeof( indexes), indexes, GL_STATIC_DRAW);

	glUseProgram( shader_program);
	vertex = glGetAttribLocation( shader_program,"vertex");
	glVertexAttribPointer( vertex, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray( vertex);

	normal_in = glGetAttribLocation( shader_program, "normal_in");
	glVertexAttribPointer(
		normal_in, 3, GL_FLOAT, GL_FALSE, 0, (void*) sizeof(vertices));
	glEnableVertexAttribArray( normal_in);

	texture_in = glGetAttribLocation( shader_program, "texture_in");
	glVertexAttribPointer( texture_in, 2, GL_FLOAT, GL_FALSE,
		0, (void*) ( sizeof( vertices) + sizeof( normals)));
	glEnableVertexAttribArray(texture_in);

	texture = loadTexture( texture_file);
	glGenTextures( 1, &texture_buffer);
	glActiveTexture( GL_TEXTURE0);
	glBindTexture( GL_TEXTURE_2D, texture_buffer);
	glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGBA8,
		texture->width, texture->height);
	glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0,
		texture->width, texture->height, GL_RGB,
		GL_UNSIGNED_BYTE, texture->data);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);}

void vars_destroy(){}

void shaders_init(){
	printf("building vertex shader\n");
	int vertshdr = build_shader( GL_VERTEX_SHADER, vertshdr_file);
	printf("building fragment shader\n");
	int fragshdr = build_shader( GL_FRAGMENT_SHADER, fragshdr_file);
	printf("building shader program\n");
	shader_program = build_program( vertshdr, fragshdr);
	if( true){
		printf("dumping shader program debug info\n");
		dump_program( shader_program);}}

void window_init( int argc, char** argv){
	printf("initializing window\n");
	//create window
	glutInit( &argc, argv);
	glutInitWindowSize( (GLsizei) window_width, (GLsizei) window_height);
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(
		( display_width - window_width) / 2,
		( display_height - window_height) / 2);
	window = glutCreateWindow( window_name);
	GLenum error = glewInit();
	if( error != GLEW_OK){
		printf("Error starting GLEW: %s\n",
			glewGetErrorString( error));
		exit( 1);}

	//connect event functions
	glutDisplayFunc( display);
	glutReshapeFunc( resize);
	glutKeyboardFunc( keydown);
	glutIgnoreKeyRepeat( true);
	atexit( vars_destroy);

	//set other flags
	glEnable( GL_DEPTH_TEST);
	glClearColor( 0.08, 0.08, 0.08, 1.0);}

void resize( int width_new, int height_new){
	if( height_new == 0)
		height_new = 1;
	window_width = width_new;
	window_height = height_new;
	float ratio = 1.0 * window_width / window_height;
	glViewport( 0, 0,
		(GLsizei) window_width, (GLsizei) window_height);
	projection = glm::perspective( 45.0f, ratio, 1.0f, 100.0f);}

void display(){
	//clear buffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram( shader_program);

	//set up view
	//glLoadIdentity();
	camera_recalc();
	glm::mat4 view = glm::lookAt(
		camera_eye, camera_center, camera_up);
	glm::mat3 normal = glm::transpose(
		glm::inverse( glm::mat3( view)));

	//bind vars
	int normal_loc = glGetUniformLocation( shader_program, "normal_mat");
	glUniformMatrix3fv( normal_loc, 1, 0, glm::value_ptr( normal));
	int view_loc = glGetUniformLocation( shader_program, "model_view");
	glUniformMatrix4fv( view_loc, 1, 0, glm::value_ptr( view));
	int proj_loc = glGetUniformLocation( shader_program, "projection");
	glUniformMatrix4fv( proj_loc, 1, 0, glm::value_ptr( projection));

	int base_loc = glGetUniformLocation( shader_program, "base");
	glUniform4fv( base_loc, 1, glm::value_ptr( sphere_colour));
	int material_loc = glGetUniformLocation( shader_program, "material");
	glUniform4f( material_loc, 0.2, 0.4, 0.4, 200.0);
	int light_loc = glGetUniformLocation( shader_program, "light");
	glUniform3fv( light_loc, 1, glm::value_ptr( light));
	int atten_loc = glGetUniformLocation( shader_program, "attenuation");
	glUniform3fv( atten_loc, 1, glm::value_ptr( atten));

	//draw sphere
	glBindVertexArray( sphere_vao);
	glDrawElements(
		GL_TRIANGLE_STRIP, vertex_count, GL_UNSIGNED_SHORT, NULL);

	//swap buffers
	glutSwapBuffers();}

void keydown( unsigned char key, int x, int y){
	//printf( "key pressed: %c %d\n", key, key);
	switch( key){
		case 'a':
			theta -= tau / 16;
			camera_recalc();
			glutPostRedisplay();
			break;
		case 'd':
			theta += tau / 16;
			camera_recalc();
			glutPostRedisplay();
			break;
		case 'w':
			phi += tau / 16;
			camera_recalc();
			glutPostRedisplay();
			break;
		case 's':
			phi -= tau / 16;
			camera_recalc();
			glutPostRedisplay();
			break;
		case 'r':
			theta = 0.0;
			phi = 0.0;
			camera_recalc();
			glutPostRedisplay();
			break;
		case 'y':
			light[2] += 0.5;
			glutPostRedisplay();
			break;
		case 'h':
			light[2] -= 0.5;
			glutPostRedisplay();
			break;
		case 'u':
			sphere_colour[0] = std::min( sphere_colour[0] + 0.1, 1.0);
			glutPostRedisplay();
			break;
		case 'j':
			sphere_colour[0] = std::max( sphere_colour[0] - 0.1, 0.0);
			glutPostRedisplay();
			break;
		case 'i':
			sphere_colour[1] = std::min( sphere_colour[1] + 0.1, 1.0);
			glutPostRedisplay();
			break;
		case 'k':
			sphere_colour[1] = std::max( sphere_colour[1] - 0.1, 0.0);
			glutPostRedisplay();
			break;
		case 'o':
			sphere_colour[2] = std::min( sphere_colour[2] + 0.1, 1.0);
			glutPostRedisplay();
			break;
		case 'l':
			sphere_colour[2] = std::max( sphere_colour[2] - 0.1, 0.0);
			glutPostRedisplay();
			break;
		case 'q':
		case 27:
			printf( "exiting\n");
			exit( 0);}}

void camera_recalc(){
	if( phi > half_pi) phi = half_pi;
	if( phi < - half_pi) phi = - half_pi;
	camera_eye[0] = camera_r * cos( phi) * cos( theta);
	camera_eye[1] = camera_r * cos( phi) * sin( theta);
	camera_eye[2] = camera_r * sin( phi);}
