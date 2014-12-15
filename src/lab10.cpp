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
#include <FreeImage.h>
//ode includes
#include <ode/ode.h>
//local includes
#include "shaders.h"

//window vars
int window;
char window_name[] = "lab10";
int window_width = 800;
int window_height = 600;
int display_width = 1600;
int display_height = 900;
//camera stuff
const double half_pi = M_PI / 2;
const double tau = 2 * M_PI;
double camera_r = 8.0;
double theta = 0.0;
double phi = 0.0;
//shaders
int shader_program = 0;
char vertshdr_file[] = "shdr/lab10.vert.glsl";
char fragshdr_file[] = "shdr/lab10.frag.glsl";
//matices and stuff
glm::vec3 camera_eye;
glm::vec3 camera_center;
glm::vec3 camera_up;
glm::mat4 projection;
//ode stuff
static dWorldID world;
static dSpaceID space;
static dGeomID ground;
static dJointGroupID contactgroup;
const dReal radius = 0.2;
const dReal mass = 1.0;
typedef struct {
	dBodyID body;
	dGeomID geom;
} MyObject;
MyObject cube;
//other
glm::vec4 object_colour;
GLuint cube_vao;
GLuint plane_vao;
glm::vec3 atten = glm::vec3( 1.0, 0.001, 0.0001);
glm::vec3 light = glm::vec3( 0.0, 0.0, 2.0);
const int plane_index_count = 6;
const int cube_index_count = 36;

//init stuff
void vars_init();
void ode_init();
void window_init( int argc, char** argv);
void shaders_init();
void plane_init();
void cube_init();
//destructors
void vars_destroy();
//event functions
void animate();
//static void handle_collision( void *data, dGeomID o1, dGeomID o2);
void display();
void resize( int width_new, int height_new);
void keydown( unsigned char key, int x, int y);
//other
void camera_recalc();

int main( int argc, char** argv){
	//init
	vars_init();
	ode_init();
	window_init( argc, argv);
	shaders_init();
	plane_init();
	cube_init();

	//start main loop
	glutMainLoop();}

void vars_init(){
	//cube_init();
	object_colour = glm::vec4( 0.2, 0.4, 0.6, 1.0);
	camera_eye = glm::vec3( 0.0f, 0.0f, 0.0f);
	camera_center = glm::vec3( 0.0f, 0.0f, 0.0f);
	camera_up = glm::vec3( 0.0f, 0.0f, 1.0f);
	camera_recalc();}

void ode_init(){
	dReal x0 = 0.0, y0 = 0.0, z0 = 5.0;
	dMass m1;
	dInitODE();
	world = dWorldCreate();
	space = dHashSpaceCreate(0);
	contactgroup = dJointGroupCreate(0);
	dWorldSetGravity(world,0,0,-0.5);
	// create a ground
	ground = dCreatePlane(space,0,0,1,0);
	// create a ball
	cube.body = dBodyCreate(world);
	dMassSetZero(&m1);
	dMassSetBoxTotal(&m1,mass,2.0, 2.0, 2.0);
	dBodySetMass(cube.body,&m1);
	dBodySetPosition(cube.body, x0, y0, z0);
	cube.geom = dCreateBox(space,2.0, 2.0, 2.0);
	dGeomSetBody(cube.geom,cube.body);}

void plane_init(){
	GLuint vertex_buffer;
	GLuint index_buffer;
	GLint vertex;
	GLint normal_in;

	glGenVertexArrays( 1, &plane_vao);
	glBindVertexArray( plane_vao);

	GLfloat vertices[4][4] = {
		{ -5.0, -5.0, 0.0, 1.0},
		{ -5.0, 5.0, 0.0, 1.0 },
		{ 5.0, -5.0, 0.0, 1.0},
		{ 5.0, 5.0, 0.0, 1.0}};
	GLfloat normals[4][3] = {
		{ 0.0, 0.0, 1.0},
		{ 0.0, 0.0, 1.0},
		{ 0.0, 0.0, 1.0},
		{ 0.0, 0.0, 1.0}};
	GLushort indices[6] = { 0, 1, 3, 0, 2, 3 };

	glGenBuffers( 1, &vertex_buffer);
	glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData( GL_ARRAY_BUFFER,
		sizeof( vertices) + sizeof( normals), NULL, GL_STATIC_DRAW);

	//bind vertices
	glBufferSubData( GL_ARRAY_BUFFER,
		0, sizeof( vertices), vertices);
	//bind normals
	glBufferSubData( GL_ARRAY_BUFFER,
		sizeof( vertices), sizeof( normals), normals);

	glGenBuffers( 1, &index_buffer);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER,
		sizeof( indices), indices, GL_STATIC_DRAW);

	vertex = glGetAttribLocation( shader_program,"vertex");
	glVertexAttribPointer( vertex, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray( vertex);

	normal_in = glGetAttribLocation( shader_program, "normal_in");
	glVertexAttribPointer( normal_in, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray( normal_in);;}

void cube_init(){
	GLuint vertex_buffer;
	GLuint index_buffer;
	GLint vertex;
	GLint normal_in;

	//bind cube vertex array object
	glGenVertexArrays( 1, &cube_vao);
	glBindVertexArray( cube_vao);

	GLfloat vertices[8][4] = {
		{ -1.0, -1.0, -1.0, 1.0},
		{ -1.0, -1.0,  1.0, 1.0},
		{ -1.0,  1.0, -1.0, 1.0},
		{ -1.0,  1.0,  1.0, 1.0},
		{  1.0, -1.0, -1.0, 1.0},
		{  1.0, -1.0,  1.0, 1.0},
		{  1.0,  1.0, -1.0, 1.0},
		{  1.0,  1.0,  1.0, 1.0}};

	GLfloat normals[8][3] = {
		{ -1.0, -1.0, -1.0},
		{ -1.0, -1.0,  1.0},
		{ -1.0,  1.0, -1.0},
		{ -1.0,  1.0,  1.0},
		{  1.0, -1.0, -1.0},
		{  1.0, -1.0,  1.0},
		{  1.0,  1.0, -1.0},
		{  1.0,  1.0,  1.0}};

	GLushort indexes[36] = {
		0, 1, 3, 0, 2, 3,
		0, 4, 5, 0, 1, 5,
		2, 6, 7, 2, 3, 7,
		0, 4, 6, 0, 2, 6,
		1, 5, 7, 1, 3, 7,
		4, 5, 7, 4, 6, 7};

	glGenBuffers( 1, &vertex_buffer);
	glBindBuffer( GL_ARRAY_BUFFER,vertex_buffer);
	glBufferData( GL_ARRAY_BUFFER,
		sizeof(vertices) + sizeof(normals), NULL,
		GL_STATIC_DRAW);
	//bind vertices
	glBufferSubData( GL_ARRAY_BUFFER,
		0, sizeof(vertices), vertices);
	//bind normals
	glBufferSubData( GL_ARRAY_BUFFER,
		sizeof(vertices), sizeof(normals), normals);

	//bind indexes
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(indexes), indexes, GL_STATIC_DRAW);

	glUseProgram( shader_program);

	vertex = glGetAttribLocation( shader_program,"vertex");
	glVertexAttribPointer( vertex, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray( vertex);

	normal_in = glGetAttribLocation( shader_program, "normal_in");
	glVertexAttribPointer( normal_in, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray( normal_in);}

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
	glutIdleFunc( animate);
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

static void handle_collision( void *data, dGeomID o1, dGeomID o2){
	const int N = 10;
	dContact contact[N];
	int isGround = ( ( ground == o1) || ( ground == o2));
	int n = dCollide( o1, o2, N, &contact[0].geom,sizeof( dContact));
	if( isGround)
		for( int i = 0; i < n; i++){
			contact[i].surface.mode = dContactRolling;
			contact[i].surface.mu = dInfinity;
			// (0.0~1.0) restitution parameter
			contact[i].surface.bounce = 0.5;
			// minimum incoming velocity for bounce
			contact[i].surface.bounce_vel = 0.1;
			dJointID new_contact = dJointCreateContact(
				world, contactgroup, &contact[i]);
			dJointAttach( new_contact,
				dGeomGetBody( contact[i].geom.g1),
				dGeomGetBody( contact[i].geom.g2));}}

void animate(){
	//const dReal *pos, *R;
	dBodyAddTorque( cube.body, 0.1, 0.1, 0.0);
	dSpaceCollide( space, 0, &handle_collision);
	dWorldStep( world, 0.01);
	dJointGroupEmpty( contactgroup);
	glutSetWindow( window);
	glutPostRedisplay();}

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

	//get cube stuff
	dReal* position = (dReal*) dBodyGetPosition( cube.body);
	dReal* rotation = (dReal*) dBodyGetRotation( cube.body);
	glm::mat4 model = glm::translate( glm::mat4( 1.0f),
		glm::vec3( position[0], position[1], position[2]));
	glm::mat4 rot_mat = glm::mat4(1.0f);
	rot_mat[0][0] = rotation[0];
	rot_mat[0][1] = rotation[1];
	rot_mat[0][2] = rotation[2];
	rot_mat[1][0] = rotation[4];
	rot_mat[1][1] = rotation[5];
	rot_mat[1][2] = rotation[6];
	rot_mat[2][0] = rotation[8];
	rot_mat[2][1] = rotation[9];
	rot_mat[2][2] = rotation[10];
	model = model * rot_mat;
	glm::mat4 model_view = view * model;

	//bind vars
	int normal_loc = glGetUniformLocation( shader_program, "normal_mat");
	glUniformMatrix3fv( normal_loc, 1, 0, glm::value_ptr( normal));
	int view_loc = glGetUniformLocation( shader_program, "model_view");
	glUniformMatrix4fv( view_loc, 1, 0, glm::value_ptr( view));
	int proj_loc = glGetUniformLocation( shader_program, "projection");
	glUniformMatrix4fv( proj_loc, 1, 0, glm::value_ptr( projection));

	int base_loc = glGetUniformLocation( shader_program, "base");
	glUniform4fv( base_loc, 1, glm::value_ptr( object_colour));
	int material_loc = glGetUniformLocation( shader_program, "material");
	glUniform4f( material_loc, 0.2, 0.4, 0.4, 200.0);
	int light_loc = glGetUniformLocation( shader_program, "light");
	glUniform3fv( light_loc, 1, glm::value_ptr( light));
	int atten_loc = glGetUniformLocation( shader_program, "attenuation");
	glUniform3fv( atten_loc, 1, glm::value_ptr( atten));

	//draw plane
	glBindVertexArray( plane_vao);
	glDrawElements( GL_TRIANGLES, plane_index_count, GL_UNSIGNED_SHORT, NULL);

	//draw cube
	glUniformMatrix4fv( view_loc, 1, 0, glm::value_ptr( model_view));
	glBindVertexArray( cube_vao);
	glDrawElements( GL_TRIANGLES, cube_index_count, GL_UNSIGNED_SHORT, NULL);

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
			object_colour[0] = std::min( object_colour[0] + 0.1, 1.0);
			glutPostRedisplay();
			break;
		case 'j':
			object_colour[0] = std::max( object_colour[0] - 0.1, 0.0);
			glutPostRedisplay();
			break;
		case 'i':
			object_colour[1] = std::min( object_colour[1] + 0.1, 1.0);
			glutPostRedisplay();
			break;
		case 'k':
			object_colour[1] = std::max( object_colour[1] - 0.1, 0.0);
			glutPostRedisplay();
			break;
		case 'o':
			object_colour[2] = std::min( object_colour[2] + 0.1, 1.0);
			glutPostRedisplay();
			break;
		case 'l':
			object_colour[2] = std::max( object_colour[2] - 0.1, 0.0);
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
