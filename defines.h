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
 * \def CURRENT_SAVE_STRUCTURE_VERSION
 *
 * defines the current version of the savefile-structure. Used when reading/writing savefiles to assure
 * the right data is read/written from/into the file. The following versions are implemented:\n
 * 1  :Step && FrequencyAtStep\n
 * 11 :Step && FrequencyAtStep && Sensordata\n
 * 2  :BP && WavelengthAtBP && FrequencyAtBP\n
 * 21 :BP && WavelengthAtBP && FrequencyAtBP_EC\n
 * 210:StepsInBP\n
 * 211:WavelengthAtStep_EC\n
 * 212:WavelengthAtBP && FrequencyAtBP_EC && StepsInBP_EC
 */
#define CURRENT_SAVE_STRUCTURE_VERSION 21

/*!
 * \def SAVE_WITH_SIBP
 * when this is set to true, the number of steps in each Bandpass "StepsInBandpass[i]" is written into the savefile.\n
 * default: false
 */
/*!
 * \def SAVESENSORDATA
 * when this is set to true, the PCB-motor sensordata at each step "SensordataAtStep[i]" is written into a separate file,
 * but only during measurement runs.\n
 * default: false
 */
/*!
 * \def SAVESENSORDATA_CALIBRATION
 * when this is set to true, the PCB-motor sensordata at each step "SensordataAtStep[i]" is written into the savefile,
 * but only during calibration runs.\n
 * default: true
 */
/*!
 * \def SAVETHEREST
 * when this is set to true, the PCB-motor sensordata number of steps in each Bandpass "StepsInBandpass[i]" and the
 * Wavelength at each step "WavelengthAtStep[i]" is written into a separate file.\n
 * default: FALSE
 */
//#define SAVE_WITH_SIBP TRUE
//#define SAVESENSORDATA TRUE
//#define SAVESENSORDATA_CALIBRATION TRUE
//#define SAVETHEREST TRUE

/*!
 * \def CALIBRATION_RUNS_NEEDED
 * Defines the number of datasets needed to calibrate the spectrometer.
 * Used for old routines which tried to calculate the stepsize in prior.\n
 * default: 5
 */
/*!
 * \def CALIBRATION_RUNS_NEEDED_ED
 * Defines the number of datasets needed to calibrate the spectrometer when Edge-detection is used.\n
 * default: 2
 */
#define CALIBRATION_RUNS_NEEDED 5
#define CALIBRATION_RUNS_NEEDED_ED 3

/*!
 * \def CALIBRATE_APPROXIMATELY_WITH_ARTIFICIAL_DATA
 *
 * This switch is useful during development/debugging of the calibration routines.
 * The maxima and their positions can be entered in the code instead of being calculated out of saved datasets.\n
 * default: FALSE
 */
#define CALIBRATE_APPROXIMATELY_WITH_ARTIFICIAL_DATA FALSE

//Should edge-detection be used during calibration?
/*!
 * \def CALIBRATE_WITH_EDGE_DETECTION
 * Specifies if the sensordata of the PCB-motor is used to calculate the scanmove-stepsize at runtime.\n
 * default: TRUE
 */
#define CALIBRATE_WITH_EDGE_DETECTION TRUE

/*!
 * \def DEBUG
 *
 * Enables the execution of various debug-paths used during development.\n
 * default: FALSE.
 */
#define DEBUG true
/*!
 * \def DEBUGERROR
 *
 * If set to TRUE, enables the execution of the DEBUG_ERROR() makro which is used to write
 * error messages (critical) to stdout.\n
 * default: true
 */
#define DEBUGERROR true
/*!
 * \def DEBUGWARNING
 *
 * If set to TRUE, enables the execution of the DEBUG_WARNING() makro which is used to write
 * warnings about unexpected behaviour to stdout.\n
 * default: true
 */
#define DEBUGWARNING true
/*!
 * \def DEBUGINFO
 *
 * If set to TRUE, enables the execution of the DEBUG_INFO() makro which is used to write
 * usefull information to stdout.\n
 * default: true
 */
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
