#ifndef RAWTEXTURE_H
#define RAWTEXTURE_H

#include <GLES/gl.h>
#include <GLES/glext.h>

typedef struct {
	GLsizei         width;
	GLsizei         height;
	GLenum          format;
	GLenum          type;
	unsigned char  *pixel_data;
} raw_texture;

const raw_texture *get_test_raw_texture_8888();
raw_texture* new_raw_texture_from_8888_to_any(const raw_texture* src, GLenum format, GLenum type);
void delete_raw_texture(raw_texture*);

#endif // RAWTEXTURE_H
