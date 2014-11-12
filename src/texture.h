#ifndef texture_h
#define texture_h

struct Texture {
	int width;
	int height;
	int depth;
	int size;
	unsigned char* data;
};

struct Texture* loadTexture( char* filename);

struct CubeMap {
	int width;
	int height;
	int depth;
	unsigned char* data[6];
};

struct CubeMap* loadCubeMap( char* basename);

#endif
