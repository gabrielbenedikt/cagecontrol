#ifndef MOTORWRAPPER_H
#define MOTORWRAPPER_H

#include "pcbmotor.h"
#include "elliptec.h"
#include <string>

class motorwrapper
{
public:
    motorwrapper(uint8_t intype, std::vector<uint8_t> mids);
    void open(std::string port);
    void close();

    void command_moveboth(int hwp_mnum, int qwp_mnum, double hwpang, double qwpang);
    void command_movethree(int hwp_mnum, int qwp_mnum, int qwp2_mnum, double hwp_ang, double qwp_ang, double qwp2_ang);

private:
    rotmotor *m;
};

#endif // MOTORWRAPPER_H
