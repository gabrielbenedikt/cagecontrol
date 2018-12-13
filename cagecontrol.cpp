#include "cagecontrol.h"
#include "motor.h"
#include <QDebug>

/************************************************************************************************
*                                                                                               *
*                              cagecontrol::cagecontrol                                         *
*                                                                                               *
************************************************************************************************/
cagecontrol::cagecontrol(QWidget *parent) :
    QMainWindow(parent)//,ui(new Ui::cagecontrol)
{
    //settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(), QApplication::applicationName());
    settings = new QSettings("cagecontrol.conf", QSettings::IniFormat);

    // UI-Setup
    QGridLayout *layout = new QGridLayout;
    setupUI(layout);
    motorName={"red","brown","green","blue","white","black"};
    HWP0.reserve(motorName.length());
    QWP0.reserve(motorName.length());
    HWPcust.reserve(motorName.length());
    QWPcust.reserve(motorName.length());
    HWPmnum.reserve(motorName.length());
    QWPmnum.reserve(motorName.length());
    setCentralWidget(tabs);
    centralWidget()->setLayout(layout);
    setWindowTitle("CageControl");

    LoadConfig();
    initconnections();
    openmotors();
    updateUI();
}

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
}
/************************************************************************************************
*                                                                                               *
*                            cagecontrol::initconnections                                       *
*                                                                                               *
************************************************************************************************/
void cagecontrol::initconnections()
{
    QList<QDoubleSpinBox*> dsboxes = tabs->findChildren<QDoubleSpinBox*>();
    for(QDoubleSpinBox* box : dsboxes) {
        connect(box,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,&cagecontrol::updatesettings);
    }

    QList<QSpinBox*> sboxes = tabs->findChildren<QSpinBox*>();
    for(QSpinBox* box : sboxes) {
        connect(box,QOverload<int>::of(&QSpinBox::valueChanged),this,&cagecontrol::updatesettings);
    }
}

void cagecontrol::updatesettings(double d)
{
    HWP0[0]=settingstab->findChild<QDoubleSpinBox*>("redHWP0sb")->value();
    HWP0[1]=settingstab->findChild<QDoubleSpinBox*>("brownHWP0sb")->value();
    HWP0[2]=settingstab->findChild<QDoubleSpinBox*>("greenHWP0sb")->value();
    HWP0[3]=settingstab->findChild<QDoubleSpinBox*>("blueHWP0sb")->value();
    HWP0[4]=settingstab->findChild<QDoubleSpinBox*>("whiteHWP0sb")->value();
    HWP0[5]=settingstab->findChild<QDoubleSpinBox*>("blackHWP0sb")->value();
    QWP0[0]=settingstab->findChild<QDoubleSpinBox*>("redQWP0sb")->value();
    QWP0[1]=settingstab->findChild<QDoubleSpinBox*>("brownQWP0sb")->value();
    QWP0[2]=settingstab->findChild<QDoubleSpinBox*>("greenQWP0sb")->value();
    QWP0[3]=settingstab->findChild<QDoubleSpinBox*>("blueQWP0sb")->value();
    QWP0[4]=settingstab->findChild<QDoubleSpinBox*>("whiteQWP0sb")->value();
    QWP0[5]=settingstab->findChild<QDoubleSpinBox*>("blackQWP0sb")->value();
    HWPcust[0]=motorstab->findChild<QGroupBox*>("redbox")->findChild<QDoubleSpinBox*>("HWPsb")->value();
    HWPcust[1]=motorstab->findChild<QGroupBox*>("brownbox")->findChild<QDoubleSpinBox*>("HWPsb")->value();
    HWPcust[2]=motorstab->findChild<QGroupBox*>("greenbox")->findChild<QDoubleSpinBox*>("HWPsb")->value();
    HWPcust[3]=motorstab->findChild<QGroupBox*>("bluebox")->findChild<QDoubleSpinBox*>("HWPsb")->value();
    HWPcust[4]=motorstab->findChild<QGroupBox*>("whitebox")->findChild<QDoubleSpinBox*>("HWPsb")->value();
    HWPcust[5]=motorstab->findChild<QGroupBox*>("blackbox")->findChild<QDoubleSpinBox*>("HWPsb")->value();
    QWPcust[0]=motorstab->findChild<QGroupBox*>("redbox")->findChild<QDoubleSpinBox*>("QWPsb")->value();
    QWPcust[1]=motorstab->findChild<QGroupBox*>("brownbox")->findChild<QDoubleSpinBox*>("QWPsb")->value();
    QWPcust[2]=motorstab->findChild<QGroupBox*>("greenbox")->findChild<QDoubleSpinBox*>("QWPsb")->value();
    QWPcust[3]=motorstab->findChild<QGroupBox*>("bluebox")->findChild<QDoubleSpinBox*>("QWPsb")->value();
    QWPcust[4]=motorstab->findChild<QGroupBox*>("whitebox")->findChild<QDoubleSpinBox*>("QWPsb")->value();
    QWPcust[5]=motorstab->findChild<QGroupBox*>("blackbox")->findChild<QDoubleSpinBox*>("QWPsb")->value();
    HWPmnum[0]=settingstab->findChild<QSpinBox*>("redHWPnum")->value();
    HWPmnum[1]=settingstab->findChild<QSpinBox*>("brownHWPnum")->value();
    HWPmnum[2]=settingstab->findChild<QSpinBox*>("greenHWPnum")->value();
    HWPmnum[3]=settingstab->findChild<QSpinBox*>("blueHWPnum")->value();
    HWPmnum[4]=settingstab->findChild<QSpinBox*>("whiteHWPnum")->value();
    HWPmnum[5]=settingstab->findChild<QSpinBox*>("blackHWPnum")->value();
    QWPmnum[0]=settingstab->findChild<QSpinBox*>("redQWPnum")->value();
    QWPmnum[1]=settingstab->findChild<QSpinBox*>("brownQWPnum")->value();
    QWPmnum[2]=settingstab->findChild<QSpinBox*>("greenQWPnum")->value();
    QWPmnum[3]=settingstab->findChild<QSpinBox*>("blueQWPnum")->value();
    QWPmnum[4]=settingstab->findChild<QSpinBox*>("whiteQWPnum")->value();
    QWPmnum[5]=settingstab->findChild<QSpinBox*>("blackQWPnum")->value();

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
    /*
    redmotor = new Motor();
    brownmotor = new Motor();
    greenmotor = new Motor();
    bluemotor = new Motor();
    whitemotor = new Motor();
    blackmotor = new Motor();

    redmotor->open(comports[0]);
    brownmotor->open(comports[1]);
    greenmotor->open(comports[2]);
    bluemotor->open(comports[3]);
    whitemotor->open(comports[4]);
    blackmotor->open(comports[5]);
    motors.append(redmotor);
    motors.append(brownmotor);
    motors.append(greenmotor);
    motors.append(bluemotor);
    motors.append(whitemotor);
    motors.append(blackmotor);
    for (QString s: comports) {
        //
    }
    */
}

/************************************************************************************************
*                                                                                               *
*                                cagecontrol::setupUI                                           *
*                                                                                               *
************************************************************************************************/
void cagecontrol::setupUI(QGridLayout *layout)
{
    tabs = new QTabWidget();
    motorstab = new QWidget();
    settingstab = new QWidget();
    tabs->addTab(motorstab, "motors");
    tabs->addTab(settingstab, "settings");

    QGridLayout *motorlayout = new QGridLayout;
    QGridLayout *settingslayout = new QGridLayout;

    QGroupBox *redbox = new QGroupBox();
    motorGB(redbox,"red");
    redbox->setObjectName("redbox");
    QGroupBox *brownbox = new QGroupBox;
    motorGB(brownbox,"brown");
    brownbox->setObjectName("brownbox");
    QGroupBox *greenbox = new QGroupBox;
    motorGB(greenbox,"green");
    greenbox->setObjectName("greenbox");
    QGroupBox *bluebox = new QGroupBox;
    motorGB(bluebox,"blue");
    bluebox->setObjectName("bluebox");
    QGroupBox *whitebox = new QGroupBox;
    motorGB(whitebox,"white");
    whitebox->setObjectName("whitebox");
    QGroupBox *blackbox = new QGroupBox;
    motorGB(blackbox,"black");
    blackbox->setObjectName("blackbox");

    motorlayout->addWidget(redbox);
    motorlayout->addWidget(brownbox);
    motorlayout->addWidget(greenbox);
    motorlayout->addWidget(bluebox);
    motorlayout->addWidget(whitebox);
    motorlayout->addWidget(blackbox);

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
    redHWP0sb->setRange(0,360);
    redQWP0sb->setRange(0,360);
    brownHWP0sb->setRange(0,360);
    brownQWP0sb->setRange(0,360);
    greenHWP0sb->setRange(0,360);
    greenQWP0sb->setRange(0,360);
    blueHWP0sb->setRange(0,360);
    blueQWP0sb->setRange(0,360);
    whiteHWP0sb->setRange(0,360);
    whiteQWP0sb->setRange(0,360);
    blackHWP0sb->setRange(0,360);
    blackQWP0sb->setRange(0,360);

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

    //
    // Signal/Slot Connections
    connect(redbox->findChild<QPushButton*>("H/V"),&QAbstractButton::pressed,this,&cagecontrol::moveredHV);
    connect(redbox->findChild<QPushButton*>("+/-"),&QAbstractButton::pressed,this,&cagecontrol::moveredPM);
    connect(redbox->findChild<QPushButton*>("set"),&QAbstractButton::pressed,this,&cagecontrol::moveredANG);
    connect(motorstab->findChild<QGroupBox*>("brownbox")->findChild<QPushButton*>("H/V"),&QAbstractButton::pressed,this,&cagecontrol::movebrownHV);
    connect(motorstab->findChild<QGroupBox*>("brownbox")->findChild<QPushButton*>("+/-"),&QAbstractButton::pressed,this,&cagecontrol::movebrownPM);
    connect(motorstab->findChild<QGroupBox*>("brownbox")->findChild<QPushButton*>("set"),&QAbstractButton::pressed,this,&cagecontrol::movebrownANG);
    connect(greenbox->findChild<QPushButton*>("H/V"),&QAbstractButton::pressed,this,&cagecontrol::movegreenHV);
    connect(greenbox->findChild<QPushButton*>("+/-"),&QAbstractButton::pressed,this,&cagecontrol::movegreenPM);
    connect(greenbox->findChild<QPushButton*>("set"),&QAbstractButton::pressed,this,&cagecontrol::movegreenANG);
    connect(bluebox->findChild<QPushButton*>("H/V"),&QAbstractButton::pressed,this,&cagecontrol::moveblueHV);
    connect(bluebox->findChild<QPushButton*>("+/-"),&QAbstractButton::pressed,this,&cagecontrol::movebluePM);
    connect(bluebox->findChild<QPushButton*>("set"),&QAbstractButton::pressed,this,&cagecontrol::moveblueANG);
    connect(whitebox->findChild<QPushButton*>("H/V"),&QAbstractButton::pressed,this,&cagecontrol::movewhiteHV);
    connect(whitebox->findChild<QPushButton*>("+/-"),&QAbstractButton::pressed,this,&cagecontrol::movewhitePM);
    connect(whitebox->findChild<QPushButton*>("set"),&QAbstractButton::pressed,this,&cagecontrol::movewhiteANG);
    connect(blackbox->findChild<QPushButton*>("H/V"),&QAbstractButton::pressed,this,&cagecontrol::moveblackHV);
    connect(blackbox->findChild<QPushButton*>("+/-"),&QAbstractButton::pressed,this,&cagecontrol::moveblackPM);
    connect(blackbox->findChild<QPushButton*>("set"),&QAbstractButton::pressed,this,&cagecontrol::moveblackANG);

}

void cagecontrol::moveredHV()
{
    movemotor("red",HWP0[0],QWP0[0]);
}
void cagecontrol::moveredPM()
{
    movemotor("red",HWP0[0]+22.5,QWP0[0]+45);
}
void cagecontrol::moveredANG()
{
    double HWPang=motorstab->findChild<QGroupBox*>("redbox")->findChild<QDoubleSpinBox*>("HWPsb")->value();
    double QWPang=motorstab->findChild<QGroupBox*>("redbox")->findChild<QDoubleSpinBox*>("QWPsb")->value();
    movemotor("red",HWPang,QWPang);
}
void cagecontrol::movebrownHV()
{
    movemotor("brown",HWP0[1],QWP0[1]);
}
void cagecontrol::movebrownPM()
{
    movemotor("brown",HWP0[1]+22.5,QWP0[1]+45);
}
void cagecontrol::movebrownANG()
{
    double HWPang=motorstab->findChild<QGroupBox*>("brownbox")->findChild<QDoubleSpinBox*>("HWPsb")->value();
    double QWPang=motorstab->findChild<QGroupBox*>("brownbox")->findChild<QDoubleSpinBox*>("QWPsb")->value();
    movemotor("brown",HWPang,QWPang);
}
void cagecontrol::movegreenHV()
{
    movemotor("green",HWP0[2],QWP0[2]);
}
void cagecontrol::movegreenPM()
{
    movemotor("green",HWP0[2]+22.5,QWP0[2]+45);
}
void cagecontrol::movegreenANG()
{
    double HWPang=motorstab->findChild<QGroupBox*>("greenbox")->findChild<QDoubleSpinBox*>("HWPsb")->value();
    double QWPang=motorstab->findChild<QGroupBox*>("greenbox")->findChild<QDoubleSpinBox*>("QWPsb")->value();
    movemotor("green",HWPang,QWPang);
}
void cagecontrol::moveblueHV()
{
    movemotor("blue",HWP0[3],QWP0[3]);
}
void cagecontrol::movebluePM()
{
    movemotor("blue",HWP0[3]+22.5,QWP0[3]+45);
}
void cagecontrol::moveblueANG()
{
    double HWPang=motorstab->findChild<QGroupBox*>("bluebox")->findChild<QDoubleSpinBox*>("HWPsb")->value();
    double QWPang=motorstab->findChild<QGroupBox*>("bluebox")->findChild<QDoubleSpinBox*>("QWPsb")->value();
    movemotor("blue",HWPang,QWPang);
}
void cagecontrol::movewhiteHV()
{
    movemotor("white",HWP0[4],QWP0[4]);
}
void cagecontrol::movewhitePM()
{
    movemotor("white",HWP0[4]+22.5,QWP0[4]+45);
}
void cagecontrol::movewhiteANG()
{
    double HWPang=motorstab->findChild<QGroupBox*>("whitebox")->findChild<QDoubleSpinBox*>("HWPsb")->value();
    double QWPang=motorstab->findChild<QGroupBox*>("whitebox")->findChild<QDoubleSpinBox*>("QWPsb")->value();
    movemotor("white",HWPang,QWPang);
}
void cagecontrol::moveblackHV()
{
    movemotor("black",HWP0[5],QWP0[5]);
}
void cagecontrol::moveblackPM()
{
    movemotor("black",HWP0[5]+22.5,QWP0[5]+45);
}
void cagecontrol::moveblackANG()
{
    double HWPang=motorstab->findChild<QGroupBox*>("blackbox")->findChild<QDoubleSpinBox*>("HWPsb")->value();
    double QWPang=motorstab->findChild<QGroupBox*>("blackbox")->findChild<QDoubleSpinBox*>("QWPsb")->value();
    movemotor("black",HWPang,QWPang);
}
/************************************************************************************************
*                                                                                               *
*                               cagecontrol::MoveMotor                                          *
*                                                                                               *
************************************************************************************************/
void cagecontrol::movemotor(QString motor, double HWPang, double QWPang)
{
    DEBUG_INFO("TODO: move motor %s:\tHWP: %4.1f\tQWP: %4.1f\n",motor.toLatin1().data(), HWPang, QWPang);
    int i = motorName.indexOf(motor);
    if ((HWPmnum.at(i)==1) and (QWPmnum.at(i)==2)) {
        motors.at(i)->command_moveboth(HWPang,QWPang);
    } else if ((HWPmnum.at(i)==2) and (QWPmnum.at(i)==1)) {
        motors.at(i)->command_moveboth(HWPang,QWPang);
    } else {
        DEBUG_ERROR("undefined motor slot: HWP %i QWP %i\n",HWPmnum.at(i),QWPmnum.at(i));
    }

}

/************************************************************************************************
*                                                                                               *
*                              cagecontrol::LoadConfig                                          *
*                                                                                               *
************************************************************************************************/
void cagecontrol::LoadConfig()
{
    double tmpdbl;
    int tmpidx;
    int tmpint;
    QString tmpstr;

    for (int i=0; i<motorName.length(); ++i){
        HWP0.append(settings->value("MOTORS/HWP/"+QString::number(i)).toDouble());
        QWP0.append(settings->value("MOTORS/QWP/"+QString::number(i)).toDouble());
        HWPcust.append(settings->value("GUI/HWP/"+QString::number(i)).toDouble());
        QWPcust.append(settings->value("GUI/QWP/"+QString::number(i)).toDouble());
        HWPmnum.append(settings->value("MOTORS/HWPNUM/"+QString::number(i)).toInt());
        QWPmnum.append(settings->value("MOTORS/QWPNUM/"+QString::number(i)).toInt());
    }

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


    tmpstr = settings->value("MOTORS/COMRED","").toString();
    tmpidx=tabs->findChild<QComboBox*>("redcom")->findText(tmpstr);
    if ((tmpstr=="") or (tmpidx==-1)) {
        tmpidx=0;
    }
    tabs->findChild<QComboBox*>("redcom")->setCurrentIndex(tmpidx);
    tmpstr = settings->value("MOTORS/COMBROWN","").toString();
    tmpidx=tabs->findChild<QComboBox*>("browncom")->findText(tmpstr);
    if ((tmpstr=="") or (tmpidx==-1)) {
        tmpidx=0;
    }
    tabs->findChild<QComboBox*>("browncom")->setCurrentIndex(tmpidx);
    tmpstr = settings->value("MOTORS/COMGREEN","").toString();
    tmpidx=tabs->findChild<QComboBox*>("greencom")->findText(tmpstr);
    if ((tmpstr=="") or (tmpidx==-1)) {
        tmpidx=0;
    }
    tabs->findChild<QComboBox*>("greencom")->setCurrentIndex(tmpidx);
    tmpstr = settings->value("MOTORS/COMBLUE","").toString();
    tmpidx=tabs->findChild<QComboBox*>("bluecom")->findText(tmpstr);
    if ((tmpstr=="") or (tmpidx==-1)) {
        tmpidx=0;
    }
    tabs->findChild<QComboBox*>("bluecom")->setCurrentIndex(tmpidx);
    tmpstr = settings->value("MOTORS/COMWHITE","").toString();
    tmpidx=tabs->findChild<QComboBox*>("whitecom")->findText(tmpstr);
    if ((tmpstr=="") or (tmpidx==-1)) {
        tmpidx=0;
    }
    tabs->findChild<QComboBox*>("whitecom")->setCurrentIndex(tmpidx);
    tmpstr = settings->value("MOTORS/COMBLACK","").toString();
    tmpidx=tabs->findChild<QComboBox*>("blackcom")->findText(tmpstr);
    if ((tmpstr=="") or (tmpidx==-1)) {
        tmpidx=0;
    }
    tabs->findChild<QComboBox*>("blackcom")->setCurrentIndex(tmpidx);

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
}

/************************************************************************************************
*                                                                                               *
*                              cagecontrol::SaveConfig                                          *
*                                                                                               *
************************************************************************************************/
void cagecontrol::SaveConfig()
{
    QStringList dbltostrlist;

    settings->setValue("WAVEPLATES/HWP/RED",tabs->findChild<QDoubleSpinBox*>("redHWP0sb")->value());
    settings->setValue("WAVEPLATES/QWP/RED",tabs->findChild<QDoubleSpinBox*>("redQWP0sb")->value());
    settings->setValue("WAVEPLATES/HWP/BROWN",tabs->findChild<QDoubleSpinBox*>("brownHWP0sb")->value());
    settings->setValue("WAVEPLATES/QWP/BROWN",tabs->findChild<QDoubleSpinBox*>("brownQWP0sb")->value());
    settings->setValue("WAVEPLATES/HWP/GREEN",tabs->findChild<QDoubleSpinBox*>("greenHWP0sb")->value());
    settings->setValue("WAVEPLATES/QWP/GREEN",tabs->findChild<QDoubleSpinBox*>("greenQWP0sb")->value());
    settings->setValue("WAVEPLATES/HWP/BLUE",tabs->findChild<QDoubleSpinBox*>("blueHWP0sb")->value());
    settings->setValue("WAVEPLATES/QWP/BLUE",tabs->findChild<QDoubleSpinBox*>("blueQWP0sb")->value());
    settings->setValue("WAVEPLATES/HWP/WHITE",tabs->findChild<QDoubleSpinBox*>("whiteHWP0sb")->value());
    settings->setValue("WAVEPLATES/QWP/WHITE",tabs->findChild<QDoubleSpinBox*>("whiteQWP0sb")->value());
    settings->setValue("WAVEPLATES/HWP/BLACK",tabs->findChild<QDoubleSpinBox*>("blackHWP0sb")->value());
    settings->setValue("WAVEPLATES/QWP/BLACK",tabs->findChild<QDoubleSpinBox*>("blackQWP0sb")->value());
    settings->setValue("MOTORS/COMRED",tabs->findChild<QComboBox*>("redcom")->currentText());
    settings->setValue("MOTORS/COMBROWN",tabs->findChild<QComboBox*>("browncom")->currentText());
    settings->setValue("MOTORS/COMGREEN",tabs->findChild<QComboBox*>("greencom")->currentText());
    settings->setValue("MOTORS/COMBLUE",tabs->findChild<QComboBox*>("bluecom")->currentText());
    settings->setValue("MOTORS/COMWHITE",tabs->findChild<QComboBox*>("whitecom")->currentText());
    settings->setValue("MOTORS/COMBLACK",tabs->findChild<QComboBox*>("blackcom")->currentText());

    for (int i=0; i<motorName.length(); ++i) {
        settings->setValue("MOTORS/HWP/"+QString::number(i),HWP0[i]);
        settings->setValue("MOTORS/QWP/"+QString::number(i),QWP0[i]);
        settings->setValue("GUI/HWP/"+QString::number(i),HWPcust[i]);
        settings->setValue("GUI/QWP/"+QString::number(i),QWPcust[i]);
        settings->setValue("MOTORS/HWPNUM/"+QString::number(i),HWPmnum[i]);
        settings->setValue("MOTORS/QWPNUM/"+QString::number(i),QWPmnum[i]);
    }

    settings->sync();
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

    layout->addWidget(HWPlabel,1,1,1,1);
    layout->addWidget(HWPsb   ,1,2,1,1);
    layout->addWidget(QWPlabel,2,1,1,1);
    layout->addWidget(QWPsb   ,2,2,1,1);
    layout->addWidget(Setbtn  ,1,3,2,1);
    layout->addWidget(HVbtn   ,1,4,1,1);
    layout->addWidget(DAbtn   ,2,4,1,1);

    gb->setTitle(id);

    gb->setLayout(layout);
}

/************************************************************************************************
*                                                                                               *
*                             cagecontrol::~cagecontrol                                         *
*                                                                                               *
************************************************************************************************/
cagecontrol::~cagecontrol()
{
    for (Motor* m : motors) {
        m->close();
    }
    SaveConfig();
}
