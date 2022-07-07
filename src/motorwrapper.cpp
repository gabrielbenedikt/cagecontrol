#include "motorwrapper.h"

motorwrapper::motorwrapper(uint8_t intype, std::string devname = "", std::vector<uint8_t> mids = std::vector<uint8_t>(0)){
    _devtype = intype;
    if (_devtype == DEV_ELLIPTEC) {
        pm = new PCBMotor(devname, mids);
        m = pm;
    } else if (_devtype == DEV_PCBM) {
        em = new elliptec(devname, mids);
        m = em;
    } else {
        DEBUG_ERROR("motor type not recognized");
    }
}

motorwrapper::~motorwrapper() {

}

void motorwrapper::close(){
    m->close();
}

void motorwrapper::move_absolute(int mnum, double pos) {
    if (_devtype == DEV_ELLIPTEC) {
        em->move_absolute(std::to_string(mnum), pos);
    } else if (_devtype == DEV_PCBM) {
        pm->command_move(mnum, pos);
    }
}

void motorwrapper::command_moveboth(int hwp_mnum, int qwp_mnum, double hwpang, double qwpang) {
    //m->command_moveboth(hwp_mnum, qwp_mnum, hwpang, qwpang);
    if (_devtype == DEV_ELLIPTEC) {
        em->move_absolute(std::to_string(hwp_mnum), hwpang);
        em->move_absolute(std::to_string(qwp_mnum), qwpang);
    } else if (_devtype == DEV_PCBM) {
        pm->command_moveboth(hwp_mnum, qwp_mnum, hwpang, qwpang);
    }
}


void motorwrapper::command_movethree(int hwp_mnum, int qwp_mnum, int qwp2_mnum, double hwp_ang, double qwp_ang, double qwp2_ang) {
    //m->command_movethree(hwp_mnum, qwp_mnum, hwpang, qwpang);
    if (_devtype == DEV_ELLIPTEC) {
        em->move_absolute(std::to_string(hwp_mnum), hwp_ang);
        em->move_absolute(std::to_string(qwp_mnum), qwp_ang);
        em->move_absolute(std::to_string(qwp2_mnum), qwp2_ang);
    } else if (_devtype == DEV_PCBM) {
        pm->command_movethree(hwp_mnum, qwp_mnum, qwp2_mnum, hwp_ang, qwp_ang, qwp2_ang);
    }
}
