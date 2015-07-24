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
 * The Original Code is Copyright (C) 2005 Blender Foundation.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): Brecht Van Lommel.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file blender/gpu/intern/gpu_extensions.c
 *  \ingroup gpu
 *
 * Wrap OpenGL features such as textures, shaders and GLSL
 * with checks for drivers and GPU support.
 */


#include "GPU_glew.h"

#include "DNA_image_types.h"

#include "MEM_guardedalloc.h"

#include "BLI_blenlib.h"
#include "BLI_utildefines.h"
#include "BLI_math_base.h"

#include "BKE_global.h"

#include "GPU_debug.h"
#include "GPU_draw.h"
#include "GPU_extensions.h"
#include "GPU_simple_shader.h"

#include "intern/gpu_codegen.h"
#include "intern/gpu_debug_private.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#  include "BLI_winstuff.h"
#endif

#define MAX_DEFINE_LENGTH 72

/* Extensions support */

/* extensions used:
 * - texture border clamp: 1.3 core
 * - fragment shader: 2.0 core
 * - framebuffer object: ext specification
 * - multitexture 1.3 core
 * - arb non power of two: 2.0 core
 * - pixel buffer objects? 2.1 core
 * - arb draw buffers? 2.0 core
 */

/* Non-generated shaders */
extern char datatoc_gpu_shader_vsm_store_vert_glsl[];
extern char datatoc_gpu_shader_vsm_store_frag_glsl[];
extern char datatoc_gpu_shader_sep_gaussian_blur_vert_glsl[];
extern char datatoc_gpu_shader_sep_gaussian_blur_frag_glsl[];

typedef struct GPUShaders {
	GPUShader *vsm_store;
	GPUShader *sep_gaussian_blur;
} GPUShaders;

static struct GPUGlobal {
	GLint maxtexsize;
	GLint maxtextures;
	GLuint currentfb;
	int glslsupport;
	int extdisabled;
	int colordepth;
	int npotdisabled; /* ATI 3xx-5xx (and more) chipsets support NPoT partially (== not enough) */
	int dlistsdisabled; /* Legacy ATI driver does not support display lists well */
	GPUDeviceType device;
	GPUOSType os;
	GPUDriverType driver;
	GPUShaders shaders;
	GPUTexture *invalid_tex_1D; /* texture used in place of invalid textures (not loaded correctly, missing) */
	GPUTexture *invalid_tex_2D;
	GPUTexture *invalid_tex_3D;
} GG = {1, 0};

/* GPU Types */

int GPU_type_matches(GPUDeviceType device, GPUOSType os, GPUDriverType driver)
{
	return (GG.device & device) && (GG.os & os) && (GG.driver & driver);
}

/* GPU Extensions */

void GPU_extensions_disable(void)
{
	GG.extdisabled = 1;
}

int GPU_max_texture_size(void)
{
	return GG.maxtexsize;
}

/*
Computes the maximum number of textures 'n' that
can be referenced by ActiveTexture(TEXTURE0+n-1)

This is for any use of ActiveTexture.

Individual limits, such as for the multitexture extension, gl_TexCoord,
vertex shaders, fragment shader, etc. will each have different limits.
*/
static GLint get_max_textures(void)
{
	GLint maxTextureUnits;
	GLint maxTextureCoords;
	GLint maxCombinedTextureImageUnits;

	/* There has to be at least one texture so count that here */
	maxTextureUnits = 1;

#ifndef GLEW_ES_ONLY
	if (MX_profile_compatibility && MX_multitexture) {
		/* Multitexture typically supports only 2 or 4 texture stages even on modern hardware. */
		glGetIntegerv(GL_MAX_TEXTURE_UNITS, &maxTextureUnits);
	}
#endif

	/* Set to zero here in case they do not get set later */
	maxTextureCoords             = 0;
	maxCombinedTextureImageUnits = 0;

	if (MX_fragment_shader) {
#ifndef GLEW_ES_ONLY
		if (MX_profile_compatibility) {
			/* size of gl_TexCoord array in GLSL */
			glGetIntegerv(GL_MAX_TEXTURE_COORDS, &maxTextureCoords);
		}
#endif

		/* Number of textures accessible by vertex, fragment, and geometry shaders combined. */
		/* Individually the limits for each of those programmable units may be smaller. */
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedTextureImageUnits);
	}

	return MAX3(maxTextureUnits, maxTextureCoords, maxCombinedTextureImageUnits);
}

int GPU_max_textures(void)
{
	return GG.maxtextures;
}

void gpu_extensions_init(void)
{
	GLint r, g, b;
	const char *vendor, *renderer;

	/* glewIsSupported("GL_VERSION_2_0") */

	GG.maxtextures = get_max_textures();

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &GG.maxtexsize);

	GG.glslsupport =
		GLEW_VERSION_2_0    ||
		GLEW_ES_VERSION_2_0 ||
		(MX_multitexture  && MX_shader_objects && MX_vertexattrib &&
		 MX_vertex_shader && MX_fragment_shader);

	glGetIntegerv(GL_RED_BITS, &r);
	glGetIntegerv(GL_GREEN_BITS, &g);
	glGetIntegerv(GL_BLUE_BITS, &b);
	GG.colordepth = r+g+b; /* assumes same depth for RGB */

	vendor = (const char *)glGetString(GL_VENDOR);
	renderer = (const char *)glGetString(GL_RENDERER);

	if (strstr(vendor, "ATI")) {
		GG.device = GPU_DEVICE_ATI;
		GG.driver = GPU_DRIVER_OFFICIAL;
	}
	else if (strstr(vendor, "NVIDIA")) {
		GG.device = GPU_DEVICE_NVIDIA;
		GG.driver = GPU_DRIVER_OFFICIAL;
	}
	else if (strstr(vendor, "Intel") ||
	        /* src/mesa/drivers/dri/intel/intel_context.c */
	        strstr(renderer, "Mesa DRI Intel") ||
		strstr(renderer, "Mesa DRI Mobile Intel")) {
		GG.device = GPU_DEVICE_INTEL;
		GG.driver = GPU_DRIVER_OFFICIAL;
	}
	else if (strstr(renderer, "Mesa DRI R") || (strstr(renderer, "Gallium ") && strstr(renderer, " on ATI "))) {
		GG.device = GPU_DEVICE_ATI;
		GG.driver = GPU_DRIVER_OPENSOURCE;
	}
	else if (strstr(renderer, "Nouveau") || strstr(vendor, "nouveau")) {
		GG.device = GPU_DEVICE_NVIDIA;
		GG.driver = GPU_DRIVER_OPENSOURCE;
	}
	else if (strstr(vendor, "Mesa")) {
		GG.device = GPU_DEVICE_SOFTWARE;
		GG.driver = GPU_DRIVER_SOFTWARE;
	}
	else if (strstr(vendor, "Microsoft")) {
		GG.device = GPU_DEVICE_SOFTWARE;
		GG.driver = GPU_DRIVER_SOFTWARE;
	}
	else if (strstr(renderer, "Apple Software Renderer")) {
		GG.device = GPU_DEVICE_SOFTWARE;
		GG.driver = GPU_DRIVER_SOFTWARE;
	}
	else {
		GG.device = GPU_DEVICE_ANY;
		GG.driver = GPU_DRIVER_ANY;
	}

	if (GG.device == GPU_DEVICE_ATI) {
		/* ATI 9500 to X2300 cards support NPoT textures poorly
		 * Incomplete list http://dri.freedesktop.org/wiki/ATIRadeon
		 * New IDs from MESA's src/gallium/drivers/r300/r300_screen.c
		 */
		/* This list is close enough to those using the legacy driver which
		 * has a bug with display lists and glVertexAttrib 
		 */
		if (strstr(renderer, "R3") || strstr(renderer, "RV3") ||
		    strstr(renderer, "R4") || strstr(renderer, "RV4") ||
		    strstr(renderer, "RS4") || strstr(renderer, "RC4") ||
		    strstr(renderer, "R5") || strstr(renderer, "RV5") ||
		    strstr(renderer, "RS600") || strstr(renderer, "RS690") ||
		    strstr(renderer, "RS740") || strstr(renderer, "X1") ||
		    strstr(renderer, "X2") || strstr(renderer, "Radeon 9") ||
		    strstr(renderer, "RADEON 9"))
		{
			GG.npotdisabled = 1;
			GG.dlistsdisabled = 1;
		}
	}

	/* make sure double side isn't used by default and only getting enabled in places where it's
	 * really needed to prevent different unexpected behaviors like with intel gme965 card (sergey) */
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

#ifdef _WIN32
	GG.os = GPU_OS_WIN;
#elif defined(__APPLE__)
	GG.os = GPU_OS_MAC;
#else
	GG.os = GPU_OS_UNIX;
#endif


	GPU_invalid_tex_init();
	GPU_simple_shaders_init();
}

void gpu_extensions_exit(void)
{
	GPU_simple_shaders_exit();
	GPU_invalid_tex_free();
}

int GPU_glsl_support(void)
{
	return !GG.extdisabled && GG.glslsupport;
}

int GPU_non_power_of_two_support(void)
{
	if (GG.npotdisabled)
		return 0;

	return GLEW_ARB_texture_non_power_of_two;
}

int GPU_display_list_support(void)
{
	return !GG.dlistsdisabled;
}

int GPU_color_depth(void)
{
	return GG.colordepth;
}

static void GPU_print_framebuffer_error(GLenum status, char err_out[256])
{
	const char *err= "unknown";

	switch (status) {
		case GL_FRAMEBUFFER_COMPLETE:
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			err= "Incomplete attachment";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			err= "Unsupported framebuffer format";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			err= "Missing attachment";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
			err= "Attached images must have same dimensions";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
			err= "Attached images must have same format";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			err= "Missing draw buffer";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			err= "Missing read buffer";
			break;
	}

	GPU_print_error(err_out, 256,
		"GPUFrameBuffer: framebuffer incomplete error %d '%s'\n",
		(int)status, err);
}

/* GPUTexture */

struct GPUTexture {
	int w, h;				/* width/height */
	int number;				/* number for multitexture binding */
	int refcount;			/* reference count */
	GLenum target;			/* GL_TEXTURE_* */
	GLuint bindcode;		/* opengl identifier for texture */
	int fromblender;		/* we got the texture from Blender */

	GPUFrameBuffer *fb;		/* GPUFramebuffer this texture is attached to */
	int depth;				/* is a depth texture? */
};

static unsigned char *GPU_texture_convert_pixels(int length, float *fpixels)
{
	unsigned char *pixels, *p;
	const float *fp;
	int a, len;

	len = 4*length;
	fp = fpixels;
	p = pixels = MEM_callocN(sizeof(unsigned char)*len, "GPUTexturePixels");

	for (a=0; a<len; a++, p++, fp++)
		*p = FTOCHAR((*fp));

	return pixels;
}

static void GPU_glTexSubImageEmpty(GLenum target, GLenum format, int x, int y, int w, int h)
{
	void *pixels = MEM_callocN(sizeof(char)*4*w*h, "GPUTextureEmptyPixels");

	if (target == GL_TEXTURE_1D)
		glTexSubImage1D(target, 0, x, w, format, GL_UNSIGNED_BYTE, pixels);
	else
		glTexSubImage2D(target, 0, x, y, w, h, format, GL_UNSIGNED_BYTE, pixels);
	
	MEM_freeN(pixels);
}

static GPUTexture *GPU_texture_create_nD(int w, int h, int n, float *fpixels, int depth, char err_out[256])
{
	GPUTexture *tex;
	GLenum type, format, internalformat;
	void *pixels = NULL;

	if (depth && !GLEW_ARB_depth_texture)
		return NULL;

	tex = MEM_callocN(sizeof(GPUTexture), "GPUTexture");
	tex->w = w;
	tex->h = h;
	tex->number = -1;
	tex->refcount = 1;
	tex->target = (n == 1)? GL_TEXTURE_1D: GL_TEXTURE_2D;
	tex->depth = depth;

	glGenTextures(1, &tex->bindcode);

	if (!tex->bindcode) {
		GPU_REPORT_GL_ERRORS(err_out, 256, "GPUTexture: texture create failed.");
		GPU_texture_free(tex);
		return NULL;
	}

	if (!GPU_non_power_of_two_support()) {
		tex->w = power_of_2_max_i(tex->w);
		tex->h = power_of_2_max_i(tex->h);
	}

	tex->number = 0;
	glBindTexture(tex->target, tex->bindcode);

	if (depth) {
		type = GL_UNSIGNED_BYTE;
		format = GL_DEPTH_COMPONENT;
		internalformat = GL_DEPTH_COMPONENT;
	}
	else {
		type = GL_UNSIGNED_BYTE;
		format = GL_RGBA;
		internalformat = GL_RGBA8;

		if (fpixels)
			pixels = GPU_texture_convert_pixels(w*h, fpixels);
	}

	if (tex->target == GL_TEXTURE_1D) {
		glTexImage1D(tex->target, 0, internalformat, tex->w, 0, format, type, NULL);

		if (fpixels) {
			glTexSubImage1D(tex->target, 0, 0, w, format, type,
				pixels ? pixels : fpixels);

			if (tex->w > w)
				GPU_glTexSubImageEmpty(tex->target, format, w, 0,
					tex->w-w, 1);
		}
	}
	else {
		glTexImage2D(tex->target, 0, internalformat, tex->w, tex->h, 0,
		             format, type, NULL);

		if (fpixels) {
			glTexSubImage2D(tex->target, 0, 0, 0, w, h,
				format, type, pixels ? pixels : fpixels);

			if (tex->w > w)
				GPU_glTexSubImageEmpty(tex->target, format, w, 0, tex->w-w, tex->h);
			if (tex->h > h)
				GPU_glTexSubImageEmpty(tex->target, format, 0, h, w, tex->h-h);
		}
	}

	if (pixels)
		MEM_freeN(pixels);

	if (depth) {
		glTexParameteri(tex->target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(tex->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(tex->target, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri(tex->target, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
		glTexParameteri(tex->target, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);  
	}
	else {
		glTexParameteri(tex->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(tex->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	if (tex->target != GL_TEXTURE_1D) {
		/* CLAMP_TO_BORDER is an OpenGL 1.3 core feature */
		GLenum wrapmode = (depth || tex->h == 1)? GL_CLAMP_TO_EDGE: GL_CLAMP_TO_BORDER;
		glTexParameteri(tex->target, GL_TEXTURE_WRAP_S, wrapmode);
		glTexParameteri(tex->target, GL_TEXTURE_WRAP_T, wrapmode);

#if 0
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); 
#endif
	}
	else
		glTexParameteri(tex->target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	return tex;
}


GPUTexture *GPU_texture_create_3D(int w, int h, int depth, int channels, float *fpixels)
{
	GPUTexture *tex;
	GLenum type, format, internalformat;
	void *pixels = NULL;
	float vfBorderColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};

	if (!GLEW_VERSION_1_2)
		return NULL;

	tex = MEM_callocN(sizeof(GPUTexture), "GPUTexture");
	tex->w = w;
	tex->h = h;
	tex->depth = depth;
	tex->number = -1;
	tex->refcount = 1;
	tex->target = GL_TEXTURE_3D;

	glGenTextures(1, &tex->bindcode);

	if (!tex->bindcode) {
		GPU_REPORT_GL_ERRORS(NULL, 0, "GPUTexture: texture create failed.");
		GPU_texture_free(tex);
		return NULL;
	}

	if (!GPU_non_power_of_two_support()) {
		tex->w = power_of_2_max_i(tex->w);
		tex->h = power_of_2_max_i(tex->h);
		tex->depth = power_of_2_max_i(tex->depth);
	}

	tex->number = 0;
	glBindTexture(tex->target, tex->bindcode);

	GPU_ASSERT_NO_GL_ERRORS("3D glBindTexture");

	type = GL_FLOAT;
	if (channels == 4) {
		format = GL_RGBA;
		internalformat = GL_RGBA;
	}
	else {
		format = GL_RED;
		internalformat = GL_INTENSITY;
	}

	//if (fpixels)
	//	pixels = GPU_texture_convert_pixels(w*h*depth, fpixels);

	glTexImage3D(tex->target, 0, internalformat, tex->w, tex->h, tex->depth, 0, format, type, NULL);

	GPU_ASSERT_NO_GL_ERRORS("3D glTexImage3D");

	if (fpixels) {
		if (!GPU_non_power_of_two_support() && (w != tex->w || h != tex->h || depth != tex->depth)) {
			/* clear first to avoid unitialized pixels */
			float *zero= MEM_callocN(sizeof(float)*tex->w*tex->h*tex->depth, "zero");
			glTexSubImage3D(tex->target, 0, 0, 0, 0, tex->w, tex->h, tex->depth, format, type, zero);
			MEM_freeN(zero);
		}

		glTexSubImage3D(tex->target, 0, 0, 0, 0, w, h, depth, format, type, fpixels);
		GPU_ASSERT_NO_GL_ERRORS("3D glTexSubImage3D");
	}


	glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, vfBorderColor);
	GPU_ASSERT_NO_GL_ERRORS("3D GL_TEXTURE_BORDER_COLOR");
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GPU_ASSERT_NO_GL_ERRORS("3D GL_LINEAR");
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	GPU_ASSERT_NO_GL_ERRORS("3D GL_CLAMP_TO_BORDER");

	if (pixels)
		MEM_freeN(pixels);

	GPU_texture_unbind(tex);

	return tex;
}

GPUTexture *GPU_texture_from_blender(Image *ima, ImageUser *iuser, bool is_data, double time, int mipmap)
{
	GPUTexture *tex;
	GLint w, h, border, lastbindcode, bindcode;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastbindcode);

	GPU_update_image_time(ima, time);
	/* this binds a texture, so that's why to restore it with lastbindcode */
	bindcode = GPU_verify_image(ima, iuser, 0, 0, mipmap, is_data);

	if (ima->gputexture) {
		ima->gputexture->bindcode = bindcode;
		glBindTexture(GL_TEXTURE_2D, lastbindcode);
		return ima->gputexture;
	}

	tex = MEM_callocN(sizeof(GPUTexture), "GPUTexture");
	tex->bindcode = bindcode;
	tex->number = -1;
	tex->refcount = 1;
	tex->target = GL_TEXTURE_2D;
	tex->fromblender = 1;

	ima->gputexture= tex;

	if (!glIsTexture(tex->bindcode)) {
		GPU_print_error(NULL, 0, "Blender Texture Not Loaded");
	}
	else {
		glBindTexture(GL_TEXTURE_2D, tex->bindcode);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_BORDER, &border);

		tex->w = w - border;
		tex->h = h - border;
	}

	glBindTexture(GL_TEXTURE_2D, lastbindcode);

	return tex;
}

GPUTexture *GPU_texture_from_preview(PreviewImage *prv, int mipmap)
{
	GPUTexture *tex = prv->gputexture[0];
	GLint w, h, lastbindcode;
	GLuint bindcode = 0;
	
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastbindcode);
	
	if (tex)
		bindcode = tex->bindcode;
	
	/* this binds a texture, so that's why to restore it */
	if (bindcode == 0) {
		GPU_create_gl_tex(&bindcode, prv->rect[0], NULL, prv->w[0], prv->h[0], mipmap, 0, NULL);
	}
	if (tex) {
		tex->bindcode = bindcode;
		glBindTexture(GL_TEXTURE_2D, lastbindcode);
		return tex;
	}

	tex = MEM_callocN(sizeof(GPUTexture), "GPUTexture");
	tex->bindcode = bindcode;
	tex->number = -1;
	tex->refcount = 1;
	tex->target = GL_TEXTURE_2D;
	
	prv->gputexture[0]= tex;
	
	if (!glIsTexture(tex->bindcode)) {
		GPU_print_error(NULL, 0, "Blender Texture Not Loaded");
	}
	else {
		glBindTexture(GL_TEXTURE_2D, tex->bindcode);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
		
		tex->w = w;
		tex->h = h;
	}
	
	glBindTexture(GL_TEXTURE_2D, lastbindcode);
	
	return tex;

}

GPUTexture *GPU_texture_create_1D(int w, float *fpixels, char err_out[256])
{
	GPUTexture *tex = GPU_texture_create_nD(w, 1, 1, fpixels, 0, err_out);

	if (tex)
		GPU_texture_unbind(tex);
	
	return tex;
}

GPUTexture *GPU_texture_create_2D(int w, int h, float *fpixels, char err_out[256])
{
	GPUTexture *tex = GPU_texture_create_nD(w, h, 2, fpixels, 0, err_out);

	if (tex)
		GPU_texture_unbind(tex);
	
	return tex;
}

GPUTexture *GPU_texture_create_depth(int w, int h, char err_out[256])
{
	GPUTexture *tex = GPU_texture_create_nD(w, h, 2, NULL, 1, err_out);

	if (tex)
		GPU_texture_unbind(tex);
	
	return tex;
}

/**
 * A shadow map for VSM needs two components (depth and depth^2)
 */
GPUTexture *GPU_texture_create_vsm_shadow_map(int size, char err_out[256])
{
	GPUTexture *tex = GPU_texture_create_nD(size, size, 2, NULL, 0, err_out);

	if (tex) {
		/* Now we tweak some of the settings */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, size, size, 0, GL_RG, GL_FLOAT, NULL);

		GPU_texture_unbind(tex);
	}

	return tex;
}

void GPU_invalid_tex_init(void)
{
	float color[4] = {1.0f, 0.0f, 1.0f, 1.0};
	GG.invalid_tex_1D = GPU_texture_create_1D(1, color, NULL);
	GG.invalid_tex_2D = GPU_texture_create_2D(1, 1, color, NULL);
	GG.invalid_tex_3D = GPU_texture_create_3D(1, 1, 1, 4, color);
}

void GPU_invalid_tex_bind(int mode)
{
	switch (mode) {
		case GL_TEXTURE_1D:
			glBindTexture(GL_TEXTURE_1D, GG.invalid_tex_1D->bindcode);
			break;
		case GL_TEXTURE_2D:
			glBindTexture(GL_TEXTURE_2D, GG.invalid_tex_2D->bindcode);
			break;
		case GL_TEXTURE_3D:
			glBindTexture(GL_TEXTURE_3D, GG.invalid_tex_3D->bindcode);
			break;
	}
}

void GPU_invalid_tex_free(void)
{
	if (GG.invalid_tex_1D)
		GPU_texture_free(GG.invalid_tex_1D);
	if (GG.invalid_tex_2D)
		GPU_texture_free(GG.invalid_tex_2D);
	if (GG.invalid_tex_3D)
		GPU_texture_free(GG.invalid_tex_3D);
}


void GPU_texture_bind(GPUTexture *tex, int number)
{
	GLenum arbnumber;

	if (GG.extdisabled || number == -1)
		return;

	if (number >= GG.maxtextures) {
		GPU_print_error(NULL, 0, "Not enough texture slots.");
		return;
	}

	GPU_ASSERT_NO_GL_ERRORS("Pre Texture Bind");

	arbnumber = (GLenum)((GLuint)GL_TEXTURE0 + number);
	if (number != 0) glActiveTexture(arbnumber);
	if (tex->bindcode != 0) {
		glBindTexture(tex->target, tex->bindcode);
	}
	else
		GPU_invalid_tex_bind(tex->target);
	glEnable(tex->target);
	if (number != 0) glActiveTexture(GL_TEXTURE0);

	tex->number = number;

	GPU_ASSERT_NO_GL_ERRORS("Post Texture Bind");
}

void GPU_texture_unbind(GPUTexture *tex)
{
	GLenum arbnumber;

	if (GG.extdisabled || tex->number == -1)
		return;

	if (tex->number >= GG.maxtextures) {
		GPU_print_error(NULL, 0, "Not enough texture slots.");
		return;
	}

	GPU_ASSERT_NO_GL_ERRORS("Pre Texture Unbind");

	arbnumber = (GLenum)((GLuint)GL_TEXTURE0 + tex->number);
	if (tex->number != 0) glActiveTexture(arbnumber);
	glBindTexture(tex->target, 0);
	glDisable(tex->target);
	if (tex->number != 0) glActiveTexture(GL_TEXTURE0);

	tex->number = -1;

	GPU_ASSERT_NO_GL_ERRORS("Post Texture Unbind");
}

void GPU_texture_free(GPUTexture *tex)
{
	tex->refcount--;

	if (tex->refcount < 0)
		GPU_print_error(NULL, 0, "GPUTexture: negative refcount\n");
	
	if (tex->refcount == 0) {
		if (tex->fb)
			GPU_framebuffer_texture_detach(tex->fb, tex);
		if (tex->bindcode && !tex->fromblender)
			glDeleteTextures(1, &tex->bindcode);

		MEM_freeN(tex);
	}
}

void GPU_texture_ref(GPUTexture *tex)
{
	tex->refcount++;
}

int GPU_texture_target(GPUTexture *tex)
{
	return tex->target;
}

int GPU_texture_opengl_width(GPUTexture *tex)
{
	return tex->w;
}

int GPU_texture_opengl_height(GPUTexture *tex)
{
	return tex->h;
}

int GPU_texture_opengl_bindcode(GPUTexture *tex)
{
	return tex->bindcode;
}

GPUFrameBuffer *GPU_texture_framebuffer(GPUTexture *tex)
{
	return tex->fb;
}

/* GPUFrameBuffer */

struct GPUFrameBuffer {
	GLuint object;
	GPUTexture *colortex;
	GPUTexture *depthtex;
};

GPUFrameBuffer *GPU_framebuffer_create(void)
{
	GPUFrameBuffer *fb;

	if (GG.extdisabled || !MX_framebuffer_object)
		return NULL;

	fb= MEM_callocN(sizeof(GPUFrameBuffer), "GPUFrameBuffer");
	glGenFramebuffers(1, &fb->object);

	if (!fb->object) {
		GPU_REPORT_GL_ERRORS(NULL, 0, "GPUFFrameBuffer: framebuffer gen failed.");
		GPU_framebuffer_free(fb);
		return NULL;
	}

	return fb;
}

int GPU_framebuffer_texture_attach(GPUFrameBuffer *fb, GPUTexture *tex, char err_out[256])
{
	GLenum status;
	GLenum attachment;

	if (GG.extdisabled || !MX_framebuffer_object)
		return 0;

	if (tex->depth)
		attachment = GL_DEPTH_ATTACHMENT;
	else
		attachment = GL_COLOR_ATTACHMENT0;

	GPU_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fb->object));
	GG.currentfb = fb->object;

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, 
		tex->target, tex->bindcode, 0);

	if (GPU_REPORT_GL_ERRORS(err_out, 256, "GPU_framebuffer_texture_attach")) {
		GPU_framebuffer_restore();
		return 0;
	}

	if (tex->depth) {
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	else {
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glReadBuffer(GL_COLOR_ATTACHMENT0);
	}

	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		GPU_framebuffer_restore();
		GPU_print_framebuffer_error(status, err_out);
		return 0;
	}

	if (tex->depth)
		fb->depthtex = tex;
	else
		fb->colortex = tex;

	tex->fb= fb;

	return 1;
}

void GPU_framebuffer_texture_detach(GPUFrameBuffer *fb, GPUTexture *tex)
{
	GLenum attachment;

	if (GG.extdisabled || !MX_framebuffer_object)
		return;

	if (!tex->fb)
		return;

	if (GG.currentfb != tex->fb->object) {
		glBindFramebuffer(GL_FRAMEBUFFER, tex->fb->object);
		GG.currentfb = tex->fb->object;
	}

	if (tex->depth) {
		fb->depthtex = NULL;
		attachment = GL_DEPTH_ATTACHMENT;
	}
	else {
		fb->colortex = NULL;
		attachment = GL_COLOR_ATTACHMENT0;
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment,
		tex->target, 0, 0);

	tex->fb = NULL;
}

void GPU_framebuffer_texture_bind(GPUFrameBuffer *UNUSED(fb), GPUTexture *tex, int w, int h)
{
	/* push attributes */
	glPushAttrib(GL_ENABLE_BIT | GL_VIEWPORT_BIT);
	glDisable(GL_SCISSOR_TEST);

	/* bind framebuffer */
	if (!GG.extdisabled && MX_framebuffer_object)
		glBindFramebuffer(GL_FRAMEBUFFER, tex->fb->object);

	/* push matrices and set default viewport and matrix */
	glViewport(0, 0, w, h);
	GG.currentfb = tex->fb->object;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}

void GPU_framebuffer_texture_unbind(GPUFrameBuffer *UNUSED(fb), GPUTexture *UNUSED(tex))
{
	/* restore matrix */
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	/* restore attributes */
	glPopAttrib();
	glEnable(GL_SCISSOR_TEST);
}

void GPU_framebuffer_free(GPUFrameBuffer *fb)
{
	if (GG.extdisabled || !MX_framebuffer_object)
		return;

	if (fb->depthtex)
		GPU_framebuffer_texture_detach(fb, fb->depthtex);
	if (fb->colortex)
		GPU_framebuffer_texture_detach(fb, fb->colortex);

	if (fb->object) {
		glDeleteFramebuffers(1, &fb->object);

		if (GG.currentfb == fb->object) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			GG.currentfb = 0;
		}
	}

	MEM_freeN(fb);
}

void GPU_framebuffer_restore(void)
{
	if (GG.extdisabled || !MX_framebuffer_object)
		return;

	if (GG.currentfb != 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		GG.currentfb = 0;
	}
}

void GPU_framebuffer_blur(GPUFrameBuffer *fb, GPUTexture *tex, GPUFrameBuffer *blurfb, GPUTexture *blurtex)
{
	float scaleh[2] = {1.0f/GPU_texture_opengl_width(blurtex), 0.0f};
	float scalev[2] = {0.0f, 1.0f/GPU_texture_opengl_height(tex)};

	GPUShader *blur_shader = GPU_shader_get_builtin_shader(GPU_SHADER_SEP_GAUSSIAN_BLUR);
	int scale_uniform, texture_source_uniform;

	if (!blur_shader)
		return;

	scale_uniform = GPU_shader_get_uniform(blur_shader, "ScaleU");
	texture_source_uniform = GPU_shader_get_uniform(blur_shader, "textureSource");
		
	/* Blurring horizontally */

	/* We do the bind ourselves rather than using GPU_framebuffer_texture_bind() to avoid
	 * pushing unnecessary matrices onto the OpenGL stack. */
	if (!GG.extdisabled && MX_framebuffer_object)
		glBindFramebuffer(GL_FRAMEBUFFER, blurfb->object);

	GPU_shader_bind(blur_shader);
	GPU_shader_uniform_vector(blur_shader, scale_uniform, 2, 1, (float *)scaleh);
	GPU_shader_uniform_texture(blur_shader, texture_source_uniform, tex);
	glViewport(0, 0, GPU_texture_opengl_width(blurtex), GPU_texture_opengl_height(blurtex));

	/* Peparing to draw quad */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	GPU_texture_bind(tex, 0);

	/* Drawing quad */
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(1, 1);
	glTexCoord2d(1, 0); glVertex2f(-1, 1);
	glTexCoord2d(1, 1); glVertex2f(-1, -1);
	glTexCoord2d(0, 1); glVertex2f(1, -1);
	glEnd();
		
	/* Blurring vertically */

	if (!GG.extdisabled && MX_framebuffer_object)
		glBindFramebuffer(GL_FRAMEBUFFER, fb->object);

	glViewport(0, 0, GPU_texture_opengl_width(tex), GPU_texture_opengl_height(tex));
	GPU_shader_uniform_vector(blur_shader, scale_uniform, 2, 1, (float *)scalev);
	GPU_shader_uniform_texture(blur_shader, texture_source_uniform, blurtex);
	GPU_texture_bind(blurtex, 0);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(1, 1);
	glTexCoord2d(1, 0); glVertex2f(-1, 1);
	glTexCoord2d(1, 1); glVertex2f(-1, -1);
	glTexCoord2d(0, 1); glVertex2f(1, -1);
	glEnd();

	GPU_shader_unbind();
}

/* GPUOffScreen */

struct GPUOffScreen {
	GPUFrameBuffer *fb;
	GPUTexture *color;
	GPUTexture *depth;

	/* requested width/height, may be smaller than actual texture size due
	 * to missing non-power of two support, so we compensate for that */
	int w, h;
};

GPUOffScreen *GPU_offscreen_create(int width, int height, char err_out[256])
{
	GPUOffScreen *ofs;

	ofs= MEM_callocN(sizeof(GPUOffScreen), "GPUOffScreen");
	ofs->w= width;
	ofs->h= height;

	ofs->fb = GPU_framebuffer_create();
	if (!ofs->fb) {
		GPU_offscreen_free(ofs);
		return NULL;
	}

	ofs->depth = GPU_texture_create_depth(width, height, err_out);
	if (!ofs->depth) {
		GPU_offscreen_free(ofs);
		return NULL;
	}

	if (!GPU_framebuffer_texture_attach(ofs->fb, ofs->depth, err_out)) {
		GPU_offscreen_free(ofs);
		return NULL;
	}

	ofs->color = GPU_texture_create_2D(width, height, NULL, err_out);
	if (!ofs->color) {
		GPU_offscreen_free(ofs);
		return NULL;
	}

	if (!GPU_framebuffer_texture_attach(ofs->fb, ofs->color, err_out)) {
		GPU_offscreen_free(ofs);
		return NULL;
	}

	GPU_framebuffer_restore();

	return ofs;
}

void GPU_offscreen_free(GPUOffScreen *ofs)
{
	if (ofs->fb)
		GPU_framebuffer_free(ofs->fb);
	if (ofs->color)
		GPU_texture_free(ofs->color);
	if (ofs->depth)
		GPU_texture_free(ofs->depth);
	
	MEM_freeN(ofs);
}

void GPU_offscreen_bind(GPUOffScreen *ofs)
{
	glDisable(GL_SCISSOR_TEST);
	GPU_framebuffer_texture_bind(ofs->fb, ofs->color, ofs->w, ofs->h);
}

void GPU_offscreen_unbind(GPUOffScreen *ofs)
{
	GPU_framebuffer_texture_unbind(ofs->fb, ofs->color);
	GPU_framebuffer_restore();
	glEnable(GL_SCISSOR_TEST);
}

void GPU_offscreen_read_pixels(GPUOffScreen *ofs, int type, void *pixels)
{
	glReadPixels(0, 0, ofs->w, ofs->h, GL_RGBA, type, pixels);
}

int GPU_offscreen_width(GPUOffScreen *ofs)
{
	return ofs->w;
}

int GPU_offscreen_height(GPUOffScreen *ofs)
{
	return ofs->h;
}

/* GPUShader */

struct GPUShader {
	GLuint object;   /* handle for full shader     */
	GLuint vertex;   /* handle for vertex shader   */
	GLuint fragment; /* handle for fragment shader */
	GLuint lib;      /* handle for libment shader  */
	int totattrib;   /* total number of attributes */
};

static void shader_print_log(GLuint object, GLboolean is_program, const char* friendly_name, const char* kind)
{
	GLchar* log;
	GLint log_size = 0;

	if (is_program)
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_size);
	else
		glGetShaderiv (object, GL_INFO_LOG_LENGTH, &log_size);

	if (log_size > 0) {
		log = (GLchar*)MEM_mallocN(log_size, "shader_print_log");

		if (is_program)
			glGetProgramInfoLog(object, log_size, NULL, log);
		else
			glGetShaderInfoLog (object, log_size, NULL, log);

		if (is_program)
			fprintf(stderr, "GPUShader: Linker Info Log: %s\n%s\n", friendly_name, log);
		else
			fprintf(stderr, "GPUShader: %s Shader: %s\n Shader Info Log:\n%s\n", kind, friendly_name, log);

		MEM_freeN(log);
	}
}

static void shader_print_src(GLuint object, GLboolean is_program, const char* friendly_name, const char *kind, const char **code, int code_count)
{
	int total_line = 1;

	int i = 0;

	fprintf(stderr, "GPUShader: %s Source Code: %s: BEGIN\n", kind, friendly_name);

	for (i = 0; i < code_count; i++) {
		const char *c= code[i];
		const char *pos;
		const char *end = code[i] + strlen(code[i]);
		int line = 1;

		fprintf(stderr, "===== shader string %d ====\n", i + 1);

		while ((c < end) && (pos = strchr(c, '\n'))) {
			fprintf(stderr, "%d:%3d:%3d: ", i + 1, line, total_line);
			fwrite(c, (pos+1)-c, 1, stderr);
			c = pos+1;
			line++;
			total_line++;
		}

		fprintf(stderr, "%s", c);
	}

	fprintf(stderr, "GPUShader: %s Source Code: %s: END\n", kind, friendly_name);
}

static bool print_status(GLuint object, GLboolean is_program, const char* friendly_name, const char* kind, const char** code, int code_count)
{
	GLint status;

	if (is_program)
		glGetProgramiv(object, GL_LINK_STATUS,    &status);
	else
		glGetShaderiv (object, GL_COMPILE_STATUS, &status);

	if ((G.debug & G_DEBUG) || !status) {
		const char* result = status ? "Succeeded" : "Failed!";

		if (is_program)
			fprintf(stderr, "GPUShader: Linking %s: %s\n", result, friendly_name);
		else
			fprintf(stderr, "GPUShader: Compilation %s: %s Shader %s\n", result, kind, friendly_name);
	}

	if ((G.debug & G_DEBUG) && code_count > 0)
		shader_print_src(object, is_program, friendly_name, kind, code, code_count);

	if ((G.debug & G_DEBUG) || !status)
		shader_print_log(object, is_program, friendly_name, kind);

	return status;
}

static const char *gpu_shader_version(void)
{
	/* turn on glsl 1.30 for bicubic bump mapping and ATI clipping support */
	if (GLEW_VERSION_3_0 &&
	    (GPU_bicubic_bump_support() || GPU_type_matches(GPU_DEVICE_ATI, GPU_OS_ANY, GPU_DRIVER_ANY)))
	{
		return "#version 130\n";
	}

	return "";
}


static const char *gpu_shader_standard_extensions(void)
{
	/* need this extensions for high quality bump mapping */
	if (GPU_bicubic_bump_support())
		return "#extension GL_ARB_texture_query_lod: enable\n";

	return "";
}

static void gpu_shader_standard_defines(char defines[MAX_DEFINE_LENGTH])
{
	/* some useful defines to detect GPU type */
	if (GPU_type_matches(GPU_DEVICE_ATI, GPU_OS_ANY, GPU_DRIVER_ANY)) {
		strcat(defines, "#define GPU_ATI\n");
		if (GLEW_VERSION_3_0)
			strcat(defines, "#define CLIP_WORKAROUND\n");
	}
	else if (GPU_type_matches(GPU_DEVICE_NVIDIA, GPU_OS_ANY, GPU_DRIVER_ANY))
		strcat(defines, "#define GPU_NVIDIA\n");
	else if (GPU_type_matches(GPU_DEVICE_INTEL, GPU_OS_ANY, GPU_DRIVER_ANY))
		strcat(defines, "#define GPU_INTEL\n");

	if (GPU_bicubic_bump_support())
		strcat(defines, "#define BUMP_BICUBIC\n");
	return;
}

GPUShader *GPU_shader_create(const char *friendly_name, const char *vertexcode, const char *fragcode, const char *libcode, const char *defines)
{
	GLsizei length = 0;
	GPUShader *shader;
	char standard_defines[MAX_DEFINE_LENGTH] = "";

	if (GG.extdisabled || !MX_shader_objects)
		return NULL;

	shader = MEM_callocN(sizeof(GPUShader), "GPUShader");

	if (vertexcode)
		shader->vertex = glCreateShader(GL_VERTEX_SHADER);
	if (fragcode)
		shader->fragment = glCreateShader(GL_FRAGMENT_SHADER);
	shader->object = glCreateProgram();

	if (!shader->object ||
	    (vertexcode && !shader->vertex) ||
	    (fragcode && !shader->fragment))
	{
		GPU_print_error(NULL, 0, "GPUShader: object creation failed.\n");
		GPU_shader_free(shader);
		return NULL;
	}

	gpu_shader_standard_defines(standard_defines);

	if (vertexcode) {
		const char *source[5];
		/* custom limit, may be too small, beware */
		int num_source = 0;

		source[num_source++] = gpu_shader_version();
		source[num_source++] = gpu_shader_standard_extensions();
		source[num_source++] = standard_defines;

		if (defines) source[num_source++] = defines;
		if (vertexcode) source[num_source++] = vertexcode;

		glAttachShader(shader->object, shader->vertex);
		glShaderSource(shader->vertex, num_source, source, NULL);

		glCompileShader(shader->vertex);

		if (!print_status(shader->vertex, GL_FALSE, friendly_name, "Vertex", source, num_source)) {
			GPU_shader_free(shader);
			return NULL;
		}
	}

	if (fragcode) {
		const char *source[6];
		int num_source = 0;

		source[num_source++] = gpu_shader_version();
		source[num_source++] = gpu_shader_standard_extensions();
		source[num_source++] = standard_defines;

		if (defines) source[num_source++] = defines;
		if (libcode) source[num_source++] = libcode;
		if (fragcode) source[num_source++] = fragcode;

		glAttachShader(shader->object, shader->fragment);
		glShaderSource(shader->fragment, num_source, source, NULL);

		glCompileShader(shader->fragment);

		if (!print_status(shader->fragment, GL_FALSE, friendly_name, "Fragment", source, num_source)) {
			GPU_shader_free(shader);
			return NULL;
		}
	}

	glLinkProgram(shader->object);

	if (!print_status(shader->object, GL_TRUE, friendly_name, NULL, NULL, 0)) {
		GPU_shader_free(shader);
		return NULL;
	}

	return shader;
}

#if 0
GPUShader *GPU_shader_create_lib(const char *code)
{
	GLsizei length = 0;
	GPUShader *shader;
	const char** source = (const char**)&code;

	if (GG.extdisabled || !MX_shader_objects)
		return NULL;

	shader = MEM_callocN(sizeof(GPUShader), "GPUShader");

	shader->lib = glCreateShader(GL_FRAGMENT_SHADER);

	if (!shader->lib) {
		GPU_print_error("GPUShader: object creation failed.\n");
		GPU_shader_free(shader);
		return NULL;
	}

	glShaderSource(shader->lib, 1, source, NULL);

	glCompileShader(shader->lib);

	if (!print_status(shader->fragment, GL_FALSE, "GPU_shader_create_lib", "Library", source, 1)) {
		GPU_shader_free(shader);
		return NULL;
	}

	return shader;
}
#endif

void GPU_shader_bind(GPUShader *shader)
{
	if (GG.extdisabled || !MX_shader_objects)
		return;

	GPU_CHECK(glUseProgram(shader->object));
}

void GPU_shader_unbind(void)
{
	if (GG.extdisabled || !MX_shader_objects)
		return;

	GPU_CHECK(glUseProgram(0));
}

void GPU_shader_free(GPUShader *shader)
{
	if (GG.extdisabled || !MX_shader_objects || shader == NULL)
		return;
	if (shader->lib)
		glDeleteShader(shader->lib);
	if (shader->vertex)
		glDeleteShader(shader->vertex);
	if (shader->fragment)
		glDeleteShader(shader->fragment);
	if (shader->object)
		glDeleteProgram(shader->object);
	MEM_freeN(shader);
}

int GPU_shader_get_uniform(GPUShader *shader, const char *name)
{
	if (GG.extdisabled || !MX_shader_objects)
		return -1;
	else
		return glGetUniformLocation(shader->object, name);
}

void GPU_shader_uniform_vector(GPUShader *UNUSED(shader), int location, int length, int arraysize, float *value)
{
	if (GG.extdisabled || !MX_shader_objects || location == -1)
		return;

	GPU_ASSERT_NO_GL_ERRORS("Pre Uniform Vector");

	if (length == 1) glUniform1fv(location, arraysize, value);
	else if (length == 2) glUniform2fv(location, arraysize, value);
	else if (length == 3) glUniform3fv(location, arraysize, value);
	else if (length == 4) glUniform4fv(location, arraysize, value);
	else if (length == 9) glUniformMatrix3fv(location, arraysize, 0, value);
	else if (length == 16) glUniformMatrix4fv(location, arraysize, 0, value);

	GPU_ASSERT_NO_GL_ERRORS("Post Uniform Vector");
}

void GPU_shader_uniform_int(GPUShader *UNUSED(shader), int location, int value)
{
	if (GG.extdisabled || !MX_shader_objects || location == -1)
		return;

	GPU_CHECK(glUniform1i(location, value));
}

void GPU_shader_uniform_texture(GPUShader *UNUSED(shader), int location, GPUTexture *tex)
{
	GLenum arbnumber;

	if (tex->number >= GG.maxtextures) {
		GPU_print_error(NULL, 0, "Not enough texture slots.");
		return;
	}
		
	if (tex->number == -1)
		return;

	if (location == -1)
		return;

	GPU_ASSERT_NO_GL_ERRORS("Pre Uniform Texture");

	arbnumber = (GLenum)((GLuint)GL_TEXTURE0 + tex->number);

	if (tex->number != 0) glActiveTexture(arbnumber);
	if (tex->bindcode != 0)
		glBindTexture(tex->target, tex->bindcode);
	else
		GPU_invalid_tex_bind(tex->target);
	glUniform1i(location, tex->number);
	glEnable(tex->target);
	if (tex->number != 0) glActiveTexture(GL_TEXTURE0);

	GPU_ASSERT_NO_GL_ERRORS("Post Uniform Texture");
}

int GPU_shader_get_attribute(GPUShader *shader, const char *name)
{
	int index;

	if (GG.extdisabled || !MX_vertex_shader)
		return -1;

	GPU_CHECK(index = glGetAttribLocation(shader->object, name));

	return index;
}

GPUShader *GPU_shader_get_builtin_shader(GPUBuiltinShader shader)
{
	GPUShader *retval = NULL;

	switch (shader) {
		case GPU_SHADER_VSM_STORE:
			if (!GG.shaders.vsm_store)
				GG.shaders.vsm_store = GPU_shader_create("VSM Store", datatoc_gpu_shader_vsm_store_vert_glsl, datatoc_gpu_shader_vsm_store_frag_glsl, NULL, NULL);
			retval = GG.shaders.vsm_store;
			break;
		case GPU_SHADER_SEP_GAUSSIAN_BLUR:
			if (!GG.shaders.sep_gaussian_blur)
				GG.shaders.sep_gaussian_blur = GPU_shader_create("Gaussian Blur", datatoc_gpu_shader_sep_gaussian_blur_vert_glsl, datatoc_gpu_shader_sep_gaussian_blur_frag_glsl, NULL, NULL);
			retval = GG.shaders.sep_gaussian_blur;
			break;
	}

	if (retval == NULL)
		GPU_print_error(NULL, 0,
			"Unable to create a GPUShader for builtin shader: %d\n",
			shader);

	return retval;
}

void GPU_shader_free_builtin_shaders(void)
{
	if (GG.shaders.vsm_store) {
		MEM_freeN(GG.shaders.vsm_store);
		GG.shaders.vsm_store = NULL;
	}

	if (GG.shaders.sep_gaussian_blur) {
		MEM_freeN(GG.shaders.sep_gaussian_blur);
		GG.shaders.sep_gaussian_blur = NULL;
	}
}

#if 0
/* GPUPixelBuffer */

typedef struct GPUPixelBuffer {
	GLuint bindcode[2];
	GLuint current;
	int datasize;
	int numbuffers;
	int halffloat;
} GPUPixelBuffer;

void GPU_pixelbuffer_free(GPUPixelBuffer *pb)
{
	if (pb->bindcode[0])
		glDeleteBuffers(pb->numbuffers, pb->bindcode);
	MEM_freeN(pb);
}

GPUPixelBuffer *gpu_pixelbuffer_create(int x, int y, int halffloat, int numbuffers)
{
	GPUPixelBuffer *pb;

	if (!MX_pixel_buffer_object || !MX_mapbuffer || !MX_texture_rectangle)
		return NULL;

	pb = MEM_callocN(sizeof(GPUPixelBuffer), "GPUPBO");
	pb->datasize = x*y*4*((halffloat)? 16: 8);
	pb->numbuffers = numbuffers;
	pb->halffloat = halffloat;

	glGenBuffers(pb->numbuffers, pb->bindcode);

	if (!pb->bindcode[0]) {
		GPU_print_error("GPUPixelBuffer: allocation failed\n");
		GPU_pixelbuffer_free(pb);
		return NULL;
	}

	return pb;
}

void GPU_pixelbuffer_texture(GPUTexture *tex, GPUPixelBuffer *pb)
{
	void *pixels;
	int i;

	glBindTexture(GL_TEXTURE_RECTANGLE, tex->bindcode);

	for (i = 0; i < pb->numbuffers; i++) {
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pb->bindcode[pb->current]);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, pb->datasize, NULL,
		GL_STREAM_DRAW);

		pixels = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
		/*memcpy(pixels, _oImage.data(), pb->datasize);*/

		if (!glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER)) {
			GPU_print_error("Could not unmap opengl PBO\n");
			break;
		}
	}

	glBindTexture(GL_TEXTURE_RECTANGLE, 0);
}

static int pixelbuffer_map_into_gpu(GLuint bindcode)
{
	void *pixels;

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, bindcode);
	pixels = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);

	/* do stuff in pixels */

	if (!glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER)) {
		GPU_print_error("Could not unmap opengl PBO\n");
		return 0;
	}
	
	return 1;
}

static void pixelbuffer_copy_to_texture(GPUTexture *tex, GPUPixelBuffer *pb, GLuint bindcode)
{
	GLenum type = (pb->halffloat)? GL_HALF_FLOAT_NV: GL_UNSIGNED_BYTE;
	glBindTexture(GL_TEXTURE_RECTANGLE, tex->bindcode);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, bindcode);

	glTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, 0, 0, tex->w, tex->h,
					GL_RGBA, type, NULL);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);
}

void GPU_pixelbuffer_async_to_gpu(GPUTexture *tex, GPUPixelBuffer *pb)
{
	int newbuffer;

	if (pb->numbuffers == 1) {
		pixelbuffer_copy_to_texture(tex, pb, pb->bindcode[0]);
		pixelbuffer_map_into_gpu(pb->bindcode[0]);
	}
	else {
		pb->current = (pb->current+1)%pb->numbuffers;
		newbuffer = (pb->current+1)%pb->numbuffers;

		pixelbuffer_map_into_gpu(pb->bindcode[newbuffer]);
		pixelbuffer_copy_to_texture(tex, pb, pb->bindcode[pb->current]);
	}
}
#endif
