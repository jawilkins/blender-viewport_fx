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
 * The Original Code is Copyright (C) 2012 Blender Foundation.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): Jason Wilkins.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file intern/gpu_debug_private.h
 *  \ingroup gpu
 */

#ifndef __GPU_DEBUG_PRIVATE_H__
#define __GPU_DEBUG_PRIVATE_H__

#include "GPU_debug.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifdef WITH_GPU_DEBUG

void gpu_debug_init(void);
void gpu_debug_exit(void);

#  define GPU_DEBUG_INIT() gpu_debug_init()
#  define GPU_DEBUG_EXIT() gpu_debug_exit()

#else

#  define GPU_DEBUG_INIT() ((void)0)
#  define GPU_DEBUG_EXIT() ((void)0)

#endif

#ifdef __cplusplus
}
#endif


#endif /* __GPU_DEBUG_PRIVATE_H__ */
