#ifndef ROTMOTOR_H
#define ROTMOTOR_H

#include <vector>
#include <iostream>
#include <memory>
#include "debug.h"
#include "defines.h"


class rotmotor {
public:

    rotmotor();
    ~rotmotor();


public:

    /*!
     * \brief close closes the serialport connection
     */
    virtual void close() = 0;
    /*!
     * \brief isopen returns the state of the serial connection
     * \return true if serial connection was established successfully, false otherwise
     */
    virtual bool isopen() =0;

    virtual void command_moveboth(int a, int b, double c, double d) = 0;
    virtual void command_movethree(int a, int b, int c, double d, double e, double f) = 0;

};

#endif // ROTMOTOR_H
