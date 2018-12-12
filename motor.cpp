#include "debug.h"
#include "motor.h"
#include "defines.h"

#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QIntValidator>
#include <QLineEdit>
#include <QMessageBox>
#include <QString>
#include <QDialog>
#include <QDebug>
#include <QLabel>
#include <QDir>
QT_USE_NAMESPACE

/************************************************************************************************
*                                                                                               *
*                                 Motor::Motor                                                  *
*                                                                                               *
************************************************************************************************/
Motor::Motor(void)
{
    DEBUG_INFO("New instance of Motor created\n");
    serial = new QSerialPort(this);
    serial->setParity(QSerialPort::Parity(0));
    serial->setStopBits(QSerialPort::StopBits(1));
    serial->setDataBits(QSerialPort::DataBits(8));
    serial->setBaudRate(QSerialPort::BaudRate(19200));
    serial->setFlowControl(QSerialPort::FlowControl(0));
    serialconnectionok = false;

    hometimer.setSingleShot(true);

    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &Motor::handleError);
    connect(serial, &QSerialPort::readyRead, this, &Motor::read);

    connect(&hometimer, &QTimer::timeout, this, &Motor::command_home);
}//Motor::Motor(void)

/************************************************************************************************
*                                                                                               *
*                                 Motor::~Motor                                                 *
*                                                                                               *
************************************************************************************************/
Motor::~Motor()
{
    DEBUG_INFO("One instance of Motor destructed\n");
    delete serial;
}//Motor::~Motor()

void Motor::open(QString port)
{
    DEBUG_INFO("called\n");

    serial->setPortName(port);
    if (serial->open(QIODevice::ReadWrite)) {
        showStatusMessage(QObject::tr("Connected to %1").arg(port));
    /*
        showStatusMessage(QObject::tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
        */
        serialconnectionok=true;
    } else {
        DEBUG_ERROR("serial connection could not be established.\n");
        qDebug()<<"Error"<<serial->errorString();
        //QMessageBox::critical(this, QObject::tr("Error"), serial->errorString());
        serialconnectionok = false;
        showStatusMessage(QObject::tr("Open error"));
    }
    DEBUG_INFO("reached End\n");
}
void Motor::close()
{

}
void Motor::read()
{ }
void Motor::write(const QByteArray &data)
{ }
void Motor::handleError(QSerialPort::SerialPortError error)
{ }
void Motor::showStatusMessage(const QString &message)
{ }
bool Motor::isopen()
{ }
void Motor::command_park()
{ }
void Motor::command_home()
{ }
void Motor::command_info()
{ }
void Motor::command_help()
{ }
void Motor::command_frequency_sweep()
{ }
void Motor::command_singlestep(QString dirstring)
{ }
void Motor::command_step(uint16_t numsteps, QString dirstring)
{ }
void Motor::command_microstep(uint16_t nummsteps, QString dirstring)
{ }
void Motor::stop(bool stop)
{ }
