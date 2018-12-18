#ifndef MOTOR_H
#define MOTOR_H

/*! \file */

#include <QDialog>
#include <QtCore/QtGlobal>
#include <QtSerialPort/QSerialPort>
#include <QObject>
#include <QTimer>
#include "defines.h"

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

namespace Ui {
class SettingsDialog;
}

class QIntValidator;

QT_END_NAMESPACE

/*!
 * \brief The Motor class operates the PCB-motor
 * \bug There are no known bugs.
 *
 * The PCBMotor is controllable by sending ASCII commands over a serial connection. This class establishes such a connection
 * and controls the movements of the motor.
 */
class Motor : public QObject {
    Q_OBJECT

private:
    QTimer hometimer;               //!<Used to iterate through the steps of 'go to the starting position' - but in an inaccurate way. \see command_home()
    QTimer bothtimer;               //!<Used to iterate through the steps of moving two motors of one controller
    //int movebothstep;               //!<Controls logic flow when two motors are to be moved consecutively
    bool serialconnectionok;        //!<False if opening the serial connection failed
    uint16_t motor1steps;           //!<number of stept the 1st motor is to be moved
    uint16_t motor2steps;           //!<number of stept the 2nd motor is to be moved
public:
    QString publicmotorstatusmessage;   //!<A string containing the current state of the serial connection

    QSerialPort *serial;                //!<Qt serial connection interface
    /*!
     * \brief Motor the contructor initializes variables and establishes the serial connection.
     */
    Motor();
    ~Motor();

    /*!
     * \brief sensordata returns the current PCBMotor optical encoder wheel sendor state
     * \return the current PCBMotor optical encoder wheel sendor state
     */
    bool sensordata();

signals:
    /*!
     * \brief motorstatusmessage emitted when the status of the serial connection changes, with a string indicating the actual state.
     * \param message the message
     */
    void motorstatusmessage(const QString &message);
    /*!
     * \brief emitted when serial connection is closed
     */
    void ConnectionClosed();

public slots:
    /*!
     * \brief open establishes a connection over a serial port
     */
    void open(QString port);

    /*!
     * \brief close closes the serialport connection
     */
    void close();

    /*!
     * \brief read reads from the serial port
     */
    void read();

    /*!
     * \brief write writes to the serialport
     * \param data data to be written to the serial port
     */
    void write(const QByteArray &data);

    /*!
     * \brief handleError prints an error message of the serialport connection and closes the connection
     * \param error
     */
    void handleError(QSerialPort::SerialPortError error);

    /*!
     * \brief showStatusMessage fills the label in the GUI with text
     * \param message the text to be shown in the label
     */
    void showStatusMessage(const QString &message);

    /*!
     * \brief isopen returns the state of the serial connection
     * \return true if serial connection was established successfully, false otherwise
     */
    bool isopen();

    /*!
     * \brief command_park moves the motor to the mechanical stop
     */
    void command_park();

    /*!
     * \brief command_home sends commands to position at the mechanical stop and afterwards go to the offset starting position, but in an inaccurate way
     */
    void command_home();

    /*!
     * \brief command_info sends the command to request the PCBMotor information
     */
    void command_info();

    /*!
     * \brief command_help sends the command to print the PCBMotor help
     */
    void command_help();

    /*!
     * \brief command_frequency_sweep sends the PCBMotor command for a frequency sweep
     */
    void command_frequency_sweep();

    /*!
     * \brief command_singlestep moves the motor a single step in a direction specified by dirstring
     * \param dirstring a string containing the desired movenent direction
     *
     * Dirstring may either be "bw" of "fw" for backward/forward movement.
     */
    void command_singlestep(QString dirstring);

    /*!
     * \brief command_step moves the motor numstep steps in a direction specified by dirstring
     * \param numsteps number of steps to go
     * \param dirstring direction to go
     *
     * Dirstring may either be "bw" of "fw" for backward/forward movement.
     */
    void command_step(uint16_t numsteps, QString dirstring);

    /*!
     * \brief command_microstep aplies nummsteps micropulses to the motor
     * \param nummsteps number of micropulses to apply
     * \param dirstring string containing the desired direction
     *
     * dirstring may either be "bw" of "fw" for backward/forward movement.
     */
    void command_microstep(uint16_t nummsteps, QString dirstring);

    /*!
     * \brief stop Tries to stop movenents if possible
     * \param stop Input: True if movents shall be stopped if possible
     */
    void stop(bool stop);

    /*!
     * \brief command_moveboth moves both motors connected to the controller
     * \param ang1 angle motor 1 is to be moved to
     * \param ang2angle motor 2 is to be moved to
     */
    void command_moveboth(double ang1, double ang2);


private:
    /*!
     * \brief command_moveboth moves both motors connected to the controller
     */
    void moveboth();

};

QT_END_NAMESPACE

#endif // MOTOR_H
