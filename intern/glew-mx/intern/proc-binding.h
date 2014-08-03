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

/** \file intern/proc-binding.h
 *  \ingroup glew-mx
 *
 * Simplified OpenGL API Bindings
 *
 * If MX_NO_PROC_BINDING is not defined then this file is
 * included by glew-mx.h and it will remove OpenGL API entries and redefine
 * others.  Removed functions are extensions that should be referred to
 * by their un-suffixed name (e.g. glActiveTexture instead of
 * glActiveTextureARB).  The redefined API entries have no extension
 * suffix and are called through the current MXContext.
 *
 * This file is NOT for checking DEPRECATED OpenGL API entries.
 */

#ifndef __PROC_BINDING_H__
#define __PROC_BINDING_H__

#ifndef __GLEW_MX_H__
#error This file is meant to be included from glew-mx.h
#endif


/* Get rid of GLEW's definitions */

#undef GLEW_ARB_multitexture

#undef glActiveTexture
#undef glClientActiveTexture
#undef glMultiTexCoord1d
#undef glMultiTexCoord1dv
#undef glMultiTexCoord1f
#undef glMultiTexCoord1fv
#undef glMultiTexCoord1i
#undef glMultiTexCoord1iv
#undef glMultiTexCoord1s
#undef glMultiTexCoord1sv
#undef glMultiTexCoord2d
#undef glMultiTexCoord2dv
#undef glMultiTexCoord2f
#undef glMultiTexCoord2fv
#undef glMultiTexCoord2i
#undef glMultiTexCoord2iv
#undef glMultiTexCoord2s
#undef glMultiTexCoord2sv
#undef glMultiTexCoord3d
#undef glMultiTexCoord3dv
#undef glMultiTexCoord3f
#undef glMultiTexCoord3fv
#undef glMultiTexCoord3i
#undef glMultiTexCoord3iv
#undef glMultiTexCoord3s
#undef glMultiTexCoord3sv
#undef glMultiTexCoord4d
#undef glMultiTexCoord4dv
#undef glMultiTexCoord4f
#undef glMultiTexCoord4fv
#undef glMultiTexCoord4i
#undef glMultiTexCoord4iv
#undef glMultiTexCoord4s
#undef glMultiTexCoord4sv

#undef GLEW_ARB_shader_objects

#undef glCreateShader
#undef glAttachShader
#undef glShaderSource
#undef glCompileShader
#undef glGetShaderiv
#undef glGetShaderInfoLog
#undef glCreateProgram
#undef glLinkProgram
#undef glGetProgramInfoLog
#undef glValidateProgram

#undef glUniform1i
#undef glUniform1f

#undef glUniform1iv
#undef glUniform2iv
#undef glUniform3iv
#undef glUniform4iv

#undef glUniform1fv
#undef glUniform2fv
#undef glUniform3fv
#undef glUniform4fv

#undef glUniformMatrix3fv
#undef glUniformMatrix4fv

#undef glUseProgram
#undef glDeleteShader
#undef glDeleteProgram

#undef GLEW_ARB_vertex_shader
#undef GLEW_ARB_fragment_shader

#undef glGetAttribLocation
#undef glBindAttribLocation
#undef glGetUniformLocation

/* Would normally remove GLEW_ARB_vertex_program and GLEW_ARB_fragment_program,
 * but Blender uses a legacy assembly language GL_ARB_fragment_program in drawvolume.c,
 * and the ARB suffixed functions required for that are not a part of MXContext,
 * so they should be accessed normally.
 */

/*#undef GLEW_ARB_vertex_program  */
/*#undef GLEW_ARB_fragment_program*/

#undef glEnableVertexAttribArray
#undef glDisableVertexAttribArray
#undef glGetProgramiv
#undef glVertexAttribPointer

#undef glVertexAttrib1d
#undef glVertexAttrib1dv
#undef glVertexAttrib1f
#undef glVertexAttrib1fv
#undef glVertexAttrib1s
#undef glVertexAttrib1sv
#undef glVertexAttrib2d
#undef glVertexAttrib2dv
#undef glVertexAttrib2f
#undef glVertexAttrib2fv
#undef glVertexAttrib2s
#undef glVertexAttrib2sv
#undef glVertexAttrib3d
#undef glVertexAttrib3dv
#undef glVertexAttrib3f
#undef glVertexAttrib3fv
#undef glVertexAttrib3s
#undef glVertexAttrib3sv
#undef glVertexAttrib4Nbv
#undef glVertexAttrib4Niv
#undef glVertexAttrib4Nsv
#undef glVertexAttrib4Nub
#undef glVertexAttrib4Nubv
#undef glVertexAttrib4Nuiv
#undef glVertexAttrib4Nusv
#undef glVertexAttrib4bv
#undef glVertexAttrib4d
#undef glVertexAttrib4dv
#undef glVertexAttrib4f
#undef glVertexAttrib4fv
#undef glVertexAttrib4iv
#undef glVertexAttrib4s
#undef glVertexAttrib4sv
#undef glVertexAttrib4ubv
#undef glVertexAttrib4uiv
#undef glVertexAttrib4usv

#undef GLEW_ARB_framebuffer_object
#undef GLEW_EXT_framebuffer_object
#undef GLEW_OES_framebuffer_object

#undef glGenFramebuffers
#undef glBindFramebuffer
#undef glDeleteFramebuffers
#undef glFramebufferTexture2D
#undef glCheckFramebufferStatus

#undef GLEW_ARB_vertex_buffer_object

#undef glGenBuffers
#undef glBindBuffer
#undef glBufferData
#undef glBufferSubData
#undef glDeleteBuffers

#undef GLEW_OES_mapbuffer

#undef glMapBuffer
#undef glUnmapBuffer

#undef GLEW_ARB_vertex_array_object
#undef GLEW_OES_vertex_array_object

#undef glGenVertexArrays
#undef glBindVertexArray
#undef glDeleteVertexArrays

#undef glGenerateMipmap

#undef GLEW_ARB_texture3D
#undef GLEW_EXT_texture3D
#undef GLEW_OES_texture3D

#undef glTexImage3D
#undef glTexSubImage3D

#undef GLEW_ARB_pixel_buffer_object
#undef GLEW_EXT_pixel_buffer_object


/* Remind developers to use the canonical functions. */

#undef glActiveTextureARB
#define glActiveTextureARB USE_glActiveTexture
#undef glClientActiveTextureARB
#define glClientActiveTextureARB USE_glClientActiveTexture
#undef glMultiTexCoord1dARB
#define glMultiTexCoord1dARB USE_glMultiTexCoord1d
#undef glMultiTexCoord1dvARB
#define glMultiTexCoord1dvARB USE_glMultiTexCoord1dv
#undef glMultiTexCoord1fARB
#define glMultiTexCoord1fARB USE_glMultiTexCoord1f
#undef glMultiTexCoord1fvARB
#define glMultiTexCoord1fvARB USE_glMultiTexCoord1fv
#undef glMultiTexCoord1iARB
#define glMultiTexCoord1iARB USE_glMultiTexCoord1i
#undef glMultiTexCoord1ivARB
#define glMultiTexCoord1ivARB USE_glMultiTexCoord1iv
#undef glMultiTexCoord1sARB
#define glMultiTexCoord1sARB USE_glMultiTexCoord1s
#undef glMultiTexCoord1svARB
#define glMultiTexCoord1svARB USE_glMultiTexCoord1sv
#undef glMultiTexCoord2dARB
#define glMultiTexCoord2dARB USE_glMultiTexCoord2d
#undef glMultiTexCoord2dvARB
#define glMultiTexCoord2dvARB USE_glMultiTexCoord2dv
#undef glMultiTexCoord2fARB
#define glMultiTexCoord2fARB USE_glMultiTexCoord2f
#undef glMultiTexCoord2fvARB
#define glMultiTexCoord2fvARB USE_glMultiTexCoord2fv
#undef glMultiTexCoord2iARB
#define glMultiTexCoord2iARB USE_glMultiTexCoord2i
#undef glMultiTexCoord2ivARB
#define glMultiTexCoord2ivARB USE_glMultiTexCoord2iv
#undef glMultiTexCoord2sARB
#define glMultiTexCoord2sARB USE_glMultiTexCoord2s
#undef glMultiTexCoord2svARB
#define glMultiTexCoord2svARB USE_glMultiTexCoord2sv
#undef glMultiTexCoord3dARB
#define glMultiTexCoord3dARB USE_glMultiTexCoord3d
#undef glMultiTexCoord3dvARB
#define glMultiTexCoord3dvARB USE_glMultiTexCoord3dv
#undef glMultiTexCoord3fARB
#define glMultiTexCoord3fARB USE_glMultiTexCoord3f
#undef glMultiTexCoord3fvARB
#define glMultiTexCoord3fvARB USE_glMultiTexCoord3fv
#undef glMultiTexCoord3iARB
#define glMultiTexCoord3iARB USE_glMultiTexCoord3i
#undef glMultiTexCoord3ivARB
#define glMultiTexCoord3ivARB USE_glMultiTexCoord3iv
#undef glMultiTexCoord3sARB
#define glMultiTexCoord3sARB USE_glMultiTexCoord3s
#undef glMultiTexCoord3svARB
#define glMultiTexCoord3svARB USE_glMultiTexCoord3sv
#undef glMultiTexCoord4dARB
#define glMultiTexCoord4dARB USE_glMultiTexCoord4d
#undef glMultiTexCoord4dvARB
#define glMultiTexCoord4dvARB USE_glMultiTexCoord4dv
#undef glMultiTexCoord4fARB
#define glMultiTexCoord4fARB USE_glMultiTexCoord4f
#undef glMultiTexCoord4fvARB
#define glMultiTexCoord4fvARB USE_glMultiTexCoord4fv
#undef glMultiTexCoord4iARB
#define glMultiTexCoord4iARB USE_glMultiTexCoord4i
#undef glMultiTexCoord4ivARB
#define glMultiTexCoord4ivARB USE_glMultiTexCoord4iv
#undef glMultiTexCoord4sARB
#define glMultiTexCoord4sARB USE_glMultiTexCoord4s
#undef glMultiTexCoord4svARB
#define glMultiTexCoord4svARB USE_glMultiTexCoord4sv

#undef glAttachObjectARB
#define glAttachObjectARB USE_glAttachObject
#undef glCompileShaderARB
#define glCompileShaderARB USE_glCompileShader
#undef glCreateProgramObjectARB
#define glCreateProgramObjectARB USE_glCreateProgram
#undef glCreateShaderObjectARB
#define glCreateShaderObjectARB USE_glCreateShader
#undef glDeleteObjectARB
#define glDeleteObjectARB USE_glDeleteShader_or_glDeleteProgram
#undef glGetInfoLogARB
#define glGetInfoLogARB USE_glGetProgramInfoLog_or_glGetShaderInfoLog
#undef glGetObjectParameterivARB
#define glGetObjectParameterivARB USE_glGetShaderiv
#undef glGetUniformLocationARB
#define glGetUniformLocationARB USE_glGetUniformLocation
#undef glLinkProgramARB
#define glLinkProgramARB USE_glLinkProgram
#undef glShaderSourceARB
#define glShaderSourceARB USE_glShaderSource
#undef glUniform1iARB
#define glUniform1iARB USE_glUniform1i
#undef glUniform1fARB
#define glUniform1fARB USE_glUniform1f
#undef glUniform1ivARB
#define glUniform1ivARB USE_glUniform1iv
#undef glUniform2ivARB
#define glUniform2ivARB USE_glUniform2iv
#undef glUniform3ivARB
#define glUniform3ivARB USE_glUniform3iv
#undef glUniform4ivARB
#define glUniform4ivARB USE_glUniform4iv
#undef glUniform1fvARB
#define glUniform1fvARB USE_glUniform1fv
#undef glUniform2fvARB
#define glUniform2fvARB USE_glUniform2fv
#undef glUniform3fvARB
#define glUniform3fvARB USE_glUniform3fv
#undef glUniform4fvARB
#define glUniform4fvARB USE_glUniform4fv
#undef glUniformMatrix3fvARB
#define glUniformMatrix3fvARB USE_glUniformMatrix3fv
#undef glUniformMatrix4fvARB
#define glUniformMatrix4fvARB USE_glUniformMatrix4fv
#undef glUseProgramObjectARB
#define glUseProgramObjectARB USE_glUseProgram
#undef glValidateProgramARB
#define glValidateProgramARB USE_glValidateProgram

#undef glBindAttribLocationARB
#define glBindAttribLocationARB USE_glBindAttribLocation
#undef glGetAttribLocationARB
#define glGetAttribLocationARB USE_glGetAttribLocation

#undef glDeleteObjectARB
#define glDeleteObjectARB USE_glDeleteShader_or_glDeleteProgram
#undef glDisableVertexAttribArrayARB
#define glDisableVertexAttribArrayARB USE_glDisableVertexAttribArray
#undef glEnableVertexAttribArrayARB
#define glEnableVertexAttribArrayARB USE_glEnableVertexAttribArray
#undef glGetProgramivARB
#define glGetProgramivARB USE_glGetProgramiv
#undef glVertexAttribPointerARB
#define glVertexAttribPointerARB USE_glVertexAttribPointer

#undef glVertexAttrib1dARB 
#define glVertexAttrib1dARB USE_glVertexAttrib1d
#undef glVertexAttrib1dvARB 
#define glVertexAttrib1dvARB USE_glVertexAttrib1dv
#undef glVertexAttrib1fARB 
#define glVertexAttrib1fARB USE_glVertexAttrib1f
#undef glVertexAttrib1fvARB 
#define glVertexAttrib1fvARB USE_glVertexAttrib1fv
#undef glVertexAttrib1sARB 
#define glVertexAttrib1sARB USE_glVertexAttrib1s
#undef glVertexAttrib1svARB 
#define glVertexAttrib1svARB USE_glVertexAttrib1sv
#undef glVertexAttrib2dARB 
#define glVertexAttrib2dARB USE_glVertexAttrib2d
#undef glVertexAttrib2dvARB 
#define glVertexAttrib2dvARB USE_glVertexAttrib2dv
#undef glVertexAttrib2fARB 
#define glVertexAttrib2fARB USE_glVertexAttrib2f
#undef glVertexAttrib2fvARB 
#define glVertexAttrib2fvARB USE_glVertexAttrib2fv
#undef glVertexAttrib2sARB 
#define glVertexAttrib2sARB USE_glVertexAttrib2s
#undef glVertexAttrib2svARB 
#define glVertexAttrib2svARB USE_glVertexAttrib2sv
#undef glVertexAttrib3dARB 
#define glVertexAttrib3dARB USE_glVertexAttrib3d
#undef glVertexAttrib3dvARB 
#define glVertexAttrib3dvARB USE_glVertexAttrib3dv
#undef glVertexAttrib3fARB 
#define glVertexAttrib3fARB USE_glVertexAttrib3f
#undef glVertexAttrib3fvARB 
#define glVertexAttrib3fvARB USE_glVertexAttrib3fv
#undef glVertexAttrib3sARB 
#define glVertexAttrib3sARB USE_glVertexAttrib3s
#undef glVertexAttrib3svARB 
#define glVertexAttrib3svARB USE_glVertexAttrib3sv
#undef glVertexAttrib4NbvARB 
#define glVertexAttrib4NbvARB USE_glVertexAttrib4Nbv
#undef glVertexAttrib4NivARB 
#define glVertexAttrib4NivARB USE_glVertexAttrib4Niv
#undef glVertexAttrib4NsvARB 
#define glVertexAttrib4NsvARB USE_glVertexAttrib4Nsv
#undef glVertexAttrib4NubARB 
#define glVertexAttrib4NubARB USE_glVertexAttrib4Nub
#undef glVertexAttrib4NubvARB
#define glVertexAttrib4NubvARB USE_glVertexAttrib4Nubv
#undef glVertexAttrib4NuivARB
#define glVertexAttrib4NuivARB USE_glVertexAttrib4Nuiv
#undef glVertexAttrib4NusvARB
#define glVertexAttrib4NusvARB USE_glVertexAttrib4Nusv
#undef glVertexAttrib4bvARB 
#define glVertexAttrib4bvARB USE_glVertexAttrib4bv
#undef glVertexAttrib4dARB 
#define glVertexAttrib4dARB USE_glVertexAttrib4d
#undef glVertexAttrib4dvARB 
#define glVertexAttrib4dvARB USE_glVertexAttrib4dv
#undef glVertexAttrib4fARB 
#define glVertexAttrib4fARB USE_glVertexAttrib4f
#undef glVertexAttrib4fvARB 
#define glVertexAttrib4fvARB USE_glVertexAttrib4fv
#undef glVertexAttrib4ivARB 
#define glVertexAttrib4ivARB USE_glVertexAttrib4i
#undef glVertexAttrib4sARB 
#define glVertexAttrib4sARB USE_glVertexAttrib4s
#undef glVertexAttrib4svARB 
#define glVertexAttrib4svARB USE_glVertexAttrib4sv
#undef glVertexAttrib4ubvARB 
#define glVertexAttrib4ubvARB USE_glVertexAttrib4ubv
#undef glVertexAttrib4uivARB 
#define glVertexAttrib4uivARB USE_glVertexAttrib4uiv
#undef glVertexAttrib4usvARB 
#define glVertexAttrib4usvARB USE_glVertexAttrib4usv

#undef glBindBufferARB
#define glBindBufferARB USE_glBindBuffer
#undef glBufferDataARB
#define glBufferDataARB USE_glBufferData
#undef glBufferSubDataARB
#define glBufferSubDataARB USE_glBufferSubData
#undef glDeleteBuffersARB
#define glDeleteBuffersARB USE_glDeleteBuffers
#undef glGenBuffersARB
#define glGenBuffersARB USE_glGenBuffers

#undef glMapBufferARB
#define glMapBufferARB USE_glMapBuffer
#undef glUnmapBufferARB
#define glUnmapBufferARB USE_glUnmapBuffer

#undef glMapBufferOES
#define glMapBufferOES USE_glMapBuffer
#undef glUnmapBufferOES
#define glUnmapBufferOES USE_glUnmapBuffer

#undef glBindFramebufferEXT
#define glBindFramebufferEXT USE_glBindFramebuffer
#undef glCheckFramebufferStatusEXT
#define glCheckFramebufferStatusEXT USE_glCheckFramebufferStatus
#undef glDeleteFramebuffersEXT
#define glDeleteFramebuffersEXT USE_glDeleteFramebuffers
#undef glFramebufferTexture2DEXT
#define glFramebufferTexture2DEXT USE_glFramebufferTexture2D
#undef glGenFramebuffersEXT
#define glGenFramebuffersEXT USE_glGenFramebuffers

#undef glBindFramebufferOES
#define glBindFramebufferOES USE_glBindFramebuffer
#undef glCheckFramebufferStatusOES
#define glCheckFramebufferStatusOES USE_glCheckFramebufferStatus
#undef glDeleteFramebuffersOES
#define glDeleteFramebuffersOES USE_glDeleteFramebuffers
#undef glFramebufferTexture2DOES
#define glFramebufferTexture2DOES USE_glFramebufferTexture2D
#undef glGenFramebuffersOES
#define glGenFramebuffersOES USE_glGenFramebuffers

#undef glBindVertexArrayOES
#define glBindVertexArrayOES USE_glBindVertexArray
#undef glDeleteVertexArraysOES
#define glDeleteVertexArraysOES USE_glDeleteVertexArrays
#undef glGenVertexArraysOES
#define glGenVertexArraysOES USE_glGenVertexArrays

#undef glGenerateMipmapEXT
#define glGenerateMipmapEXT USE_glGenerateMipmapEXT

#undef glGenerateMipmapOES
#define glGenerateMipmapOES USE_glGenerateMipmapOES

#undef glTexImage3DEXT
#define glTexImage3DEXT USE_glTexImage3D
#undef glTexSubImage3DEXT
#define glTexSubImage3DEXT USE_glTexSubImage3D

#undef glTexImage3DOES
#define glTexImage3DOES USE_glTexImage3D
#undef glTexSubImage3DOES
#define glTexSubImage3DOES USE_glTexSubImage3D


/* Define the canonical functions */

#define MX_multitexture MX_CONTEXT(multitexture)

#define glActiveTexture       MX_CONTEXT(ActiveTexture      )
#define glClientActiveTexture MX_CONTEXT(ClientActiveTexture)
#ifdef WITH_GL_PROFILE_COMPAT
#  define glMultiTexCoord1d  MX_CONTEXT(MultiTexCoord1d )
#  define glMultiTexCoord1dv MX_CONTEXT(MultiTexCoord1dv)
#  define glMultiTexCoord1f  MX_CONTEXT(MultiTexCoord1f )
#  define glMultiTexCoord1fv MX_CONTEXT(MultiTexCoord1fv)
#  define glMultiTexCoord1i  MX_CONTEXT(MultiTexCoord1i )
#  define glMultiTexCoord1iv MX_CONTEXT(MultiTexCoord1iv)
#  define glMultiTexCoord1s  MX_CONTEXT(MultiTexCoord1s )
#  define glMultiTexCoord1sv MX_CONTEXT(MultiTexCoord1sv)
#  define glMultiTexCoord2d  MX_CONTEXT(MultiTexCoord2d )
#  define glMultiTexCoord2dv MX_CONTEXT(MultiTexCoord2dv)
#  define glMultiTexCoord2f  MX_CONTEXT(MultiTexCoord2f )
#  define glMultiTexCoord2fv MX_CONTEXT(MultiTexCoord2fv)
#  define glMultiTexCoord2i  MX_CONTEXT(MultiTexCoord2i )
#  define glMultiTexCoord2iv MX_CONTEXT(MultiTexCoord2iv)
#  define glMultiTexCoord2s  MX_CONTEXT(MultiTexCoord2s )
#  define glMultiTexCoord2sv MX_CONTEXT(MultiTexCoord2sv)
#  define glMultiTexCoord3d  MX_CONTEXT(MultiTexCoord3d )
#  define glMultiTexCoord3dv MX_CONTEXT(MultiTexCoord3dv)
#  define glMultiTexCoord3f  MX_CONTEXT(MultiTexCoord3f )
#  define glMultiTexCoord3fv MX_CONTEXT(MultiTexCoord3fv)
#  define glMultiTexCoord3i  MX_CONTEXT(MultiTexCoord3i )
#  define glMultiTexCoord3iv MX_CONTEXT(MultiTexCoord3iv)
#  define glMultiTexCoord3s  MX_CONTEXT(MultiTexCoord3s )
#  define glMultiTexCoord3sv MX_CONTEXT(MultiTexCoord3sv)
#  define glMultiTexCoord4d  MX_CONTEXT(MultiTexCoord4d )
#  define glMultiTexCoord4dv MX_CONTEXT(MultiTexCoord4dv)
#  define glMultiTexCoord4f  MX_CONTEXT(MultiTexCoord4f )
#  define glMultiTexCoord4fv MX_CONTEXT(MultiTexCoord4fv)
#  define glMultiTexCoord4i  MX_CONTEXT(MultiTexCoord4i )
#  define glMultiTexCoord4iv MX_CONTEXT(MultiTexCoord4iv)
#  define glMultiTexCoord4s  MX_CONTEXT(MultiTexCoord4s )
#  define glMultiTexCoord4sv MX_CONTEXT(MultiTexCoord4sv)
#endif

#define MX_shader_objects MX_CONTEXT(shader_objects)

#define glAttachShader     MX_CONTEXT(AttachShader    )
#define glCreateShader     MX_CONTEXT(CreateShader    )
#define glDeleteShader     MX_CONTEXT(DeleteShader    )
#define glShaderSource     MX_CONTEXT(ShaderSource    )
#define glCompileShader    MX_CONTEXT(CompileShader   )
#define glGetShaderiv      MX_CONTEXT(GetShaderiv     )
#define glGetShaderInfoLog MX_CONTEXT(GetShaderInfoLog)

#define glCreateProgram     MX_CONTEXT(CreateProgram    )
#define glDeleteProgram     MX_CONTEXT(DeleteProgram    )
#define glLinkProgram       MX_CONTEXT(LinkProgram      )
#define glGetProgramInfoLog MX_CONTEXT(GetProgramInfoLog)
#define glUseProgram        MX_CONTEXT(UseProgram       )
#define glValidateProgram   MX_CONTEXT(ValidateProgram  )

#define glUniform1i MX_CONTEXT(Uniform1i)
#define glUniform1f MX_CONTEXT(Uniform1f)

#define glUniform1iv MX_CONTEXT(Uniform1iv)
#define glUniform2iv MX_CONTEXT(Uniform2iv)
#define glUniform3iv MX_CONTEXT(Uniform3iv)
#define glUniform4iv MX_CONTEXT(Uniform4iv)

#define glUniform1fv MX_CONTEXT(Uniform1fv)
#define glUniform2fv MX_CONTEXT(Uniform2fv)
#define glUniform3fv MX_CONTEXT(Uniform3fv)
#define glUniform4fv MX_CONTEXT(Uniform4fv)

#define glUniformMatrix3fv MX_CONTEXT(UniformMatrix3fv)
#define glUniformMatrix4fv MX_CONTEXT(UniformMatrix4fv)

#define glGetUniformLocation MX_CONTEXT(GetUniformLocation)

#define MX_vertex_shader   MX_CONTEXT(vertex_shader  )
#define MX_fragment_shader MX_CONTEXT(fragment_shader)

#define glGetAttribLocation  MX_CONTEXT(GetAttribLocation )
#define glBindAttribLocation MX_CONTEXT(BindAttribLocation)

#define MX_vertexattrib MX_CONTEXT(vertexattrib)

#define glEnableVertexAttribArray  MX_CONTEXT(EnableVertexAttribArray)
#define glGetProgramiv             MX_CONTEXT(GetProgramiv           )
#define glDisableVertexAttribArray MX_CONTEXT(EnableVertexAttribArray)
#define glVertexAttribPointer      MX_CONTEXT(VertexAttribPointer    )

#ifdef WITH_GL_PROFILE_COMPAT
#  define glVertexAttrib1d    MX_CONTEXT(VertexAttrib1d   )
#  define glVertexAttrib1dv   MX_CONTEXT(VertexAttrib1dv  )
#  define glVertexAttrib1f    MX_CONTEXT(VertexAttrib1f   )
#  define glVertexAttrib1fv   MX_CONTEXT(VertexAttrib1fv  )
#  define glVertexAttrib1s    MX_CONTEXT(VertexAttrib1s   )
#  define glVertexAttrib1sv   MX_CONTEXT(VertexAttrib1sv  )
#  define glVertexAttrib2d    MX_CONTEXT(VertexAttrib2d   )
#  define glVertexAttrib2dv   MX_CONTEXT(VertexAttrib2dv  )
#  define glVertexAttrib2f    MX_CONTEXT(VertexAttrib2f   )
#  define glVertexAttrib2fv   MX_CONTEXT(VertexAttrib2fv  )
#  define glVertexAttrib2s    MX_CONTEXT(VertexAttrib2s   )
#  define glVertexAttrib2sv   MX_CONTEXT(VertexAttrib2sv  )
#  define glVertexAttrib3d    MX_CONTEXT(VertexAttrib3d   )
#  define glVertexAttrib3dv   MX_CONTEXT(VertexAttrib3dv  )
#  define glVertexAttrib3f    MX_CONTEXT(VertexAttrib3f   )
#  define glVertexAttrib3fv   MX_CONTEXT(VertexAttrib3fv  )
#  define glVertexAttrib3s    MX_CONTEXT(VertexAttrib3s   )
#  define glVertexAttrib3sv   MX_CONTEXT(VertexAttrib3sv  )
#  define glVertexAttrib4Nbv  MX_CONTEXT(VertexAttrib4Nbv )
#  define glVertexAttrib4Niv  MX_CONTEXT(VertexAttrib4Niv )
#  define glVertexAttrib4Nsv  MX_CONTEXT(VertexAttrib4Nsv )
#  define glVertexAttrib4Nub  MX_CONTEXT(VertexAttrib4Nub )
#  define glVertexAttrib4Nubv MX_CONTEXT(VertexAttrib4Nubv)
#  define glVertexAttrib4Nuiv MX_CONTEXT(VertexAttrib4Nuiv)
#  define glVertexAttrib4Nusv MX_CONTEXT(VertexAttrib4Nusv)
#  define glVertexAttrib4bv   MX_CONTEXT(VertexAttrib4bv  )
#  define glVertexAttrib4d    MX_CONTEXT(VertexAttrib4d   )
#  define glVertexAttrib4dv   MX_CONTEXT(VertexAttrib4dv  )
#  define glVertexAttrib4f    MX_CONTEXT(VertexAttrib4f   )
#  define glVertexAttrib4fv   MX_CONTEXT(VertexAttrib4fv  )
#  define glVertexAttrib4iv   MX_CONTEXT(VertexAttrib4iv  )
#  define glVertexAttrib4s    MX_CONTEXT(VertexAttrib4s   )
#  define glVertexAttrib4sv   MX_CONTEXT(VertexAttrib4sv  )
#  define glVertexAttrib4ubv  MX_CONTEXT(VertexAttrib4ubv )
#  define glVertexAttrib4uiv  MX_CONTEXT(VertexAttrib4uiv )
#  define glVertexAttrib4usv  MX_CONTEXT(VertexAttrib4usv )
#endif

#define MX_framebuffer_object MX_CONTEXT(framebuffer_object)

#define glGenFramebuffers        MX_CONTEXT(GenFramebuffers       )
#define glBindFramebuffer        MX_CONTEXT(BindFramebuffer       )
#define glDeleteFramebuffers     MX_CONTEXT(DeleteFramebuffers    )
#define glFramebufferTexture2D   MX_CONTEXT(FramebufferTexture2D  )
#define glCheckFramebufferStatus MX_CONTEXT(CheckFramebufferStatus)

/* mx_glGenerateMipmap contains a bug workaround */
#define glGenerateMipmap mx_glGenerateMipmap

#define MX_vertex_buffer_object MX_CONTEXT(vertex_buffer_object)

#define glGenBuffers    MX_CONTEXT(GenBuffers   )
#define glBindBuffer    MX_CONTEXT(BindBuffer   )
#define glBufferData    MX_CONTEXT(BufferData   )
#define glBufferSubData MX_CONTEXT(BufferSubData)
#define glDeleteBuffers MX_CONTEXT(DeleteBuffers)

#define MX_mapbuffer MX_CONTEXT(mapbuffer)

#define glMapBuffer   MX_CONTEXT(MapBuffer  )
#define glUnmapBuffer MX_CONTEXT(UnmapBuffer)

#define MX_vertex_array_object MX_CONTEXT(vertex_array_object)

#define glGenVertexArrays    MX_CONTEXT(GenVertexArrays   )
#define glBindVertexArray    MX_CONTEXT(BindVertexArray   )
#define glDeleteVertexArrays MX_CONTEXT(DeleteVertexArrays)

#define MX_texture3D MX_CONTEXT(texture3D)

#define glTexImage3D    MX_CONTEXT(TexImage3D   )
#define glTexSubImage3D MX_CONTEXT(TexSubImage3D)

#define MX_pixel_buffer_object MX_CONTEXT(pixel_buffer_object)

#define MX_texture_rectangle MX_CONTEXT(texture_rectangle)


/* we can optimize a little if only one profile is supported, otherwise we need to check at runtime */
#if defined(WITH_GL_PROFILE_CORE) && !defined(WITH_GL_PROFILE_ES20) && !defined(WITH_GL_PROFILE_COMPAT)
#  define MX_profile_compatibility 0
#  define MX_profile_core          1
#  define MX_profile_es20          0
#elif !defined(WITH_GL_PROFILE_CORE) && defined(WITH_GL_PROFILE_ES20) && !defined(WITH_GL_PROFILE_COMPAT)
#  define MX_profile_compatibility 0
#  define MX_profile_core          0
#  define MX_profile_es20          1
#elif !defined(WITH_GL_PROFILE_CORE) && !defined(WITH_GL_PROFILE_ES20) && defined(WITH_GL_PROFILE_COMPAT)
#  define MX_profile_compatibility 1
#  define MX_profile_core          0
#  define MX_profile_es20          0
#else
#  define MX_profile_compatibility (!GLEW_VERSION_3_1 || GLEW_ARB_compatibility)
#  define MX_profile_core          (!MX_profile_compatibility)
#  define MX_profile_es20          GLEW_VERSION_ES_2_0
#endif


void _mx_init_proc_binding(void);


#endif /* __PROC_BINDING_H__ */
