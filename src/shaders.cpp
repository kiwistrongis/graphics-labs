//standard libary includes
#include <cstdarg>
#include <cstdio>
//opengl includes
#include <GL/glew.h>
//local includes
#include "shaders.h"

char* readShaderFile( char* filename){
	//vars
	FILE *fid;
	char *buffer;
	int len;
	int n;
	//open file
	printf("opening file\n");
	fid = fopen( filename, "r");
	if( fid == NULL){
		printf("can't open shader file: %s\n", filename);
		return(0);}
	//read the file
	printf("reading file\n");
	fseek(fid, 0, SEEK_END);
	len = ftell( fid);
	rewind( fid);
	buffer = new char[len+1];
	n = fread( buffer, sizeof(char), len, fid);
	buffer[n] = 0;
	return buffer;}

int build_shader( int type, char* filename){
	//vars
	int shader;
	char* source;
	int result;
	char* buffer;
	//load source
	shader = glCreateShader( type);
	printf("loading source\n");
	source = readShaderFile( filename);
	if( source == 0) return 0;
	//compile shader
	printf("compiling source\n");
	glShaderSource( shader, 1, (const GLchar **) &source, 0);
	glCompileShader( shader);
	//check compilation
	printf("checking result\n");
	glGetShaderiv( shader, GL_COMPILE_STATUS, &result);
	if( result == GL_TRUE){
		delete[] source;
		return shader;}
	else{
		printf( "error compiling shader file: %s\n", filename);
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &result);
		buffer = new char[result];
		glGetShaderInfoLog( shader, result, 0, buffer);
		printf( "%s\n", buffer);
		//clean up
		delete[] source;
		delete[] buffer;
		return 0;}}

int build_program( int first, ...){
	//vars
	int result;
	char *buffer;
	int program;
	va_list argptr;
	int shader;
	int vs = 0;
	int fs = 0;
	int type;
	//create program for first shader
	program = glCreateProgram();
	if( first != 0){
		glAttachShader( program, first);
		glGetShaderiv( first, GL_SHADER_TYPE, &type);
		if( type == GL_VERTEX_SHADER) vs++;
		if( type == GL_FRAGMENT_SHADER) fs++;}
	//create program for the rest
	va_start( argptr, first);
	while( ( shader = va_arg( argptr, int)) != 0){
		glAttachShader( program, shader);
		glGetShaderiv( shader, GL_SHADER_TYPE, &type);
		if( type == GL_VERTEX_SHADER) vs++;
		if( type == GL_FRAGMENT_SHADER) fs++;}
	//checks
	if( vs == 0)
		printf("no vertex shader\n");
	if( fs == 0)
		printf("no fragment shader\n");
	//link the program
	glLinkProgram( program);
	glGetProgramiv( program, GL_LINK_STATUS, &result);
	if( result != GL_TRUE) {
		printf("program link error\n");
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &result);
		buffer = new char[result];
		glGetProgramInfoLog( program, result, 0, buffer);
		printf( "%s\n", buffer);
		delete[] buffer;
		return(0);}
	return program;
}

void dump_program( int program){
	//vars
	char name[256];
	GLsizei length;
	GLint size;
	GLenum type;
	int uniforms;
	int attributes;
	int shaders;

	if( ! glIsProgram( program)){
		printf( "not a valid shader program\n");
		return;}

	//print shader components
	glGetProgramiv( program, GL_ATTACHED_SHADERS, &shaders);
	printf( "Number of shaders: %d\n", shaders);

	//print uniforms
	glGetProgramiv( program, GL_ACTIVE_UNIFORMS, &uniforms);
	printf( "uniforms: %d\n", uniforms);
	for( int i = 0; i < uniforms; i++){
		glGetActiveUniform(program, i, 256, &length, &size ,&type, name);
		printf( " name: %s\n",name);}

	//print attributes
	glGetProgramiv( program, GL_ACTIVE_ATTRIBUTES, &attributes);
	printf( "attributes: %d\n", attributes);
	for( int i=0; i < attributes; i++) {
		glGetActiveAttrib( program, i, 256, &length, &size, &type, name);
		printf( " name: %s\n",name);}}