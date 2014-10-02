#ifndef shaders_h
#define shaders_h

int build_shader( int type, char* filename);
int build_program( int first, ...);
void dump_program( int program);

#endif