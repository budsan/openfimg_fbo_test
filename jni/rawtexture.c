#include "rawtexture.h"

#include <stdlib.h>
#include <string.h>

typedef unsigned int   pixel_8888;
typedef unsigned int   pixel_8880;
typedef unsigned short pixel_4444;
typedef unsigned short pixel_5551;
typedef unsigned short pixel_565;

   typedef void(*pixcpy)(void *dst, const void *src, unsigned int pixels);
void pixcpy_8888_to_8888(void *dst, const void *src, unsigned int pixels);
void pixcpy_8888_to_8880(void *dst, const void *src, unsigned int pixels);
void pixcpy_8888_to_4444(void *dst, const void *src, unsigned int pixels);
void pixcpy_8888_to_5551(void *dst, const void *src, unsigned int pixels);
void pixcpy_8888_to_565 (void *dst, const void *src, unsigned int pixels);

raw_texture* new_raw_texture_from_8888_to_any(const raw_texture* src, GLenum format, GLenum type)
{
	if (src->format != GL_RGBA && src->type != GL_UNSIGNED_BYTE) return 0;

	unsigned int bpp;
	pixcpy pixcpy_func;

	switch (type) {
	case GL_UNSIGNED_BYTE:
		switch (format) {
		case GL_RGB:
			bpp = sizeof(pixel_8880);
			pixcpy_func = pixcpy_8888_to_8880;
			break;
		case GL_RGBA:
			bpp = sizeof(pixel_8888);
			pixcpy_func = pixcpy_8888_to_8888;
			break;
		default:
			return 0;
		}
		break;
	case GL_UNSIGNED_SHORT_5_6_5:
		if (format != GL_RGB) return 0;
		bpp = sizeof(pixel_565);
		pixcpy_func = pixcpy_8888_to_565;
		break;
	case GL_UNSIGNED_SHORT_4_4_4_4:
		if (format != GL_RGBA) return 0;
		bpp = sizeof(pixel_4444);
		pixcpy_func = pixcpy_8888_to_4444;
		break;
	case GL_UNSIGNED_SHORT_5_5_5_1:
		if (format != GL_RGBA) return 0;
		bpp = sizeof(pixel_5551);
		pixcpy_func = pixcpy_8888_to_5551;
		break;
	default:
		return 0;
	}

	unsigned int pixels = src->width * src->height;
	raw_texture* dst = (raw_texture*)malloc(sizeof(raw_texture));

	dst->format = format;
	dst->type   = type;
	dst->width  = src->width;
	dst->height = src->height;
	dst->pixel_data = (unsigned char*)malloc(pixels*bpp);

	pixcpy_func(dst->pixel_data, src->pixel_data, pixels);
	return dst;
}

void delete_raw_texture(raw_texture* tex)
{
	if (tex != NULL) free(tex);
}

/**
 * Pixelcopy functions
 */

#define RED_8888 0xFF000000
#define GRE_8888 0x00FF0000
#define BLU_8888 0x0000FF00
#define ALP_8888 0x000000FF

#define RED_4444 0xF000
#define GRE_4444 0x0F00
#define BLU_4444 0x00F0
#define ALP_4444 0x000F

#define RED_5551 0xF800
#define GRE_5551 0x07C0
#define BLU_5551 0x003E
#define ALP_5551 0x0001

#define RED_565 0xF800
#define GRE_565 0x07E0
#define BLU_565 0x001F

void pixcpy_8888_to_8888(void *dst, const void *src, unsigned int pixels)
{
	pixel_8888 *s = (pixel_8888 *)src;
	pixel_8888 *d = (pixel_8888 *)dst;

	while(pixels--) *d++ = *s++;
}

void pixcpy_8888_to_8880(void *dst, const void *src, unsigned int pixels)
{
	pixel_8888 *s = (pixel_8888 *)src;
	pixel_8888 *d = (pixel_8888 *)dst;

	while(pixels--) *d++ = *s++;
}

void pixcpy_8888_to_4444(void *dst, const void *src, unsigned int pixels)
{
	pixel_8888 *s = (pixel_8888 *)src;
	pixel_4444 *d = (pixel_4444 *)dst;

	while(pixels--)
	{
		*d = (((*s)&(RED_4444<<16))>>16)|
		     (((*s)&(GRE_4444<<12))>>12)|
		     (((*s)&(BLU_4444<< 8))>> 8)|
		     (((*s)&(ALP_4444<< 4))>> 4);
		d++; s++;
	}
}

void pixcpy_8888_to_5551(void *dst, const void *src, unsigned int pixels)
{
	pixel_8888 *s = (pixel_8888 *)src;
	pixel_5551 *d = (pixel_5551 *)dst;

	while(pixels--)
	{
		*d = (((*s)&(RED_5551<<16))>>16)|
		     (((*s)&(GRE_5551<<13))>>13)|
		     (((*s)&(BLU_5551<<10))>>10)|
		     (ALP_5551&((((*s)&ALP_8888)!=ALP_8888)-1));
		d++; s++;
	}
}

void pixcpy_8888_to_565(void *dst, const void *src, unsigned int pixels)
{
	pixel_8888 *s = (pixel_8888 *)src;
	pixel_565  *d = (pixel_565  *)dst;

	while(pixels--)
	{
		*d = (((*s)&(RED_565<<16))>>16)|
		     (((*s)&(GRE_565<<13))>>13)|
		     (((*s)&(BLU_565<<11))>>11);
		d++; s++;
	}
}

