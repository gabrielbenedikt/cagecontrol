#ifndef ELLIPTEC_H
#define ELLIPTEC_H

/*! \file */

#include "defines.h"
#include "rotmotor.h"
#include "boost_serial.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <optional>
#include <sstream>
#include <stdio.h>
#include <string>
#include <thread>

struct ell_device {
    std::string address;    //address of device on controller
    uint16_t type;          //type of device
    uint64_t serial;        //serial number
    uint16_t year;          //manufacturing year
    uint8_t fw;             //firmware revision
    uint8_t hw;             //firmware revision
    uint32_t travel;        //units of travel range
    uint64_t pulses;        //pulses per unit
};

struct ell_response {
    uint8_t address = 0;
    std::string type = "";
    std::string data = "";
};

/*!
 * \brief The Motor class operates Thorlabs Elliptec-motors
 * \bug There are no known bugs.
 *
 */


class elliptec : public rotmotor{

public:
    /*!
     * \brief Motor the contructor initializes variables and establishes the serial connection.
     */
    elliptec(std::vector<uint8_t> inmids = std::vector<uint8_t>(0), std::string devname = "");
    ~elliptec();

public:
    void write(const std::string &data);
    void home(std::string addr, std::string dir);
    void move_absolute(std::string addr, double pos);
    void move_relative(std::string addr, double pos);
    void get_position(std::string addr);
    void get_status(std::string addr);
    void get_velocity(std::string addr);
    void set_velocity(std::string addr, uint8_t percent);
    void save_userdata(std::string addr);
    void change_address(std::string addr, std::string newaddr);
    void command_moveboth(int hwp_mnum, int qwp_mnum, double hwpang, double qwpang);
    void command_movethree(int hwp_mnum, int qwp_mnum, int qwp2_mnum, double hwpang, double qwpang, double qwp2ang);

private:
    void read();
    void close();
    bool isopen();
    void open(std::string port);
    std::string query(const std::string &data);
    Boost_serial *bserial;

    std::string response;
    // Direction constants
    const uint8_t CW = 0;
    const uint8_t CCW = 1;

    std::unordered_map<std::string, std::vector<uint8_t>> devtype;

    // Acceptable accuracy
    const double DEGERR = 0.1;
    const double MMERR = 0.05;


    std::vector<std::string> mids;      //!< motor ids
    std::vector<ell_device> devices;
    void init();
    bool devintype(std::string type, uint8_t id);
    std::optional<ell_device> devinfo_at_addr(std::string addr);
    int64_t deg2step(std::string addr, double deg);
    int64_t mm2step(std::string addr, double mm);
    double step2deg(std::string addr, int64_t step);
    double step2mm(std::string addr, int64_t step);
    std::string step2hex(int64_t step);
    int64_t hex2step(std::string hex);
    std::string ll2hex(int64_t i);

    void getinfo(std::string addr);
    void get_motor1_info(std::string addr);
    void get_motor2_info(std::string addr);
    void get_motor3_info(std::string addr);
    void search_motor1_freq(std::string addr);
    void search_motor2_freq(std::string addr);
    void search_motor3_freq(std::string addr);
    void optimize_motors(std::string addr);
    void clean_mechanics(std::string addr);
    void stop_clean(std::string addr);

    ell_response process_response();
    void handle_devinfo(ell_device dev);
    void print_dev_info(ell_device dev);
    std::string int2addr(uint8_t id);

};

#endif // ELLIPTEC_H
