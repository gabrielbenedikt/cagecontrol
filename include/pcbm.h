#ifndef PCBM_H
#define PCBM_H

#include "boost_serial.h"

#include <cstdlib>
#include <exception>
#include <string>
#include <vector>
#include <iostream>

class PCBMotor {
private:
    Boost_serial *bserial;
public:

    PCBMotor(std::string devname="");
    ~PCBMotor();

    bool sensordata();

public:
    bool isopen();
    void info();
    void help();
    
    void home(uint8_t mnum);
    void home(std::vector<uint8_t> mnum);
    
    void stop(uint8_t mnum);                //for D, S, G, U
    void stop(std::vector<uint8_t> mnum);   //for D, S, G, U
    
    void frequency_sweep(uint8_t mnum);
    void frequency_sweep(std::vector<uint8_t> mnum);

    void micropulse(uint8_t mnum, int32_t nummpulses, std::string edge="");
    void micropulse(std::vector<uint8_t> mnum, std::vector<int32_t> numpulses, std::vector<std::string> edge=std::vector<std::string>());
    
    void sweeppulse(uint8_t mnum, int32_t numpulses, std::string edge="");
    void sweeppulse(std::vector<uint8_t> mnum, std::vector<int32_t> numpulses, std::vector<std::string> edge=std::vector<std::string>());
    
    void sweepstep(uint8_t mnum, int32_t numsteps);
    void sweepstep(std::vector<uint8_t> mnum, std::vector<int32_t> numsteps);

    void moveabsolute(uint8_t mnum, double ang);
    void moveabsolute(std::vector<uint8_t> mnum, std::vector<double> ang);

    void moverelative(uint8_t mnum, double ang);
    void moverelative(std::vector<uint8_t> mnum, std::vector<double> ang);
    
    void goto_pos(uint8_t mnum, double angle, bool restrack);
    void goto_pos(std::vector<uint8_t> mnum, std::vector<double> angle, std::vector<bool> restrack);
    
    void set_voltage(uint8_t mnum, uint16_t mV);
    void set_voltage(std::vector<uint8_t> mnum, std::vector<uint16_t> mV);
    uint16_t get_voltage(uint8_t mnum);
    std::vector<uint16_t> get_voltage(std::vector<uint8_t> mnum);
    
    void set_min_voltage(uint8_t mnum, uint16_t mV);
    void set_min_voltage(std::vector<uint8_t> mnum, std::vector<uint16_t> mV);
    uint16_t get_min_voltage(uint8_t mnum);
    std::vector<uint16_t> get_min_voltage(std::vector<uint8_t> mnum);
    
    void set_max_voltage(uint8_t mnum, uint16_t mV);
    void set_max_voltage(std::vector<uint8_t> mnum, std::vector<uint16_t> mV);
    uint16_t get_max_voltage(uint8_t mnum);
    std::vector<uint16_t> get_max_voltage(std::vector<uint8_t> mnum);
    
    void optimize_sensor_current(uint8_t mnum);
    void optimize_sensor_current(std::vector<uint8_t> mnum);
    
    void set_sensor_current(uint8_t mnum, uint8_t cur);
    void set_sensor_current(std::vector<uint8_t> mnum, std::vector<uint8_t> cur);
    uint8_t get_sensor_current(uint8_t mnum);
    std::vector<uint8_t> get_sensor_current(std::vector<uint8_t> mnum);
    
    /* mode=="reverse"
     * mode=="alternating"
     * mode=="std"
     */
    void resonance_sweep(uint8_t mnum, std::string mode="");
    void resonance_sweep(std::vector<uint8_t> mnum, std::vector<std::string> mode=std::vector<std::string>());
    
    //dir: "fwd" or "bwd"
    void set_voltage_scaling(uint8_t mnum, std::string dir, uint8_t scale);
    void set_voltage_scaling(std::vector<uint8_t> mnum, std::vector<std::string> dir, std::vector<uint8_t> scale);
    uint8_t get_voltage_scaling(uint8_t mnum, std::string dir);
    std::vector<uint8_t> get_voltage_scaling(std::vector<uint8_t> mnum, std::vector<std::string> dir);
    
    void set_vco_freq(uint8_t mnum, uint32_t freq);
    void set_vco_freq(std::vector<uint8_t> mnum, std::vector<uint32_t> freq);
    uint32_t get_vco_freq(uint8_t mnum);
    std::vector<uint32_t> get_vco_freq(std::vector<uint8_t> mnum);
    
    void backup_memory(uint8_t mnum);
    void backup_memory(std::vector<uint8_t> mnum);
    
    void restore_memory(uint8_t mnum);
    void restore_memory(std::vector<uint8_t> mnum);
    
private:
    void open(std::string port);
    std::string read();
    std::string query(const std::string &data);
    void close();
    void write(const std::string &data);
};

#endif // PCBMOTOR_H
