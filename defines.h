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
 * \ def CURRENT_SAVE_STRUCTURE_VERSION
 *
 * defines the current version of the savefile-structure. Used when reading/writing savefiles to assure
 * the right data is read/written from/into the file. The following versions are implemented:\n
 *
#define CURRENT_SAVE_STRUCTURE_VERSION 1
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

//Allow measurements with uncalibrated spectrometers
/*!
 * \def ALLOW_UNCALIBRATED_MEASUREMENT
 * Allow measurement-runs to be carried out although the spectrometer is uncalibrated.\n
 * default: FALSE
 */
#define ALLOW_UNCALIBRATED_MEASUREMENT FALSE

/*!
 * \def CHAR_READ_MOTOR_MEMORY_POSITION_SPECTROMETER_TYPE
 * Command to access memory position on PCB-Motor controller in which major version of the Spectrometer is stored.
 */
/*!
 * \def CHAR_READ_MOTOR_MEMORY_POSITION_SPECTROMETER_SUBVERSION
 * Command to access memory position on PCB-Motor controller in which minor version of the Spectrometer is stored.
 */
#define CHAR_READ_MOTOR_MEMORY_POSITION_SPECTROMETER_TYPE "rd243"
#define CHAR_READ_MOTOR_MEMORY_POSITION_SPECTROMETER_SUBVERSION "rd245"

#endif // DEFINES_H
