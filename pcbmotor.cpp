#include "pcbmotor.h"

QT_USE_NAMESPACE

/************************************************************************************************
*                                PCBMotor::Motor                                                *
************************************************************************************************/
PCBMotor::PCBMotor(std::vector<uint8_t> mids, std::string devname)
{
    DEBUG_INFO("New instance of PCBMotor created\n");

    serial = new QSerialPort();
    serial->setParity(QSerialPort::Parity(0));
    serial->setStopBits(QSerialPort::StopBits(1));
    serial->setDataBits(QSerialPort::DataBits(8));
    serial->setFlowControl(QSerialPort::FlowControl(0));
    serial->setBaudRate(19200);
    connect(serial, &QSerialPort::readyRead, this, &PCBMotor::read);

    movebothstep=0;
    movethreestep=0;
    motor1steps=0;
    motor1steps=0;

    for (uint8_t m : mids)
        std::cout  << (unsigned)m<< std::endl;
}//PCBMotor::PCBMotor(void)

/************************************************************************************************
*                                PCBMotor::~Motor                                               *
************************************************************************************************/
PCBMotor::~PCBMotor()
{
    DEBUG_INFO("One instance of PCBMotor destructed\n");
}//PCBMotor::~PCBMotor()

void PCBMotor::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        std::cout <<"Critical Error"<<serial->errorString().toStdString() << std::endl;
        close();
    }
}

void PCBMotor::read()
{
    data.append(serial->readAll());
    while (serial->waitForReadyRead(500))
        data.append(serial->readAll());
    response = std::string(data);
    std::cout  << "response in rotmotor" << response << std::endl;
    data.clear();
}

void PCBMotor::write(const QByteArray &data)
{
    #if DEBUG
        std::cout <<serial->portName()<<":write\t"<<QString(data);
    #endif
    serial->write(data);
    serial->waitForBytesWritten(0);//you tell me wh
    serial->waitForBytesWritten(250);
    std::cout << "write to serial: " << std::string(data) << std::endl;
}

void PCBMotor::close()
{
    if (serial->isOpen())
        serial->close();
}

bool PCBMotor::isopen()
{
    return serial->isOpen();
}

void PCBMotor::open(std::string port)
{
    DEBUG_INFO("called\n");

    serial->setPortName(QString::fromStdString(port));
    if (serial->open(QIODevice::ReadWrite)) {
    } else {
        DEBUG_ERROR("serial connection could not be established.\n");
        std::cout <<"Error"<<serial->errorString().toStdString() << std::endl;
    }
    DEBUG_INFO("reached End\n");
}


/************************************************************************************************
*                                PCBMotor::command_park                                         *
************************************************************************************************/
void PCBMotor::command_park()
{
    DEBUG_INFO("Going back to mechanical stop.");
    write("s2880\r");
}

/************************************************************************************************
*                                PCBMotor::command_home                                         *
************************************************************************************************/
void PCBMotor::command_home()
{ }

/************************************************************************************************
*                                PCBMotor::command_info                                         *
************************************************************************************************/
void PCBMotor::command_info()
{
    write("i\r");
}

/************************************************************************************************
*                                PCBMotor::command_help                                         *
************************************************************************************************/
void PCBMotor::command_help()
{
    write("h\r");
}

/************************************************************************************************
*                            PCBMotor::command_frequency_sweep                                  *
************************************************************************************************/
void PCBMotor::command_frequency_sweep()
{
    write("p\r");
}

/************************************************************************************************
*                             PCBMotor::command_singlestep                                      *
************************************************************************************************/
void PCBMotor::command_singlestep(std::string dirstring)
{
    DEBUG_INFO("%s%s, Going 1 step in direction: ", dirstring.data(), "\n");
    std::string commandText;

    commandText = "s";

    if (dirstring=="bw") {
        commandText.append("-");
    }
    commandText.append(std::to_string(1));
    commandText.append("\r");
    write(commandText.data());
}

/************************************************************************************************
*                                  PCBMotor::command_step                                       *
************************************************************************************************/
void PCBMotor::command_step(uint16_t numsteps, std::string dirstring)
{
    DEBUG_INFO("%d %s %s%s, Going ", numsteps, "steps in direction: ", dirstring.data(), "\n");
    std::string commandText;

    commandText = "s";
    if (dirstring=="bw")
        commandText.append("-");
    commandText.append(std::to_string(numsteps));
    commandText.append("\r");
    write(commandText.data());
}

/************************************************************************************************
*                             PCBMotor::command_microstep                                       *
************************************************************************************************/
void PCBMotor::command_microstep(uint16_t nummsteps, std::string dirstring)
{
    DEBUG_INFO("%d %s %s%s, Going ", nummsteps, "microsteps in direction: ", dirstring.data(), "\n");
    std::string commandText;

    commandText = "u";
    if (dirstring=="bw")
        commandText.append("-");
    commandText.append(std::to_string(nummsteps));
    commandText.append("\r");
    write(commandText.data());
}

/************************************************************************************************
*                                    PCBMotor::stop                                             *
************************************************************************************************/
void PCBMotor::stop(bool stop)
{ }

/************************************************************************************************
*                              PCBMotor::command_moveboth                                       *
************************************************************************************************/
void PCBMotor::command_moveboth(int hwp_mnum, int qwp_mnum, double hwpang, double qwpang)
{
    motor1idx = hwp_mnum;
    motor2idx = qwp_mnum;
    motor1steps=hwpang*2880/360;
    motor2steps=qwpang*2880/360;
    movebothstep=0;

    moveboth();
}

/************************************************************************************************
*                              PCBMotor::command_movethree                                      *
************************************************************************************************/
void PCBMotor::command_movethree(int hwp_mnum, int qwp_mnum, int qwp2_mnum, double hwp_ang, double qwp_ang, double qwp2_ang)
{
    motor1idx = hwp_mnum;
    motor2idx = qwp_mnum;
    motor3idx = qwp2_mnum;
    motor1steps=hwp_ang*2880/360;
    motor2steps=qwp_ang*2880/360;
    motor3steps=qwp2_ang*2880/360;
    movethreestep=0;

    movethree();
}

/************************************************************************************************
*                                  PCBMotor::moveboth                                           *
************************************************************************************************/
void PCBMotor::moveboth()
{
    //DEBUG_INFO("%d %s %s%s, Going ", numsteps, "steps in direction: ", dirstring.toLocal8Bit().data(), "\n");
    std::string commandText;

    commandText = "M"+ std::to_string(motor1idx) +",s-2880,s";
    commandText.append(std::to_string(motor1steps));
    commandText.append(",M"+std::to_string(motor2idx)+",s-2880,s");
    commandText.append(std::to_string(motor2steps));
    commandText.append("\r");
    write(commandText.data());
}

/************************************************************************************************
*                                  PCBMotor::movethree                                          *
************************************************************************************************/
void PCBMotor::movethree()
{
    //DEBUG_INFO("%d %s %s%s, Going ", numsteps, "steps in direction: ", dirstring.toLocal8Bit().data(), "\n");
    std::string commandText;

    commandText = "M"+std::to_string(motor1idx)+",s-2880,s";
    commandText.append(std::to_string(motor1steps));
    commandText.append(",M"+std::to_string(motor2idx)+",s-2880,s");
    commandText.append(std::to_string(motor2steps));
    commandText.append(",M"+std::to_string(motor3idx)+",s-2880,s");
    commandText.append(std::to_string(motor3steps));
    commandText.append("\r");
    write(commandText.data());
}
