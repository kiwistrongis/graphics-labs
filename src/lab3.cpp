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

//window vars
int window;
char window_name[] = "lab1";
int window_width = 800;
int window_height = 600;
int display_width = 1600;
int display_height = 900;
//camera stuff
const double half_pi = M_PI / 2;
const double tau = 2 * M_PI;
double radius = 8.0;
double theta = 0.0;
double phi = 0.0;
//shaders
int shader_program = 0;
char vertshdr_file[] = "shdr/lab3_vertex.glsl";
char fragshdr_file[] = "shdr/lab3_fragment.glsl";
//matices and stuff
glm::vec3 camera_eye;
glm::vec3 camera_center;
glm::vec3 camera_up;
glm::mat4 projection;
//other
bool wire_enabled = false;
glm::vec4 teapot_colour;

//init stuff
void vars_init();
void window_init( int argc, char** argv);
void shaders_init();
//destructors
void vars_destroy();
//event functions
void display();
void resize( int width_new, int height_new);
void keydown( unsigned char key, int x, int y);
//other
void camera_recalc();
void camera_reset();

int main( int argc, char** argv){
	//init
	vars_init();
	window_init( argc, argv);
	shaders_init();

	//start main loop
	glutMainLoop();}

void vars_init(){
	teapot_colour = glm::vec4( 0.2, 0.4, 0.6, 1.0);
	camera_eye = glm::vec3( 0.0f, 0.0f, 0.0f);
	camera_center = glm::vec3( 0.0f, 0.0f, 1.2f);
	camera_up = glm::vec3( 0.0f, 0.0f, 1.0f);
	camera_recalc();}

void vars_destroy(){}

void shaders_init(){
	printf("building vertex shader\n");
	int vertshdr = build_shader( GL_VERTEX_SHADER, vertshdr_file);
	printf("building fragment shader\n");
	int fragshdr = build_shader( GL_FRAGMENT_SHADER, fragshdr_file);
	printf("building shader program\n");
	shader_program = build_program( vertshdr, fragshdr);
	printf("dumping shader program debug info\n");
	dump_program( shader_program);}

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

	//bind
	int view_loc = glGetUniformLocation( shader_program, "model_view");
	glUniformMatrix4fv( view_loc, 1, 0, glm::value_ptr( view));
	int proj_loc = glGetUniformLocation( shader_program, "projection");
	glUniformMatrix4fv( proj_loc, 1, 0, glm::value_ptr( projection));

	int base_loc = glGetUniformLocation( shader_program, "base");
	glUniform4fv( base_loc, 1, glm::value_ptr( teapot_colour));

	//actually draw the teapot
	if( wire_enabled)
		glutWireTeapot( 1);
	else
		glutSolidTeapot( 1);
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
			wire_enabled = ! wire_enabled;
			glutPostRedisplay();
			break;
		case 'u':
			teapot_colour[0] = std::min( teapot_colour[0] + 0.1, 1.0);
			glutPostRedisplay();
			break;
		case 'j':
			teapot_colour[0] = std::max( teapot_colour[0] - 0.1, 0.0);
			glutPostRedisplay();
			break;
		case 'i':
			teapot_colour[1] = std::min( teapot_colour[1] + 0.1, 1.0);
			glutPostRedisplay();
			break;
		case 'k':
			teapot_colour[1] = std::max( teapot_colour[1] - 0.1, 0.0);
			glutPostRedisplay();
			break;
		case 'o':
			teapot_colour[2] = std::min( teapot_colour[2] + 0.1, 1.0);
			glutPostRedisplay();
			break;
		case 'l':
			teapot_colour[2] = std::max( teapot_colour[2] - 0.1, 0.0);
			glutPostRedisplay();
			break;
		case 'q':
		case 27:
			//printf("exiting?");
			exit( 0);}}

void camera_recalc(){
	if( phi > half_pi) phi = half_pi;
	if( phi < - half_pi) phi = - half_pi;
	camera_eye[0] = radius * cos( phi) * cos( theta);
	camera_eye[1] = radius * cos( phi) * sin( theta);
	camera_eye[2] = radius * sin( phi);}
