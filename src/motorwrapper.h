#ifndef MOTORWRAPPER_H
#define MOTORWRAPPER_H

#include "../libpcbm/include/pcbmotor.h"
#include "../elliptecpp/include/elliptec.h"
#include "defines.h"
#include <string>
#include <vector>

class motorwrapper
{
public:
    motorwrapper(uint8_t intype, std::string devname, std::vector<uint8_t> mids);
    ~motorwrapper();

    void open(std::string port);
    void close();

    void move_absolute(int mnum, double ang);
    void command_moveboth(int hwp_mnum, int qwp_mnum, double hwpang, double qwpang);
    void command_movethree(int hwp_mnum, int qwp_mnum, int qwp2_mnum, double hwp_ang, double qwp_ang, double qwp2_ang);

    enum devlist {
        DEV_UNKNOWN = -1,
        DEV_ELLIPTEC = 0,
        DEV_PCBM = 1
    };

private:
    uint8_t _devtype;
    elliptec *em;
    PCBMotor *pm;
};

#endif // MOTORWRAPPER_H
