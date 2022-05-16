#ifndef ROTMOTOR_H
#define ROTMOTOR_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <vector>
#include <iostream>
#include <memory>
#include "debug.h"
#include "defines.h"

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

class rotmotor: public QObject {
    Q_OBJECT
public:

    std::string publicmotorstatusmessage;   //!<A string containing the current state of the serial connection
    rotmotor();
    ~rotmotor();
    QSerialPort *serial;                //!<Qt serial connection interface

    std::string response;

private:
    bool serialconnectionok;        //!<False if opening the serial connection failed
    uint8_t type;                   //!< 0: PCBMotor, 1: elliptec
    QByteArray data;

private slots:

    /*!
     * \brief handleError prints an error message of the serialport connection and closes the connection
     * \param error
     */
    void handleError(QSerialPort::SerialPortError error);
signals:
    /*!
     * \brief motorstatusmessage emitted when the status of the serial connection changes, with a string indicating the actual state.
     * \param message the message
     */
    void motorstatusmessage(const std::string &message);
    /*!
     * \brief emitted when serial connection is closed
     */
    void ConnectionClosed();

public slots:

    void read();
    /*!
     * \brief write writes to the serialport
     * \param data data to be written to the serial port
     */
    void write(const QByteArray &data);


    /*!
     * \brief close closes the serialport connection
     */
    void close();
    /*!
     * \brief open establishes a connection over a serial port
     */
    void open(std::string port);

    /*!
     * \brief showStatusMessage fills the label in the GUI with text
     * \param message the text to be shown in the label
     */
    void showStatusMessage(const std::string &message);

    /*!
     * \brief isopen returns the state of the serial connection
     * \return true if serial connection was established successfully, false otherwise
     */
    bool isopen();

    virtual void init();

    virtual void command_moveboth(int a, int b, double c, double d);
    virtual void command_movethree(int a, int b, int c, double d, double e, double f);

};

#endif // ROTMOTOR_H
