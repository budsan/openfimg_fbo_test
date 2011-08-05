/* San Angeles Observation OpenGL ES version example
 * Copyright 2004-2005 Jetro Lauha
 * All rights reserved.
 * Web: http://iki.fi/jetro/
 *
 * This source is free software; you can redistribute it and/or
 * modify it under the terms of EITHER:
 *   (1) The GNU Lesser General Public License as published by the Free
 *       Software Foundation; either version 2.1 of the License, or (at
 *       your option) any later version. The text of the GNU Lesser
 *       General Public License is included with this source in the
 *       file LICENSE-LGPL.txt.
 *   (2) The BSD-style license that is included with this source in
 *       the file LICENSE-BSD.txt.
 *
 * This source is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files
 * LICENSE-LGPL.txt and LICENSE-BSD.txt for more details.
 *
 * $Id: demo.c,v 1.10 2005/02/08 20:54:39 tonic Exp $
 * $Revision: 1.10 $
 */

#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include "importgl.h"
#include "rawtexture.h"

#include "app.h"

#undef PI
#define PI 3.1415926535897932f
#define RANDOM_UINT_MAX 65535

static long floatToFixed(float value)
{
    if (value < -32768) value = -32768;
    if (value > 32767) value = 32767;
    return (long)(value * 65536);
}

#define FIXED(value) floatToFixed(value)

#define FBO_SIZE 128
#define TEX_SIZE (FBO_SIZE*FBO_SIZE*4)

static long sStartTick = 0;
static long sTick = 0;

static float xrot = 0.0f;
static float yrot = 0.0f;
static float zrot = 0.0f;

static GLuint tx_id = 0;
static GLuint fb_id = 0;
static GLuint rb_id = 0;

#define NUM_MODES 5

static struct {
	GLenum format;
	GLenum type;
} modes[NUM_MODES] = {
	//RGB
	GL_RGB , GL_UNSIGNED_BYTE,
	GL_RGB , GL_UNSIGNED_SHORT_5_6_5,
	//RGBA
	GL_RGBA, GL_UNSIGNED_BYTE,
	GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4,
	GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1
};

const char *formatToString(GLenum format)
{
	switch(format)
	{
	case GL_RGBA: return "GL_RGBA";
	case GL_RGB:  return "GL_RGB";
	default:      return "GL_NONE";
	}
}

const char *typeToString(GLenum type)
{
	switch(type)
	{
	case GL_UNSIGNED_BYTE:          return "GL_UNSIGNED_BYTE";
	case GL_UNSIGNED_SHORT_4_4_4_4: return "GL_UNSIGNED_SHORT_4_4_4_4";
	case GL_UNSIGNED_SHORT_5_5_5_1: return "GL_UNSIGNED_SHORT_5_5_5_1";
	case GL_UNSIGNED_SHORT_5_6_5:   return "GL_UNSIGNED_SHORT_5_6_5";
	default:                        return "GL_NONE";
	}
}

static unsigned char change_mode = 0;
static unsigned char use_mode = 0;
static unsigned char use_fbo  = 1;

void setupTexture();
void changeMode();

void appInit()
{
	//glClearDepthf(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
	//glDepthFunc(GL_LESS);

	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//initFunctionsPtr();

	//CREATING FRAMEBUFFER OBJECT
	glGenTextures(1, &tx_id);
	if (tx_id == 0) debug("NO glGenTextures");
	setupTexture();

	glGenRenderbuffersOES(1, &rb_id);
	if (rb_id == 0) debug("NO glGenRenderbuffersOES");
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, rb_id);
	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT24_OES, FBO_SIZE, FBO_SIZE);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, 0);

	glGenFramebuffersOES(1, &fb_id);
	if (fb_id == 0) debug("NO glGenRenderbuffersOES");
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, fb_id);
	glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, tx_id, 0);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, rb_id);

	GLenum status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
	if (status != GL_FRAMEBUFFER_COMPLETE_OES) {
		debug("NO GL_FRAMEBUFFER_COMPLETE_OES");
	}

	glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);
}

void setupTexture()
{
	glBindTexture(GL_TEXTURE_2D, tx_id);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (use_fbo)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, modes[use_mode].format, FBO_SIZE, FBO_SIZE,
			     0, modes[use_mode].format, modes[use_mode].type, NULL);
	}
	else
	{
		const raw_texture *awe = get_test_raw_texture_8888();
		raw_texture *tex = new_raw_texture_from_8888_to_any(awe, modes[use_mode].format, modes[use_mode].type);

		unsigned char *pixels = NULL;
		unsigned int witdh = 1, height = 1;
		if (tex != NULL) {
			witdh  = tex->width;
			height = tex->height;
			pixels = tex->pixel_data;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, modes[use_mode].format, witdh, height,
			     0, modes[use_mode].format, modes[use_mode].type, pixels);

		delete_raw_texture(tex);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void changeMode()
{
	use_mode = (use_mode+1) % NUM_MODES;
	if (use_mode == 0) use_fbo = !use_fbo;
	setupTexture();

	debug("fbo_%d, %s, %s", use_fbo,
	      formatToString(modes[use_mode].format), typeToString(modes[use_mode].type));
}

void appResize(int width, int height)
{

}

void appDeinit()
{

}

void appClicked()
{
	change_mode = 1;
}

static void gluPerspective(GLfloat fovy, GLfloat aspect,
                           GLfloat zNear, GLfloat zFar)
{
    GLfloat xmin, xmax, ymin, ymax;

    ymax = zNear * (GLfloat)tan(fovy * PI / 360);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    glFrustumx((GLfixed)(xmin * 65536), (GLfixed)(xmax * 65536),
               (GLfixed)(ymin * 65536), (GLfixed)(ymax * 65536),
               (GLfixed)(zNear * 65536), (GLfixed)(zFar * 65536));
}

/* Following gluLookAt implementation is adapted from the
 * Mesa 3D Graphics library. http://www.mesa3d.org
 */
static void gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
		      GLfloat centerx, GLfloat centery, GLfloat centerz,
		      GLfloat upx, GLfloat upy, GLfloat upz)
{
    GLfloat m[16];
    GLfloat x[3], y[3], z[3];
    GLfloat mag;

    /* Make rotation matrix */

    /* Z vector */
    z[0] = eyex - centerx;
    z[1] = eyey - centery;
    z[2] = eyez - centerz;
    mag = (float)sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) {			/* mpichler, 19950515 */
	z[0] /= mag;
	z[1] /= mag;
	z[2] /= mag;
    }

    /* Y vector */
    y[0] = upx;
    y[1] = upy;
    y[2] = upz;

    /* X vector = Y cross Z */
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];

    /* Recompute Y = Z cross X */
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];

    /* mpichler, 19950515 */
    /* cross product gives area of parallelogram, which is < 1.0 for
     * non-perpendicular unit-length vectors; so normalize x, y here
     */

    mag = (float)sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
	x[0] /= mag;
	x[1] /= mag;
	x[2] /= mag;
    }

    mag = (float)sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
	y[0] /= mag;
	y[1] /= mag;
	y[2] /= mag;
    }

#define M(row,col)  m[col*4+row]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0;
    M(3, 0) = 0.0;
    M(3, 1) = 0.0;
    M(3, 2) = 0.0;
    M(3, 3) = 1.0;
#undef M
    {
	int a;
	GLfixed fixedM[16];
	for (a = 0; a < 16; ++a)
	    fixedM[a] = (GLfixed)(m[a] * 65536);
	glMultMatrixx(fixedM);
    }

    /* Translate Eye to Origin */
    glTranslatex((GLfixed)(-eyex * 65536),
		 (GLfixed)(-eyey * 65536),
		 (GLfixed)(-eyez * 65536));
}

void drawPoly(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,20.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef( 0.0f, 0.0f,-5.0f);
	glRotatef(yrot*(PI/2.0f), 0, 0, 1);

	static float colors[] = {
		1.0f,0.0f,0.0f,1.0f,
		0.0f,1.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,1.0f,
	};

	static float vertices[] = {
		 0.0f, 1.0f, 0.0f,
		-1.0f,-1.0f, 0.0f,
		 1.0f,-1.0f, 0.0f
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(4, GL_FLOAT, 0, colors);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawCube(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.001f,100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float zfactor = sin((sTick*30.0f)*0.0001)*3.0f;
	glTranslatef(0.0f,0.0f,-6.0f+zfactor);
	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);
	glRotatef(zrot,0.0f,0.0f,1.0f);

	static float texCoords[] = {
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, // Front Face
		1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Back Face
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // Top Face
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom Face
		1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Right face
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Left Face
	};

	static float vertices[] = {
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, // Front Face
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f, // Back Face
		-1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f, // Top Face
		-1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, // Bottom Face
		 1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, // Right face
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f  // Left Face
	};

	static unsigned char indices [] = {
		 0, 1, 2, 0, 2, 3, // Front Face
		 4, 5, 6, 4, 6, 7, // Back Face
		 8, 9,10, 8,10,11, // Top Face
		12,13,14,12,14,15, // Bottom Face
		16,17,18,16,18,19, // Right face
		20,21,22,20,22,23  // Left Face
	};

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void appRender(long tick, int width, int height)
{
	if (sStartTick == 0)
		sStartTick = tick;
	if (!gAppAlive)
		return;

	if (change_mode) {
		change_mode = 0;
		changeMode();
	}

	sTick = (sTick + tick - sStartTick) >> 1;
	xrot = sin(sTick*2.0f*PI*0.0001f)*24.0f;
	yrot = (sTick*45)/1024;
	zrot = 0;

	if (use_fbo)
	{
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, fb_id);
		glViewport(0, 0, FBO_SIZE, FBO_SIZE);
		glViewport(0, 0, FBO_SIZE, FBO_SIZE);

		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		drawPoly(FBO_SIZE, FBO_SIZE);

		glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);
	}

	glViewport(0, 0, width, height);
	glViewport(0, 0, width, height);

	glEnable(GL_TEXTURE_2D);
	glClearColor(0.4f, 0.4f, 0.8f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, tx_id);
	//glGenerateMipmapOES(GL_TEXTURE_2D);

	drawCube(width, height);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}
