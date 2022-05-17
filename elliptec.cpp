#include "elliptec.h"


/*
GS Error codes

Example: Shutter status value when shutter is at its default address “0”
RX “0GS00”
Means the shutter is OK
Status/Error
code value
Meaning
0 OK, no error
1 Communication time out
2 Mechanical time out
3 Command error or not supported
4 Value out of range
5 Module isolated
6 Module out of isolation
7 Initializing error
8 Thermal error
9 Busy
10 Sensor Error (May appear during self test. If code persists there is an error)
11 Motor Error (May appear during self test. If code persists there is an error)
12 Out of Range (e.g. stage has been instructed to move beyond its travel
range).
13 Over Current error
14-255 Reserved
*/


elliptec::elliptec(std::vector<uint8_t> inmids, std::string devname)
{
    devtype["rotary"] = {8, 14, 18};
    devtype["linear"] = {7, 10, 17, 20};
    devtype["indexed"] = {6, 9, 12};
    devtype["hasclean"] = {14, 17, 18, 20};

    for (uint8_t id: inmids) {
        if (id > 15) {
            std::cout << "ERROR: elliptec motor id has to be 0 < id < 15" << std::endl;
        } else {
            mids.push_back(int2addr(id));
        }
    }

    for (std::string m : mids) {
        std::cout << m << std::endl;
    }
    std::cout  << "";

    bserial = new Boost_serial(devname, 9600,
                               boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
                               boost::asio::serial_port_base::character_size(8),
                               boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none),
                               boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
    bserial->setTimeout(boost::posix_time::seconds(5));

}

elliptec::~elliptec()
{

}

void elliptec::init() {
    for (std::string id : mids) {
        getinfo(id);
    }
}

/*****************************************
 *
 * Serial
 *
 *****************************************/
void elliptec::read()
{
    std::cout << "read in elliptec"<< std::endl;
    response = bserial->readStringUntil("\r\n");
    std::cout << "response in elliptec" << response << std::endl;
}

void elliptec::write(const std::string &data)
{
    std::cout  << "write in elliptec" << std::endl;
    bserial->writeString(data);
    std::cout << "write to serial: " << data << std::endl;
}

std::string elliptec::query(const std::string &data) {
    bserial->writeString(data);
    response = bserial->readStringUntil("\r\n");
    return response;
}

void elliptec::close()
{
    if (bserial->isOpen())
        bserial->close();
}

bool elliptec::isopen()
{
    return bserial->isOpen();
}

void elliptec::open(std::string port)
{
   try {
        bserial->open(port, 9600,
                      boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
                      boost::asio::serial_port_base::character_size(8),
                      boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none),
                      boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
    }  catch (std::exception & ex) {
        std::cout << ex.what() << std::endl;
    }


    init();
}

/*****************************************
 *
 * Utility
 *
 *****************************************/
std::string elliptec::int2addr(uint8_t id) {
    char hexstring[] = {'0', '0'};
    sprintf(hexstring, "%X", id);
    std::string s = std::string(hexstring);
    return s;
}

bool elliptec::devintype(std::string type, uint8_t id) {
    return std::find(devtype[type].begin(), devtype[type].end(), id) != devtype[type].end();
}

int64_t elliptec::deg2step(std::string addr, double deg) {
    auto dev = devinfo_at_addr(addr);
    return std::round(dev.value().pulses * deg / 360);
}

int64_t elliptec::mm2step(std::string addr, double mm){
    auto dev = devinfo_at_addr(addr);
    return std::round(dev.value().pulses * mm);
}

double elliptec::step2deg(std::string addr, int64_t step) {
    auto dev = devinfo_at_addr(addr);
    double deg = 360.0 * step / dev.value().pulses;
    return deg;
}

double elliptec::step2mm(std::string addr, int64_t step){
    auto dev = devinfo_at_addr(addr);
    return 1.0 * step / dev.value().pulses;
}

std::string elliptec::step2hex(int64_t step) {
    if (step < 0) {
        step += (1ULL << 32);
    }
    std::stringstream ss;
    ss << std::setfill ('0') << std::setw(8) << std::hex << step;
    std::string s = ss.str();
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);

    return s;
}

int64_t elliptec::hex2step(std::string s) {
    int64_t i = stoll(s, nullptr, 16);
    if (i > (1LL << 31)) {
        i -= (1LL << 32);
    }
    return i;
}

std::string elliptec::ll2hex(int64_t i) {
    if (i < 0) {
        i += (1LL<<32);
    }

    std::stringstream ss;
    ss << std::setfill ('0') << std::setw(8) << std::hex << i;
    std::string s = ss.str();

    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

std::optional<ell_device> elliptec::devinfo_at_addr(std::string addr) {
    for (ell_device d : devices) {
        if (d.address == addr) {
            return d;
        }
    }
    return {};
}

void elliptec::print_dev_info(ell_device dev) {
    std::cout << "motor address: " << dev.address << std::endl;
    std::cout << "device type: " << dev.type << std::endl;
    std::cout << "serial number: " << dev.serial << std::endl;
    std::cout << "manufacturing year: " << dev.year << std::endl;
    std::cout << "fw revision: " << (unsigned)dev.fw << std::endl;
    std::cout << "hw revision: " << (unsigned)dev.hw << std::endl;
    std::cout << "travel: " << dev.travel << std::endl;
    std::cout << "pulses per unit: " << dev.pulses << std::endl;
}

void elliptec::handle_devinfo(ell_device dev) {
    bool is_new_dev = true;
    for (ell_device d : devices) {
        if (d.serial == dev.serial) {
            std::cout << "old serial: " << d.serial << std::endl;
            is_new_dev = false;
        }
    }
    if (is_new_dev) {
        devices.push_back(dev);
    }

    print_dev_info(dev);
}

ell_response elliptec::process_response() {
    read();
    std::cout << response << std::endl;
    std::string addstr = response.substr(0,1);
    std::string command = response.substr(1,2);
    std::string data = response.substr(3,response.npos);
    uint8_t addr = std::stoi(addstr.data(),nullptr,16);

    ell_response ret;
    ret.address = addr;
    ret.type = command;
    ret.data = data;

    if (command.compare(std::string("IN")) == 0) {
        uint16_t type   = std::stoi(response.substr(3,2).data(),  nullptr, 16);
        uint64_t sn     = std::stoi(response.substr(5,8).data(),  nullptr, 10);
        uint16_t year   = std::stoi(response.substr(13,4).data(), nullptr, 10);
        uint8_t fwrel   = std::stoi(response.substr(17,2).data(), nullptr, 10);
        uint8_t hwrel   = std::stoi(response.substr(19,2).data(), nullptr, 10);
        uint16_t travel = std::stoi(response.substr(21,4).data(), nullptr, 16);
        uint64_t pulses = std::stoi(response.substr(25,8).data(), nullptr, 16);

        ell_device dev;
        dev.address = addstr;
        dev.type = type;
        dev.serial = sn;
        dev.year = year;
        dev.fw = fwrel;
        dev.hw = hwrel;
        dev.travel = travel;
        dev.pulses = pulses;
        handle_devinfo(dev);
    } else if (command.compare(std::string("GS"))) {
        //TODO
        std::cout << "got status" << std::endl;
    } else if (command.compare(std::string("i1"))) {
        //TODO
        std::cout << "got motor1 info" << std::endl;
    } else if (command.compare(std::string("i2"))) {
        //TODO
        std::cout << "got motor2 info" << std::endl;
    } else if (command.compare(std::string("i3"))) {
        //TODO
        std::cout << "got motor3 info" << std::endl;
    } else if (command.compare(std::string("s1"))) {
        //TODO
        std::cout << "got s1" << std::endl;
        save_userdata(addstr);
    } else if (command.compare(std::string("s2"))) {
        //TODO
        std::cout << "got s2" << std::endl;
        save_userdata(addstr);
    } else if (command.compare(std::string("s3"))) {
        //TODO
        std::cout << "got s3" << std::endl;
        save_userdata(addstr);
    } else if (command.compare(std::string("PO"))) {
        //TODO
        ret.data = response.substr(3,8);
        std::cout << "got position " << ret.data << std::endl;
        return ret;
    } else if (command.compare(std::string("GV"))) {
        //TODO
        std::cout << "got velocity" << std::endl;
    } else {
        std::cout << "command not recognized" << std::endl;
    }

    return ret;
}

/*****************************************
 *
 * Low levels
 *
 *****************************************/
void elliptec::getinfo(std::string addr){
    std::string msg = addr + "in";
    write(msg.data());
    process_response();
    //reply with info response
}

void elliptec::get_motor1_info(std::string addr){
    std::string msg = addr + "i1";
    write(msg.data());
    process_response();
    //reply with info response
}

void elliptec::get_motor2_info(std::string addr){
    std::string msg = addr + "i2";
    write(msg.data());
    process_response();
    //reply with info response
}

void elliptec::get_motor3_info(std::string addr){
    std::string msg = addr + "i3";
    write(msg.data());
    process_response();
    //reply with info response
}

void elliptec::search_motor1_freq(std::string addr){
    std::string msg = addr + "s1";
    write(msg.data());
    process_response();
    //reply with info response
}

void elliptec::search_motor2_freq(std::string addr){
    std::string msg = addr + "s2";
    write(msg.data());
    process_response();
    //reply with info response
}

void elliptec::search_motor3_freq(std::string addr){
    std::string msg = addr + "s3";
    write(msg.data());
    process_response();
    //reply with info response
}

void elliptec::home(std::string addr, std::string dir = "0") {
    std::string msg = addr + "ho" + dir;
    write(msg.data());
    process_response();
    //reply with GS (while moving) or PO
}

void elliptec::move_absolute(std::string addr, double pos) {
    std::string msg = addr + "ma";
    std::string hstepstr = "";
    auto dev = devinfo_at_addr(addr);
    if (dev.has_value()) {
        if (devintype("rotary", dev.value().type)) {
            int64_t step = deg2step(addr, pos);
            hstepstr = step2hex(step);
        } else if (devintype("linear", dev.value().type)) {
            int64_t step = mm2step(addr, pos);
            hstepstr = step2hex(step);
        } else {
            std::cout << "device of type" << dev.value().type << " neither linear nor rotary." << std::endl;
        }
    } else {
        std::cout << "device with address " << addr << " not in connected device list" << std::endl;
    }

    if (hstepstr == "") {
        std::cout << "something went wrong in move_relative" << std::endl;
    } else {
        msg += hstepstr;

        uint8_t retcnt = 0;
        while (retcnt < 5) {
            uint64_t steps = 0;
            write(msg.data());
            ell_response ret = process_response();
            steps = hex2step(ret.data);
            double ERR=0, retpos=0;
            if (devintype("linear", devinfo_at_addr(addr)->type)) {
                ERR = MMERR;
                retpos = step2mm(addr, steps);
            } else {
                ERR = DEGERR;
                retpos = step2deg(addr, steps);
            }
            if (std::abs(pos-retpos) > ERR) {
                std::cout << "ERROR: rotation failed: moved " << retpos << " while trying to move " << pos << std::endl;
                ++retcnt;
            } else {
                std::cout << "rotation succeeded: moved " << retpos << " while trying to move " << pos << std::endl;
                retcnt = 5;
            }
        }

    }
    //reply with GS (while moving) or PO
}

void elliptec::move_relative(std::string addr, double pos) {
    std::string msg = addr + "mr";
    std::string hstepstr = "";
    auto dev = devinfo_at_addr(addr);
    if (dev.has_value()) {
        if (devintype("rotary", dev.value().type)) {
            int64_t step = deg2step(addr, pos);
            hstepstr = step2hex(step);
        } else if (devintype("linear", dev.value().type)) {
            int64_t step = mm2step(addr, pos);
            hstepstr = step2hex(step);
        } else {
            std::cout << "device of type" << dev.value().type << " neither linear nor rotary." << std::endl;
        }
    } else {
        std::cout << "device with address " << addr << " not in connected device list" << std::endl;
    }

    if (hstepstr == "") {
        std::cout << "something went wrong in move_relative" << std::endl;
    } else {
        msg += hstepstr;

        uint8_t retcnt = 0;
        while (retcnt < 5) {
            write(msg.data());
            ell_response ret = process_response();
            uint64_t steps = hex2step(ret.data);
            double ERR=0, retpos=0;
            if (devintype("linear", devinfo_at_addr(addr)->type)) {
                ERR = MMERR;
                retpos = step2mm(addr, steps);
            } else {
                ERR = DEGERR;
                retpos = step2deg(addr, steps);
            }
            if (std::abs(pos-retpos) > ERR) {
                std::cout << "ERROR: rotation failed: moved " << retpos << " while trying to move " << pos<< std::endl;
                ++retcnt;
            } else {
                std::cout << "rotation succeeded: moved " << retpos << " while trying to move " << pos  << std::endl;
                retcnt = 5;
            }
        }
    }
    //reply with GS (while moving) or PO
}

void elliptec::get_position(std::string addr) {
    std::string msg = addr + "gp";
    write(msg.data());
    process_response();
    //reply with GS (while moving) or PO
}

void elliptec::get_velocity(std::string addr) {
    std::string msg = addr + "gv";
    write(msg.data());
    process_response();
    //reply with GV
}

void elliptec::set_velocity(std::string addr, uint8_t percent) {
    std::string msg = addr + "sv" + std::to_string(percent);
    write(msg.data());
    process_response();
    //reply with GV
}

void elliptec::save_userdata(std::string addr) {
    std::string msg = addr + "us";
    write(msg.data());
    process_response();
}

void elliptec::optimize_motors(std::string addr) {
    std::string msg = addr + "om";
    write(msg.data());
    process_response();
    //reply with GS (while busy) 0 when done
}

void elliptec::clean_mechanics(std::string addr) {
    std::string msg = addr + "cm";
    write(msg.data());
    process_response();
    //reply with GS (while busy) 0 when done
}

void elliptec::stop_clean(std::string addr) {
    std::string msg = addr + "st";
    write(msg.data());
    process_response();
    //reply with GS (while busy) 0 when done
}

void elliptec::change_address(std::string addr, std::string newaddr) {
    std::string msg = addr + "ca" + newaddr;
    write(msg.data());
    process_response();
    //reply with OK Status Packet
    save_userdata(addr);
}

void elliptec::get_status(std::string addr) {
    std::string msg = addr + "gs";
    write(msg.data());
    process_response();
}


/*****************************************
 *
 * High levels
 *
 *****************************************/
void elliptec::command_moveboth(int hwp_mnum, int qwp_mnum, double hwpang, double qwpang){
    move_absolute(int2addr(hwp_mnum), std::fmod(hwpang, 360));
    move_absolute(int2addr(qwp_mnum), std::fmod(qwpang, 360));
}

void elliptec::command_movethree(int hwp_mnum, int qwp_mnum, int qwp2_mnum, double hwpang, double qwpang, double qwp2ang){
    move_absolute(int2addr(hwp_mnum), std::fmod(hwpang, 360));
    move_absolute(int2addr(qwp_mnum), std::fmod(qwpang, 360));
    move_absolute(int2addr(qwp2_mnum), std::fmod(qwp2ang, 360));
}

