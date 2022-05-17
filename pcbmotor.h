#ifndef PCBMOTOR_H
#define PCBMOTOR_H

/*! \file */

#include "defines.h"
#include "rotmotor.h"
#include "boost_serial.h"

#include <chrono>
#include <future>
#include <string>
#include <thread>
#include <vector>

/*!
 * \brief The Motor class operates the PCB-motor
 * \bug There are no known bugs.
 *
 * The PCBMotor is controllable by sending ASCII commands over a serial connection. This class establishes such a connection
 * and controls the movements of the motor.
 */
class PCBMotor : public rotmotor{
    Q_OBJECT
private:
    int movebothstep;               //!<Controls logic flow when two motors are to be moved consecutively
    int movethreestep;              //!<Controls logic flow when two motors are to be moved consecutively
    bool serialconnectionok;        //!<False if opening the serial connection failed
    uint16_t motor1steps;           //!<number of steps the 1st motor is to be moved
    uint16_t motor2steps;           //!<number of steps the 2nd motor is to be moved
    uint16_t motor3steps;           //!<number of steps the 3rd motor is to be moved
    uint16_t motor1idx;             //!<controller index of motor 1
    uint16_t motor2idx;             //!<controller index of motor 2
    uint16_t motor3idx;             //!<controller index of motor 3
    QByteArray data;
    QSerialPort *serial;
    Boost_serial *bserial;
    std::string response;
public:

    /*!
     * \brief Motor the contructor initializes variables and establishes the serial connection.
     */
    PCBMotor(std::vector<uint8_t> mids = std::vector<uint8_t>(1), std::string devname="");
    ~PCBMotor();

    /*!
     * \brief sensordata returns the current PCBMotor optical encoder wheel sendor state
     * \return the current PCBMotor optical encoder wheel sendor state
     */
    bool sensordata();


public slots:
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
    void command_singlestep(std::string dirstring);

    /*!
     * \brief command_step moves the motor numstep steps in a direction specified by dirstring
     * \param numsteps number of steps to go
     * \param dirstring direction to go
     *
     * Dirstring may either be "bw" of "fw" for backward/forward movement.
     */
    void command_step(uint16_t numsteps, std::string dirstring);

    /*!
     * \brief command_microstep aplies nummsteps micropulses to the motor
     * \param nummsteps number of micropulses to apply
     * \param dirstring string containing the desired direction
     *
     * dirstring may either be "bw" of "fw" for backward/forward movement.
     */
    void command_microstep(uint16_t nummsteps, std::string dirstring);

    /*!
     * \brief stop Tries to stop movenents if possible
     * \param stop Input: True if movents shall be stopped if possible
     */
    void stop(bool stop);

    /*!
     * \brief command_moveboth moves both motors connected to the controller
     * \param hwp_mnum motor number of HWP
     * \param hwp_mnum motor number of QWP
     * \param hwpang angle HWP is to be moved to
     * \param qwpang angle QWP is to be moved to
     */
    void command_moveboth(int hwp_mnum, int qwp_mnum, double hwpang, double qwpang);

    /*!
     * \brief command_movethree moves three motors connected to the controller
     * \param hwp_mnum index of HWP
     * \param qwp_mnum index of QWP
     * \param qwp2_mnum index of QWP2
     * \param hwp_ang angle to rotate HWP to
     * \param qwp_ang angle to rotate QWP to
     * \param qwp2_ang angle to rotate QWP2 to
     */
    void command_movethree(int hwp_mnum, int qwp_mnum, int qwp2_mnum, double hwp_ang, double qwp_ang, double qwp2_ang);
private:
    /*!
     * \brief command_moveboth moves both motors connected to the controller
     */
    void moveboth();

    /*!
     * \brief command_movethree moves three motors connected to the controller
     */
    void movethree();

    void open(std::string port);
    void read();
    std::string query(const std::string &data);
    void close();
    bool isopen();
    /*!
     * \brief write writes to the serialport
     * \param data data to be written to the serial port
     */
    void write(const std::string &data);
};

QT_END_NAMESPACE

#endif // PCBMOTOR_H
