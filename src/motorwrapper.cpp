#include "motorwrapper.h"

motorwrapper::motorwrapper(uint8_t intype, std::string devname = "", std::vector<uint8_t> mids = std::vector<uint8_t>(0)){
    _devtype = intype;
    if (_devtype == DEV_ELLIPTEC) {
        em = new elliptec(devname, mids);
    } else if (_devtype == DEV_PCBM) {
        pm = new PCBMotor(devname);
    } else {
        throw std::invalid_argument("motor type not recognized");
    }
}

motorwrapper::~motorwrapper() {

}

void motorwrapper::close(){
    if (_devtype == DEV_ELLIPTEC) {
        em->close();
    } else if (_devtype == DEV_PCBM) {
        pm->close();
    }
}

void motorwrapper::move_absolute(int mnum, double pos) {
    if (_devtype == DEV_ELLIPTEC) {
        em->move_absolute(std::to_string(mnum), pos);
    } else if (_devtype == DEV_PCBM) {
        pm->moveabsolute(mnum, pos);
    }
}

void motorwrapper::command_moveboth(int hwp_mnum, int qwp_mnum, double hwpang, double qwpang) {
    //m->command_moveboth(hwp_mnum, qwp_mnum, hwpang, qwpang);
    if (_devtype == DEV_ELLIPTEC) {
        em->move_absolute(std::to_string(hwp_mnum), hwpang);
        em->move_absolute(std::to_string(qwp_mnum), qwpang);
    } else if (_devtype == DEV_PCBM) {
        pm->moveabsolute(hwp_mnum, hwpang);
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        pm->moveabsolute(qwp_mnum, qwpang);
    }
}


void motorwrapper::command_movethree(int hwp_mnum, int qwp_mnum, int qwp2_mnum, double hwp_ang, double qwp_ang, double qwp2_ang) {
    //m->command_movethree(hwp_mnum, qwp_mnum, hwpang, qwpang);
    if (_devtype == DEV_ELLIPTEC) {
        em->move_absolute(std::to_string(hwp_mnum), hwp_ang);
        em->move_absolute(std::to_string(qwp_mnum), qwp_ang);
        em->move_absolute(std::to_string(qwp2_mnum), qwp2_ang);
    } else if (_devtype == DEV_PCBM) {
        pm->moveabsolute(hwp_mnum, hwp_ang);
        pm->moveabsolute(qwp_mnum, qwp_ang);
        pm->moveabsolute(qwp2_mnum, qwp2_ang);
    }
}
