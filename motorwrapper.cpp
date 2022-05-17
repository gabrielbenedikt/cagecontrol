#include "motorwrapper.h"

motorwrapper::motorwrapper(uint8_t intype, std::vector<uint8_t> mids = std::vector<uint8_t>(0), std::string devname = ""){
    if (intype == 0) {
        PCBMotor *pm = new PCBMotor(mids, devname);
        m = pm;
    } else if (intype == 1) {
        elliptec *em = new elliptec(mids, devname);
        m = em;
    } else {
        DEBUG_ERROR("motor type not recognized");
    }
}

motorwrapper::~motorwrapper() {
    delete m;
}

void motorwrapper::open(std::string port){
    m->open(port);
}

void motorwrapper::close(){
    m->close();
}

void motorwrapper::command_moveboth(int hwp_mnum, int qwp_mnum, double hwpang, double qwpang) {
    m->command_moveboth(hwp_mnum, qwp_mnum, hwpang, qwpang);
}


void motorwrapper::command_movethree(int hwp_mnum, int qwp_mnum, int qwp2_mnum, double hwp_ang, double qwp_ang, double qwp2_ang) {
    m->command_movethree(hwp_mnum, qwp_mnum, qwp2_mnum, hwp_ang, qwp_ang, qwp2_ang);
}
