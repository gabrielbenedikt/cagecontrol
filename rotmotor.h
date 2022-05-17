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

    rotmotor();
    ~rotmotor();

private:

private slots:

    /*!
     * \brief handleError prints an error message of the serialport connection and closes the connection
     * \param error
     */
    virtual void handleError(QSerialPort::SerialPortError error) = 0;

public slots:

    /*!
     * \brief close closes the serialport connection
     */
    virtual void close() = 0;
    /*!
     * \brief open establishes a connection over a serial port
     */
    virtual void open(std::string port) = 0;

    /*!
     * \brief isopen returns the state of the serial connection
     * \return true if serial connection was established successfully, false otherwise
     */
    virtual bool isopen() =0;

    virtual void command_moveboth(int a, int b, double c, double d) = 0;
    virtual void command_movethree(int a, int b, int c, double d, double e, double f) = 0;

};

#endif // ROTMOTOR_H
