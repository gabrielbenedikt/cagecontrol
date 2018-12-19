#include "cagecontrol.h"

/************************************************************************************************
*                                                                                               *
*                                cagecontrol::setupUI                                           *
*                                                                                               *
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

    /* TODO: this might already work. but cleanup/adaption is needed afterwards
    for (QString s : motorName) {
        int i=motorName.indexOf(s);
        uiMotorGroupBoxes[i] = new QGroupBox();
        motorGB(uiMotorGroupBoxes[i],s);
        uiMotorGroupBoxes[i]->setObjectName(s+"box");
        motorlayout->addWidget(uiMotorGroupBoxes[i]);
        qDebug()<<s+"box";
    }
    */

    QGroupBox *redbox = new QGroupBox();
    motorGB(redbox,"red");
    redbox->setObjectName("redbox");
    QGroupBox *brownbox = new QGroupBox();
    motorGB(brownbox,"brown");
    brownbox->setObjectName("brownbox");
    QGroupBox *greenbox = new QGroupBox();
    motorGB(greenbox,"green");
    greenbox->setObjectName("greenbox");
    QGroupBox *bluebox = new QGroupBox();
    motorGB(bluebox,"blue");
    bluebox->setObjectName("bluebox");
    QGroupBox *whitebox = new QGroupBox();
    motorGB(whitebox,"white");
    whitebox->setObjectName("whitebox");
    QGroupBox *blackbox = new QGroupBox();
    motorGB(blackbox,"black");
    blackbox->setObjectName("blackbox");

    QGridLayout *buttons = new QGridLayout();
    buttons->setObjectName("buttonbox");
    QPushButton *allsetbtn = new QPushButton("Set all");
    allsetbtn->setObjectName("allsetbutton");
    QPushButton *allhvbtn = new QPushButton("H/V all");
    allhvbtn->setObjectName("allhvbutton");
    QPushButton *allpmbtn = new QPushButton("+/- all");
    allpmbtn->setObjectName("allpmbutton");
    QPushButton *alllrbtn = new QPushButton("R/L all");
    alllrbtn->setObjectName("alllrbutton");
    buttons->addWidget(allsetbtn,1,1,1,1);
    buttons->addWidget(allhvbtn,1,2,1,1);
    buttons->addWidget(allpmbtn,1,3,1,1);
    buttons->addWidget(alllrbtn,1,4,1,1);

    motorlayout->addWidget(redbox,1,1);
    motorlayout->addWidget(brownbox,2,1);
    motorlayout->addWidget(greenbox,3,1);
    motorlayout->addWidget(bluebox,4,1);
    motorlayout->addWidget(whitebox,5,1);
    motorlayout->addWidget(blackbox,6,1);
    motorlayout->addLayout(buttons,7,1);

    QLabel *settinglabel_red = new QLabel("red");
    QLabel *settinglabel_brown = new QLabel("brown");
    QLabel *settinglabel_green = new QLabel("green");
    QLabel *settinglabel_blue = new QLabel("blue");
    QLabel *settinglabel_white = new QLabel("white");
    QLabel *settinglabel_black = new QLabel("black");
    QLabel *settinglabel_HWP = new QLabel("HWP");
    QLabel *settinglabel_QWP = new QLabel("QWP");
    QLabel *settinglabel_COM = new QLabel("Port");
    QLabel *settinglabel_HWPmnum = new QLabel("HWP motor #");
    QLabel *settinglabel_QWPmnum = new QLabel("QWP motor #");

    QComboBox *settingredcombox = new QComboBox();
    settingredcombox->setObjectName("redcom");
    QComboBox *settingbrowncombox = new QComboBox();
    settingbrowncombox->setObjectName("browncom");
    QComboBox *settinggreencombox = new QComboBox();
    settinggreencombox->setObjectName("greencom");
    QComboBox *settingbluecombox = new QComboBox();
    settingbluecombox->setObjectName("bluecom");
    QComboBox *settingwhitecombox = new QComboBox();
    settingwhitecombox->setObjectName("whitecom");
    QComboBox *settingblackcombox = new QComboBox();
    settingblackcombox->setObjectName("blackcom");

    QDoubleSpinBox *redHWP0sb = new QDoubleSpinBox();
    redHWP0sb->setObjectName("redHWP0sb");
    QDoubleSpinBox *redQWP0sb = new QDoubleSpinBox();
    redQWP0sb->setObjectName("redQWP0sb");
    QDoubleSpinBox *brownHWP0sb = new QDoubleSpinBox();
    brownHWP0sb->setObjectName("brownHWP0sb");
    QDoubleSpinBox *brownQWP0sb = new QDoubleSpinBox();
    brownQWP0sb->setObjectName("brownQWP0sb");
    QDoubleSpinBox *greenHWP0sb = new QDoubleSpinBox();
    greenHWP0sb->setObjectName("greenHWP0sb");
    QDoubleSpinBox *greenQWP0sb = new QDoubleSpinBox();
    greenQWP0sb->setObjectName("greenQWP0sb");
    QDoubleSpinBox *blueHWP0sb = new QDoubleSpinBox();
    blueHWP0sb->setObjectName("blueHWP0sb");
    QDoubleSpinBox *blueQWP0sb = new QDoubleSpinBox();
    blueQWP0sb->setObjectName("blueQWP0sb");
    QDoubleSpinBox *whiteHWP0sb = new QDoubleSpinBox();
    whiteHWP0sb->setObjectName("whiteHWP0sb");
    QDoubleSpinBox *whiteQWP0sb = new QDoubleSpinBox();
    whiteQWP0sb->setObjectName("whiteQWP0sb");
    QDoubleSpinBox *blackHWP0sb = new QDoubleSpinBox();
    blackHWP0sb->setObjectName("blackHWP0sb");
    QDoubleSpinBox *blackQWP0sb = new QDoubleSpinBox();
    blackQWP0sb->setObjectName("blackQWP0sb");
    redHWP0sb->setRange(0,330);
    redQWP0sb->setRange(0,330);
    brownHWP0sb->setRange(0,330);
    brownQWP0sb->setRange(0,330);
    greenHWP0sb->setRange(0,330);
    greenQWP0sb->setRange(0,330);
    blueHWP0sb->setRange(0,330);
    blueQWP0sb->setRange(0,330);
    whiteHWP0sb->setRange(0,330);
    whiteQWP0sb->setRange(0,330);
    blackHWP0sb->setRange(0,330);
    blackQWP0sb->setRange(0,330);

    QSpinBox *redHWPnum = new QSpinBox();
    redHWPnum->setObjectName("redHWPnum");
    QSpinBox *brownHWPnum = new QSpinBox();
    brownHWPnum->setObjectName("brownHWPnum");
    QSpinBox *greenHWPnum = new QSpinBox();
    greenHWPnum->setObjectName("greenHWPnum");
    QSpinBox *blueHWPnum = new QSpinBox();
    blueHWPnum->setObjectName("blueHWPnum");
    QSpinBox *whiteHWPnum = new QSpinBox();
    whiteHWPnum->setObjectName("whiteHWPnum");
    QSpinBox *blackHWPnum = new QSpinBox();
    blackHWPnum->setObjectName("blackHWPnum");
    QSpinBox *redQWPnum = new QSpinBox();
    redQWPnum->setObjectName("redQWPnum");
    QSpinBox *brownQWPnum = new QSpinBox();
    brownQWPnum->setObjectName("brownQWPnum");
    QSpinBox *greenQWPnum = new QSpinBox();
    greenQWPnum->setObjectName("greenQWPnum");
    QSpinBox *blueQWPnum = new QSpinBox();
    blueQWPnum->setObjectName("blueQWPnum");
    QSpinBox *whiteQWPnum = new QSpinBox();
    whiteQWPnum->setObjectName("whiteQWPnum");
    QSpinBox *blackQWPnum = new QSpinBox();
    blackQWPnum->setObjectName("blackQWPnum");

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
    settingslayout->addWidget(settinglabel_red,     2,1,1,1);
    settingslayout->addWidget(settinglabel_brown,   3,1,1,1);
    settingslayout->addWidget(settinglabel_green,   4,1,1,1);
    settingslayout->addWidget(settinglabel_blue,    5,1,1,1);
    settingslayout->addWidget(settinglabel_white,   6,1,1,1);
    settingslayout->addWidget(settinglabel_black,   7,1,1,1);
    settingslayout->addWidget(redHWP0sb,            2,2,1,1);
    settingslayout->addWidget(brownHWP0sb,          3,2,1,1);
    settingslayout->addWidget(greenHWP0sb,          4,2,1,1);
    settingslayout->addWidget(blueHWP0sb,           5,2,1,1);
    settingslayout->addWidget(whiteHWP0sb,          6,2,1,1);
    settingslayout->addWidget(blackHWP0sb,          7,2,1,1);
    settingslayout->addWidget(redQWP0sb,            2,3,1,1);
    settingslayout->addWidget(brownQWP0sb,          3,3,1,1);
    settingslayout->addWidget(greenQWP0sb,          4,3,1,1);
    settingslayout->addWidget(blueQWP0sb,           5,3,1,1);
    settingslayout->addWidget(whiteQWP0sb,          6,3,1,1);
    settingslayout->addWidget(blackQWP0sb,          7,3,1,1);
    settingslayout->addWidget(settingredcombox,     2,4,1,1);
    settingslayout->addWidget(settingbrowncombox,   3,4,1,1);
    settingslayout->addWidget(settinggreencombox,   4,4,1,1);
    settingslayout->addWidget(settingbluecombox,    5,4,1,1);
    settingslayout->addWidget(settingwhitecombox,   6,4,1,1);
    settingslayout->addWidget(settingblackcombox,   7,4,1,1);
    settingslayout->addWidget(redHWPnum,            2,5,1,1);
    settingslayout->addWidget(brownHWPnum,          3,5,1,1);
    settingslayout->addWidget(greenHWPnum,          4,5,1,1);
    settingslayout->addWidget(blueHWPnum,           5,5,1,1);
    settingslayout->addWidget(whiteHWPnum,          6,5,1,1);
    settingslayout->addWidget(blackHWPnum,          7,5,1,1);
    settingslayout->addWidget(redQWPnum,            2,6,1,1);
    settingslayout->addWidget(brownQWPnum,          3,6,1,1);
    settingslayout->addWidget(greenQWPnum,          4,6,1,1);
    settingslayout->addWidget(blueQWPnum,           5,6,1,1);
    settingslayout->addWidget(whiteQWPnum,          6,6,1,1);
    settingslayout->addWidget(blackQWPnum,          7,6,1,1);
    settingslayout->addWidget(portlabel,            9,1,1,1);
    settingslayout->addWidget(portsb,               9,2,1,1);
    settingslayout->addWidget(offsetcb,             8,1,1,3);

    settingredcombox->clear();
    settingbrowncombox->clear();
    settinggreencombox->clear();
    settingbluecombox->clear();
    settingwhitecombox->clear();
    settingblackcombox->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        list << info.portName();

        settingredcombox->addItem(list.first(), list);
        settingbrowncombox->addItem(list.first(), list);
        settinggreencombox->addItem(list.first(), list);
        settingbluecombox->addItem(list.first(), list);
        settingwhitecombox->addItem(list.first(), list);
        settingblackcombox->addItem(list.first(), list);
    }

    settingslayout->setAlignment(Qt::AlignTop);

    motorstab->setLayout(motorlayout);
    settingstab->setLayout(settingslayout);

}

/************************************************************************************************
*                                                                                               *
*                                cagecontrol::updateUI                                          *
*                                                                                               *
************************************************************************************************/
void cagecontrol::updateUI()
{
    tabs->findChild<QDoubleSpinBox*>("redHWP0sb")->setValue(HWP0[0]);
    tabs->findChild<QDoubleSpinBox*>("redQWP0sb")->setValue(QWP0[0]);
    tabs->findChild<QDoubleSpinBox*>("brownHWP0sb")->setValue(HWP0[1]);
    tabs->findChild<QDoubleSpinBox*>("brownQWP0sb")->setValue(QWP0[1]);
    tabs->findChild<QDoubleSpinBox*>("greenHWP0sb")->setValue(HWP0[2]);
    tabs->findChild<QDoubleSpinBox*>("greenQWP0sb")->setValue(QWP0[2]);
    tabs->findChild<QDoubleSpinBox*>("blueHWP0sb")->setValue(HWP0[3]);
    tabs->findChild<QDoubleSpinBox*>("blueQWP0sb")->setValue(QWP0[3]);
    tabs->findChild<QDoubleSpinBox*>("whiteHWP0sb")->setValue(HWP0[4]);
    tabs->findChild<QDoubleSpinBox*>("whiteQWP0sb")->setValue(QWP0[4]);
    tabs->findChild<QDoubleSpinBox*>("blackHWP0sb")->setValue(HWP0[5]);
    tabs->findChild<QDoubleSpinBox*>("blackQWP0sb")->setValue(QWP0[5]);
    motorstab->findChild<QGroupBox*>("redbox")->findChild<QDoubleSpinBox*>("HWPsb")->setValue(HWPcust[0]);
    motorstab->findChild<QGroupBox*>("brownbox")->findChild<QDoubleSpinBox*>("HWPsb")->setValue(HWPcust[1]);
    motorstab->findChild<QGroupBox*>("greenbox")->findChild<QDoubleSpinBox*>("HWPsb")->setValue(HWPcust[2]);
    motorstab->findChild<QGroupBox*>("bluebox")->findChild<QDoubleSpinBox*>("HWPsb")->setValue(HWPcust[3]);
    motorstab->findChild<QGroupBox*>("whitebox")->findChild<QDoubleSpinBox*>("HWPsb")->setValue(HWPcust[4]);
    motorstab->findChild<QGroupBox*>("blackbox")->findChild<QDoubleSpinBox*>("HWPsb")->setValue(HWPcust[5]);
    motorstab->findChild<QGroupBox*>("redbox")->findChild<QDoubleSpinBox*>("QWPsb")->setValue(QWPcust[0]);
    motorstab->findChild<QGroupBox*>("brownbox")->findChild<QDoubleSpinBox*>("QWPsb")->setValue(QWPcust[1]);
    motorstab->findChild<QGroupBox*>("greenbox")->findChild<QDoubleSpinBox*>("QWPsb")->setValue(QWPcust[2]);
    motorstab->findChild<QGroupBox*>("bluebox")->findChild<QDoubleSpinBox*>("QWPsb")->setValue(QWPcust[3]);
    motorstab->findChild<QGroupBox*>("whitebox")->findChild<QDoubleSpinBox*>("QWPsb")->setValue(QWPcust[4]);
    motorstab->findChild<QGroupBox*>("blackbox")->findChild<QDoubleSpinBox*>("QWPsb")->setValue(QWPcust[5]);
    tabs->findChild<QSpinBox*>("redHWPnum")->setValue(HWPmnum[0]);
    tabs->findChild<QSpinBox*>("brownHWPnum")->setValue(HWPmnum[1]);
    tabs->findChild<QSpinBox*>("greenHWPnum")->setValue(HWPmnum[2]);
    tabs->findChild<QSpinBox*>("blueHWPnum")->setValue(HWPmnum[3]);
    tabs->findChild<QSpinBox*>("whiteHWPnum")->setValue(HWPmnum[4]);
    tabs->findChild<QSpinBox*>("blackHWPnum")->setValue(HWPmnum[5]);
    tabs->findChild<QSpinBox*>("redQWPnum")->setValue(QWPmnum[0]);
    tabs->findChild<QSpinBox*>("brownQWPnum")->setValue(QWPmnum[1]);
    tabs->findChild<QSpinBox*>("greenQWPnum")->setValue(QWPmnum[2]);
    tabs->findChild<QSpinBox*>("blueQWPnum")->setValue(QWPmnum[3]);
    tabs->findChild<QSpinBox*>("whiteQWPnum")->setValue(QWPmnum[4]);
    tabs->findChild<QSpinBox*>("blackQWPnum")->setValue(QWPmnum[5]);

    motorstab->findChild<QGroupBox*>("redbox")->findChild<QCheckBox*>("invertcb")->setChecked(invert[0]);
    motorstab->findChild<QGroupBox*>("brownbox")->findChild<QCheckBox*>("invertcb")->setChecked(invert[1]);
    motorstab->findChild<QGroupBox*>("greenbox")->findChild<QCheckBox*>("invertcb")->setChecked(invert[2]);
    motorstab->findChild<QGroupBox*>("bluebox")->findChild<QCheckBox*>("invertcb")->setChecked(invert[3]);
    motorstab->findChild<QGroupBox*>("whitebox")->findChild<QCheckBox*>("invertcb")->setChecked(invert[4]);
    motorstab->findChild<QGroupBox*>("blackbox")->findChild<QCheckBox*>("invertcb")->setChecked(invert[5]);
    tabs->findChild<QSpinBox*>("port")->setValue(udpport);
    tabs->findChild<QCheckBox*>("offsetcb")->setChecked(useoffset);
}

/************************************************************************************************
*                                                                                               *
*                                cagecontrol::motorGB                                           *
*                                                                                               *
************************************************************************************************/
void cagecontrol::motorGB(QGroupBox *gb, QString id)
{
    QGridLayout *layout = new QGridLayout();
    QPushButton *HVbtn = new QPushButton("H/V");
    HVbtn->setObjectName("H/V");
    QPushButton *DAbtn = new QPushButton("+/-");
    DAbtn->setObjectName("+/-");
    QPushButton *LRbtn = new QPushButton("R/L");
    LRbtn->setObjectName("L/R");
    QPushButton *Setbtn = new QPushButton("set");
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

void cagecontrol::updatestatus(QString msg)
{
    status->showMessage(msg);
    //TODO: write to logfile
}
