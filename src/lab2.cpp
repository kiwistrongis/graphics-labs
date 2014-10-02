//standard libary includes
#include <cmath>
#include <cstdio>
//opengl includes
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
//freeimage includes
#include <FreeImage.h>
//local includes
#include "shaders.h"

//window vars
int window;
char window_name[] = "lab1";
int window_width = 800;
int window_height = 600;
//camera stuff
const double half_pi = M_PI / 2;
const double tau = 2 * M_PI;
double* camera;
double radius = 5.0;
double theta = 0.0;
double phi = 0.0;
//shaders
int shader_program = 0;
char vertshdr_file[] = "data/lab2_vertex.glsl";
char fragshdr_file[] = "data/lab2_fragment.glsl";

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
	camera = new double[3];
	camera_recalc();}

void vars_destroy(){
	delete[] camera;}

void shaders_init(){
	printf("building vertex shader\n");
	int vertshdr = build_shader( GL_VERTEX_SHADER, vertshdr_file);
	printf("building fragment shader\n");
	int fragshdr = build_shader( GL_FRAGMENT_SHADER, fragshdr_file);
	printf("building shader program\n");
	shader_program = build_program( vertshdr, fragshdr);}

void window_init( int argc, char** argv){
	printf("initializing window\n");
	//create window
	glutInit( &argc, argv);
	glutInitWindowSize( (GLsizei) window_width, (GLsizei) window_height);
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition( -1, -1);
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
	atexit( vars_destroy);
	glutIgnoreKeyRepeat( true);

	//set other flags
	glEnable( GL_DEPTH_TEST);
	glClearColor( 0.08, 0.08, 0.08, 1.0);}

void resize( int width_new, int height_new){
	if( height_new == 0)
		height_new = 1;
	window_width = width_new;
	window_height = height_new;
	glViewport( 0, 0,
		(GLsizei) window_width, (GLsizei) window_height);
	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 40.0,
		(GLfloat) window_width / (GLfloat) window_height, 1.0, 10.0);
	glMatrixMode( GL_MODELVIEW);
	camera_reset();}

void display(){
	//clear buffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//set shader program
	glUseProgram( shader_program);
	//set perspective
	camera_reset();
	//draw teapot
	//glColor3f( 0.8, 0.2, 0.2);
	glPushMatrix();
	glRotatef( 90, 1, 0, 0);
	glRotatef( 90, 0, 1, 0);
	if( 1)
		glutWireTeapot( 1);
	else
		glutSolidTeapot( 1);
	glPopMatrix();
	//flush buffer
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
		case 'u':
			camera[0] += 0.5;
			glutPostRedisplay();
			break;
		case 'j':
			camera[0] -= 0.5;
			glutPostRedisplay();
			break;
		case 'i':
			camera[1] += 0.5;
			glutPostRedisplay();
			break;
		case 'k':
			camera[1] -= 0.5;
			glutPostRedisplay();
			break;
		case 'o':
			camera[2] += 0.5;
			glutPostRedisplay();
			break;
		case 'l':
			camera[2] -= 0.5;
			glutPostRedisplay();
			break;
		case 'q':
		case 27:
			//printf("exiting?");
			exit( 0);}}

void camera_recalc(){
	if( phi > half_pi) phi = half_pi;
	if( phi < - half_pi) phi = - half_pi;
	camera[0] = radius * cos( phi) * cos( theta);
	camera[1] = radius * cos( phi) * sin( theta);
	camera[2] = radius * sin( phi);}

void camera_reset(){
	glLoadIdentity();
	gluLookAt(
		camera[0], camera[1], camera[2],
		0.0, 0.0, 0.0,
		0.0f, 0.0f, 1.0f);}

