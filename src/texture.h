/*************************************************
 *
 *                 texture.h
 *
 *  Simple texture loader.  Data structure and
 *  procedure declarations
 *
 *************************************************/

struct Texture {
	int width;
	int height;
	int depth;
	int size;
	unsigned char *data;
};

struct Texture *loadTexture(char *filename);