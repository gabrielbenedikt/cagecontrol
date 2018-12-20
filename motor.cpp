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
*                                 Motor::Motor                                                  *
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
    bothtimer.setSingleShot(true);

    connect(&bothtimer, &QTimer::timeout, this, &Motor::moveboth);

    movebothstep=0;
    motor1steps=0;
    motor1steps=0;
}//Motor::Motor(void)

/************************************************************************************************
*                                 Motor::~Motor                                                 *
************************************************************************************************/
Motor::~Motor()
{
    DEBUG_INFO("One instance of Motor destructed\n");
    delete serial;
}//Motor::~Motor()

/************************************************************************************************
*                                    Motor::open                                                *
************************************************************************************************/
void Motor::open(QString port)
{
    DEBUG_INFO("called\n");

    serial->setPortName(port);
    if (serial->open(QIODevice::ReadWrite)) {
        showStatusMessage(QObject::tr("Connected to %1").arg(port));
        serialconnectionok=true;
    } else {
        DEBUG_ERROR("serial connection could not be established.\n");
        qDebug()<<"Error"<<serial->errorString();
        serialconnectionok = false;
        showStatusMessage(QObject::tr("Open error"));
    }
    DEBUG_INFO("reached End\n");
}

/************************************************************************************************
*                                    Motor::close                                               *
************************************************************************************************/
void Motor::close()
{
    if (serial->isOpen())
        serial->close();
    serialconnectionok = false;

    emit(ConnectionClosed());
}

/************************************************************************************************
*                                    Motor::read                                                *
************************************************************************************************/
void Motor::read()
{ }

/************************************************************************************************
*                                    Motor::write                                               *
************************************************************************************************/
void Motor::write(const QByteArray &data)
{
    #if DEBUG
        qDebug()<<serial->portName()<<":write\t"<<QString::fromLocal8Bit(data);
    #endif
    serial->write(data);
}
/************************************************************************************************
*                                    Motor::open                                                *
************************************************************************************************/
void Motor::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        qDebug()<<"Critical Error"<<serial->errorString();
        close();
    }
}

/************************************************************************************************
*                               Motor::showStatusMessage                                        *
************************************************************************************************/
void Motor::showStatusMessage(const QString &message)
{
    publicmotorstatusmessage = message;
    emit motorstatusmessage(publicmotorstatusmessage);
}

/************************************************************************************************
*                                    Motor::isopen                                              *
************************************************************************************************/
bool Motor::isopen()
{
    return serialconnectionok;
}

/************************************************************************************************
*                                Motor::command_park                                            *
************************************************************************************************/
void Motor::command_park()
{
    DEBUG_INFO("Going back to mechanical stop.");
    write("s2880\r");
}

/************************************************************************************************
*                                Motor::command_home                                            *
************************************************************************************************/
void Motor::command_home()
{ }

/************************************************************************************************
*                                Motor::command_info                                            *
************************************************************************************************/
void Motor::command_info()
{
    write("i\r");
}

/************************************************************************************************
*                                Motor::command_help                                            *
************************************************************************************************/
void Motor::command_help()
{
    write("h\r");
}

/************************************************************************************************
*                            Motor::command_frequency_sweep                                     *
************************************************************************************************/
void Motor::command_frequency_sweep()
{
    write("p\r");
}

/************************************************************************************************
*                             Motor::command_singlestep                                         *
************************************************************************************************/
void Motor::command_singlestep(QString dirstring)
{
    DEBUG_INFO("%s%s, Going 1 step in direction: ", dirstring.toLocal8Bit().data(), "\n");
    QString commandText;

    commandText = "s";

    if (dirstring=="bw") {
        commandText.append("-");
    }
    commandText.append(QString::number(1));
    commandText.append(QChar::CarriageReturn);
    write(commandText.toLocal8Bit());
}

/************************************************************************************************
*                                  Motor::command_step                                          *
************************************************************************************************/
void Motor::command_step(uint16_t numsteps, QString dirstring)
{
    DEBUG_INFO("%d %s %s%s, Going ", numsteps, "steps in direction: ", dirstring.toLocal8Bit().data(), "\n");
    QString commandText;

    commandText = "s";
    if (dirstring=="bw")
        commandText.append("-");
    commandText.append(QString::number(numsteps));
    commandText.append(QChar::CarriageReturn);
    write(commandText.toLocal8Bit());
}

/************************************************************************************************
*                             Motor::command_microstep                                          *
************************************************************************************************/
void Motor::command_microstep(uint16_t nummsteps, QString dirstring)
{
    DEBUG_INFO("%d %s %s%s, Going ", nummsteps, "microsteps in direction: ", dirstring.toLocal8Bit().data(), "\n");
    QString commandText;

    commandText = "u";
    if (dirstring=="bw")
        commandText.append("-");
    commandText.append(QString::number(nummsteps));
    commandText.append(QChar::CarriageReturn);
    write(commandText.toLocal8Bit());
}

/************************************************************************************************
*                                    Motor::stop                                                *
************************************************************************************************/
void Motor::stop(bool stop)
{ }

/************************************************************************************************
*                              Motor::command_moveboth                                          *
************************************************************************************************/
void Motor::command_moveboth(double ang1, double ang2)
{
    motor1steps=ang1*2880/360;
    motor2steps=ang2*2880/360;
    movebothstep=0;
    bothtimer.start(1);
}

/************************************************************************************************
*                                  Motor::moveboth                                              *
************************************************************************************************/
void Motor::moveboth()
{
#if 0
    qDebug()<<"been here. stepnum: "<<movebothstep;
    if (movebothstep==0) {
        write("M1\r");
        ++movebothstep;
        bothtimer.start(100);
    } else if (movebothstep==1) {
        command_park();
        ++movebothstep;
        bothtimer.start(4000);
    } else if (movebothstep==2) {
        command_step(motor1steps,"bw");
        ++movebothstep;
        bothtimer.start(4000);
    } else if (movebothstep==3) {
        write("M2\r");
        ++movebothstep;
        bothtimer.start(100);
    } else if (movebothstep==4) {
        command_park();
        ++movebothstep;
        bothtimer.start(4000);
    } else if (movebothstep==5) {
        command_step(motor2steps,"bw");
        movebothstep=0;
        motor1steps=0;
        motor2steps=0;
    } else {
        DEBUG_ERROR("movebothstep unhandled value %i",movebothstep);
    }
#else
    //DEBUG_INFO("%d %s %s%s, Going ", numsteps, "steps in direction: ", dirstring.toLocal8Bit().data(), "\n");
    QString commandText;

    commandText = "M1,s-2880,s";
    commandText.append(QString::number(motor1steps));
    commandText.append(",M2,s-2880,s");
    commandText.append(QString::number(motor2steps));
    commandText.append(QChar::CarriageReturn);
    write(commandText.toLocal8Bit());
#endif
}

