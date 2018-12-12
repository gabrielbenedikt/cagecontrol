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
#define DEBUG true
#define DEBUGERROR true
#define DEBUGWARNING true
#define DEBUGINFO true

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

#endif // DEFINES_H
