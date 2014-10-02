//standard libary includes
#include <stdio.h>
//opengl includes
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
//freeimage includes
#include <FreeImage.h>

int window;
char window_name[] = "lab1";
const int window_width = 512;
const int window_height = 512;
unsigned char image[ window_height][ window_width][3];
char image_filename[] = "gen/image.png";
bool flip = false;

void image_create();
void display();
void window_init( int argc, char** argv);
void keydown( unsigned char key, int x, int y);
void image_save( char* filename);
void image_load( char* filename);

int main( int argc, char** argv){
	image_create();
	image_save( image_filename);
	image_load( image_filename);
	window_init( argc, argv);
}

void window_init( int argc, char** argv){
	glutInit( &argc, argv);
	glutInitWindowSize( (GLsizei) window_width, (GLsizei) window_height);
	glutInitDisplayMode( GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition( 0, 0);
	window = glutCreateWindow( window_name);
	glutDisplayFunc( display);
	glutKeyboardFunc( keydown);
	glutIgnoreKeyRepeat( true);
	glutMainLoop();}

void image_create(){
	for( int i = 0; i < window_height; i++)
		for( int j = 0; j < window_width; j++){
			image[i][j][0] = i % 256;
			image[i][j][1] = 0;
			image[i][j][2] = 0;}}

void display(){
	glLoadIdentity();
	glViewport( 0,0, window_width - 1, window_height - 1);
	gluOrtho2D( 0.0, window_width, 0.0, window_height);
	glClearColor( 0.0, 0.0, 0.0, 0.0);
	glClear( GL_COLOR_BUFFER_BIT);
	if( flip) {
		glRasterPos2i( 0, window_width - 1);
		glPixelZoom( 1.0f, -1.0f);}
	else {
		glRasterPos2i( 0,0);
		glPixelZoom( 1.0, 1.0);}
	glDrawPixels(
		window_width, window_height,
		GL_RGB, GL_UNSIGNED_BYTE, image);
	glFlush();
}

void keydown( unsigned char key, int x, int y){
	//printf( "%c %d\n", key, key);
	switch( key){
		case 'f':
			flip = !flip;
			glutPostRedisplay();
			break;
		case 27:
			//we should really be freeing memory here first
			exit( 0);}}

void image_save( char* filename){
	FIBITMAP* bitmap;
	BYTE* bits;
	bitmap = FreeImage_Allocate( window_width, window_height, 24);
	for( int i=0; i< window_height; i++) {
		bits = FreeImage_GetScanLine( bitmap, i);
		for( int j = 0; j < window_width; j++) {
			bits[ FI_RGBA_RED] = image[i][j][0];
			bits[ FI_RGBA_GREEN] = image[i][j][1];
			bits[ FI_RGBA_BLUE] = image[i][j][2];
			bits += 3;}}
	FreeImage_Save( FIF_PNG, bitmap, filename, PNG_DEFAULT);}

void image_load( char* filename){
	FIBITMAP *bitmap;
	BYTE *bits;
	bitmap = FreeImage_Load( FIF_PNG, filename, PNG_DEFAULT);
	for( int i = 0; i < window_height; i++) {
		bits = FreeImage_GetScanLine( bitmap, i);
		for( int j = 0; j < window_width; j++){
			image[i][j][0] = bits[ FI_RGBA_RED];
			image[i][j][1] = bits[ FI_RGBA_GREEN];
			image[i][j][2] = bits[ FI_RGBA_BLUE];
			bits += 3;}}
	FreeImage_Unload( bitmap);}
