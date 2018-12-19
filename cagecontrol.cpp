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
