#ifndef RAWTEXTURE_H
#define RAWTEXTURE_H

typedef struct {
	unsigned int 	 width;
	unsigned int 	 height;
	unsigned int 	 bytes_per_pixel; /* 3:RGB, 4:RGBA */
	unsigned char	*pixel_data;
} raw_texture;

//test textures
const raw_texture getAwesomeTexture();

#endif // RAWTEXTURE_H
