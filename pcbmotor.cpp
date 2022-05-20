#include "pcbmotor.h"

/************************************************************************************************
*                                PCBMotor::Motor                                                *
************************************************************************************************/
PCBMotor::PCBMotor(std::string devname, std::vector<uint8_t> mids)
{
    DEBUG_INFO("New instance of PCBMotor created\n");

    bserial = new Boost_serial(devname, 19200,
                               boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
                               boost::asio::serial_port_base::character_size(8),
                               boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none),
                               boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
    bserial->setTimeout(boost::posix_time::seconds(5));

    movebothstep=0;
    movethreestep=0;
    motor1steps=0;
    motor1steps=0;

    for (uint8_t m : mids)
        std::cout  << (unsigned)m<< std::endl;

    open(devname);
}//PCBMotor::PCBMotor(void)

/************************************************************************************************
*                                PCBMotor::~Motor                                               *
************************************************************************************************/
PCBMotor::~PCBMotor()
{
    DEBUG_INFO("One instance of PCBMotor destructed\n");
}//PCBMotor::~PCBMotor()


void PCBMotor::read()
{
    response = bserial->readStringUntil("\r\n");
}

void PCBMotor::write(const std::string &data)
{
    bserial->writeString(data);
}

std::string PCBMotor::query(const std::string &data) {
    bserial->writeString(data);
    response = bserial->readStringUntil("\r\n");
    return response;
}

void PCBMotor::close()
{
    if (bserial->isOpen())
        bserial->close();
}

bool PCBMotor::isopen()
{
    return bserial->isOpen();
}

void PCBMotor::open(std::string port)
{
    try {
         bserial->open(port, 19200,
                       boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
                       boost::asio::serial_port_base::character_size(8),
                       boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none),
                       boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
     }  catch (std::exception & ex) {
         std::cout << ex.what() << std::endl;
     }
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
void PCBMotor::command_move(int mnum, double ang)
{
    motor1idx = mnum;
    motor1steps=ang*2880/360;
    movebothstep=0;

    move();
}

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
void PCBMotor::move()
{
    //DEBUG_INFO("%d %s %s%s, Going ", numsteps, "steps in direction: ", dirstring.toLocal8Bit().data(), "\n");
    std::string commandText;

    commandText = "M"+ std::to_string(motor1idx) +",s-2880,s";
    commandText.append(std::to_string(motor1steps));
    commandText.append("\r");
    write(commandText.data());
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
