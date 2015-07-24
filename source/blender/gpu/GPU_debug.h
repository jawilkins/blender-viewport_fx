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
 * Contributor(s): Brecht Van Lommel, Jason Wilkins.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file source/blender/gpu/GPU_debug.h
 *  \ingroup gpu
 */

#ifndef __GPU_DEBUG_H__
#define __GPU_DEBUG_H__

#include "GPU_glew.h"

#include "BLI_compiler_attrs.h"
#include "BLI_utildefines.h"

#ifdef WITH_GPU_DEBUG
#include <stdlib.h> /* for abort */
#endif


#ifdef __cplusplus
extern "C" {
#endif




#ifdef WITH_GPU_DEBUG
/* Define some useful, but potentially slow, checks for correct API usage. */

#  define GPU_ASSERT(test) BLI_assert(test)

#  if WITH_ASSERT_ABORT
#    define GPU_ABORT abort
#  else
#    define GPU_ABORT() ((void)0)
#  endif


bool gpu_report_gl_errors  (const char *__restrict file, int line, char *__restrict out, size_t size, const char *__restrict str, GLenum test);
void gpu_debug_print       (const char *format, ... ) ATTR_NONNULL(1) ATTR_PRINTF_FORMAT(1);
void gpu_state_print       (void);
void gpu_string_marker     (size_t size, const char *str);
bool gpu_assert_no_gl_errors(const char *__restrict file, int line, const char *__restrict str, bool do_abort);

#  define GPU_REPORT_GL_ERRORS(out, size, str) gpu_report_gl_errors(__FILE__, __LINE__, (out), (size), (str), GL_NO_ERROR)
#  define GPU_TEST_GL_ERROR(str, test)         gpu_report_gl_errors(__FILE__, __LINE__, NULL, 0, (str), (test))
#  define GPU_DEBUG_PRINT(str)                 gpu_debug_print(str)
#  define GPU_STATE_PRINT()                    gpu_state_print()
#  define GPU_STRING_MARKER(size, str)         gpu_string_marker((size), (str))
#  define GPU_ASSERT_NO_GL_ERRORS(str)         gpu_assert_no_gl_errors(__FILE__, __LINE__, (str), true)

#  define GPU_DEBUG_CHECK(glProcCall)              \
       (                                           \
       GPU_REPORT_GL_ERRORS("Pre:  " #glProcCall), \
       (glProcCall),                               \
       GPU_REPORT_GL_ERRORS("Post: " #glProcCall)  \
       )

#  define GPU_CHECK(glProcCall)                       \
       (                                              \
       GPU_ASSERT_NO_GL_ERRORS("Pre:  " #glProcCall), \
       (glProcCall),                                  \
       GPU_ASSERT_NO_GL_ERRORS("Post: " #glProcCall)  \
       )


#else /* !WITH_GPU_DEBUG */


#  define GPU_ASSERT(test) ((void)0)
#  define GPU_ABORT()      ((void)0)

bool gpu_report_gl_errors(char *__restrict out, size_t size, const char *__restrict str, GLenum test);

#  define GPU_REPORT_GL_ERRORS(out, size, str) gpu_report_gl_errors((out), (size), (str), GL_NO_ERROR)
#  define GPU_TEST_GL_ERROR(str, test)         gpu_report_gl_errors(NULL, 0, (str), (test))
#  define GPU_DEBUG_PRINT(str)                 ((void)(str))
#  define GPU_STATE_PRINT()                    ((void)0)
#  define GPU_STRING_MARKER(len, str)          ((void)(size),(void)(str))
#  define GPU_ASSERT_NO_GL_ERRORS(str)         ((void)(str))
#  define GPU_DEBUG_CHECK(glProcCall)          (glProcCall)

#  define GPU_CHECK(glProcCall)                    \
       (                                           \
       GPU_REPORT_GL_ERRORS("Pre:  " #glProcCall), \
       (glProcCall),                               \
       GPU_REPORT_GL_ERRORS("Post: " #glProcCall)  \
       )


#endif /* WITH_GPU_DEBUG */


/* GPU_ASSERT_RETURN bails out of a function even if assert or abort are disabled.
 * Can be used in functions that return void if third argument is left blank.
 * There needs to be a local bool variable 'var' to catch the test result.
 * That is so the test result can be inspected more easily in a debugger.
 * The test is done twice, to keep the assert message meaningful,
 * but assertion tests shouldn't have side effects anyway!
 */
#define GPU_ASSERT_RETURN(test, var, ret) \
    do {                                  \
        var = (bool)(test);               \
        GPU_ASSERT(test);                 \
        if (!var)                         \
            return ret;                   \
    } while (0)


size_t GPU_print_error(char *__restrict out, size_t size, const char *__restrict format, ... ) ATTR_NONNULL(1, 3) ATTR_PRINTF_FORMAT(4, 5);

const char *gpuErrorString(GLenum err); /* replacement for gluErrorString */


#ifdef __cplusplus
}
#endif


#endif /* __GPU_DEBUG_H__ */
