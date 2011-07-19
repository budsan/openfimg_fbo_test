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
 * $Id: importgl.h,v 1.4 2005/02/24 20:29:33 tonic Exp $
 * $Revision: 1.4 $
 */

#ifndef IMPORTGL_H_INCLUDED
#define IMPORTGL_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

#include <GLES/gl.h>
#include <GLES/glext.h>
#ifndef ANDROID_NDK
#include <GLES/egl.h>
#endif /* !ANDROID_NDK */

/* Use DISABLE_IMPORTGL if you want to link the OpenGL ES at
 * compile/link time and not import it dynamically runtime.
 */
#ifndef DISABLE_IMPORTGL


/* Dynamically fetches pointers to the egl & gl functions.
 * Should be called once on application initialization.
 * Returns non-zero on success and 0 on failure.
 */
extern int importGLInit();

/* Frees the handle to egl & gl functions library.
 */
extern void importGLDeinit();


#ifndef IMPORTGL_API
#define IMPORTGL_API extern
#endif
#ifndef IMPORTGL_FNPTRINIT
#define IMPORTGL_FNPTRINIT
#endif

#define FNDEF(retType, funcName, args) IMPORTGL_API retType (*funcPtr_##funcName) args IMPORTGL_FNPTRINIT

#ifndef ANDROID_NDK
FNDEF(EGLBoolean, eglChooseConfig, (EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config));
FNDEF(EGLContext, eglCreateContext, (EGLDisplay dpy, EGLConfig config, EGLContext share_list, const EGLint *attrib_list));
FNDEF(EGLSurface, eglCreateWindowSurface, (EGLDisplay dpy, EGLConfig config, NativeWindowType window, const EGLint *attrib_list));
FNDEF(EGLBoolean, eglDestroyContext, (EGLDisplay dpy, EGLContext ctx));
FNDEF(EGLBoolean, eglDestroySurface, (EGLDisplay dpy, EGLSurface surface));
FNDEF(EGLBoolean, eglGetConfigAttrib, (EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value));
FNDEF(EGLBoolean, eglGetConfigs, (EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config));
FNDEF(EGLDisplay, eglGetDisplay, (NativeDisplayType display));
FNDEF(EGLint, eglGetError, (void));
FNDEF(EGLBoolean, eglInitialize, (EGLDisplay dpy, EGLint *major, EGLint *minor));
FNDEF(EGLBoolean, eglMakeCurrent, (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx));
FNDEF(EGLBoolean, eglSwapBuffers, (EGLDisplay dpy, EGLSurface draw));
FNDEF(EGLBoolean, eglTerminate, (EGLDisplay dpy));
#endif /* !ANDROID_NDK */

FNDEF(void, glBlendFunc, (GLenum sfactor, GLenum dfactor));
FNDEF(void, glClear, (GLbitfield mask));
FNDEF(void, glClearColorx, (GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha));
FNDEF(void, glColor4x, (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha));
FNDEF(void, glColorPointer, (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer));
FNDEF(void, glDisable, (GLenum cap));
FNDEF(void, glDisableClientState, (GLenum array));
FNDEF(void, glDrawArrays, (GLenum mode, GLint first, GLsizei count));
FNDEF(void, glEnable, (GLenum cap));
FNDEF(void, glEnableClientState, (GLenum array));
FNDEF(void, glFrustumx, (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar));
FNDEF(GLenum, glGetError, (void));
FNDEF(void, glLightxv, (GLenum light, GLenum pname, const GLfixed *params));
FNDEF(void, glLoadIdentity, (void));
FNDEF(void, glMaterialx, (GLenum face, GLenum pname, GLfixed param));
FNDEF(void, glMaterialxv, (GLenum face, GLenum pname, const GLfixed *params));
FNDEF(void, glMatrixMode, (GLenum mode));
FNDEF(void, glMultMatrixx, (const GLfixed *m));
FNDEF(void, glNormalPointer, (GLenum type, GLsizei stride, const GLvoid *pointer));
FNDEF(void, glPopMatrix, (void));
FNDEF(void, glPushMatrix, (void));
FNDEF(void, glRotatex, (GLfixed angle, GLfixed x, GLfixed y, GLfixed z));
FNDEF(void, glScalex, (GLfixed x, GLfixed y, GLfixed z));
FNDEF(void, glShadeModel, (GLenum mode));
FNDEF(void, glTranslatex, (GLfixed x, GLfixed y, GLfixed z));
FNDEF(void, glVertexPointer, (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer));
FNDEF(void, glViewport, (GLint x, GLint y, GLsizei width, GLsizei height));

FNDEF(GLboolean, glIsRenderbufferOES,(GLuint renderbuffer));
FNDEF(void, glBindRenderbufferOES, (GLenum target, GLuint renderbuffer));
FNDEF(void, glDeleteRenderbuffersOES, (GLsizei n, const GLuint* renderbuffers));
FNDEF(void, glGenRenderbuffersOES, (GLsizei n, GLuint* renderbuffers));
FNDEF(void, glRenderbufferStorageOES, (GLenum target, GLenum internalformat, GLsizei width, GLsizei height));
FNDEF(void, glGetRenderbufferParameterivOES, (GLenum target, GLenum pname, GLint* params));
FNDEF(GLboolean, glIsFramebufferOES, (GLuint framebuffer));
FNDEF(void, glBindFramebufferOES, (GLenum target, GLuint framebuffer));
FNDEF(void, glDeleteFramebuffersOES, (GLsizei n, const GLuint* framebuffers));
FNDEF(void, glGenFramebuffersOES, (GLsizei n, GLuint* framebuffers));
FNDEF(GLenum, glCheckFramebufferStatusOES, (GLenum target));
FNDEF(void, glFramebufferRenderbufferOES, (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer));
FNDEF(void, glFramebufferTexture2DOES, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level));
FNDEF(void, glGetFramebufferAttachmentParameterivOES, (GLenum target, GLenum attachment, GLenum pname, GLint* params));
FNDEF(void, glGenerateMipmapOES, (GLenum target));



#undef FN
#define FNPTR(name) funcPtr_##name

#ifndef IMPORTGL_NO_FNPTR_DEFS

// Redirect egl* and gl* function calls to funcPtr_egl* and funcPtr_gl*.

#ifndef ANDROID_NDK
#define eglChooseConfig         FNPTR(eglChooseConfig)
#define eglCreateContext        FNPTR(eglCreateContext)
#define eglCreateWindowSurface  FNPTR(eglCreateWindowSurface)
#define eglDestroyContext       FNPTR(eglDestroyContext)
#define eglDestroySurface       FNPTR(eglDestroySurface)
#define eglGetConfigAttrib      FNPTR(eglGetConfigAttrib)
#define eglGetConfigs           FNPTR(eglGetConfigs)
#define eglGetDisplay           FNPTR(eglGetDisplay)
#define eglGetError             FNPTR(eglGetError)
#define eglInitialize           FNPTR(eglInitialize)
#define eglMakeCurrent          FNPTR(eglMakeCurrent)
#define eglSwapBuffers          FNPTR(eglSwapBuffers)
#define eglTerminate            FNPTR(eglTerminate)
#endif /* !ANDROID_NDK */

#define glBlendFunc             FNPTR(glBlendFunc)
#define glClear                 FNPTR(glClear)
#define glClearColorx           FNPTR(glClearColorx)
#define glColor4x               FNPTR(glColor4x)
#define glColorPointer          FNPTR(glColorPointer)
#define glDisable               FNPTR(glDisable)
#define glDisableClientState    FNPTR(glDisableClientState)
#define glDrawArrays            FNPTR(glDrawArrays)
#define glEnable                FNPTR(glEnable)
#define glEnableClientState     FNPTR(glEnableClientState)
#define glFrustumx              FNPTR(glFrustumx)
#define glGetError              FNPTR(glGetError)
#define glLightxv               FNPTR(glLightxv)
#define glLoadIdentity          FNPTR(glLoadIdentity)
#define glMaterialx             FNPTR(glMaterialx)
#define glMaterialxv            FNPTR(glMaterialxv)
#define glMatrixMode            FNPTR(glMatrixMode)
#define glMultMatrixx           FNPTR(glMultMatrixx)
#define glNormalPointer         FNPTR(glNormalPointer)
#define glPopMatrix             FNPTR(glPopMatrix)
#define glPushMatrix            FNPTR(glPushMatrix)
#define glRotatex               FNPTR(glRotatex)
#define glScalex                FNPTR(glScalex)
#define glShadeModel            FNPTR(glShadeModel)
#define glTranslatex            FNPTR(glTranslatex)
#define glVertexPointer         FNPTR(glVertexPointer)
#define glViewport              FNPTR(glViewport)

#define glIsRenderbufferOES                      FNPTR(glIsRenderbufferOES);
#define glBindRenderbufferOES                    FNPTR(glBindRenderbufferOES);
#define glDeleteRenderbuffersOES                 FNPTR(glDeleteRenderbuffersOES);
#define glGenRenderbuffersOES                    FNPTR(glGenRenderbuffersOES);
#define glRenderbufferStorageOES                 FNPTR(glRenderbufferStorageOES);
#define glGetRenderbufferParameterivOES          FNPTR(glGetRenderbufferParameterivOES);
#define glIsFramebufferOES                       FNPTR(glIsFramebufferOES);
#define glBindFramebufferOES                     FNPTR(glBindFramebufferOES);
#define glDeleteFramebuffersOES                  FNPTR(glDeleteFramebuffersOES);
#define glGenFramebuffersOES                     FNPTR(glGenFramebuffersOES);
#define glCheckFramebufferStatusOES              FNPTR(glCheckFramebufferStatusOES);
#define glFramebufferRenderbufferOES             FNPTR(glFramebufferRenderbufferOES);
#define glFramebufferTexture2DOES                FNPTR(glFramebufferTexture2DOES);
#define glGetFramebufferAttachmentParameterivOES FNPTR(glGetFramebufferAttachmentParameterivOES);
#define glGenerateMipmapOES                      FNPTR(glGenerateMipmapOES);


#endif // !IMPORTGL_NO_FNPTR_DEFS


#endif // !DISABLE_IMPORTGL


#ifdef __cplusplus
}
#endif


#endif // !IMPORTGL_H_INCLUDED
