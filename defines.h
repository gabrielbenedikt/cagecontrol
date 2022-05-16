#ifndef DEFINES_H
#define DEFINES_H

#define DEBUGSPECTROMETERCONFIG FALSE


/*!
 * \file defines.h
 * \brief Various compile-time definitions.
 * \bug There are no known bugs.
 *
 * Contains definitions of various kind - mathematical, version constants, debug-variables, ...
 */

/*!
 * \def UNUSED()
 *
 * Use UNUSED(var) in function f(...,type var,...) to silence compiler warnings about unused parameter var.
 */
#define UNUSED(expr) do { (void)(expr); } while (0)

/*!
 * \def DEBUG
 *
 * Enables the execution of various debug-paths used during development.\n
 * default: FALSE.
 */
/*!
 * \def DEBUGERROR
 *
 * If set to TRUE, enables the execution of the DEBUG_ERROR() makro which is used to write
 * error messages (critical) to stdout.\n
 * default: true
 */
/*!
 * \def DEBUGWARNING
 * If set to TRUE, enables the execution of the DEBUG_WARNING() makro which is used to write
 * warnings about unexpected behaviour to stdout.\n
 * default: true
 */
/*!
 * \def DEBUGINFO
 * If set to TRUE, enables the execution of the DEBUG_INFO() makro which is used to write
 * usefull information to stdout.\n
 * default: true
 */
#define DEBUG True
#define DEBUGERROR 1
#define DEBUGWARNING 1
#define DEBUGINFO 1

/*!
 * \def EPS
 * 'epsilon' used to check floatingpoint variables in if-conditions.\n
 * default: 0.0000001
 */
#define EPS 0.0000001

/*!
 * \def PI
 * Pi.\n
 * default: 3.14159265358979323846
 */
#define PI 3.14159265358979323846

/*!
 * \def DEGTORAD
 * Conversion factor from degree to radians.
 * PI/180
 */
/*!
 * \def RADTODEG
 * Conversion factor from radians to degree.
 * 180/PI
 */
#define DEGTORAD PI/180
#define RADTODEG 180/PI

// comiler quirks
#if defined(_WIN32)
// Note:
//   Visual Studio 6  --> _MSC_VER = 1200
//   Visual Studio 7  --> _MSC_VER = 1300
//   Visual Studio 2003 (MSVC++ 7.1)  --> _MSC_VER = 1310
//   Visual Studio 2005 (MSVC++ 8.0)  --> _MSC_VER = 1400
//   Visual Studio 2008 (MSVC++ 9.0)  --> _MSC_VER = 1500
//   Visual Studio 2010 (MSVC++ 10.0) --> _MSC_VER = 1600
//   Visual Studio 2012 (MSVC++ 11.0) --> _MSC_VER = 1700
//   Visual Studio 2013 (MSVC++ 12.0) --> _MSC_VER = 1800
//   Visual Studio 2015 (MSVC++ 14.0) --> _MSC_VER = 1900
//   Visual Studio 2017v15.0 (MSVC++ 14.1) --> _MSC_VER = 1910
//   Visual Studio 2017v15.3 (MSVC++ 14.11) --> _MSC_VER = 1911
//   Visual Studio 2017v15.5 (MSVC++ 14.12) --> _MSC_VER = 1912
//   Visual Studio 2017v15.6 (MSVC++ 14.13) --> _MSC_VER = 1913
//   Visual Studio 2017v15.7 (MSVC++ 14.14) --> _MSC_VER = 1914
//   Visual Studio 2017v15.8 (MSVC++ 14.15) --> _MSC_VER = 1915
//   Visual Studio 2017v15.9 (MSVC++ 14.16) --> _MSC_VER = 1916
#if defined (_MSC_VER)  // Microsoft Visual Studio
#pragma comment(lib, "ws2_32.lib")
#endif
#ifndef _STDINT
#ifndef _STDINT_H
#if 1
#if (_MSC_VER < 1900)
typedef unsigned char    uint8_t;
typedef unsigned short   uint16_t;
typedef unsigned long    uint32_t;
typedef unsigned __int64 uint64_t;

typedef signed char      int8_t;
typedef signed short     int16_t;
typedef signed int       int32_t;
typedef __int64          int64_t;
#else
#include <stdint.h>
#endif
#else
      typedef unsigned char    uint8_t;
      typedef unsigned short   uint16_t;
      typedef unsigned int     uint32_t;

      typedef __int8      int8_t;
      typedef __int16     int16_t;
      typedef __int32     int32_t;
      typedef __int64     int64_t;
#endif
#endif
#endif
#endif

#endif // DEFINES_H
