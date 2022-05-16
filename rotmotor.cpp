#include "rotmotor.h"

QT_USE_NAMESPACE

rotmotor::rotmotor()
{
    serial = new QSerialPort();
    serial->setParity(QSerialPort::Parity(0));
    serial->setStopBits(QSerialPort::StopBits(1));
    serial->setDataBits(QSerialPort::DataBits(8));
    serial->setFlowControl(QSerialPort::FlowControl(0));

    connect(serial, &QSerialPort::readyRead, this, &rotmotor::read);
}

rotmotor::~rotmotor()
{
    DEBUG_INFO("One instance of Motor destructed\n");
    delete serial;
}

void rotmotor::command_moveboth(int a, int b, double c, double d) {
    UNUSED(a);
    UNUSED(b);
    UNUSED(c);
    UNUSED(d);
}
void rotmotor::command_movethree(int a, int b, int c, double d, double e, double f){
    UNUSED(a);
    UNUSED(b);
    UNUSED(c);
    UNUSED(d);
    UNUSED(e);
    UNUSED(f);
}
/************************************************************************************************
*                                    rotmotor::close                                            *
************************************************************************************************/
void rotmotor::close()
{
    if (serial->isOpen())
        serial->close();
}

/************************************************************************************************
*                                    rotmotor::open                                             *
************************************************************************************************/
void rotmotor::open(std::string port)
{
    DEBUG_INFO("called\n");

    serial->setPortName(QString::fromStdString(port));
    if (serial->open(QIODevice::ReadWrite)) {
    } else {
        DEBUG_ERROR("serial connection could not be established.\n");
        qDebug()<<"Error"<<serial->errorString();
    }
    DEBUG_INFO("reached End\n");
}

/************************************************************************************************
*                                    rotmotor::isopen                                           *
************************************************************************************************/
bool rotmotor::isopen()
{
    return serial->isOpen();
}

/************************************************************************************************
*                                    rotmotor::read                                             *
************************************************************************************************/
void rotmotor::read()
{
    data.append(serial->readAll());
    while (serial->waitForReadyRead(500))
        data.append(serial->readAll());
    response = std::string(data);
    qDebug() << "response in rotmotor" << QString::fromStdString(response);
    data.clear();
}

/************************************************************************************************
*                                    rotmotor::write                                            *
************************************************************************************************/
void rotmotor::write(const QByteArray &data)
{
    #if DEBUG
        qDebug()<<serial->portName()<<":write\t"<<QString(data);
    #endif
    serial->write(data);
    serial->waitForBytesWritten(0);//you tell me wh
    serial->waitForBytesWritten(250);
    std::cout << "write to serial: " << std::string(data) << std::endl;
}

void rotmotor::init() {
}
/************************************************************************************************
*                                    rotmotor::handleError                                      *
************************************************************************************************/
void rotmotor::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        qDebug()<<"Critical Error"<<serial->errorString();
        close();
    }
}
