#ifndef CAGECONTROL_H
#define CAGECONTROL_H

#include "debug.h"
#include "udplistener.h"
#include <QDebug>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSettings>
#include <QTabWidget>
#include <QComboBox>
#include <QSerialPortInfo>

namespace Ui {
class cagecontrol;
}

class Motor;
class UDPlistener;

class cagecontrol : public QMainWindow
{
    Q_OBJECT

public:
    explicit cagecontrol(QWidget *parent = nullptr);
    ~cagecontrol();

private slots:
    /*!
     * \brief updatesettings fills variables with data from GUI
     * \param d unused
     */
    void updatesettings(double d);
    /*!
     * \brief updatesettingsint wrapper, just calls \see updatesettings(double d)
     * \param i unused
     */
    void updatesettingsint(int i);
    /*!
     * \brief updateUI updates UI with supposedly new numbers (loaded from conf file, e.g.)
     */
    void updateUI();

public slots:
    /*!
     * \brief slot_changeoffsetusage changes the usage of the waveplate offset
     * \param useoffset true if waveplate offset is to be used
     *
     * The term 'offset' refers to the waveplate angles specified in the \see settingstab.
     * E.g.: 'H' of HWP specified in he settingstab is 50° and one wants to rotate the waveplate to H+10°.
     * If (useoffset==true), one needs to rotate the motor to 10°.
     * If (useoffset==false), one needs to rotate the motor to 60°.
     */
    void slot_changeoffsetusage(bool uo_in);
    /*!
     * \brief moveHV
     * \param color colorcode of the cage, or 'all'
     */
    void slot_moveHV(QString color);
    /*!
     * \brief movePM
     * \param color colorcode of the cage, or 'all'
     */
    void slot_movePM(QString color);
    /*!
     * \brief moveLR
     * \param color colorcode of the cage, or 'all'
     */
    void slot_moveLR(QString color);
    /*!
     * \brief movemotors
     * \param color colorcode of the cage, or 'all'
     */
    void slot_movemotors(QString color, double HWPang, double QWPang);
private:
    int udpport;                                        //!< Hold the UDP port to listen to for commandds
    bool pauseupdating;                                 //!< Keep updateUI and updatesettings from interfering with each other
    bool useoffset;                                     //!< If true, the angles in the settings-tab will be used as '0'. \see slot_changeoffsetusage
    QSettings *settings;                                //!< A QSettings object, used to store settings in a config file
    UDPlistener *udplistener;                           //!< Listens to a UDP port, aquiires & checks commands send to it
    QTabWidget *tabs;                                   //!< GUI tab widget
    QWidget *settingstab;                               //!< GUI tab containing settings
    QWidget *motorstab;                                 //!< GUI tab containing motor controls
    QStatusBar *status;                                 //!< Status bar
    QVector<QString> comports;                          //!< Vector containing available serial ports names ports

    Motor *redmotor;                                    //!< Serial connections to the red cage
    Motor *brownmotor;                                  //!< Serial connections to the brown cage
    Motor *greenmotor;                                  //!< Serial connections to the green cage
    Motor *bluemotor;                                   //!< Serial connections to the blue cage
    Motor *whitemotor;                                  //!< Serial connections to the white cage
    Motor *blackmotor;                                  //!< Serial connections to the black cage

    /*WIP - cleaner code*/
    /*Even nicer: put everything in motor class*/
    QVector<bool> invert;                               //!< True: invert predefined bases (H/V -> V/H, P/M->M/P, L/R->R/L)
    QVector<Motor*> motors;                             //!< List of serial connections to the cages
    QVector<QString> motorName;                         //!< List of colorcodes of the cages
    QVector<QDoubleSpinBox> HWP0sp;                     //!< List of QSpinBoxes to set the '0' of the HWPs
    QVector<QDoubleSpinBox> QWP0sp;                     //!< List of QSpinBoxes to set the '0' of the QWPs
    QVector<int> HWPmnum;                               //!< Motornumber of controller the HWP is connected to
    QVector<int> QWPmnum;                               //!< Motornumber of controller the QWP is connected to
    QVector<double> HWP0;                               //!< '0' of HWPs
    QVector<double> QWP0;                               //!< '0' of QWPs
    QVector<double> HWPcust;                            //!< custum set angle to rotate HWP to
    QVector<double> QWPcust;                            //!< custom set angle to rotate QWP to
    //UI
    QVector<QGroupBox*> uiMotorGroupBoxes;              //!< List of Groupboxes containing cage controls

    void setupUI(QGridLayout *layout);                  //!< Puts together the GUI
    void openmotors();                                  //!< Opens serial connections to the PCB motor controlllers

    /*!
     * \brief updatestatus writes message to statusbar and to a logfile
     * \param msg Message to write
     */
    void updatestatus(QString msg);

    /*!
     * \brief LoadConfig loads config from a file
     *
     * The dialog is set up with values already stored in the QSettings object. If a specific quantity
     * does not exist there, it is set to a standard value.
     */
    void LoadConfig();
    /*!
     * \brief SaveConfig stores config to a file
     *
     * The QSettings object is updated with the values received from the dialog and saved immediately.
     */
    void SaveConfig();

    /*!
     * \brief motorGB fills an empty QGroupBox with motor controls
     * \param gb empty QGroupBox
     * \param id colorcode of the cage
     */
    void motorGB(QGroupBox *gb,QString id);

    /*!
     * \brief initconnections connects Qt Signals to slots
     *
     * Defines what happens when a button is clicked, a number is changet, et cetera
     */
    void initconnections();

    /*!
     * \brief movemotor moves both motors of a cage to certain angles
     * \param motor colorcode of the cage
     * \param HWPang angle of the HWP in degrees
     * \param QWPang angle of the QWP in degrees
     */
    void movemotor(QString motor, double HWPang, double QWPang);

    //Note: there must be a better way to do this...
    /*!
     * \brief moveredHV moves red cage to HV basis
     */
    void moveredHV();
    /*!
     * \brief moveredPM moves red cage to PM basis
     */
    void moveredPM();
    /*!
     * \brief moveredLR moves red cage to RL basis
     */
    void moveredLR();
    /*!
     * \brief moveredANG  moves red cage to the angles set in the GUI
     */
    void moveredANG();
    /*!
     * \brief movebrownHV moves brown cage to HV basis
     */
    void movebrownHV();
    /*!
     * \brief movebrownPM moves brown cage to PM basis
     */
    void movebrownPM();
    /*!
     * \brief movebrownLR moves brown cage to RL basis
     */
    void movebrownLR();
    /*!
     * \brief movebrownANG moves brown cage to the angles set in the GUI
     */
    void movebrownANG();
    /*!
     * \brief movegreenHV moves green cage to HV basis
     */
    void movegreenHV();
    /*!
     * \brief movegreenPM moves green cage to PM basis
     */
    void movegreenPM();
    /*!
     * \brief movegreenLR moves green cage to RL basis
     */
    void movegreenLR();
    /*!
     * \brief movegreenANG moves green cage to the angles set in the GUI
     */
    void movegreenANG();
    /*!
     * \brief moveblueHV moves blue cage to HV basis
     */
    void moveblueHV();
    /*!
     * \brief movebluePM moves blue cage to PM basis
     */
    void movebluePM();
    /*!
     * \brief moveblueLR moves blue cage to RL basis
     */
    void moveblueLR();
    /*!
     * \brief moveblueANG moves white cage to the angles set in the GUI
     */
    void moveblueANG();
    /*!
     * \brief movewhiteHV moves white cage to HV basis
     */
    void movewhiteHV();
    /*!
     * \brief movewhitePM moves white cage to PM basis
     */
    void movewhitePM();
    /*!
     * \brief movewhiteLR moves white cage to RL basis
     */
    void movewhiteLR();
    /*!
     * \brief movewhiteANG moves black cage to the angles set in the GUI
     */
    void movewhiteANG();
    /*!
     * \brief moveblackHV moves black cage to HV basis
     */
    void moveblackHV();
    /*!
     * \brief moveblackPM moves black cage to PM basis
     */
    void moveblackPM();
    /*!
     * \brief moveblackLR moves black cage to RL basis
     */
    void moveblackLR();
    /*!
     * \brief moveblackANG
     */
    void moveblackANG();
    /*!
     * \brief moveallhv
     */
    void moveallhv();
    /*!
     * \brief moveallpm
     */
    void moveallpm();
    /*!
     * \brief movealllr
     */
    void movealllr();
    /*!
     * \brief moveallarb
     */
    void moveallarb();
};

#endif // CAGECONTROL_H
