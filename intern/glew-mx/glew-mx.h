/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2014 Blender Foundation.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): Jason Wilkins
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file glew-mx.h
 *  \ingroup glew-mx
 *
 * Support for GLEW Multiple rendering conteXts (MX)
 * Maintained as a Blender Library.
 *
 * Different rendering contexts may have different entry points
 * to extension functions of the same name.  So it can cause
 * problems if, for example, a second context uses a pointer to
 * say, ActiveTextureARB, that was queried from the first context.
 *
 * GLEW has basic support for multiple contexts by enabling WITH_GLEW_MX,
 * but it does not provide a full implementation.  This is because
 * there are too many questions about thread safety and memory
 * allocation that are up to the user of GLEW.
 *
 * This implementation is very basic and isn't thread safe.
 * For a single context the overhead should be
 * no more than using GLEW without WITH_GLEW_MX enabled.
 */

#ifndef __GLEW_MX_H__
#define __GLEW_MX_H__

#ifdef WITH_GLEW_MX
/* glew itself expects this */
#  define GLEW_MX 1
#  define glewGetContext() (&(_mx_context->glew_context))
#endif

#include <GL/glew.h>


#ifdef __cplusplus
extern "C" {
#endif

/* MXContext is used instead of GLEWContext directly so that
   extending what data is held by a context is easier.
 */
typedef struct MXContext {
#ifdef WITH_GLEW_MX
	GLEWContext glew_context;
#endif

	/** 
	 * The purpose of these function pointers is to smooth over the differences 
	 * in OpenGL and OpenGL ES and various extensions so that other
	 * code is not littered with preprocessor commands and runtime
	 * extension checks.
	 *
	 * It goes further than GLEW by consolidating equivalent functions
	 * under a single name.  An example would be Uniform1f and
	 * Uniform1fARB.  On the desktop one could fairly reliably use Uniform1fARB
	 * since the ARB extension version would be available on a system which
	 * supports GLSL.  However, OpenGL ES 2.0 has no need for that extension,
	 * so relying on Uniform1fARB does not work.
	 *
	 * This solves that problem by assigning any name an API might
	 * have to a single canonical name that can be used on any system.
	 */

	GLboolean multitexture;

	PFNGLACTIVETEXTUREPROC       ActiveTexture;
#ifdef WITH_GL_PROFILE_COMPAT
	PFNGLCLIENTACTIVETEXTUREPROC ClientActiveTexture;
	PFNGLMULTITEXCOORD1DPROC     MultiTexCoord1d;
	PFNGLMULTITEXCOORD1DVPROC    MultiTexCoord1dv;
	PFNGLMULTITEXCOORD1FPROC     MultiTexCoord1f;
	PFNGLMULTITEXCOORD1FVPROC    MultiTexCoord1fv;
	PFNGLMULTITEXCOORD1IPROC     MultiTexCoord1i;
	PFNGLMULTITEXCOORD1IVPROC    MultiTexCoord1iv;
	PFNGLMULTITEXCOORD1SPROC     MultiTexCoord1s;
	PFNGLMULTITEXCOORD1SVPROC    MultiTexCoord1sv;
	PFNGLMULTITEXCOORD2DPROC     MultiTexCoord2d;
	PFNGLMULTITEXCOORD2DVPROC    MultiTexCoord2dv;
	PFNGLMULTITEXCOORD2FPROC     MultiTexCoord2f;
	PFNGLMULTITEXCOORD2FVPROC    MultiTexCoord2fv;
	PFNGLMULTITEXCOORD2IPROC     MultiTexCoord2i;
	PFNGLMULTITEXCOORD2IVPROC    MultiTexCoord2iv;
	PFNGLMULTITEXCOORD2SPROC     MultiTexCoord2s;
	PFNGLMULTITEXCOORD2SVPROC    MultiTexCoord2sv;
	PFNGLMULTITEXCOORD3DPROC     MultiTexCoord3d;
	PFNGLMULTITEXCOORD3DVPROC    MultiTexCoord3dv;
	PFNGLMULTITEXCOORD3FPROC     MultiTexCoord3f;
	PFNGLMULTITEXCOORD3FVPROC    MultiTexCoord3fv;
	PFNGLMULTITEXCOORD3IPROC     MultiTexCoord3i;
	PFNGLMULTITEXCOORD3IVPROC    MultiTexCoord3iv;
	PFNGLMULTITEXCOORD3SPROC     MultiTexCoord3s;
	PFNGLMULTITEXCOORD3SVPROC    MultiTexCoord3sv;
	PFNGLMULTITEXCOORD4DPROC     MultiTexCoord4d;
	PFNGLMULTITEXCOORD4DVPROC    MultiTexCoord4dv;
	PFNGLMULTITEXCOORD4FPROC     MultiTexCoord4f;
	PFNGLMULTITEXCOORD4FVPROC    MultiTexCoord4fv;
	PFNGLMULTITEXCOORD4IPROC     MultiTexCoord4i;
	PFNGLMULTITEXCOORD4IVPROC    MultiTexCoord4iv;
	PFNGLMULTITEXCOORD4SPROC     MultiTexCoord4s;
	PFNGLMULTITEXCOORD4SVPROC    MultiTexCoord4sv;
#endif

	/* This is not actually the ARB_shader_objects interface,
	 * but refactored version that made it into OpenGL 2.0
	 * If ARB_shader_objects is all that is available then it
	 * gets mapped onto this new interface.
	 */
	GLboolean shader_objects;

	PFNGLATTACHSHADERPROC     AttachShader;
	PFNGLCREATESHADERPROC     CreateShader;
	PFNGLDELETESHADERPROC     DeleteShader;
	PFNGLSHADERSOURCEPROC     ShaderSource;
	PFNGLCOMPILESHADERPROC    CompileShader;
	PFNGLGETSHADERIVPROC      GetShaderiv;
	PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog;

	PFNGLCREATEPROGRAMPROC     CreateProgram;
	PFNGLDELETEPROGRAMPROC     DeleteProgram;
	PFNGLLINKPROGRAMPROC       LinkProgram;
	PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog;
	PFNGLUSEPROGRAMPROC        UseProgram;
	PFNGLVALIDATEPROGRAMPROC   ValidateProgram;

	PFNGLUNIFORM1IPROC Uniform1i;
	PFNGLUNIFORM1FPROC Uniform1f;

	PFNGLUNIFORM1IVPROC Uniform1iv;
	PFNGLUNIFORM2IVPROC Uniform2iv;
	PFNGLUNIFORM3IVPROC Uniform3iv;
	PFNGLUNIFORM4IVPROC Uniform4iv;

	PFNGLUNIFORM1FVPROC Uniform1fv;
	PFNGLUNIFORM2FVPROC Uniform2fv;
	PFNGLUNIFORM3FVPROC Uniform3fv;
	PFNGLUNIFORM4FVPROC Uniform4fv;

	PFNGLUNIFORMMATRIX3FVPROC UniformMatrix3fv;
	PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4fv;

	PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation;

	/* ARB_vertex_shader
	 */
	GLboolean vertex_shader;

	PFNGLGETATTRIBLOCATIONPROC  GetAttribLocation;
	PFNGLBINDATTRIBLOCATIONPROC BindAttribLocation;

	/* ARB_fragment_shader
	 * Does not add any new API entries.
	 */
	GLboolean fragment_shader;

	/* vertexattrib is the part of ARB_vertex_program made it into OpenGL 2.0 and OpenGL ES 2.0
	 * The parts used for compiling legacy assembly language style shaders is left out.
	 */
	GLboolean vertexattrib;

	PFNGLDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray;
	PFNGLENABLEVERTEXATTRIBARRAYPROC  EnableVertexAttribArray;
	PFNGLGETPROGRAMIVPROC             GetProgramiv;
	PFNGLVERTEXATTRIBPOINTERPROC      VertexAttribPointer;
#ifdef WITH_GL_PROFILE_COMPAT
	PFNGLVERTEXATTRIB1DPROC           VertexAttrib1d;
	PFNGLVERTEXATTRIB1DVPROC          VertexAttrib1dv;
	PFNGLVERTEXATTRIB1FPROC           VertexAttrib1f;
	PFNGLVERTEXATTRIB1FVPROC          VertexAttrib1fv;
	PFNGLVERTEXATTRIB1SPROC           VertexAttrib1s;
	PFNGLVERTEXATTRIB1SVPROC          VertexAttrib1sv;
	PFNGLVERTEXATTRIB2DPROC           VertexAttrib2d;
	PFNGLVERTEXATTRIB2DVPROC          VertexAttrib2dv;
	PFNGLVERTEXATTRIB2FPROC           VertexAttrib2f;
	PFNGLVERTEXATTRIB2FVPROC          VertexAttrib2fv;
	PFNGLVERTEXATTRIB2SPROC           VertexAttrib2s;
	PFNGLVERTEXATTRIB2SVPROC          VertexAttrib2sv;
	PFNGLVERTEXATTRIB3DPROC           VertexAttrib3d;
	PFNGLVERTEXATTRIB3DVPROC          VertexAttrib3dv;
	PFNGLVERTEXATTRIB3FPROC           VertexAttrib3f;
	PFNGLVERTEXATTRIB3FVPROC          VertexAttrib3fv;
	PFNGLVERTEXATTRIB3SPROC           VertexAttrib3s;
	PFNGLVERTEXATTRIB3SVPROC          VertexAttrib3sv;
	PFNGLVERTEXATTRIB4NBVPROC         VertexAttrib4Nbv;
	PFNGLVERTEXATTRIB4NIVPROC         VertexAttrib4Niv;
	PFNGLVERTEXATTRIB4NSVPROC         VertexAttrib4Nsv;
	PFNGLVERTEXATTRIB4NUBPROC         VertexAttrib4Nub;
	PFNGLVERTEXATTRIB4NUBVPROC        VertexAttrib4Nubv;
	PFNGLVERTEXATTRIB4NUIVPROC        VertexAttrib4Nuiv;
	PFNGLVERTEXATTRIB4NUSVPROC        VertexAttrib4Nusv;
	PFNGLVERTEXATTRIB4BVPROC          VertexAttrib4bv;
	PFNGLVERTEXATTRIB4DPROC           VertexAttrib4d;
	PFNGLVERTEXATTRIB4DVPROC          VertexAttrib4dv;
	PFNGLVERTEXATTRIB4FPROC           VertexAttrib4f;
	PFNGLVERTEXATTRIB4FVPROC          VertexAttrib4fv;
	PFNGLVERTEXATTRIB4IVPROC          VertexAttrib4iv;
	PFNGLVERTEXATTRIB4SPROC           VertexAttrib4s;
	PFNGLVERTEXATTRIB4SVPROC          VertexAttrib4sv;
	PFNGLVERTEXATTRIB4UBVPROC         VertexAttrib4ubv;
	PFNGLVERTEXATTRIB4UIVPROC         VertexAttrib4uiv;
	PFNGLVERTEXATTRIB4USVPROC         VertexAttrib4usv;
#endif

	/* ARB_framebuffer_object
	 */
	GLboolean framebuffer_object;

	PFNGLGENFRAMEBUFFERSPROC        GenFramebuffers;
	PFNGLBINDFRAMEBUFFERPROC        BindFramebuffer;
	PFNGLDELETEFRAMEBUFFERSPROC     DeleteFramebuffers;
	PFNGLFRAMEBUFFERTEXTURE2DPROC   FramebufferTexture2D;
	PFNGLGENERATEMIPMAPPROC         GenerateMipmap;
	PFNGLCHECKFRAMEBUFFERSTATUSPROC CheckFramebufferStatus;

	/* ARB_vertex_buffer_objects
	 */
	GLboolean vertex_buffer_objects;

	PFNGLGENBUFFERSPROC    GenBuffers;
	PFNGLBINDBUFFERPROC    BindBuffer;
	PFNGLDELETEBUFFERSPROC DeleteBuffers;

	PFNGLBUFFERDATAPROC    BufferData;
	PFNGLBUFFERSUBDATAPROC BufferSubData;

	/* mapbuffer is a part of ARB_vertex_buffer_object and OpenGL 2.0,
	 * but in OpenGL ES, MapBuffer and UnmapBuffer are extensions
	 */
	GLboolean mapbuffer; 

	PFNGLMAPBUFFERPROC   MapBuffer;
	PFNGLUNMAPBUFFERPROC UnmapBuffer;

	/* ARB_vertex_array_object
	 */
	GLboolean vertex_array_object;

	PFNGLGENVERTEXARRAYSPROC    GenVertexArrays;
	PFNGLBINDVERTEXARRAYPROC    BindVertexArray;
	PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;

	/* OpenGL 1.2
	 * EXT_texture3D
	 * OES_texture3D
	 */
	GLboolean texture3D;

	PFNGLTEXIMAGE3DPROC    TexImage3D;
	PFNGLTEXSUBIMAGE3DPROC TexSubImage3D;

	/* OpenGL 2.1
	 * ARB_pixel_buffer_object
	 * EXT_pixel_buffer_object
	 *  NV_pixel_buffer_object
	 */
	GLboolean pixel_buffer_object;

	/* OpenGL 3.1
	 * ARB_texture_rectangle
	 * EXT_texture_rectangle
	 *  NV_texture_rectangle
	 */
	GLboolean texture_rectangle;

	/* miscellaneous flags
	 */
	GLuint flags;

} MXContext;

#ifdef WITH_GLEW_MX
extern MXContext *_mx_context;
#define MX_CONTEXT(x) (_mx_context->x)
#else
extern MXContext _mx_context;
#define MX_CONTEXT(x) (_mx_context.x)
#endif


#include "intern/symbol-binding.h"


/* proc-binding.h declares a simplified GL interface.
 * If the original glew interface is needed then define MX_NO_PROC_BINDING.
 */
#ifndef MX_NO_PROC_BINDING
#include "intern/proc-binding.h"
#endif


/* If compiling only for OpenGL 3.2 Core Profile then we should make sure
 * no legacy API entries or symbolic constants are used.
 */
#if defined(WITH_GL_PROFILE_CORE) && !defined(WITH_GL_PROFILE_COMPAT) && !defined(WITH_GL_PROFILE_ES20)
#  include "intern/gl-deprecated.h"
#endif


void mx_glGenerateMipmap(GLenum target);


MXContext *mxCreateContext     (void);
MXContext *mxGetCurrentContext (void);
void       mxMakeCurrentContext(MXContext *ctx);
void       mxDestroyContext    (MXContext *ctx);


#ifdef __cplusplus
}
#endif


#endif  /* __GLEW_MX_H__ */
