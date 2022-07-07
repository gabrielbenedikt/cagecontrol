#ifndef HELPER_H
#define HELPER_H

/*! \file */

#include "debug.h"
#include "defines.h"
#include <QMessageBox>

/*!
 * \namespace helper
 * \brief contains small functions to display messages
 * \bug There are no known bugs.
 */
namespace helper {

/*!
 * \brief message displays a message box
 * \param msg the message to be displayed
 */
void message(QString msg);

/*!
 * \brief error  displays an error-messagebox and writes a debug_error message to stdout
 * \param msg the message to be displayed
 */
void error(QString msg);

/*!
 * \brief warning  displays warning-messagebox and writes a debug_warning message to stdout
 * \param msg the message to be displayed
 */
void warning(QString msg);

/*!
 * \brief info displays an info-messagebox and writes a debug_info message to stdout
 * \param msg the message to be displayed
 */
void info(QString msg);

std::string str_tolower(std::string s);
}

#endif // HELPER_H
