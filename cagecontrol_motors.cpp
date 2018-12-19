#include "cagecontrol.h"

/************************************************************************************************
*                                                                                               *
*                                cagecontrol::moveHV                                            *
*                                                                                               *
************************************************************************************************/
void cagecontrol::slot_moveHV(QString color)
{
    double hwprot=0;
    double qwprot=0;
    updatestatus("From UDP: move " + color + " to H/V");
    if (motorName.contains(color.toLower())) {
        int idx = motorName.indexOf(color.toLower());
        if (invert[idx]) {
            hwprot = 45;
            qwprot = 0;
        } else {
            hwprot = 0;
            qwprot = 0;
        }
        movemotor(color.toLower(),HWP0[idx]+hwprot,QWP0[idx]+qwprot);
        if (useoffset) {
            HWPcust[idx]=hwprot;
            updateUI();
            QWPcust[idx]=qwprot;
            updateUI();
        } else {
            HWPcust[idx]=HWP0[idx]+hwprot;
            updateUI();
            QWPcust[idx]=QWP0[idx]+qwprot;
            updateUI();
        }
    } else if (color.toLower()=="all") {
        for (Motor* m : motors) {
            int idx = motors.indexOf(m);
            if (invert[idx]) {
                hwprot = 45;
                qwprot = 0;
            } else {
                hwprot = 0;
                qwprot = 0;
            }
            movemotor(motorName[idx],HWP0[idx]+hwprot,QWP0[idx]+qwprot);
            if (useoffset) {
                HWPcust[idx]=hwprot;
                updateUI();
                QWPcust[idx]=qwprot;
                updateUI();
            } else {
                HWPcust[idx]=HWP0[idx]+hwprot;
                updateUI();
                QWPcust[idx]=QWP0[idx]+qwprot;
                updateUI();
            }
        }
    } else {
        DEBUG_ERROR("Cage color unknown. Got: %s\n", color.toLocal8Bit().data());
    }
}
/************************************************************************************************
*                                                                                               *
*                                cagecontrol::movePM                                            *
*                                                                                               *
************************************************************************************************/
void cagecontrol::slot_movePM(QString color)
{
    double hwprot=0;
    double qwprot=0;
    updatestatus("From UDP: move " + color + " to P/M");
    if (motorName.contains(color.toLower())) {
        int idx = motorName.indexOf(color.toLower());
        if (invert[idx]) {
            hwprot = 67.5;
            qwprot = 45;
        } else {
            hwprot = 22.5;
            qwprot = 45;
        }
        movemotor(color.toLower(),HWP0[idx]+hwprot,QWP0[idx]+qwprot);
        if (useoffset) {
            HWPcust[idx]=hwprot;
            updateUI();
            QWPcust[idx]=qwprot;
            updateUI();
        } else {
            HWPcust[idx]=HWP0[idx]+hwprot;
            updateUI();
            QWPcust[idx]=QWP0[idx]+qwprot;
            updateUI();
        }
    } else if (color.toLower()=="all") {
        for (Motor* m : motors) {
            int idx = motors.indexOf(m);
            if (invert[idx]) {
                hwprot = 67.5;
                qwprot = 45;
            } else {
                hwprot = 22.5;
                qwprot = 45;
            }
            movemotor(motorName[idx],HWP0[idx]+hwprot,QWP0[idx]+qwprot);
            if (useoffset) {
                HWPcust[idx]=hwprot;
                updateUI();
                QWPcust[idx]=qwprot;
                updateUI();
            } else {
                HWPcust[idx]=HWP0[idx]+hwprot;
                updateUI();
                QWPcust[idx]=QWP0[idx]+qwprot;
                updateUI();
            }
        }
    } else {
        DEBUG_ERROR("Cage color unknown. Got: %s\n", color.toLocal8Bit().data());
    }
}
/************************************************************************************************
*                                                                                               *
*                                cagecontrol::moveLR                                            *
*                                                                                               *
************************************************************************************************/
void cagecontrol::slot_moveLR(QString color)
{
    double hwprot=0;
    double qwprot=0;
    updatestatus("From UDP: move " + color + " to R/L");
    // TODO: waveplate settings for circular
    if (motorName.contains(color.toLower())) {
        int idx = motorName.indexOf(color.toLower());
        if (invert[idx]) {
            hwprot = 67.5;
            qwprot = 0;
        } else {
            hwprot = 22.5;
            qwprot = 0;
        }
        movemotor(color.toLower(),HWP0[idx]+hwprot,QWP0[idx]+qwprot);
        if (useoffset) {
            HWPcust[idx]=hwprot;
            updateUI();
            QWPcust[idx]=qwprot;
            updateUI();
        } else {
            HWPcust[idx]=HWP0[idx]+hwprot;
            updateUI();
            QWPcust[idx]=QWP0[idx]+qwprot;
            updateUI();
        }
    } else if (color.toLower()=="all") {
        for (Motor* m : motors) {
            int idx = motors.indexOf(m);
            if (invert[idx]) {
                hwprot = 67.5;
                qwprot = 0;
            } else {
                hwprot = 22.5;
                qwprot = 0;
            }
            movemotor(motorName[idx],HWP0[idx]+hwprot,QWP0[idx]+qwprot);
            if (useoffset) {
                HWPcust[idx]=hwprot;
                updateUI();
                QWPcust[idx]=qwprot;
                updateUI();
            } else {
                HWPcust[idx]=HWP0[idx]+hwprot;
                updateUI();
                QWPcust[idx]=QWP0[idx]+qwprot;
                updateUI();
            }
        }
    } else {
        DEBUG_ERROR("Cage color unknown. Got: %s\n", color.toLocal8Bit().data());
    }
}
/************************************************************************************************
*                                                                                               *
*                              cagecontrol::movemotors                                          *
*                                                                                               *
************************************************************************************************/
void cagecontrol::slot_movemotors(QString color, double HWPang, double QWPang)
{
    updatestatus("From UDP: move " + color + " to HWP " + QString::number(HWPang) + " QWP " + QString::number(QWPang));
    if (motorName.contains(color.toLower())) {
        int idx = motorName.indexOf(color.toLower());
        movemotor(color.toLower(),HWPang,QWPang);
        if (useoffset) {
            HWPcust[idx]=HWPang;
            updateUI();
            QWPcust[idx]=QWPang;
            updateUI();
        } else {
            HWPcust[idx]=HWP0[idx]+HWPang;
            updateUI();
            QWPcust[idx]=QWP0[idx]+QWPang;
            updateUI();
        }
    } else if (color.toLower()=="all") {
        for (Motor* m : motors) {
            int idx = motors.indexOf(m);
            movemotor(motorName[idx],HWPang,QWPang);
            if (useoffset) {
                HWPcust[idx]=HWPang;
                updateUI();
                QWPcust[idx]=QWPang;
                updateUI();
            } else {
                HWPcust[idx]=HWP0[idx]+HWPang;
                updateUI();
                QWPcust[idx]=QWP0[idx]+QWPang;
                updateUI();
            }
        }
    } else {
        DEBUG_ERROR("Cage color unknown. Got: %s\n", color.toLocal8Bit().data());
    }
}


/************************************************************************************************
*                                                                                               *
*                              cagecontrol::openmotors                                          *
*                                                                                               *
************************************************************************************************/
void cagecontrol::openmotors()
{
    comports.clear();
    comports.append(tabs->findChild<QComboBox*>("redcom")->currentText());
    comports.append(tabs->findChild<QComboBox*>("browncom")->currentText());
    comports.append(tabs->findChild<QComboBox*>("greencom")->currentText());
    comports.append(tabs->findChild<QComboBox*>("bluecom")->currentText());
    comports.append(tabs->findChild<QComboBox*>("whitecom")->currentText());
    comports.append(tabs->findChild<QComboBox*>("blackcom")->currentText());
    for (QString s : motorName) {
        Motor *motor = new Motor();
        motors.append(motor);
        motor->open(comports.at(motors.indexOf(motor)));
    }
}

void cagecontrol::moveallhv()
{
    for (Motor* m : motors)
    {
        int idx = motors.indexOf(m);
        double hwprot;
        double qwprot;
        if (invert[idx]) {
            hwprot=45;
            qwprot=0;
        } else {
            hwprot=0;
            qwprot=0;
        }
        movemotor(motorName[idx],HWP0[idx]+hwprot,QWP0[idx]+qwprot);
        if (useoffset) {
            HWPcust[idx]=hwprot;
            updateUI();
            QWPcust[idx]=qwprot;
            updateUI();
        } else {
            HWPcust[idx]=HWP0[idx]+hwprot;
            updateUI();
            QWPcust[idx]=QWP0[idx]+qwprot;
            updateUI();
        }
    }
}

void cagecontrol::moveallpm()
{
    for (Motor* m : motors)
    {
        int idx = motors.indexOf(m);
        double hwprot;
        double qwprot;
        if (invert[idx]) {
            hwprot=67.5;
            qwprot=45;
        } else {
            hwprot=22.5;
            qwprot=45;
        }
        movemotor(motorName[idx],HWP0[idx]+hwprot,QWP0[idx]+qwprot);
        if (useoffset) {
            HWPcust[idx]=hwprot;
            updateUI();
            QWPcust[idx]=qwprot;
            updateUI();
        } else {
            HWPcust[idx]=HWP0[idx]+hwprot;
            updateUI();
            QWPcust[idx]=QWP0[idx]+qwprot;
            updateUI();
        }
    }
}

void cagecontrol::movealllr()
{
    for (Motor* m : motors)
    {
        int idx = motors.indexOf(m);
        double hwprot;
        double qwprot;
        if (invert[idx]) {
            hwprot=67.5;
            qwprot=0;
        } else {
            hwprot=22.5;
            qwprot=0;
        }
        movemotor(motorName[idx], HWP0[idx]+hwprot,QWP0[idx]+qwprot);
        if (useoffset) {
            HWPcust[idx]=hwprot;
            updateUI();
            QWPcust[idx]=qwprot;
            updateUI();
        } else {
            HWPcust[idx]=HWP0[idx]+hwprot;
            updateUI();
            QWPcust[idx]=QWP0[idx]+qwprot;
            updateUI();
        }
    }
}

void cagecontrol::moveallarb()
{
    for (Motor* m : motors)
    {
        int idx = motors.indexOf(m);
        if (useoffset) {
            double hrot=HWPcust[idx]+HWP0[idx];
            double qrot=QWPcust[idx]+QWP0[idx];
            while (hrot > 330) {hrot=hrot-180;}
            while (qrot > 330) {qrot=qrot-180;}
            movemotor(motorName[idx],hrot,qrot);
        } else {
            movemotor(motorName[idx],HWPcust[idx],QWPcust[idx]);
        }
    }
}

void cagecontrol::moveredHV()
{
    std::vector<double> angles = {0,0};
    double hwprot;
    double qwprot;
    if (invert[0]) {
        hwprot=45;
        qwprot=0;
    } else {
        hwprot=0;
        qwprot=0;
    }
    movemotor("red",HWP0[0]+hwprot,QWP0[0]+qwprot);
    if (useoffset) {
        HWPcust[0]=hwprot;
        updateUI();
        QWPcust[0]=qwprot;
        updateUI();
    } else {
        HWPcust[0]=HWP0[0]+hwprot;
        updateUI();
        QWPcust[0]=QWP0[0]+qwprot;
        updateUI();
    }
}

void cagecontrol::moveredPM()
{
    double hwprot;
    double qwprot;
    if (invert[0]) {
        hwprot=67.5;
        qwprot=45;
    } else {
        hwprot=22.5;
        qwprot=45;
    }
    movemotor("red",HWP0[0]+hwprot,QWP0[0]+qwprot);
    if (useoffset) {
        HWPcust[0]=hwprot;
        updateUI();
        QWPcust[0]=qwprot;
        updateUI();
    } else {
        HWPcust[0]=HWP0[0]+hwprot;
        updateUI();
        QWPcust[0]=QWP0[0]+qwprot;
        updateUI();
    }
}

void cagecontrol::moveredLR()
{
    double hwprot;
    double qwprot;
    if (invert[0]) {
        hwprot=67.5;
        qwprot=0;
    } else {
        hwprot=22.5;
        qwprot=0;
    }
    movemotor("red",HWP0[0]+hwprot,QWP0[0]+qwprot);
    if (useoffset) {
        HWPcust[0]=hwprot;
        updateUI();
        QWPcust[0]=qwprot;
        updateUI();
    } else {
        HWPcust[0]=HWP0[0]+hwprot;
        updateUI();
        QWPcust[0]=QWP0[0]+qwprot;
        updateUI();
    }
}

void cagecontrol::moveredANG()
{
    double HWPang=motorstab->findChild<QGroupBox*>("redbox")->findChild<QDoubleSpinBox*>("HWPsb")->value();
    double QWPang=motorstab->findChild<QGroupBox*>("redbox")->findChild<QDoubleSpinBox*>("QWPsb")->value();
    if (useoffset) {
        double hrot=HWPang+HWP0[0];
        double qrot=QWPang+QWP0[0];
        while (hrot > 330) {hrot=hrot-180;}
        while (qrot > 330) {qrot=qrot-180;}
        movemotor("red",hrot,qrot);
    } else {
        movemotor("red",HWPang,QWPang);
    }
}

void cagecontrol::movebrownHV()
{
    double hwprot;
    double qwprot;
    if (invert[1]) {
        hwprot=45;
        qwprot=0;
    } else {
        hwprot=0;
        qwprot=0;
    }
    movemotor("brown",HWP0[1]+hwprot,QWP0[1]+qwprot);
    if (useoffset) {
        HWPcust[1]=hwprot;
        updateUI();
        QWPcust[1]=qwprot;
        updateUI();
    } else {
        HWPcust[1]=HWP0[1]+hwprot;
        updateUI();
        QWPcust[1]=QWP0[1]+qwprot;
        updateUI();
    }
}

void cagecontrol::movebrownPM()
{
    double hwprot;
    double qwprot;
    if (invert[1]) {
        hwprot=67.5;
        qwprot=45;
    } else {
        hwprot=22.5;
        qwprot=45;
    }
    movemotor("brown",HWP0[1]+hwprot,QWP0[1]+qwprot);
    if (useoffset) {
        HWPcust[1]=hwprot;
        updateUI();
        QWPcust[1]=qwprot;
        updateUI();
    } else {
        HWPcust[1]=HWP0[1]+hwprot;
        updateUI();
        QWPcust[1]=QWP0[1]+qwprot;
        updateUI();
    }
}

void cagecontrol::movebrownLR()
{
    double hwprot;
    double qwprot;
    if (invert[1]) {
        hwprot=67.5;
        qwprot=0;
    } else {
        hwprot=22.5;
        qwprot=0;
    }
    movemotor("brown",HWP0[1]+hwprot,QWP0[1]+qwprot);
    if (useoffset) {
        HWPcust[1]=hwprot;
        updateUI();
        QWPcust[1]=qwprot;
        updateUI();
    } else {
        HWPcust[1]=HWP0[1]+hwprot;
        updateUI();
        QWPcust[1]=QWP0[1]+qwprot;
        updateUI();
    }
}

void cagecontrol::movebrownANG()
{
    double HWPang=motorstab->findChild<QGroupBox*>("brownbox")->findChild<QDoubleSpinBox*>("HWPsb")->value();
    double QWPang=motorstab->findChild<QGroupBox*>("brownbox")->findChild<QDoubleSpinBox*>("QWPsb")->value();
    if (useoffset) {
        double hrot=HWPang+HWP0[1];
        double qrot=QWPang+QWP0[1];
        while (hrot > 330) {hrot=hrot-180;}
        while (qrot > 330) {qrot=qrot-180;}
        movemotor("brown",hrot,qrot);
    } else {
        movemotor("brown",HWPang,QWPang);
    }
}

void cagecontrol::movegreenHV()
{
    double hwprot;
    double qwprot;
    if (invert[2]) {
        hwprot=45;
        qwprot=0;
    } else {
        hwprot=0;
        qwprot=0;
    }
    movemotor("green",HWP0[2]+hwprot,QWP0[2]+qwprot);
    if (useoffset) {
        HWPcust[2]=hwprot;
        updateUI();
        QWPcust[2]=qwprot;
        updateUI();
    } else {
        HWPcust[2]=HWP0[2]+hwprot;
        updateUI();
        QWPcust[2]=QWP0[2]+qwprot;
        updateUI();
    }
}

void cagecontrol::movegreenPM()
{
    double hwprot;
    double qwprot;
    if (invert[2]) {
        hwprot=67.5;
        qwprot=45;
    } else {
        hwprot=22.5;
        qwprot=45;
    }
    movemotor("green",HWP0[2]+hwprot,QWP0[2]+qwprot);
    if (useoffset) {
        HWPcust[2]=hwprot;
        updateUI();
        QWPcust[2]=qwprot;
        updateUI();
    } else {
        HWPcust[2]=HWP0[2]+hwprot;
        updateUI();
        QWPcust[2]=QWP0[2]+qwprot;
        updateUI();
    }
}

void cagecontrol::movegreenLR()
{
    double hwprot;
    double qwprot;
    if (invert[2]) {
        hwprot=67.5;
        qwprot=0;
    } else {
        hwprot=22.5;
        qwprot=0;
    }
    movemotor("green",HWP0[2]+hwprot,QWP0[2]+qwprot);
    if (useoffset) {
        HWPcust[2]=hwprot;
        updateUI();
        QWPcust[2]=qwprot;
        updateUI();
    } else {
        HWPcust[2]=HWP0[2]+hwprot;
        updateUI();
        QWPcust[2]=QWP0[2]+qwprot;
        updateUI();
    }
}

void cagecontrol::movegreenANG()
{
    double HWPang=motorstab->findChild<QGroupBox*>("greenbox")->findChild<QDoubleSpinBox*>("HWPsb")->value();
    double QWPang=motorstab->findChild<QGroupBox*>("greenbox")->findChild<QDoubleSpinBox*>("QWPsb")->value();
    if (useoffset) {
        double hrot=HWPang+HWP0[2];
        double qrot=QWPang+QWP0[2];
        while (hrot > 330) {hrot=hrot-180;}
        while (qrot > 330) {qrot=qrot-180;}
        movemotor("green",hrot,qrot);
    } else {
        movemotor("green",HWPang,QWPang);
    }
}

void cagecontrol::moveblueHV()
{
    double hwprot;
    double qwprot;
    if (invert[3]) {
        hwprot=45;
        qwprot=0;
    } else {
        hwprot=0;
        qwprot=0;
    }
    movemotor("blue",HWP0[3]+hwprot,QWP0[3]+qwprot);
    if (useoffset) {
        HWPcust[3]=hwprot;
        updateUI();
        QWPcust[3]=qwprot;
        updateUI();
    } else {
        HWPcust[3]=HWP0[3]+hwprot;
        updateUI();
        QWPcust[3]=QWP0[3]+qwprot;
        updateUI();
    }
}

void cagecontrol::movebluePM()
{
    double hwprot;
    double qwprot;
    if (invert[3]) {
        hwprot=67.5;
        qwprot=45;
    } else {
        hwprot=22.5;
        qwprot=45;
    }
    movemotor("blue",HWP0[3]+hwprot,QWP0[3]+qwprot);
    if (useoffset) {
        HWPcust[3]=hwprot;
        updateUI();
        QWPcust[3]=qwprot;
        updateUI();
    } else {
        HWPcust[3]=HWP0[3]+hwprot;
        updateUI();
        QWPcust[3]=QWP0[3]+qwprot;
        updateUI();
    }
}

void cagecontrol::moveblueLR()
{
    double hwprot;
    double qwprot;
    if (invert[3]) {
        hwprot=67.5;
        qwprot=0;
    } else {
        hwprot=22.5;
        qwprot=0;
    }
    movemotor("blue",HWP0[3]+hwprot,QWP0[3]+qwprot);
    if (useoffset) {
        HWPcust[3]=hwprot;
        updateUI();
        QWPcust[3]=qwprot;
        updateUI();
    } else {
        HWPcust[3]=HWP0[3]+hwprot;
        updateUI();
        QWPcust[3]=QWP0[3]+qwprot;
        updateUI();
    }
}

void cagecontrol::moveblueANG()
{
    double HWPang=motorstab->findChild<QGroupBox*>("bluebox")->findChild<QDoubleSpinBox*>("HWPsb")->value();
    double QWPang=motorstab->findChild<QGroupBox*>("bluebox")->findChild<QDoubleSpinBox*>("QWPsb")->value();
    if (useoffset) {
        double hrot=HWPang+HWP0[3];
        double qrot=QWPang+QWP0[3];
        while (hrot > 330) {hrot=hrot-180;}
        while (qrot > 330) {qrot=qrot-180;}
        movemotor("blue",hrot,qrot);
    } else {
        movemotor("blue",HWPang,QWPang);
    }
}

void cagecontrol::movewhiteHV()
{
    double hwprot;
    double qwprot;
    if (invert[4]) {
        hwprot=45;
        qwprot=0;
    } else {
        hwprot=0;
        qwprot=0;
    }
    movemotor("white",HWP0[4]+hwprot,QWP0[4]+qwprot);
    if (useoffset) {
        HWPcust[4]=hwprot;
        updateUI();
        QWPcust[4]=qwprot;
        updateUI();
    } else {
        HWPcust[4]=HWP0[4]+hwprot;
        updateUI();
        QWPcust[4]=QWP0[4]+qwprot;
        updateUI();
    }
}

void cagecontrol::movewhitePM()
{
    double hwprot;
    double qwprot;
    if (invert[4]) {
        hwprot=67.5;
        qwprot=45;
    } else {
        hwprot=22.5;
        qwprot=45;
    }
    movemotor("white",HWP0[4]+hwprot,QWP0[4]+qwprot);
    if (useoffset) {
        HWPcust[4]=hwprot;
        updateUI();
        QWPcust[4]=qwprot;
        updateUI();
    } else {
        HWPcust[4]=HWP0[4]+hwprot;
        updateUI();
        QWPcust[4]=QWP0[4]+qwprot;
        updateUI();
    }
}

void cagecontrol::movewhiteLR()
{
    double hwprot;
    double qwprot;
    if (invert[4]) {
        hwprot=67.5;
        qwprot=0;
    } else {
        hwprot=22.5;
        qwprot=0;
    }
    movemotor("white",HWP0[4]+hwprot,QWP0[4]+qwprot);
    if (useoffset) {
        HWPcust[4]=hwprot;
        updateUI();
        QWPcust[4]=qwprot;
        updateUI();
    } else {
        HWPcust[4]=HWP0[4]+hwprot;
        updateUI();
        QWPcust[4]=QWP0[4]+qwprot;
        updateUI();
    }
}

void cagecontrol::movewhiteANG()
{
    double HWPang=motorstab->findChild<QGroupBox*>("whitebox")->findChild<QDoubleSpinBox*>("HWPsb")->value();
    double QWPang=motorstab->findChild<QGroupBox*>("whitebox")->findChild<QDoubleSpinBox*>("QWPsb")->value();
    if (useoffset) {
        double hrot=HWPang+HWP0[4];
        double qrot=QWPang+QWP0[4];
        while (hrot > 330) {hrot=hrot-180;}
        while (qrot > 330) {qrot=qrot-180;}
        movemotor("white",hrot,qrot);
    } else {
        movemotor("white",HWPang,QWPang);
    }
}

void cagecontrol::moveblackHV()
{
    double hwprot;
    double qwprot;
    if (invert[5]) {
        hwprot=45;
        qwprot=0;
    } else {
        hwprot=0;
        qwprot=0;
    }
    movemotor("black",HWP0[5]+hwprot,QWP0[5]+qwprot);
    if (useoffset) {
        HWPcust[5]=hwprot;
        updateUI();
        QWPcust[5]=qwprot;
        updateUI();
    } else {
        HWPcust[5]=HWP0[5]+hwprot;
        updateUI();
        QWPcust[5]=QWP0[5]+qwprot;
        updateUI();
    }
}

void cagecontrol::moveblackPM()
{
    double hwprot;
    double qwprot;
    if (invert[5]) {
        hwprot=67.5;
        qwprot=45;
    } else {
        hwprot=22.5;
        qwprot=45;
    }
    movemotor("black",HWP0[5]+hwprot,QWP0[5]+qwprot);
    if (useoffset) {
        HWPcust[5]=hwprot;
        updateUI();
        QWPcust[5]=qwprot;
        updateUI();
    } else {
        HWPcust[5]=HWP0[5]+hwprot;
        updateUI();
        QWPcust[5]=QWP0[5]+qwprot;
        updateUI();
    }
}

void cagecontrol::moveblackLR()
{
    double hwprot;
    double qwprot;
    if (invert[5]) {
        hwprot=67.5;
        qwprot=0;
    } else {
        hwprot=22.5;
        qwprot=0;
    }
    movemotor("black",HWP0[5]+hwprot,QWP0[5]+qwprot);
    if (useoffset) {
        HWPcust[5]=hwprot;
        updateUI();
        QWPcust[5]=qwprot;
        updateUI();
    } else {
        HWPcust[5]=HWP0[5]+hwprot;
        updateUI();
        QWPcust[5]=QWP0[5]+qwprot;
        updateUI();
    }
}

void cagecontrol::moveblackANG()
{
    double HWPang=motorstab->findChild<QGroupBox*>("blackbox")->findChild<QDoubleSpinBox*>("HWPsb")->value();
    double QWPang=motorstab->findChild<QGroupBox*>("blackbox")->findChild<QDoubleSpinBox*>("QWPsb")->value();
    if (useoffset) {
        double hrot=HWPang+HWP0[5];
        double qrot=QWPang+QWP0[5];
        while (hrot > 330) {hrot=hrot-180;}
        while (qrot > 330) {qrot=qrot-180;}
        movemotor("black",hrot,qrot);
    } else {
        movemotor("black",HWPang,QWPang);
    }
}
/************************************************************************************************
*                                                                                               *
*                               cagecontrol::MoveMotor                                          *
*                                                                                               *
************************************************************************************************/
void cagecontrol::movemotor(QString motor, double HWPang, double QWPang)
{
    int i = motorName.indexOf(motor);
    if ((HWPmnum.at(i)==1) && (QWPmnum.at(i)==2)) {
        motors.at(i)->command_moveboth(HWPang,QWPang);
    } else if ((HWPmnum.at(i)==2) && (QWPmnum.at(i)==1)) {
        motors.at(i)->command_moveboth(QWPang,HWPang);
    } else {
        DEBUG_ERROR("undefined motor slot: HWP %i QWP %i\n",HWPmnum.at(i),QWPmnum.at(i));
    }

}
