#ifndef DEBUG_H
#define DEBUG_H

/*! \file debug.h
 * \brief contains debug macros
 * \bug Printing to console does not work on Windows. Workaround: Redirect stderr to stdout and redirect stdout to a file.
 *
 * This file defines macros to style and simplify output to console.
 */

#include "stdio.h"
#include "defines.h"

#if defined(_WIN32)
/*!
 * \def DEBUG_INFO(fmt, ...)
 *
 * Prints Filename, Line, Function from which call takes place, and message.
 * Accepts additional arguments to include into the message, using the fprintf-style
 * Used for noncritical information.
 */
/*!
 * \def DEBUG_WARNING(fmt, ...)
 *
 * Prints Filename, Line, Function from which call takes place, and message.
 * Accepts additional arguments to include into the message, using the fprintf-style
 * Used for Warnings.
 */
/*!
 * \def DEBUG_ERROR(fmt, ...)
 *
 * Prints Filename, Line, Function from which call takes place, and message.
 * Accepts additional arguments to include into the message, using the fprintf-style
 * Used for Errors.
 */
#define DEBUG_INFO(fmt, ...) \
    do {if (DEBUGINFO) fprintf(stdout, "%s %s:%d:%s: " fmt, "(II)", __FILE__, __LINE__, Q_FUNC_INFO, ##__VA_ARGS__); } while (0)

#define DEBUG_WARNING(fmt, ...) \
    do {if (DEBUGWARNING) fprintf(stdout, "%s %s:%d:%s: " fmt, "(WW)", __FILE__, __LINE__, Q_FUNC_INFO, ##__VA_ARGS__); } while (0)

#define DEBUG_ERROR(fmt, ...) \
    do {if (DEBUGERROR) fprintf(stdout, "%s %s:%d:%s: " fmt, "(EE)", __FILE__, __LINE__, Q_FUNC_INFO, ##__VA_ARGS__); } while (0)
#elif defined (__linux__) || defined(__MacOSX__)
/*!
 * \def DEBUG_INFO(fmt, ...)
 *
 * Prints Filename, Line, Function from which call takes place, and message.
 * Accepts additional arguments to include into the message, using the fprintf-style
 * Used for noncritical information.
 */
/*!
 * \def DEBUG_WARNING(fmt, ...)
 *
 * Prints Filename, Line, Function from which call takes place, and message.
 * Accepts additional arguments to include into the message, using the fprintf-style
 * Used for Warnings.
 */
/*!
 * \def DEBUG_ERROR(fmt, ...)
 *
 * Prints Filename, Line, Function from which call takes place, and message.
 * Accepts additional arguments to include into the message, using the fprintf-style
 * Used for Errors.
 */
#define DEBUG_INFO(fmt, ...) \
    do {if (DEBUGINFO) fprintf(stderr, "%s %s:%d:%s: " fmt, "(II)", __FILE__, __LINE__, Q_FUNC_INFO, ##__VA_ARGS__); } while (0)

#define DEBUG_WARNING(fmt, ...) \
    do {if (DEBUGWARNING) fprintf(stderr, "%s %s:%d:%s: " fmt, "(WW)", __FILE__, __LINE__, Q_FUNC_INFO, ##__VA_ARGS__); } while (0)

#define DEBUG_ERROR(fmt, ...) \
    do {if (DEBUGERROR) fprintf(stderr, "%s %s:%d:%s: " fmt, "(EE)", __FILE__, __LINE__, Q_FUNC_INFO, ##__VA_ARGS__); } while (0)
#endif
#endif // DEBUG_H

