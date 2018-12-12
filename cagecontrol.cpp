#include "cagecontrol.h"
#include "ui_cagecontrol.h"

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


    //
    // UI-Setup
    QGridLayout *layout = new QGridLayout;
    tabs = new QTabWidget();
    motorstab = new QWidget();
    settingstab = new QWidget();
    tabs->addTab(motorstab, "motors");
    tabs->addTab(settingstab, "settings");

    QGridLayout *motorlayout = new QGridLayout;
    QGridLayout *settingslayout = new QGridLayout;

    QGroupBox *redbox = new QGroupBox();
    motorGB(redbox,"red");
    QGroupBox *brownbox = new QGroupBox;
    motorGB(brownbox,"brown");
    QGroupBox *greenbox = new QGroupBox;
    motorGB(greenbox,"green");
    QGroupBox *bluebox = new QGroupBox;
    motorGB(bluebox,"blue");
    QGroupBox *whitebox = new QGroupBox;
    motorGB(whitebox,"white");
    QGroupBox *blackbox = new QGroupBox;
    motorGB(blackbox,"black");

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

    settingslayout->addWidget(settinglabel_HWP,     1,2,1,1);
    settingslayout->addWidget(settinglabel_QWP,     1,3,1,1);
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

    settingslayout->setAlignment(Qt::AlignTop);

    motorstab->setLayout(motorlayout);
    settingstab->setLayout(settingslayout);
    setCentralWidget(tabs);
    centralWidget()->setLayout(layout);
    setWindowTitle("CageControl");

    //
    // Signal/Slot Connections
    connect(redbox->findChild<QPushButton*>("H/V"),&QAbstractButton::pressed,this,&cagecontrol::moveredHV);
    connect(redbox->findChild<QPushButton*>("+/-"),&QAbstractButton::pressed,this,&cagecontrol::moveredPM);
    connect(redbox->findChild<QPushButton*>("set"),&QAbstractButton::pressed,this,&cagecontrol::moveredANG);
    connect(brownbox->findChild<QPushButton*>("H/V"),&QAbstractButton::pressed,this,&cagecontrol::movebrownHV);
    connect(brownbox->findChild<QPushButton*>("+/-"),&QAbstractButton::pressed,this,&cagecontrol::movebrownPM);
    connect(brownbox->findChild<QPushButton*>("set"),&QAbstractButton::pressed,this,&cagecontrol::movebrownANG);
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

    LoadConfig();


}


void cagecontrol::moveredHV()
{
    double HWPang=0.0;
    double QWPang=0.0;
    movemotor("red",HWPang,QWPang);
}
void cagecontrol::moveredPM()
{
    double HWPang=0.0;
    double QWPang=0.0;
    movemotor("red",HWPang,QWPang);
}
void cagecontrol::moveredANG()
{
    double HWPang=0.0;
    double QWPang=0.0;
    movemotor("red",HWPang,QWPang);
}
void cagecontrol::movebrownHV()
{
    double HWPang=0.0;
    double QWPang=0.0;
    movemotor("brown",HWPang,QWPang);
}
void cagecontrol::movebrownPM()
{
    double HWPang=0.0;
    double QWPang=0.0;
    movemotor("brown",HWPang,QWPang);
}
void cagecontrol::movebrownANG()
{
    double HWPang=0.0;
    double QWPang=0.0;
    movemotor("brown",HWPang,QWPang);
}
void cagecontrol::movegreenHV()
{
    double HWPang=0.0;
    double QWPang=0.0;
    movemotor("green",HWPang,QWPang);
}
void cagecontrol::movegreenPM()
{
    double HWPang=0.0;
    double QWPang=0.0;
    movemotor("green",HWPang,QWPang);
}
void cagecontrol::movegreenANG()
{
    double HWPang=0.0;
    double QWPang=0.0;
    movemotor("green",HWPang,QWPang);
}
void cagecontrol::moveblueHV()
{
    double HWPang=0.0;
    double QWPang=0.0;
    movemotor("blue",HWPang,QWPang);
}
void cagecontrol::movebluePM()
{
    double HWPang=0.0;
    double QWPang=0.0;
    movemotor("blue",HWPang,QWPang);
}
void cagecontrol::moveblueANG()
{
    double HWPang=0.0;
    double QWPang=0.0;
    movemotor("blue",HWPang,QWPang);
}
void cagecontrol::movewhiteHV()
{
    double HWPang=0.0;
    double QWPang=0.0;
    movemotor("white",HWPang,QWPang);
}
void cagecontrol::movewhitePM()
{
    double HWPang=0.0;
    double QWPang=0.0;
    movemotor("white",HWPang,QWPang);
}
void cagecontrol::movewhiteANG()
{
    double HWPang=0.0;
    double QWPang=0.0;
    movemotor("white",HWPang,QWPang);
}
void cagecontrol::moveblackHV()
{
    double HWPang=0.0;
    double QWPang=0.0;
    movemotor("black",HWPang,QWPang);
}
void cagecontrol::moveblackPM()
{
    double HWPang=0.0;
    double QWPang=0.0;
    movemotor("black",HWPang,QWPang);
}
void cagecontrol::moveblackANG()
{
    double HWPang=0.0;
    double QWPang=0.0;
    movemotor("black",HWPang,QWPang);
}
/************************************************************************************************
*                                                                                               *
*                               cagecontrol::MoveMotor                                          *
*                                                                                               *
************************************************************************************************/
void cagecontrol::movemotor(QString motor, double HWPang, double QWPang)
{
    DEBUG_INFO("TODO: move motor");

}

/************************************************************************************************
*                                                                                               *
*                              cagecontrol::LoadConfig                                          *
*                                                                                               *
************************************************************************************************/
void cagecontrol::LoadConfig()
{
    double tmpdbl;
    DEBUG_INFO("TODO: Load config");
    tmpdbl = settings->value("WAVEPLATES/HWP/RED",0).toDouble();
    tabs->findChild<QDoubleSpinBox*>("redHWP0sb")->setValue(tmpdbl);
    tmpdbl = settings->value("WAVEPLATES/QWP/RED",0).toDouble();
    tabs->findChild<QDoubleSpinBox*>("redQWP0sb")->setValue(tmpdbl);
    tmpdbl = settings->value("WAVEPLATES/HWP/BROWN",0).toDouble();
    tabs->findChild<QDoubleSpinBox*>("brownHWP0sb")->setValue(tmpdbl);
    tmpdbl = settings->value("WAVEPLATES/QWP/BROWN",0).toDouble();
    tabs->findChild<QDoubleSpinBox*>("brownQWP0sb")->setValue(tmpdbl);
    tmpdbl = settings->value("WAVEPLATES/HWP/GREEN",0).toDouble();
    tabs->findChild<QDoubleSpinBox*>("greenHWP0sb")->setValue(tmpdbl);
    tmpdbl = settings->value("WAVEPLATES/QWP/GREEN",0).toDouble();
    tabs->findChild<QDoubleSpinBox*>("greenQWP0sb")->setValue(tmpdbl);
    tmpdbl = settings->value("WAVEPLATES/HWP/BLUE",0).toDouble();
    tabs->findChild<QDoubleSpinBox*>("blueHWP0sb")->setValue(tmpdbl);
    tmpdbl = settings->value("WAVEPLATES/QWP/BLUE",0).toDouble();
    tabs->findChild<QDoubleSpinBox*>("blueQWP0sb")->setValue(tmpdbl);
    tmpdbl = settings->value("WAVEPLATES/HWP/WHITE",0).toDouble();
    tabs->findChild<QDoubleSpinBox*>("whiteHWP0sb")->setValue(tmpdbl);
    tmpdbl = settings->value("WAVEPLATES/QWP/WHITE",0).toDouble();
    tabs->findChild<QDoubleSpinBox*>("whiteQWP0sb")->setValue(tmpdbl);
    tmpdbl = settings->value("WAVEPLATES/HWP/BLACK",0).toDouble();
    tabs->findChild<QDoubleSpinBox*>("blackHWP0sb")->setValue(tmpdbl);
    tmpdbl = settings->value("WAVEPLATES/QWP/BLACK",0).toDouble();
    tabs->findChild<QDoubleSpinBox*>("blackQWP0sb")->setValue(tmpdbl);
}

/************************************************************************************************
*                                                                                               *
*                              cagecontrol::SaveConfig                                          *
*                                                                                               *
************************************************************************************************/
void cagecontrol::SaveConfig()
{
    DEBUG_INFO("TODO: Save config");
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

    settings->sync();
}


void cagecontrol::motorGB(QGroupBox *gb, QString id)
{
    QGridLayout *layout = new QGridLayout();
    QPushButton *HVbtn = new QPushButton("H/V");
    HVbtn->setObjectName("H/V");
    QPushButton *DAbtn = new QPushButton("+/-");
    DAbtn->setObjectName("+/-");
    QPushButton *Setbtn = new QPushButton("set");
    Setbtn->setObjectName("set");
    QDoubleSpinBox *HWPsp = new QDoubleSpinBox();
    HWPsp->setObjectName("HWPsp");
    QDoubleSpinBox *QWPsp = new QDoubleSpinBox();
    QWPsp->setObjectName("QWPsp");
    QLabel *HWPlabel = new QLabel("HWP");
    QLabel *QWPlabel = new QLabel("QWP");

    layout->addWidget(HWPlabel,1,1,1,1);
    layout->addWidget(HWPsp   ,1,2,1,1);
    layout->addWidget(QWPlabel,2,1,1,1);
    layout->addWidget(QWPsp   ,2,2,1,1);
    layout->addWidget(Setbtn  ,1,3,2,1);
    layout->addWidget(HVbtn   ,1,4,1,1);
    layout->addWidget(DAbtn   ,2,4,1,1);

    gb->setTitle(id);

    gb->setLayout(layout);
}

cagecontrol::~cagecontrol()
{
    SaveConfig();
}
