//standard libary includes
#include <cstdlib>
#include <cstring>
//freeimage includes
#include <FreeImage.h>
//local includes
#include "texture.h"

char posx_file[] = "posx.jpg";
char negx_file[] = "negx.jpg";
char posz_file[] = "posz.jpg";
char negz_file[] = "negz.jpg";
char posy_file[] = "posy.jpg";
char negy_file[] = "negy.jpg";


struct Texture* loadTexture( char* filename){
	FIBITMAP* bitmap = FreeImage_Load( FIF_JPEG, filename, JPEG_DEFAULT);
	struct Texture* result = new Texture;

	(*result).width = FreeImage_GetWidth( bitmap);
	(*result).height = FreeImage_GetHeight( bitmap);
	(*result).depth = FreeImage_GetBPP( bitmap)/8;
	int size = (*result).width * (*result).height * (*result).depth;
	(*result).size = size;
	unsigned char* data = (unsigned char*) malloc( size);
	(*result).data = data;

	int k = 0;
	BYTE* bits;
	for( int j = 0; j < (*result).height; j++){
		bits = FreeImage_GetScanLine( bitmap, j);
			for( int i = 0; i < (*result).width; i++){
				data[ k++] = bits[ FI_RGBA_RED];
				data[ k++] = bits[ FI_RGBA_GREEN];
				data[ k++] = bits[ FI_RGBA_BLUE];
				bits += 3;}}

	return result;}

struct CubeMap* loadCubeMap( char* basename) {
	char filename[256];
	char* extensions[] = {
		posx_file, negx_file,
		posz_file, negz_file,
		posy_file, negy_file};

	struct CubeMap* result = new CubeMap();
	strcpy( filename, basename);
	strcat( filename, "/");
	strcat( filename, extensions[0]);

	struct Texture* tex = loadTexture( filename);
	result->width = tex->width;
	result->height = tex->height;
	result->depth = tex->depth;
	result->data[0] = tex->data;
	for( int i = 1; i < 6; i++) {
		strcpy( filename, basename);
		strcat( filename, "/");
		strcat( filename, extensions[i]);
		tex = loadTexture( filename);
		result->data[i] = tex->data;}
	return result;
}
