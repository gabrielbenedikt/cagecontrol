#include "cagecontrol.h"

/************************************************************************************************
*                              cagecontrol::cagecontrol                                         *
************************************************************************************************/
cagecontrol::cagecontrol(QWidget *parent) :
    QMainWindow(parent)//,ui(new Ui::cagecontrol)
{
    settings = new QSettings("cagecontrol.conf", QSettings::IniFormat);

    // UI-Setup
    pauseupdating=false;
    motorName<<"red"<<"brown"<<"green"<<"blue"<<"white"<<"black";
    uiMotorGroupBoxes.reserve(motorName.length());
    QWidget *mainwidget = new QWidget();
    QGridLayout *mainlayout = new QGridLayout;
    setupUI(mainlayout);
    HWP0.reserve(motorName.length());
    QWP0.reserve(motorName.length());
    HWPcust.reserve(motorName.length());
    QWPcust.reserve(motorName.length());
    HWPmnum.reserve(motorName.length());
    QWPmnum.reserve(motorName.length());
    invert.reserve(motorName.length());
    useoffset=true;
    currentbasisidx=-1;
    basestime=0;
    basestimer.setSingleShot(true);
    mainlayout->addWidget(tabs);
    mainlayout->addWidget(status);
    mainwidget->setLayout(mainlayout);
    setCentralWidget(mainwidget);
    centralWidget()->setLayout(mainlayout);
    setWindowTitle("CageControl");

    udplistener = new UDPlistener(settings);

    LoadConfig();
    initconnections();
    openmotors();
    updateUI();
}

/************************************************************************************************
*                            cagecontrol::initconnections                                       *
************************************************************************************************/
void cagecontrol::initconnections()
{
    for (QGroupBox *gb : uiMotorGroupBoxes) {
        connect(gb->findChild<CQPushButton*>("H/V"),&CQPushButton::pressed_id,this,&cagecontrol::moveHV);
        connect(gb->findChild<CQPushButton*>("+/-"),&CQPushButton::pressed_id,this,&cagecontrol::movePM);
        connect(gb->findChild<CQPushButton*>("R/L"),&CQPushButton::pressed_id,this,&cagecontrol::moveRL);
        connect(gb->findChild<CQPushButton*>("set"),&CQPushButton::pressed_id,this,&cagecontrol::moveANG);
    }

    connect(motorstab->findChild<CQPushButton*>("allhvbutton"),&CQPushButton::pressed_id,this,&cagecontrol::moveHV);
    connect(motorstab->findChild<CQPushButton*>("allpmbutton"),&CQPushButton::pressed_id,this,&cagecontrol::movePM);
    connect(motorstab->findChild<CQPushButton*>("allrlbutton"),&CQPushButton::pressed_id,this,&cagecontrol::moveRL);
    connect(motorstab->findChild<CQPushButton*>("allsetbutton"),&CQPushButton::pressed_id,this,&cagecontrol::moveANG);

    connect(tabs->findChild<QPushButton*>("filebtn"),&QAbstractButton::pressed,this,&cagecontrol::setbasesfile);
    connect(tabs->findChild<QPushButton*>("startbasesbtn"),&QAbstractButton::pressed,this,&cagecontrol::changebases);


#if defined (_WIN32) && (_MSC_VER<1900)
    connect(settingstab->findChild<QSpinBox*>("port"),SIGNAL(valueChanged),this,SLOT(updatesettings));
#else
    connect(settingstab->findChild<QSpinBox*>("port"),QOverload<int>::of(&QSpinBox::valueChanged),this,&cagecontrol::updatesettings);
#endif


    QList<QDoubleSpinBox*> dsboxes = tabs->findChildren<QDoubleSpinBox*>();
    for(QDoubleSpinBox* box : dsboxes) {
#if defined (_WIN32) && (_MSC_VER<1900)
        connect(box,SIGNAL(valueChanged(double)),this,SLOT(updatesettings(double)));
#else
        connect(box,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,&cagecontrol::updatesettings);
#endif
    }

    QList<QSpinBox*> sboxes = tabs->findChildren<QSpinBox*>();
    for(QSpinBox* box : sboxes) {
#if defined (_WIN32) && (_MSC_VER<1900)
        connect(box,SIGNAL(valueChanged(int)),this,SLOT(updatesettingsint(int)));
#else
        connect(box,QOverload<int>::of(&QSpinBox::valueChanged),this,&cagecontrol::updatesettingsint);
#endif
    }

    QList<QCheckBox*> cboxes = tabs->findChildren<QCheckBox*>();
    for(QCheckBox* box : cboxes) {
#if defined (_WIN32) && (_MSC_VER<1900)
        connect(box,SIGNAL(stateChanged(int)),this,SLOT(updatesettingsint(int)));
#else
        connect(box,&QCheckBox::stateChanged,this,&cagecontrol::updatesettingsint);
#endif
    }

    //UDP
    connect(udplistener, &UDPlistener::Move, this, &cagecontrol::slot_movemotors);
    connect(udplistener, &UDPlistener::MoveHV, this, &cagecontrol::moveHV);
    connect(udplistener, &UDPlistener::MovePM, this, &cagecontrol::movePM);
    connect(udplistener, &UDPlistener::MoveRL, this, &cagecontrol::moveRL);
    connect(udplistener, &UDPlistener::invert, this, &cagecontrol::useinvertedbases);
    connect(udplistener, &UDPlistener::changeoffsetusage, this, &cagecontrol::slot_changeoffsetusage);
    connect(udplistener, &UDPlistener::changeWPangles,this,&cagecontrol::slot_changeWPangles);
    connect(udplistener, &UDPlistener::showmsg,this,&cagecontrol::updatestatus);

    connect(&basestimer, &QTimer::timeout,this,&cagecontrol::changebases);
}

/************************************************************************************************
*                            cagecontrol::updatesettings                                        *
************************************************************************************************/
void cagecontrol::updatesettingsint(int i)
{
    UNUSED(i);
    updatesettings(3.1415);
}

/************************************************************************************************
*                            cagecontrol::updatesettings                                        *
************************************************************************************************/
void cagecontrol::updatesettings(double d)
{
    UNUSED(d); //just there to create a SLOT with matching parameter for signal &Q(Double)SpinBox::valueChanged
    if (!pauseupdating) {
        for (QString s : motorName) {
            int idx = motorName.indexOf(s);
            HWP0[idx]=tabs->findChild<QDoubleSpinBox*>(s+"HWP0sb")->value();
            QWP0[idx]=tabs->findChild<QDoubleSpinBox*>(s+"QWP0sb")->value();
            HWPcust[idx]=uiMotorGroupBoxes[idx]->findChild<QDoubleSpinBox*>("HWPsb")->value();
            QWPcust[idx]=uiMotorGroupBoxes[idx]->findChild<QDoubleSpinBox*>("QWPsb")->value();
            HWPmnum[idx]=tabs->findChild<QSpinBox*>(s+"HWPnum")->value();
            QWPmnum[idx]=tabs->findChild<QSpinBox*>(s+"QWPnum")->value();
            invert[idx]=uiMotorGroupBoxes[idx]->findChild<QCheckBox*>("invertcb")->isChecked();
        }

        udpport=settingstab->findChild<QSpinBox*>("port")->value();

        useoffset = settingstab->findChild<QCheckBox*>("offsetcb")->isChecked();

        basesfname = tabs->findChild<QLineEdit*>("filele")->text();
        basestime = tabs->findChild<QSpinBox*>("baseschangesb")->value();
    }

}

/************************************************************************************************
*                              cagecontrol::LoadConfig                                          *
************************************************************************************************/
void cagecontrol::LoadConfig()
{
    int tmpint;
    QString tmpstr;

    for (QGroupBox *gb : uiMotorGroupBoxes) {
        int i = uiMotorGroupBoxes.indexOf(gb);
        QString id = motorName[i];
        HWP0.append(settings->value("MOTORS/HWP/"+QString::number(i)).toDouble());
        QWP0.append(settings->value("MOTORS/QWP/"+QString::number(i)).toDouble());
        HWPcust.append(settings->value("GUI/HWP/"+QString::number(i)).toDouble());
        QWPcust.append(settings->value("GUI/QWP/"+QString::number(i)).toDouble());
        HWPmnum.append(settings->value("MOTORS/HWPNUM/"+QString::number(i)).toInt());
        QWPmnum.append(settings->value("MOTORS/QWPNUM/"+QString::number(i)).toInt());
        invert.append(settings->value("GUI/INVERT/"+QString::number(i)).toBool());
        gb->findChild<QCheckBox*>("invertcb")->setChecked(invert[i]);
        gb->findChild<QDoubleSpinBox*>("HWPsb")->setValue(HWPcust[i]);
        gb->findChild<QDoubleSpinBox*>("QWPsb")->setValue(QWPcust[i]);
        tabs->findChild<QDoubleSpinBox*>(id+"HWP0sb")->setValue(HWP0[i]);
        tabs->findChild<QDoubleSpinBox*>(id+"QWP0sb")->setValue(QWP0[i]);
        tabs->findChild<QSpinBox*>(id+"HWPnum")->setValue(HWPmnum[i]);
        tabs->findChild<QSpinBox*>(id+"QWPnum")->setValue(QWPmnum[i]);

        tmpstr = settings->value("MOTORS/COM"+id.toUpper(),"").toString();
        tmpint = tabs->findChild<QComboBox*>(id+"com")->findText(tmpstr);
        if ((tmpstr=="") || (tmpint==-1)) {
            tmpint=0;
        }
        tabs->findChild<QComboBox*>(id+"com")->setCurrentIndex(tmpint);
    }

    useoffset=settings->value("GUI/OFFSET").toBool();
    tabs->findChild<QCheckBox*>("offsetcb")->setChecked(useoffset);

    tmpint=settings->value("NETWORK/UDPPORT",0).toInt();
    udpport=tmpint;
    tabs->findChild<QSpinBox*>("port")->setValue(tmpint);

    basesfname = settings->value("GUI/BASESFNAME").toString();
    tabs->findChild<QLineEdit*>("filele")->setText(basesfname);
    tmpint = settings->value("GUI/BASESTIME").toInt();
    basestime=tmpint;
    tabs->findChild<QSpinBox*>("baseschangesb")->setValue(basestime);
}

/************************************************************************************************
*                              cagecontrol::SaveConfig                                          *
************************************************************************************************/
void cagecontrol::SaveConfig()
{
    QStringList dbltostrlist;

    for (QString s : motorName) {
        int i = motorName.indexOf(s);
        settings->setValue("WAVEPLATES/HWP/"+s.toUpper(),tabs->findChild<QDoubleSpinBox*>(s+"HWP0sb")->value());
        settings->setValue("WAVEPLATES/QWP/"+s.toUpper(),tabs->findChild<QDoubleSpinBox*>(s+"QWP0sb")->value());
        settings->setValue("MOTORS/COM"+s.toUpper(),tabs->findChild<QComboBox*>(s+"com")->currentText());
        settings->setValue("MOTORS/HWP/"+QString::number(i),HWP0[i]);
        settings->setValue("MOTORS/QWP/"+QString::number(i),QWP0[i]);
        settings->setValue("GUI/HWP/"+QString::number(i),HWPcust[i]);
        settings->setValue("GUI/QWP/"+QString::number(i),QWPcust[i]);
        settings->setValue("MOTORS/HWPNUM/"+QString::number(i),HWPmnum[i]);
        settings->setValue("MOTORS/QWPNUM/"+QString::number(i),QWPmnum[i]);
        settings->setValue("GUI/INVERT/"+QString::number(i),invert[i]);
    }

    settings->setValue("NETWORK/UDPPORT",tabs->findChild<QSpinBox*>("port")->value());

    settings->setValue("GUI/OFFSET",useoffset);
    settings->setValue("GUI/BASESTIME",basestime);
    settings->setValue("GUI/BASESFNAME",basesfname);

    settings->sync();
}

/************************************************************************************************
*                          cagecontrol::slot_changeoffsetusage                                  *
************************************************************************************************/
void cagecontrol::slot_changeoffsetusage(bool uo_in)
{
    useoffset=uo_in;
    updatestatus("UDP: useoffset "+QString::number(uo_in));
}

/************************************************************************************************
*                          cagecontrol::slot_changeWPangles                                     *
************************************************************************************************/
void cagecontrol::slot_changeWPangles(QVector<double> angles)
{
    if (angles.length()==(2*motorName.length())) {
        int len = motorName.length();
        HWP0=angles.mid(0,len);
        QWP0=angles.mid(len,2*len);
        pauseupdating=true;
        updateUI();
        SaveConfig();
        pauseupdating=false;
    } else {
        DEBUG_ERROR("Wrong number of entries: %i). Expected %i each.\n",angles.length(),motorName.length());
    }
}

/************************************************************************************************
*                             cagecontrol::~cagecontrol                                         *
************************************************************************************************/
cagecontrol::~cagecontrol()
{
    for (Motor* m : motors) {
        m->close();
    }
    SaveConfig();
    delete udplistener;
    delete settings;
}

/************************************************************************************************
*                           cagecontrol::slot_movemotors                                        *
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
*                              cagecontrol::openmotors                                          *
************************************************************************************************/
void cagecontrol::openmotors()
{
    comports.clear();
    for (QString s : motorName) {
        comports.append(tabs->findChild<QComboBox*>(s+"com")->currentText());
    }
    for (QString s : motorName) {
        Motor *motor = new Motor();
        motors.append(motor);
        motor->open(comports.at(motors.indexOf(motor)));
    }
}

/************************************************************************************************
*                                   cagecontrol::moveHV                                         *
************************************************************************************************/
void cagecontrol::moveHV(QString id)
{
    id=id.toLower();
    qDebug()<<"move " << id << " HV";
    double hwprot;
    double qwprot;
    for (QString s : motorName) {
        int idx = motorName.indexOf(s);
        if ((id=="all") || (id==s)) {
            if (invert[idx]) {
                hwprot=45;
                qwprot=0;
            } else {
                hwprot=0;
                qwprot=0;
            }
            movemotor(s,HWP0[idx]+hwprot,QWP0[idx]+qwprot);
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
}

/************************************************************************************************
*                                   cagecontrol::movePM                                         *
************************************************************************************************/
void cagecontrol::movePM(QString id)
{
    id=id.toLower();
    qDebug()<<"move " << id << " PM";
    double hwprot;
    double qwprot;
    for (QString s : motorName) {
        int idx = motorName.indexOf(s);
        if ((id=="all") || (id==s)) {
            if (invert[idx]) {
                hwprot=67.5;
                qwprot=45;
            } else {
                hwprot=22.5;
                qwprot=45;
            }
            movemotor(s,HWP0[idx]+hwprot,QWP0[idx]+qwprot);
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
}

/************************************************************************************************
*                                   cagecontrol::moveRL                                         *
************************************************************************************************/
void cagecontrol::moveRL(QString id)
{
    id=id.toLower();
    qDebug()<<"move " << id << " RL";
    double hwprot;
    double qwprot;
    for (QString s : motorName) {
        int idx = motorName.indexOf(s);
        if ((id=="all") || (id==s)) {
            if (invert[idx]) {
                hwprot=67.5;
                qwprot=0;
            } else {
                hwprot=22.5;
                qwprot=0;
            }
            movemotor(s,HWP0[idx]+hwprot,QWP0[idx]+qwprot);
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
}

/************************************************************************************************
*                                   cagecontrol::moveANG                                        *
************************************************************************************************/
void cagecontrol::moveANG(QString id)
{
    id=id.toLower();
    for (QString s : motorName) {
        if ((id=="all") || (id==s)) {
            int idx = motorName.indexOf(s);
            double HWPang = uiMotorGroupBoxes[idx]->findChild<QDoubleSpinBox*>("HWPsb")->value();
            double QWPang = uiMotorGroupBoxes[idx]->findChild<QDoubleSpinBox*>("QWPsb")->value();
            qDebug()<<"move " << s << " HWP:" << HWPang << ", QPW:" << QWPang;
            if (useoffset) {
                double hrot=HWPang+HWP0[idx];
                double qrot=QWPang+QWP0[idx];
                while (hrot > 340) {hrot=hrot-180;}
                while (qrot > 340) {qrot=qrot-180;}
                movemotor(s,hrot,qrot);
            } else {
                movemotor(s,HWPang,QWPang);
            }
        }
    }
}


/************************************************************************************************
*                               cagecontrol::movemotor                                          *
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

/************************************************************************************************
*                                cagecontrol::setupUI                                           *
************************************************************************************************/
void cagecontrol::setupUI(QGridLayout *layout)
{
    status = new QStatusBar();
    tabs = new QTabWidget();
    motorstab = new QWidget();
    settingstab = new QWidget();
    tabs->addTab(motorstab, "motors");
    tabs->addTab(settingstab, "settings");

    QGridLayout *motorlayout = new QGridLayout;
    QGridLayout *settingslayout = new QGridLayout;

    uiMotorGroupBoxes.clear();
    for (QString s : motorName) {
        int i=motorName.indexOf(s);
        uiMotorGroupBoxes.append(new QGroupBox());
        motorGB(uiMotorGroupBoxes[i],s);
        uiMotorGroupBoxes[i]->setObjectName(s+"box");
        motorlayout->addWidget(uiMotorGroupBoxes[i],i,1);
        qDebug()<<s+"box";
    }

    QGridLayout *buttons = new QGridLayout();
    buttons->setObjectName("buttonbox");
    CQPushButton *allsetbtn = new CQPushButton("Set all","all");
    allsetbtn->setObjectName("allsetbutton");
    CQPushButton *allhvbtn = new CQPushButton("H/V all","all");
    allhvbtn->setObjectName("allhvbutton");
    CQPushButton *allpmbtn = new CQPushButton("+/- all","all");
    allpmbtn->setObjectName("allpmbutton");
    CQPushButton *allrlbtn = new CQPushButton("R/L all","all");
    allrlbtn->setObjectName("allrlbutton");
    buttons->addWidget(allsetbtn,1,1,1,1);
    buttons->addWidget(allhvbtn,1,2,1,1);
    buttons->addWidget(allpmbtn,1,3,1,1);
    buttons->addWidget(allrlbtn,1,4,1,1);

    QLabel *filelabel = new QLabel("Load bases from");
    QLabel *baseschangelabel = new QLabel("change every");
    QPushButton *filebtn = new QPushButton("File");
    QPushButton *startbasesbtn = new QPushButton("start");
    startbasesbtn->setObjectName("startbasesbtn");
    QSpinBox *baseschangesb = new QSpinBox();
    QLineEdit *filele = new QLineEdit();
    filele->setObjectName("filele");
    baseschangesb->setObjectName("baseschangesb");
    baseschangesb->setSuffix(" s");
    filebtn->setObjectName("filebtn");
    QGridLayout *baseschangegrid = new QGridLayout();
    baseschangegrid->addWidget(filelabel,       1,1,1,1);
    baseschangegrid->addWidget(filebtn,         1,2,1,1);
    baseschangegrid->addWidget(filele,          2,1,1,2);
    baseschangegrid->addWidget(baseschangelabel,1,3,1,1);
    baseschangegrid->addWidget(baseschangesb,   1,4,1,1);
    baseschangegrid->addWidget(startbasesbtn,   2,4,1,1);

    motorlayout->addLayout(buttons,         motorName.length()+1,1);
    motorlayout->addLayout(baseschangegrid, motorName.length()+2,1);

    for (QString s : motorName) {
        int idx = motorName.indexOf(s);
        QLabel *label = new QLabel(s);
        QDoubleSpinBox *Hdsb = new QDoubleSpinBox();
        Hdsb->setRange(0,340);
        Hdsb->setObjectName(s+"HWP0sb");
        QDoubleSpinBox *Qdsb = new QDoubleSpinBox();
        Qdsb->setRange(0,340);
        Qdsb->setObjectName(s+"QWP0sb");
        QComboBox *cb = new QComboBox();
        cb->setObjectName(s+"com");
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            QStringList list;
            list << info.portName();
            cb->addItem(list.first(), list);
        }
        QSpinBox *Hsb = new QSpinBox();
        Hsb->setObjectName(s+"HWPnum");
        QSpinBox *Qsb = new QSpinBox();
        Qsb->setObjectName(s+"QWPnum");
        settingslayout->addWidget(label,    idx+2,1,1,1);
        settingslayout->addWidget(Hdsb,     idx+2,2,1,1);
        settingslayout->addWidget(Qdsb,     idx+2,3,1,1);
        settingslayout->addWidget(cb,       idx+2,4,1,1);
        settingslayout->addWidget(Hsb,      idx+2,5,1,1);
        settingslayout->addWidget(Qsb,      idx+2,6,1,1);
    }
    QLabel *settinglabel_HWP = new QLabel("HWP");
    QLabel *settinglabel_QWP = new QLabel("QWP");
    QLabel *settinglabel_COM = new QLabel("Port");
    QLabel *settinglabel_HWPmnum = new QLabel("HWP motor #");
    QLabel *settinglabel_QWPmnum = new QLabel("QWP motor #");

    QLabel *portlabel = new QLabel("UDP port: ");
    QSpinBox *portsb = new QSpinBox();
    portsb->setObjectName("port");
    portsb->setRange(0,65535);

    QCheckBox *offsetcb = new QCheckBox("use these angles as offset");
    offsetcb->setObjectName("offsetcb");

    settingslayout->addWidget(settinglabel_HWP,     1,2,1,1);
    settingslayout->addWidget(settinglabel_QWP,     1,3,1,1);
    settingslayout->addWidget(settinglabel_COM,     1,4,1,1);
    settingslayout->addWidget(settinglabel_HWPmnum, 1,5,1,1);
    settingslayout->addWidget(settinglabel_QWPmnum, 1,6,1,1);
    settingslayout->addWidget(portlabel,            9,1,1,1);
    settingslayout->addWidget(portsb,               9,2,1,1);
    settingslayout->addWidget(offsetcb,             8,1,1,3);

    settingslayout->setAlignment(Qt::AlignTop);

    motorstab->setLayout(motorlayout);
    settingstab->setLayout(settingslayout);

    }

/************************************************************************************************
*                                cagecontrol::updateUI                                          *
************************************************************************************************/
void cagecontrol::updateUI()
{
    for (QGroupBox *gb : uiMotorGroupBoxes) {
        int idx = uiMotorGroupBoxes.indexOf(gb);
        //QString id = motorName[idx];
        gb->findChild<QDoubleSpinBox*>("HWPsb")->setValue(HWPcust[idx]);
        gb->findChild<QDoubleSpinBox*>("QWPsb")->setValue(QWPcust[idx]);
        gb->findChild<QCheckBox*>("invertcb")->setChecked(invert[idx]);
    }
    for (QString s : motorName) {
        int idx = motorName.indexOf(s);
        tabs->findChild<QDoubleSpinBox*>(s+"HWP0sb")->setValue(HWP0[idx]);
        tabs->findChild<QDoubleSpinBox*>(s+"QWP0sb")->setValue(QWP0[idx]);
        tabs->findChild<QSpinBox*>(s+"HWPnum")->setValue(HWPmnum[idx]);
        tabs->findChild<QSpinBox*>(s+"QWPnum")->setValue(QWPmnum[idx]);
    }
    tabs->findChild<QSpinBox*>("port")->setValue(udpport);
    tabs->findChild<QCheckBox*>("offsetcb")->setChecked(useoffset);

    tabs->findChild<QLineEdit*>("filele")->setText(basesfname);
    tabs->findChild<QSpinBox*>("baseschangesb")->setValue(basestime);

}

/************************************************************************************************
*                                cagecontrol::motorGB                                           *
************************************************************************************************/
void cagecontrol::motorGB(QGroupBox *gb, QString id)
{
    QGridLayout *layout = new QGridLayout();
    CQPushButton *HVbtn = new CQPushButton("H/V",id);
    HVbtn->setObjectName("H/V");
    CQPushButton *DAbtn = new CQPushButton("+/-",id);
    DAbtn->setObjectName("+/-");
    CQPushButton *LRbtn = new CQPushButton("R/L",id);
    LRbtn->setObjectName("R/L");
    CQPushButton *Setbtn = new CQPushButton("set",id);
    Setbtn->setObjectName("set");
    QDoubleSpinBox *HWPsb = new QDoubleSpinBox();
    HWPsb->setObjectName("HWPsb");
    HWPsb->setRange(0,360);
    QDoubleSpinBox *QWPsb = new QDoubleSpinBox();
    QWPsb->setObjectName("QWPsb");
    QWPsb->setRange(0,360);
    QLabel *HWPlabel = new QLabel("HWP");
    QLabel *QWPlabel = new QLabel("QWP");
    QCheckBox *invcb = new QCheckBox("invert");
    invcb->setObjectName("invertcb");
    invcb->setToolTip("Rotates predefined bases: HV->VH, PM->MP, RL->LR");


    layout->addWidget(HWPlabel  ,1,1,1,1);
    layout->addWidget(HWPsb     ,1,2,1,1);
    layout->addWidget(QWPlabel  ,2,1,1,1);
    layout->addWidget(QWPsb     ,2,2,1,1);
    layout->addWidget(Setbtn    ,1,3,1,1);
    layout->addWidget(HVbtn     ,1,4,1,1);
    layout->addWidget(LRbtn     ,2,3,1,1);
    layout->addWidget(DAbtn     ,2,4,1,1);
    layout->addWidget(invcb     ,1,5,1,1);

    gb->setTitle(id);

    gb->setLayout(layout);
}

/************************************************************************************************
*                                cagecontrol::updatestatus                                      *
************************************************************************************************/
void cagecontrol::updatestatus(QString msg)
{
    status->showMessage(msg);
    DEBUG_INFO("%s\n",msg.toLocal8Bit().data());
    //TODO: write to logfile
}

/************************************************************************************************
*                                cagecontrol::setbasesfile                                      *
************************************************************************************************/
void cagecontrol::setbasesfile()
{
    QFileDialog *bfd = new QFileDialog();
    bfd->setAcceptMode(QFileDialog::AcceptOpen);
    bfd->setFileMode(QFileDialog::ExistingFile);
    bfd->setDirectory(QDir::current());
    if(bfd->exec()) {
        basesfname = bfd->selectedFiles()[0];
        tabs->findChild<QLineEdit*>("filele")->setText(basesfname);
    }

}

/************************************************************************************************
*                                cagecontrol::readbasesfile                                     *
************************************************************************************************/
int cagecontrol::readbasesfile()
{
    basesf.setFileName(basesfname);
    if (!basesf.open(QIODevice::ReadOnly | QIODevice::Text)) {
        DEBUG_ERROR("Error opening file");
        return 1;
    } else {
        bases.clear();
        while (!basesf.atEnd()) {
            QString line = basesf.readLine();
            line.remove("\n");
            line=line.simplified().toLower();
            QString tmpline = line;
            if (!tmpline.remove('x').remove('y').remove('z').simplified().isEmpty()) {
                DEBUG_ERROR("Line contains unknown basis: %s\n",line.toLocal8Bit().data());
                return 1;
            }
            QStringList bline = line.split(' ');
            if (bline.length()==motorName.length()) {
                bases.append(bline);
                qDebug()<<bases;
            } else {
                DEBUG_ERROR("wrong number of bases in file: %i. Expected %i.\n",bline.length(),motorName.length());
            }
        }
        return 0;
    }
}

/************************************************************************************************
*                                 cagecontrol::changebases                                      *
************************************************************************************************/
void cagecontrol::changebases()
{
    if (currentbasisidx<=-1) {
        updatesettings(3.141592654);//to get fname from QLineEdit in GUI
        if (!readbasesfile()) {
            currentbasisidx=0;
            //Change basis here
            QStringList currentbasis = bases[currentbasisidx];
            updatestatus("current basis: " + currentbasis.join(' '));
            for (QString s : motorName) {
                int idx = motorName.indexOf(s);
                if (currentbasis[idx].toLower()=="x") { //PM
                    movemotor(motorName[idx],HWP0[idx]+22.5,QWP0[idx]+45);
                } else if (currentbasis[idx].toLower()=="y") { //RL
                    movemotor(motorName[idx],HWP0[idx]+22.5,QWP0[idx]);
                } else if (currentbasis[idx].toLower()=="z") { //HV
                    movemotor(motorName[idx],HWP0[idx],QWP0[idx]);
                } else {
                    DEBUG_ERROR("unknown basis: %s\n",currentbasis[idx].toLower().toLocal8Bit().data());
                }
            }
            basestimer.start(basestime);
        } else {
            DEBUG_ERROR("Error reading file\n");
        }
    } else if (currentbasisidx>=bases.length()){
        currentbasisidx=-1;
        updatestatus("all bases from file set.\n");
    } else {
        QStringList currentbasis = bases[currentbasisidx];
        updatestatus("current basis: " + currentbasis.join(' '));
        for (QString s : motorName) {
            int idx = motorName.indexOf(s);
            if (currentbasis[idx].toLower()=="x") { //PM
                movemotor(motorName[idx],HWP0[idx]+22.5,QWP0[idx]+45);
            } else if (currentbasis[idx].toLower()=="y") { //RL
                movemotor(motorName[idx],HWP0[idx]+22.5,QWP0[idx]);
            } else if (currentbasis[idx].toLower()=="z") { //HV
                movemotor(motorName[idx],HWP0[idx],QWP0[idx]);
            } else {
                DEBUG_ERROR("unknown basis: %s\n",currentbasis[idx].toLower().toLocal8Bit().data());
            }
        }
        ++currentbasisidx;
        basestimer.start(basestime*1000);
    }
}

void cagecontrol::useinvertedbases(QString id, bool inv)
{
    pauseupdating=true;
    if (id.toLower()=="all") {
        for (int i=0; i<invert.length(); ++i) {
            invert[i]=inv;
        }
        updateUI();
    } else if (motorName.contains(id.toLower())) {
        int idx = motorName.indexOf(id);
        invert[idx]=inv;
        updateUI();
    } else {
        DEBUG_ERROR("no matching colorcode of cage found in vector motorName: %s", id.toLocal8Bit().data());
    }
    pauseupdating=false;
}
