#include "cagecontrol.h"

/************************************************************************************************
*                                                                                               *
*                              cagecontrol::cagecontrol                                         *
*                                                                                               *
************************************************************************************************/
cagecontrol::cagecontrol(QWidget *parent) :
    QMainWindow(parent)//,ui(new Ui::cagecontrol)
{
    settings = new QSettings("cagecontrol.conf", QSettings::IniFormat);

    // UI-Setup
    pauseupdating=false;
    QWidget *mainwidget = new QWidget();
    QGridLayout *mainlayout = new QGridLayout;
    setupUI(mainlayout);
    motorName<<"red"<<"brown"<<"green"<<"blue"<<"white"<<"black";
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
    uiMotorGroupBoxes.reserve(motorName.length());
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
*                                                                                               *
*                            cagecontrol::initconnections                                       *
*                                                                                               *
************************************************************************************************/
void cagecontrol::initconnections()
{
    //
    // Signal/Slot Connections

    connect(motorstab->findChild<QGroupBox*>("redbox")->findChild<QPushButton*>("H/V"),&QAbstractButton::pressed,this,&cagecontrol::moveredHV);
    connect(motorstab->findChild<QGroupBox*>("redbox")->findChild<QPushButton*>("+/-"),&QAbstractButton::pressed,this,&cagecontrol::moveredPM);
    connect(motorstab->findChild<QGroupBox*>("redbox")->findChild<QPushButton*>("set"),&QAbstractButton::pressed,this,&cagecontrol::moveredANG);
    connect(motorstab->findChild<QGroupBox*>("redbox")->findChild<QPushButton*>("L/R"),&QAbstractButton::pressed,this,&cagecontrol::moveredLR);
    connect(motorstab->findChild<QGroupBox*>("brownbox")->findChild<QPushButton*>("H/V"),&QAbstractButton::pressed,this,&cagecontrol::movebrownHV);
    connect(motorstab->findChild<QGroupBox*>("brownbox")->findChild<QPushButton*>("+/-"),&QAbstractButton::pressed,this,&cagecontrol::movebrownPM);
    connect(motorstab->findChild<QGroupBox*>("brownbox")->findChild<QPushButton*>("set"),&QAbstractButton::pressed,this,&cagecontrol::movebrownANG);
    connect(motorstab->findChild<QGroupBox*>("brownbox")->findChild<QPushButton*>("L/R"),&QAbstractButton::pressed,this,&cagecontrol::movebrownLR);
    connect(motorstab->findChild<QGroupBox*>("greenbox")->findChild<QPushButton*>("H/V"),&QAbstractButton::pressed,this,&cagecontrol::movegreenHV);
    connect(motorstab->findChild<QGroupBox*>("greenbox")->findChild<QPushButton*>("+/-"),&QAbstractButton::pressed,this,&cagecontrol::movegreenPM);
    connect(motorstab->findChild<QGroupBox*>("greenbox")->findChild<QPushButton*>("set"),&QAbstractButton::pressed,this,&cagecontrol::movegreenANG);
    connect(motorstab->findChild<QGroupBox*>("greenbox")->findChild<QPushButton*>("L/R"),&QAbstractButton::pressed,this,&cagecontrol::movegreenLR);
    connect(motorstab->findChild<QGroupBox*>("bluebox")->findChild<QPushButton*>("H/V"),&QAbstractButton::pressed,this,&cagecontrol::moveblueHV);
    connect(motorstab->findChild<QGroupBox*>("bluebox")->findChild<QPushButton*>("+/-"),&QAbstractButton::pressed,this,&cagecontrol::movebluePM);
    connect(motorstab->findChild<QGroupBox*>("bluebox")->findChild<QPushButton*>("set"),&QAbstractButton::pressed,this,&cagecontrol::moveblueANG);
    connect(motorstab->findChild<QGroupBox*>("bluebox")->findChild<QPushButton*>("L/R"),&QAbstractButton::pressed,this,&cagecontrol::moveblueLR);
    connect(motorstab->findChild<QGroupBox*>("whitebox")->findChild<QPushButton*>("H/V"),&QAbstractButton::pressed,this,&cagecontrol::movewhiteHV);
    connect(motorstab->findChild<QGroupBox*>("whitebox")->findChild<QPushButton*>("+/-"),&QAbstractButton::pressed,this,&cagecontrol::movewhitePM);
    connect(motorstab->findChild<QGroupBox*>("whitebox")->findChild<QPushButton*>("set"),&QAbstractButton::pressed,this,&cagecontrol::movewhiteANG);
    connect(motorstab->findChild<QGroupBox*>("whitebox")->findChild<QPushButton*>("L/R"),&QAbstractButton::pressed,this,&cagecontrol::movewhiteLR);
    connect(motorstab->findChild<QGroupBox*>("blackbox")->findChild<QPushButton*>("H/V"),&QAbstractButton::pressed,this,&cagecontrol::moveblackHV);
    connect(motorstab->findChild<QGroupBox*>("blackbox")->findChild<QPushButton*>("+/-"),&QAbstractButton::pressed,this,&cagecontrol::moveblackPM);
    connect(motorstab->findChild<QGroupBox*>("blackbox")->findChild<QPushButton*>("set"),&QAbstractButton::pressed,this,&cagecontrol::moveblackANG);
    connect(motorstab->findChild<QGroupBox*>("blackbox")->findChild<QPushButton*>("L/R"),&QAbstractButton::pressed,this,&cagecontrol::moveblackLR);

    connect(motorstab->findChild<QPushButton*>("allhvbutton"),&QAbstractButton::pressed,this,&cagecontrol::moveallhv);
    connect(motorstab->findChild<QPushButton*>("allpmbutton"),&QAbstractButton::pressed,this,&cagecontrol::moveallpm);
    connect(motorstab->findChild<QPushButton*>("alllrbutton"),&QAbstractButton::pressed,this,&cagecontrol::movealllr);
    connect(motorstab->findChild<QPushButton*>("allsetbutton"),&QAbstractButton::pressed,this,&cagecontrol::moveallarb);

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
    connect(udplistener, &UDPlistener::MoveHV, this, &cagecontrol::slot_moveHV);
    connect(udplistener, &UDPlistener::MovePM, this, &cagecontrol::slot_movePM);
    connect(udplistener, &UDPlistener::MoveLR, this, &cagecontrol::slot_moveLR);
    connect(udplistener, &UDPlistener::changeoffsetusage, this, &cagecontrol::slot_changeoffsetusage);
    connect(udplistener, &UDPlistener::changeWPangles,this,&cagecontrol::slot_changeWPangles);

    connect(&basestimer, &QTimer::timeout,this,&cagecontrol::changebases);


}

/************************************************************************************************
*                                                                                               *
*                            cagecontrol::updatesettings                                        *
*                                                                                               *
************************************************************************************************/
void cagecontrol::updatesettingsint(int i)
{
    UNUSED(i);
    updatesettings(3.1415);
}

void cagecontrol::updatesettings(double d)
{
    UNUSED(d); //just there to create a SLOT with matching parameter for signal &Q(Double)SpinBox::valueChanged
    if (!pauseupdating) {
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

        invert[0]=motorstab->findChild<QGroupBox*>("redbox")->findChild<QCheckBox*>("invertcb")->isChecked();
        invert[1]=motorstab->findChild<QGroupBox*>("brownbox")->findChild<QCheckBox*>("invertcb")->isChecked();
        invert[2]=motorstab->findChild<QGroupBox*>("greenbox")->findChild<QCheckBox*>("invertcb")->isChecked();
        invert[3]=motorstab->findChild<QGroupBox*>("bluebox")->findChild<QCheckBox*>("invertcb")->isChecked();
        invert[4]=motorstab->findChild<QGroupBox*>("whitebox")->findChild<QCheckBox*>("invertcb")->isChecked();
        invert[5]=motorstab->findChild<QGroupBox*>("blackbox")->findChild<QCheckBox*>("invertcb")->isChecked();

        udpport=settingstab->findChild<QSpinBox*>("port")->value();

        useoffset = settingstab->findChild<QCheckBox*>("offsetcb")->isChecked();

        basesfname = tabs->findChild<QLineEdit*>("filele")->text();
        basestime = tabs->findChild<QSpinBox*>("baseschangesb")->value();
    }

}

/************************************************************************************************
*                                                                                               *
*                              cagecontrol::LoadConfig                                          *
*                                                                                               *
************************************************************************************************/
void cagecontrol::LoadConfig()
{
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
        invert.append(settings->value("GUI/INVERT/"+QString::number(i)).toBool());
    }
    motorstab->findChild<QGroupBox*>("redbox")->findChild<QCheckBox*>("invertcb")->setChecked(invert[0]);
    motorstab->findChild<QGroupBox*>("brownbox")->findChild<QCheckBox*>("invertcb")->setChecked(invert[1]);
    motorstab->findChild<QGroupBox*>("greenbox")->findChild<QCheckBox*>("invertcb")->setChecked(invert[2]);
    motorstab->findChild<QGroupBox*>("bluebox")->findChild<QCheckBox*>("invertcb")->setChecked(invert[3]);
    motorstab->findChild<QGroupBox*>("whitebox")->findChild<QCheckBox*>("invertcb")->setChecked(invert[4]);
    motorstab->findChild<QGroupBox*>("blackbox")->findChild<QCheckBox*>("invertcb")->setChecked(invert[5]);

    useoffset=settings->value("GUI/OFFSET").toBool();
    tabs->findChild<QCheckBox*>("offsetcb")->setChecked(useoffset);

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
    if ((tmpstr=="") || (tmpidx==-1)) {
        tmpidx=0;
    }
    tabs->findChild<QComboBox*>("redcom")->setCurrentIndex(tmpidx);
    tmpstr = settings->value("MOTORS/COMBROWN","").toString();
    tmpidx=tabs->findChild<QComboBox*>("browncom")->findText(tmpstr);
    if ((tmpstr=="") || (tmpidx==-1)) {
        tmpidx=0;
    }
    tabs->findChild<QComboBox*>("browncom")->setCurrentIndex(tmpidx);
    tmpstr = settings->value("MOTORS/COMGREEN","").toString();
    tmpidx=tabs->findChild<QComboBox*>("greencom")->findText(tmpstr);
    if ((tmpstr=="") || (tmpidx==-1)) {
        tmpidx=0;
    }
    tabs->findChild<QComboBox*>("greencom")->setCurrentIndex(tmpidx);
    tmpstr = settings->value("MOTORS/COMBLUE","").toString();
    tmpidx=tabs->findChild<QComboBox*>("bluecom")->findText(tmpstr);
    if ((tmpstr=="") || (tmpidx==-1)) {
        tmpidx=0;
    }
    tabs->findChild<QComboBox*>("bluecom")->setCurrentIndex(tmpidx);
    tmpstr = settings->value("MOTORS/COMWHITE","").toString();
    tmpidx=tabs->findChild<QComboBox*>("whitecom")->findText(tmpstr);
    if ((tmpstr=="") || (tmpidx==-1)) {
        tmpidx=0;
    }
    tabs->findChild<QComboBox*>("whitecom")->setCurrentIndex(tmpidx);
    tmpstr = settings->value("MOTORS/COMBLACK","").toString();
    tmpidx=tabs->findChild<QComboBox*>("blackcom")->findText(tmpstr);
    if ((tmpstr=="") || (tmpidx==-1)) {
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

    tmpint=settings->value("NETWORK/UDDPPORT",0).toInt();
    udpport=tmpint;
    tabs->findChild<QSpinBox*>("port")->setValue(tmpint);

    basesfname = settings->value("GUI/BASESFNAME").toString();
    tabs->findChild<QLineEdit*>("filele")->setText(basesfname);
    tmpint = settings->value("GUI/BASESTIME").toInt();
    basestime=tmpint;
    tabs->findChild<QSpinBox*>("baseschangesb")->setValue(basestime);


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

    settings->setValue("NETWORK/UDDPPORT",tabs->findChild<QSpinBox*>("port")->value());

    for (int i=0; i<motorName.length(); ++i) {
        settings->setValue("MOTORS/HWP/"+QString::number(i),HWP0[i]);
        settings->setValue("MOTORS/QWP/"+QString::number(i),QWP0[i]);
        settings->setValue("GUI/HWP/"+QString::number(i),HWPcust[i]);
        settings->setValue("GUI/QWP/"+QString::number(i),QWPcust[i]);
        settings->setValue("MOTORS/HWPNUM/"+QString::number(i),HWPmnum[i]);
        settings->setValue("MOTORS/QWPNUM/"+QString::number(i),QWPmnum[i]);
        settings->setValue("GUI/INVERT/"+QString::number(i),invert[i]);
    }

    settings->setValue("GUI/OFFSET",useoffset);
    settings->setValue("GUI/BASESTIME",basestime);
    settings->setValue("GUI/BASESFNAME",basesfname);

    settings->sync();
}



/************************************************************************************************
*                                                                                               *
*                          cagecontrol::slot_changeoffsetusage                                  *
*                                                                                               *
************************************************************************************************/
void cagecontrol::slot_changeoffsetusage(bool uo_in)
{
    useoffset=uo_in;
    updatestatus("UDP: useoffset "+QString::number(uo_in));
}

/************************************************************************************************
*                                                                                               *
*                          cagecontrol::slot_changeWPangles                                     *
*                                                                                               *
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
    delete udplistener;
    delete settings;
}

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


    motorlayout->addWidget(redbox,          1,1);
    motorlayout->addWidget(brownbox,        2,1);
    motorlayout->addWidget(greenbox,        3,1);
    motorlayout->addWidget(bluebox,         4,1);
    motorlayout->addWidget(whitebox,        5,1);
    motorlayout->addWidget(blackbox,        6,1);
    motorlayout->addLayout(buttons,         7,1);
    motorlayout->addLayout(baseschangegrid, 8,1);

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

    tabs->findChild<QLineEdit*>("filele")->setText(basesfname);
    tabs->findChild<QSpinBox*>("baseschangesb")->setValue(basestime);

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
    DEBUG_INFO("%s\n",msg.toLocal8Bit().data());
    //TODO: write to logfile
}

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
