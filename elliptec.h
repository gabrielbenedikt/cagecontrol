#ifndef ELLIPTEC_H
#define ELLIPTEC_H

/*! \file */

#include "defines.h"
#include "rotmotor.h"
#include "boost_serial.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <optional>
#include <sstream>
#include <stdio.h>
#include <string>

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

enum ell_errors {
    OK = 0,
    COMM_TIMEOUT = 1,
    MECH_TIMEOUT = 2,
    COMMAND_ERR = 3,
    VAL_OUT_OF_RANGE = 4,
    MOD_ISOLATED = 5,
    MOD_OUT_OF_ISOL = 6,
    INIT_ERROR = 7,
    THERMAL_ERROR = 8,
    BUSY = 9,
    SENSOR_ERROR = 10,
    MOTOR_ERROR = 11,
    OUT_OF_RANGE = 12,
    OVER_CURRENT = 13,
    GENERAL_ERROR = 14
};

const std::vector<std::string> error_msgs = {
    "OK, no error",
    "Communication time out",
    "Mechanical time out",
    "Command error or not supported",
    "Value out of range",
    "Module isolated",
    "Module out of isolation",
    "Initializing error",
    "Thermal error",
    "Busy",
    "Sensor Error (May appear during self test. If code persists there is an error)",
    "Motor Error (May appear during self test. If code persists there is an error)",
    "Out of Range (e.g. stage has been instructed to move beyond its travel range)",
    "Over Current error",
    //14-255 Reserved
};

const std::unordered_map<std::string, std::vector<uint8_t>> dt = {
{"rotary", std::vector<uint8_t>({8, 14, 18})},
{"linear",  std::vector<uint8_t>({7, 10, 17, 20})},
{"linrot", std::vector<uint8_t>({7, 8, 10, 14, 17, 18, 20})},
{"indexed",  std::vector<uint8_t>({6, 9, 12})},
{"hasclean",  std::vector<uint8_t>({14, 17, 18, 20})}
};

class elliptec : public rotmotor{

public:
    /*!
     * \brief Motor the contructor initializes variables and establishes the serial connection.
     */
    elliptec(std::string devname = "", std::vector<uint8_t> inmids = std::vector<uint8_t>(0), bool dohome = true, bool freqsearch = true);
    ~elliptec();

public:
    void open(std::string port, bool dohome, bool freqsearch);
    void close();
    bool isopen();
    void home(std::string addr, std::string dir = "0");
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

    //public:
    //    const std::string& bar() const { return _bar; }
    //    void bar(const std::string& bar) { _bar = bar; }
    //private:
    //    std::string _bar;


    // Direction constants
    const uint8_t CW = 0;
    const uint8_t CCW = 1;


    // Acceptable accuracy
    const double DEGERR = 0.1;
    const double MMERR = 0.05;


    // serial
    std::string query(const std::string &data);
    Boost_serial *bserial;
    std::string response;
    void read();
    uint16_t _ser_timeout;



    std::unordered_map<std::string, std::vector<uint8_t>> devtype;


    std::vector<std::string> mids;      //!< motor ids
    std::vector<ell_device> devices;

    void write(const std::string &data);

    void getinfo(std::string addr);
    void get_motor1_info(std::string addr);
    void get_motor2_info(std::string addr);
    void get_motor3_info(std::string addr);
    void search_motor1_freq(std::string addr);
    void search_motor2_freq(std::string addr);
    void search_motor3_freq(std::string addr);
    void search_freq(std::string addr);
    void optimize_motors(std::string addr);
    void clean_mechanics(std::string addr);
    void stop_clean(std::string addr);

    ell_response process_response();
    void handle_devinfo(ell_device dev);
    void print_dev_info(ell_device dev);

    bool devintype(std::string type, uint8_t id);
    std::optional<ell_device> devinfo_at_addr(std::string addr);
    int64_t deg2step(std::string addr, double deg);
    int64_t mm2step(std::string addr, double mm);
    double step2deg(std::string addr, int64_t step);
    double step2mm(std::string addr, int64_t step);
    std::string step2hex(int64_t step);
    int64_t hex2step(std::string hex);
    std::string ll2hex(int64_t i);
    std::string int2addr(uint8_t id);

};

#endif // ELLIPTEC_H
