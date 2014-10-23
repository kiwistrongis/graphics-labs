/********************************
 *
 *           texture.cpp
 *
 *  Simple texture loader
 *
 *****************************/

#include <FreeImage.h>
#include "texture.h"
#include <stdlib.h>

struct Texture *loadTexture(char *filename) {
	int i,j;
	int k;
	FIBITMAP *bitmap;
	BYTE *bits;
	unsigned char *data;
	struct Texture *result;
	int size;

	bitmap = FreeImage_Load(FIF_JPEG, filename, JPEG_DEFAULT);
	result = new Texture;

	result->width = FreeImage_GetWidth(bitmap);
	result->height = FreeImage_GetHeight(bitmap);
	result->depth = FreeImage_GetBPP(bitmap)/8;
	size = result->width * result->height * result->depth;
	result->size = size;
	data = (unsigned char*) malloc(size);
	result->data = data;

	k = 0;
	for(j=0; j<result->height; j++) {
		bits = FreeImage_GetScanLine(bitmap,j);
			for(i=0; i<result->width; i++) {
				data[k++] = bits[FI_RGBA_RED];
				data[k++] = bits[FI_RGBA_GREEN];
				data[k++] = bits[FI_RGBA_BLUE];
				bits += 3;
			}
	}

	return(result);

}