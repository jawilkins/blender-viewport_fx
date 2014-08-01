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

/** \file intern/proc-binding.c
 *  \ingroup glew-mx
 */

#define MX_NO_PROC_BINDING
#include "glew-mx.h"

#include <assert.h>
#include <string.h>


static GLboolean mx_init_multitexture()
{
	if (GLEW_VERSION_1_3 || GLEW_ES_VERSION_2_0) {
		MX_CONTEXT(ActiveTexture      ) = glActiveTexture;
		MX_CONTEXT(ClientActiveTexture) = glClientActiveTexture;
		MX_CONTEXT(MultiTexCoord1d    ) = glMultiTexCoord1d;
		MX_CONTEXT(MultiTexCoord1dv   ) = glMultiTexCoord1dv;
		MX_CONTEXT(MultiTexCoord1f    ) = glMultiTexCoord1f;
		MX_CONTEXT(MultiTexCoord1fv   ) = glMultiTexCoord1fv;
		MX_CONTEXT(MultiTexCoord1i    ) = glMultiTexCoord1i;
		MX_CONTEXT(MultiTexCoord1iv   ) = glMultiTexCoord1iv;
		MX_CONTEXT(MultiTexCoord1s    ) = glMultiTexCoord1s;
		MX_CONTEXT(MultiTexCoord1sv   ) = glMultiTexCoord1sv;
		MX_CONTEXT(MultiTexCoord2d    ) = glMultiTexCoord2d;
		MX_CONTEXT(MultiTexCoord2dv   ) = glMultiTexCoord2dv;
		MX_CONTEXT(MultiTexCoord2f    ) = glMultiTexCoord2f;
		MX_CONTEXT(MultiTexCoord2fv   ) = glMultiTexCoord2fv;
		MX_CONTEXT(MultiTexCoord2i    ) = glMultiTexCoord2i;
		MX_CONTEXT(MultiTexCoord2iv   ) = glMultiTexCoord2iv;
		MX_CONTEXT(MultiTexCoord2s    ) = glMultiTexCoord2s;
		MX_CONTEXT(MultiTexCoord2sv   ) = glMultiTexCoord2sv;
		MX_CONTEXT(MultiTexCoord3d    ) = glMultiTexCoord3d;
		MX_CONTEXT(MultiTexCoord3dv   ) = glMultiTexCoord3dv;
		MX_CONTEXT(MultiTexCoord3f    ) = glMultiTexCoord3f;
		MX_CONTEXT(MultiTexCoord3fv   ) = glMultiTexCoord3fv;
		MX_CONTEXT(MultiTexCoord3i    ) = glMultiTexCoord3i;
		MX_CONTEXT(MultiTexCoord3iv   ) = glMultiTexCoord3iv;
		MX_CONTEXT(MultiTexCoord3s    ) = glMultiTexCoord3s;
		MX_CONTEXT(MultiTexCoord3sv   ) = glMultiTexCoord3sv;
		MX_CONTEXT(MultiTexCoord4d    ) = glMultiTexCoord4d;
		MX_CONTEXT(MultiTexCoord4dv   ) = glMultiTexCoord4dv;
		MX_CONTEXT(MultiTexCoord4f    ) = glMultiTexCoord4f;
		MX_CONTEXT(MultiTexCoord4fv   ) = glMultiTexCoord4fv;
		MX_CONTEXT(MultiTexCoord4i    ) = glMultiTexCoord4i;
		MX_CONTEXT(MultiTexCoord4iv   ) = glMultiTexCoord4iv;
		MX_CONTEXT(MultiTexCoord4s    ) = glMultiTexCoord4s;
		MX_CONTEXT(MultiTexCoord4sv   ) = glMultiTexCoord4sv;

		return GL_TRUE;
	}

#ifndef GLEW_ES_ONLY
	if (GLEW_ARB_multitexture) {
		MX_CONTEXT(ActiveTexture      ) = glActiveTextureARB;
		MX_CONTEXT(ClientActiveTexture) = glClientActiveTextureARB;
		MX_CONTEXT(MultiTexCoord1d    ) = glMultiTexCoord1dARB;
		MX_CONTEXT(MultiTexCoord1dv   ) = glMultiTexCoord1dvARB;
		MX_CONTEXT(MultiTexCoord1f    ) = glMultiTexCoord1fARB;
		MX_CONTEXT(MultiTexCoord1fv   ) = glMultiTexCoord1fvARB;
		MX_CONTEXT(MultiTexCoord1i    ) = glMultiTexCoord1iARB;
		MX_CONTEXT(MultiTexCoord1iv   ) = glMultiTexCoord1ivARB;
		MX_CONTEXT(MultiTexCoord1s    ) = glMultiTexCoord1sARB;
		MX_CONTEXT(MultiTexCoord1sv   ) = glMultiTexCoord1svARB;
		MX_CONTEXT(MultiTexCoord2d    ) = glMultiTexCoord2dARB;
		MX_CONTEXT(MultiTexCoord2dv   ) = glMultiTexCoord2dvARB;
		MX_CONTEXT(MultiTexCoord2f    ) = glMultiTexCoord2fARB;
		MX_CONTEXT(MultiTexCoord2fv   ) = glMultiTexCoord2fvARB;
		MX_CONTEXT(MultiTexCoord2i    ) = glMultiTexCoord2iARB;
		MX_CONTEXT(MultiTexCoord2iv   ) = glMultiTexCoord2ivARB;
		MX_CONTEXT(MultiTexCoord2s    ) = glMultiTexCoord2sARB;
		MX_CONTEXT(MultiTexCoord2sv   ) = glMultiTexCoord2svARB;
		MX_CONTEXT(MultiTexCoord3d    ) = glMultiTexCoord3dARB;
		MX_CONTEXT(MultiTexCoord3dv   ) = glMultiTexCoord3dvARB;
		MX_CONTEXT(MultiTexCoord3f    ) = glMultiTexCoord3fARB;
		MX_CONTEXT(MultiTexCoord3fv   ) = glMultiTexCoord3fvARB;
		MX_CONTEXT(MultiTexCoord3i    ) = glMultiTexCoord3iARB;
		MX_CONTEXT(MultiTexCoord3iv   ) = glMultiTexCoord3ivARB;
		MX_CONTEXT(MultiTexCoord3s    ) = glMultiTexCoord3sARB;
		MX_CONTEXT(MultiTexCoord3sv   ) = glMultiTexCoord3svARB;
		MX_CONTEXT(MultiTexCoord4d    ) = glMultiTexCoord4dARB;
		MX_CONTEXT(MultiTexCoord4dv   ) = glMultiTexCoord4dvARB;
		MX_CONTEXT(MultiTexCoord4f    ) = glMultiTexCoord4fARB;
		MX_CONTEXT(MultiTexCoord4fv   ) = glMultiTexCoord4fvARB;
		MX_CONTEXT(MultiTexCoord4i    ) = glMultiTexCoord4iARB;
		MX_CONTEXT(MultiTexCoord4iv   ) = glMultiTexCoord4ivARB;
		MX_CONTEXT(MultiTexCoord4s    ) = glMultiTexCoord4sARB;
		MX_CONTEXT(MultiTexCoord4sv   ) = glMultiTexCoord4svARB;

		return GL_TRUE;
	}
#endif

	return GL_FALSE;
}


static GLboolean mx_init_shader_objects(void)
{
	if (GLEW_VERSION_2_0 || GLEW_ES_VERSION_2_0) {
		MX_CONTEXT(AttachShader      ) = glAttachShader;
		MX_CONTEXT(CompileShader     ) = glCompileShader;
		MX_CONTEXT(CreateProgram     ) = glCreateProgram;
		MX_CONTEXT(CreateShader      ) = glCreateShader;
		MX_CONTEXT(DeleteProgram     ) = glDeleteProgram;
		MX_CONTEXT(DeleteShader      ) = glDeleteShader;
		MX_CONTEXT(GetProgramInfoLog ) = glGetProgramInfoLog;
		MX_CONTEXT(GetShaderiv       ) = glGetShaderiv;
		MX_CONTEXT(GetShaderInfoLog  ) = glGetShaderInfoLog;
		MX_CONTEXT(GetUniformLocation) = glGetUniformLocation;
		MX_CONTEXT(LinkProgram       ) = glLinkProgram;
		MX_CONTEXT(ShaderSource      ) = glShaderSource;
		MX_CONTEXT(Uniform1i         ) = glUniform1i;
		MX_CONTEXT(Uniform1f         ) = glUniform1f;
		MX_CONTEXT(Uniform1iv        ) = glUniform1iv;
		MX_CONTEXT(Uniform2iv        ) = glUniform2iv;
		MX_CONTEXT(Uniform3iv        ) = glUniform3iv;
		MX_CONTEXT(Uniform4iv        ) = glUniform4iv;
		MX_CONTEXT(Uniform1fv        ) = glUniform1fv;
		MX_CONTEXT(Uniform2fv        ) = glUniform2fv;
		MX_CONTEXT(Uniform3fv        ) = glUniform3fv;
		MX_CONTEXT(Uniform4fv        ) = glUniform4fv;
		MX_CONTEXT(UniformMatrix3fv  ) = glUniformMatrix3fv;
		MX_CONTEXT(UniformMatrix4fv  ) = glUniformMatrix4fv;
		MX_CONTEXT(UseProgram        ) = glUseProgram;
		MX_CONTEXT(ValidateProgram   ) = glValidateProgram;

		return GL_TRUE;
	}

#ifndef GLEW_ES_ONLY
	if (GLEW_ARB_shader_objects) {
		MX_CONTEXT(AttachShader      ) = glAttachObjectARB;
		MX_CONTEXT(CompileShader     ) = glCompileShaderARB;
		MX_CONTEXT(CreateProgram     ) = glCreateProgramObjectARB;
		MX_CONTEXT(CreateShader      ) = glCreateShaderObjectARB;
		MX_CONTEXT(DeleteProgram     ) = glDeleteObjectARB;
		MX_CONTEXT(DeleteShader      ) = glDeleteObjectARB;
		MX_CONTEXT(GetProgramInfoLog ) = glGetInfoLogARB;
		MX_CONTEXT(GetShaderiv       ) = glGetObjectParameterivARB;
		MX_CONTEXT(GetShaderInfoLog  ) = glGetInfoLogARB;
		MX_CONTEXT(GetUniformLocation) = glGetUniformLocationARB;
		MX_CONTEXT(LinkProgram       ) = glLinkProgramARB;
		MX_CONTEXT(ShaderSource      ) = glShaderSourceARB;
		MX_CONTEXT(Uniform1i         ) = glUniform1iARB;
		MX_CONTEXT(Uniform1f         ) = glUniform1fARB;
		MX_CONTEXT(Uniform1iv        ) = glUniform1ivARB;
		MX_CONTEXT(Uniform2iv        ) = glUniform2ivARB;
		MX_CONTEXT(Uniform3iv        ) = glUniform3ivARB;
		MX_CONTEXT(Uniform4iv        ) = glUniform4ivARB;
		MX_CONTEXT(Uniform1fv        ) = glUniform1fvARB;
		MX_CONTEXT(Uniform2fv        ) = glUniform2fvARB;
		MX_CONTEXT(Uniform3fv        ) = glUniform3fvARB;
		MX_CONTEXT(Uniform4fv        ) = glUniform4fvARB;
		MX_CONTEXT(UniformMatrix3fv  ) = glUniformMatrix3fvARB;
		MX_CONTEXT(UniformMatrix4fv  ) = glUniformMatrix4fvARB;
		MX_CONTEXT(UseProgram        ) = glUseProgramObjectARB;
		MX_CONTEXT(ValidateProgram   ) = glValidateProgramARB;

		return GL_TRUE;
	}
#endif

	return GL_FALSE;
}


static GLboolean mx_init_vertex_shader(void)
{
	if (GLEW_VERSION_2_0 || GLEW_ES_VERSION_2_0) {
		MX_CONTEXT(GetAttribLocation  ) = glGetAttribLocation;
		MX_CONTEXT(BindAttribLocation ) = glBindAttribLocation;

		return GL_TRUE;
	}

#if !defined(GLEW_ES_ONLY)
	if (GLEW_ARB_vertex_shader) {
		MX_CONTEXT(BindAttribLocation) = (PFNGLBINDATTRIBLOCATIONPROC)glBindAttribLocationARB;
		MX_CONTEXT(GetAttribLocation ) = glGetAttribLocationARB;

		return GL_TRUE;
	}
#endif

	return GL_FALSE;
}


static GLboolean mx_init_vertexattrib(void)
{
	if (GLEW_VERSION_2_0 || GLEW_ES_VERSION_2_0) {
		MX_CONTEXT(DisableVertexAttribArray) = glDisableVertexAttribArray;
		MX_CONTEXT(EnableVertexAttribArray ) = glEnableVertexAttribArray;
		MX_CONTEXT(GetProgramiv            ) = glGetProgramiv;
		MX_CONTEXT(VertexAttribPointer     ) = glVertexAttribPointer;
#ifdef WITH_GL_PROFILE_COMPAT
		MX_CONTEXT(VertexAttrib1d          ) = glVertexAttrib1d;
		MX_CONTEXT(VertexAttrib1dv         ) = glVertexAttrib1dv;
		MX_CONTEXT(VertexAttrib1f          ) = glVertexAttrib1f;
		MX_CONTEXT(VertexAttrib1fv         ) = glVertexAttrib1fv;
		MX_CONTEXT(VertexAttrib1s          ) = glVertexAttrib1s;
		MX_CONTEXT(VertexAttrib1sv         ) = glVertexAttrib1sv;
		MX_CONTEXT(VertexAttrib2d          ) = glVertexAttrib2d;
		MX_CONTEXT(VertexAttrib2dv         ) = glVertexAttrib2dv;
		MX_CONTEXT(VertexAttrib2f          ) = glVertexAttrib2f;
		MX_CONTEXT(VertexAttrib2fv         ) = glVertexAttrib2fv;
		MX_CONTEXT(VertexAttrib2s          ) = glVertexAttrib2s;
		MX_CONTEXT(VertexAttrib2sv         ) = glVertexAttrib2sv;
		MX_CONTEXT(VertexAttrib3d          ) = glVertexAttrib3d;
		MX_CONTEXT(VertexAttrib3dv         ) = glVertexAttrib3dv;
		MX_CONTEXT(VertexAttrib3f          ) = glVertexAttrib3f;
		MX_CONTEXT(VertexAttrib3fv         ) = glVertexAttrib3fv;
		MX_CONTEXT(VertexAttrib3s          ) = glVertexAttrib3s;
		MX_CONTEXT(VertexAttrib3sv         ) = glVertexAttrib3sv;
		MX_CONTEXT(VertexAttrib4Nbv        ) = glVertexAttrib4Nbv;
		MX_CONTEXT(VertexAttrib4Niv        ) = glVertexAttrib4Niv;
		MX_CONTEXT(VertexAttrib4Nsv        ) = glVertexAttrib4Nsv;
		MX_CONTEXT(VertexAttrib4Nub        ) = glVertexAttrib4Nub;
		MX_CONTEXT(VertexAttrib4Nubv       ) = glVertexAttrib4Nubv;
		MX_CONTEXT(VertexAttrib4Nuiv       ) = glVertexAttrib4Nuiv;
		MX_CONTEXT(VertexAttrib4Nusv       ) = glVertexAttrib4Nusv;
		MX_CONTEXT(VertexAttrib4bv         ) = glVertexAttrib4bv;
		MX_CONTEXT(VertexAttrib4d          ) = glVertexAttrib4d;
		MX_CONTEXT(VertexAttrib4dv         ) = glVertexAttrib4dv;
		MX_CONTEXT(VertexAttrib4f          ) = glVertexAttrib4f;
		MX_CONTEXT(VertexAttrib4fv         ) = glVertexAttrib4fv;
		MX_CONTEXT(VertexAttrib4iv         ) = glVertexAttrib4iv;
		MX_CONTEXT(VertexAttrib4s          ) = glVertexAttrib4s;
		MX_CONTEXT(VertexAttrib4sv         ) = glVertexAttrib4sv;
		MX_CONTEXT(VertexAttrib4ubv        ) = glVertexAttrib4ubv;
		MX_CONTEXT(VertexAttrib4uiv        ) = glVertexAttrib4uiv;
		MX_CONTEXT(VertexAttrib4usv        ) = glVertexAttrib4usv;
#endif

		return GL_TRUE;
	}

#ifndef GLEW_ES_ONLY
	if (GLEW_ARB_vertex_program) {
		MX_CONTEXT(DisableVertexAttribArray) = glDisableVertexAttribArrayARB;
		MX_CONTEXT(EnableVertexAttribArray ) = glEnableVertexAttribArrayARB;
		MX_CONTEXT(GetProgramiv            ) = glGetProgramivARB;
		MX_CONTEXT(VertexAttribPointer     ) = glVertexAttribPointerARB;
#  ifdef WITH_GL_PROFILE_COMPAT
		MX_CONTEXT(VertexAttrib1d          ) = glVertexAttrib1dARB;
		MX_CONTEXT(VertexAttrib1dv         ) = glVertexAttrib1dvARB;
		MX_CONTEXT(VertexAttrib1f          ) = glVertexAttrib1fARB;
		MX_CONTEXT(VertexAttrib1fv         ) = glVertexAttrib1fvARB;
		MX_CONTEXT(VertexAttrib1s          ) = glVertexAttrib1sARB;
		MX_CONTEXT(VertexAttrib1sv         ) = glVertexAttrib1svARB;
		MX_CONTEXT(VertexAttrib2d          ) = glVertexAttrib2dARB;
		MX_CONTEXT(VertexAttrib2dv         ) = glVertexAttrib2dvARB;
		MX_CONTEXT(VertexAttrib2f          ) = glVertexAttrib2fARB;
		MX_CONTEXT(VertexAttrib2fv         ) = glVertexAttrib2fvARB;
		MX_CONTEXT(VertexAttrib2s          ) = glVertexAttrib2sARB;
		MX_CONTEXT(VertexAttrib2sv         ) = glVertexAttrib2svARB;
		MX_CONTEXT(VertexAttrib3d          ) = glVertexAttrib3dARB;
		MX_CONTEXT(VertexAttrib3dv         ) = glVertexAttrib3dvARB;
		MX_CONTEXT(VertexAttrib3f          ) = glVertexAttrib3fARB;
		MX_CONTEXT(VertexAttrib3fv         ) = glVertexAttrib3fvARB;
		MX_CONTEXT(VertexAttrib3s          ) = glVertexAttrib3sARB;
		MX_CONTEXT(VertexAttrib3sv         ) = glVertexAttrib3svARB;
		MX_CONTEXT(VertexAttrib4Nbv        ) = glVertexAttrib4NbvARB;
		MX_CONTEXT(VertexAttrib4Niv        ) = glVertexAttrib4NivARB;
		MX_CONTEXT(VertexAttrib4Nsv        ) = glVertexAttrib4NsvARB;
		MX_CONTEXT(VertexAttrib4Nub        ) = glVertexAttrib4NubARB;
		MX_CONTEXT(VertexAttrib4Nubv       ) = glVertexAttrib4NubvARB;
		MX_CONTEXT(VertexAttrib4Nuiv       ) = glVertexAttrib4NuivARB;
		MX_CONTEXT(VertexAttrib4Nusv       ) = glVertexAttrib4NusvARB;
		MX_CONTEXT(VertexAttrib4bv         ) = glVertexAttrib4bvARB;
		MX_CONTEXT(VertexAttrib4d          ) = glVertexAttrib4dARB;
		MX_CONTEXT(VertexAttrib4dv         ) = glVertexAttrib4dvARB;
		MX_CONTEXT(VertexAttrib4f          ) = glVertexAttrib4fARB;
		MX_CONTEXT(VertexAttrib4fv         ) = glVertexAttrib4fvARB;
		MX_CONTEXT(VertexAttrib4iv         ) = glVertexAttrib4ivARB;
		MX_CONTEXT(VertexAttrib4s          ) = glVertexAttrib4sARB;
		MX_CONTEXT(VertexAttrib4sv         ) = glVertexAttrib4svARB;
		MX_CONTEXT(VertexAttrib4ubv        ) = glVertexAttrib4ubvARB;
		MX_CONTEXT(VertexAttrib4uiv        ) = glVertexAttrib4uivARB;
		MX_CONTEXT(VertexAttrib4usv        ) = glVertexAttrib4usvARB;
#  endif

		return GL_TRUE;
	}
#endif

	return GL_FALSE;
}


static GLboolean mx_init_fragment_shader(void)
{
	if (GLEW_VERSION_2_0 || GLEW_ES_VERSION_2_0) {
		return GL_TRUE;
	}

#ifndef GLEW_ES_ONLY
	if (GLEW_ARB_fragment_shader) {
		return GL_TRUE;
	}
#endif

	return GL_FALSE;
}


static GLboolean mx_init_framebuffer_object(void)
{
	if (GLEW_VERSION_3_0 || GLEW_ES_VERSION_2_0 || GLEW_ARB_framebuffer_object) {
		MX_CONTEXT(GenFramebuffers       ) = glGenFramebuffers;
		MX_CONTEXT(BindFramebuffer       ) = glBindFramebuffer;
		MX_CONTEXT(DeleteFramebuffers    ) = glDeleteFramebuffers;
		MX_CONTEXT(FramebufferTexture2D  ) = glFramebufferTexture2D;
		MX_CONTEXT(GenerateMipmap        ) = glGenerateMipmapEXT;
		MX_CONTEXT(CheckFramebufferStatus) = glCheckFramebufferStatus;

		return GL_TRUE;
	}

#ifndef GLEW_ES_ONLY
	if (GLEW_EXT_framebuffer_object) {
		MX_CONTEXT(GenFramebuffers       ) = glGenFramebuffersEXT;
		MX_CONTEXT(BindFramebuffer       ) = glBindFramebufferEXT;
		MX_CONTEXT(DeleteFramebuffers    ) = glDeleteFramebuffersEXT;
		MX_CONTEXT(FramebufferTexture2D  ) = glFramebufferTexture2DEXT;
		MX_CONTEXT(GenerateMipmap        ) = glGenerateMipmapEXT;
		MX_CONTEXT(CheckFramebufferStatus) = glCheckFramebufferStatusEXT;

		return GL_TRUE;
	}
#endif

#ifndef GLEW_NO_ES
	if (GLEW_OES_framebuffer_object) {
		MX_CONTEXT(GenFramebuffers       ) = glGenFramebuffersOES;
		MX_CONTEXT(BindFramebuffer       ) = glBindFramebufferOES;
		MX_CONTEXT(DeleteFramebuffers    ) = glDeleteFramebuffersOES;
		MX_CONTEXT(FramebufferTexture2D  ) = glFramebufferTexture2DOES;
		MX_CONTEXT(GenerateMipmap        ) = glGenerateMipmapOES;
		MX_CONTEXT(CheckFramebufferStatus) = glCheckFramebufferStatusOES;

		return GL_TRUE;
	}
#endif

	return GL_FALSE;
}


static GLboolean mx_init_vertex_array_object(void)
{
#ifndef GLEW_ES_ONLY
	if (GLEW_VERSION_3_0 || GLEW_ARB_vertex_array_object) {
		MX_CONTEXT(GenVertexArrays   ) = glGenVertexArrays;
		MX_CONTEXT(BindVertexArray   ) = glBindVertexArray;
		MX_CONTEXT(DeleteVertexArrays) = glDeleteVertexArrays;

		return GL_TRUE;
	}
#endif

#ifndef GLEW_NO_ES
	if (GLEW_OES_vertex_array_object) {
		MX_CONTEXT(GenVertexArrays   ) = glGenVertexArraysOES;
		MX_CONTEXT(BindVertexArray   ) = glBindVertexArrayOES;
		MX_CONTEXT(DeleteVertexArrays) = glDeleteVertexArraysOES;

		return GL_TRUE;
	}
#endif

	return GL_FALSE;
}


static GLboolean mx_init_texture3D(void)
{
	if (GLEW_VERSION_1_2) {
		MX_CONTEXT(TexImage3D   ) = glTexImage3D;
		MX_CONTEXT(TexSubImage3D) = glTexSubImage3D;

		return GL_TRUE;
	}

#ifndef GLEW_ES_ONLY
	if (GLEW_EXT_texture3D) {
		MX_CONTEXT(TexImage3D   ) = glTexImage3DEXT;
		MX_CONTEXT(TexSubImage3D) = glTexSubImage3DEXT;

		return GL_TRUE;
	}
#endif

#ifndef GLEW_NO_ES
	if (GLEW_OES_texture3D) {
		MX_CONTEXT(TexImage3D   ) = glTexImage3DOES;
		MX_CONTEXT(TexSubImage3D) = glTexSubImage3DOES;

		return GL_TRUE;
	}
#endif

	return GL_FALSE;
}


static GLboolean mx_init_vertex_buffer_objects(void)
{
	if (GLEW_VERSION_1_5 || GLEW_ES_VERSION_2_0) {
		MX_CONTEXT(BindBuffer   ) = glBindBuffer;
		MX_CONTEXT(BufferData   ) = glBufferData;
		MX_CONTEXT(BufferSubData) = glBufferSubData;
		MX_CONTEXT(DeleteBuffers) = glDeleteBuffers;
		MX_CONTEXT(GenBuffers   ) = glGenBuffers;

		return GL_TRUE;
	}

#ifndef GLEW_ES_ONLY
	if (GLEW_ARB_vertex_buffer_object) {
		MX_CONTEXT(BindBuffer   ) = glBindBufferARB;
		MX_CONTEXT(BufferData   ) = glBufferDataARB;
		MX_CONTEXT(BufferSubData) = glBufferSubDataARB;
		MX_CONTEXT(DeleteBuffers) = glDeleteBuffersARB;
		MX_CONTEXT(GenBuffers   ) = glGenBuffersARB;

		return GL_TRUE;
	}
#endif

	return GL_FALSE;
}


static GLboolean mx_init_mapbuffer(void)
{
#ifndef GLEW_ES_ONLY
	if (GLEW_VERSION_1_5) {
		MX_CONTEXT(MapBuffer  ) = glMapBuffer;
		MX_CONTEXT(UnmapBuffer) = glUnmapBuffer;

		return GL_TRUE;
	}

	if (GLEW_ARB_vertex_buffer_object) {
		MX_CONTEXT(MapBuffer  ) = glMapBufferARB;
		MX_CONTEXT(UnmapBuffer) = glUnmapBufferARB;

		return GL_TRUE;
	}
#endif

#ifndef GLEW_NO_ES
	if (GLEW_OES_mapbuffer) {
		MX_CONTEXT(MapBuffer  ) = glMapBufferOES;
		MX_CONTEXT(UnmapBuffer) = glUnmapBufferOES;

		return GL_TRUE;
	}
#endif

	return GL_FALSE;
}


static GLboolean mx_init_pixel_buffer_object(void)
{
	if (GLEW_VERSION_2_1 || GLEW_ARB_pixel_buffer_object || GLEW_EXT_pixel_buffer_object) {
		return GL_TRUE;
	}

#ifndef GLEW_NO_ES
	if (GLEW_NV_pixel_buffer_object) {
		return GL_TRUE;
	}
#endif

	return GL_FALSE;
}


static GLboolean mx_init_texture_rectangle(void)
{
	return
		GLEW_VERSION_3_1 ||
		GLEW_ARB_texture_rectangle ||
		GLEW_EXT_texture_rectangle ||
		 GLEW_NV_texture_rectangle;
}


#define MX_FLAG_USE_GENERATE_MIPMAP_WORKAROUND 1


static GLuint mx_use_generate_mipmap_workaround()
{
	const char* vendor   = (const char *)glGetString(GL_VENDOR  );
	const char* renderer = (const char *)glGetString(GL_RENDERER);

	if (strstr(vendor, "ATI")          ||
		strstr(renderer, "Mesa DRI R") ||
		(strstr(renderer, "Gallium ") && strstr(renderer, " on ATI ")))
	{
		return MX_FLAG_USE_GENERATE_MIPMAP_WORKAROUND;
	}
	else
	{
		return 0;
	}
}


void _mx_init_proc_binding(void)
{
	MX_CONTEXT(flags) |= mx_use_generate_mipmap_workaround();

	MX_CONTEXT(multitexture       ) = mx_init_multitexture();
	MX_CONTEXT(shader_objects     ) = mx_init_shader_objects();
	MX_CONTEXT(vertex_shader      ) = mx_init_vertex_shader();
	MX_CONTEXT(vertexattrib       ) = mx_init_vertexattrib();
	MX_CONTEXT(fragment_shader    ) = mx_init_fragment_shader();
	MX_CONTEXT(framebuffer_object ) = mx_init_framebuffer_object();
	MX_CONTEXT(vertex_array_object) = mx_init_vertex_array_object();
	MX_CONTEXT(texture3D          ) = mx_init_texture3D();
	MX_CONTEXT(pixel_buffer_object) = mx_init_pixel_buffer_object();
	MX_CONTEXT(texture_rectangle  ) = mx_init_texture_rectangle();

	MX_CONTEXT(vertex_buffer_objects) = mx_init_vertex_buffer_objects();
	if (MX_CONTEXT(vertex_buffer_objects))
		MX_CONTEXT(mapbuffer) = mx_init_mapbuffer();
	else
		MX_CONTEXT(mapbuffer) = GL_FALSE;
}


void mx_glGenerateMipmap(GLenum target)
{
	GLboolean workaround;

	assert(MX_CONTEXT(framebuffer_object));

	/* Work around bug in ATI driver, need to have GL_TEXTURE_2D enabled.
	 * http://www.opengl.org/wiki/Common_Mistakes#Automatic_mipmap_generation */
	if (MX_CONTEXT(flags) & MX_FLAG_USE_GENERATE_MIPMAP_WORKAROUND) {
		workaround = !glIsEnabled(target);

		if (workaround)
			glEnable(target);
	}
	else {
		workaround = GL_FALSE;
	}

	MX_CONTEXT(GenerateMipmap(target));

	if (workaround)
		glDisable(target);
}
