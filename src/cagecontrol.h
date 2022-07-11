#ifndef CAGECONTROL_H
#define CAGECONTROL_H

#include <algorithm>
#include "cqpushbutton.h"
#include "debug.h"
#include "motorwrapper.h"
#include "udplistener.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QDir>
#include <QDoubleSpinBox>
#include <QFile>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QSettings>
#include <QStatusBar>
#include <QTabWidget>
#include <QTimer>
#include <QSerialPortInfo>

namespace Ui {
class cagecontrol;
}

class rotmotor;
class motorwrapper;
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
     * \brief slot_changeWPangles sets offsetangles for all waveplates
     * \param angles vector containing all angles. Ordering: HWP0,HWP1,...,HWPn,QWP0,QWP1,...,QWPn
     */
    void slot_changeWPangles(QVector<double> angles);
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
     * \brief slot_movemotors
     * \param color colorcode of the cage, or 'all'
     * \param HWPang angle of the HWP in degrees
     * \param QWPang angle of the 1st QWP in degrees
     * \param QWP2ang angle of the 2nd QWP in degrees. defaults to 0 because of 2/3 WP cages
     */
    void slot_movemotors(QString color, double HWPang, double QWPang, double QWP2ang=0);

    /*!
     * \brief useinvertedbases
     * \param id colorcode of cage, or 'all'
     * \param inv true if consecutive bases changes shall be inverted (using e.g. V/H instead of H/V)
     */
    void useinvertedbases(QString id, bool inv);

    /*!
     * \brief moveHV moves cage with colorcode id to H/V basis
     * \param id colorcode of cage, or "all"
     */
    void moveHV(QString id);
    /*!
     * \brief movePM moves cage with colorcode id to P/M basis
     * \param id colorcode of cage, or "all"
     */
    void movePM(QString id);
    /*!
     * \brief moveRL moves cage with colorcode id to R/L basis
     * \param id colorcode of cage, or "all"
     */
    void moveRL(QString id);

    /*!
     * \brief moveANG moves cage with colorcode id to angles set in GUI
     * \param id colorcode of cage, or "all"
     */
    void moveANG(QString id);
private:
    QString basesfname;                                 //!< path and filename of file containing bases
    QTimer basestimer;                                  //!< Runs out every \see basestime seconds to change bases
    QDir basesdir;                                      //!< Directory of basesfile
    QFile basesf;                                       //!< Bases file
    int currentbasisidx;                                //!< index of current basis
    int basestime;                                      //!< When reading bases from file: Number of seconds after which a basischange occurs
    int udpport;                                        //!< Hold the UDP port to listen to for commandds
    bool pauseupdating;                                 //!< Keep updateUI and updatesettings from interfering with each other
    bool useoffset;                                     //!< If true, the angles in the settings-tab will be used as '0'. \see slot_changeoffsetusage
    QSettings *settings;                                //!< A QSettings object, used to store settings in a config file
    UDPlistener *udplistener;                           //!< Listens to a UDP port, aquiires & checks commands send to it
    QTabWidget *tabs;                                   //!< GUI tab widget
    QWidget *settingstab;                               //!< GUI tab containing settings
    QWidget *motorstab;                                 //!< GUI tab containing motor controls
    QStatusBar *status;                                 //!< Status bar
    QVector<std::string> comports;                          //!< Vector containing available serial ports names ports
    QVector<QStringList> bases;                         //!< vector holding all bases for automatic basis change

    /*WIP - cleaner code*/
    /*Even nicer: put everything in motor class*/
    QVector<bool> invert;                               //!< True: invert predefined bases (H/V -> V/H, P/M->M/P, L/R->R/L)
    QVector<bool> isthreewps;                           //!< True: cage has three waveplates. False: cage has two waveplates
    QVector<motorwrapper*> motors;                          //!< List of serial connections to the cages
    QVector<int> motorType;                             //!< 0: PCBmotor, 1 Thorlabs Elliptec
    QVector<QString> motorName;                         //!< List of colorcodes of the cages
    QVector<QDoubleSpinBox> HWP0sp;                     //!< List of QSpinBoxes to set the '0' of the HWPs
    QVector<QDoubleSpinBox> QWP0sp;                     //!< List of QSpinBoxes to set the '0' of the QWPs
    QVector<uint8_t> HWPmnum;                               //!< Motornumber of controller the HWP is connected to
    QVector<uint8_t> QWPmnum;                               //!< Motornumber of controller the first QWP is connected to
    QVector<uint8_t> QWP2mnum;                              //!< Motornumber of controller the second QWP is connected to
    QVector<double> HWP0;                               //!< '0' of HWPs
    QVector<double> QWP0;                               //!< '0' of first QWPs
    QVector<double> QWP20;                              //!< '0' of second QWPs
    QVector<double> HWPcust;                            //!< custum set angle to rotate HWP to
    QVector<double> QWPcust;                            //!< custom set angle to rotate first QWP to
    QVector<double> QWP2cust;                           //!< custom set angle to rotate second QWP to

    //UI
    QVector<QGroupBox*> uiMotorGroupBoxes;              //!< List of Groupboxes containing cage controls

    void setupUI(QGridLayout *layout);                  //!< Puts together the GUI
    void openmotors();                                  //!< Opens serial connections to the PCB motor controlllers


    /*!
     * \brief changebases changes bases periodically
     */
    void changebases();
    /*!
     * \brief setbasesfile reads filename from dialog
     */
    void setbasesfile();
    /*!
     * \brief readbasesfile reads file containing bases
     * \return 0 on success
     */
    int readbasesfile();
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
     * \brief invertall convenient way to tick all 'invert' boxes. does not rotate WPs.
     */
    void invertall();

    /*!
     * \brief movemotor moves both motors of a cage to certain angles
     * \param motor colorcode of the cage
     * \param HWPang angle of the HWP in degrees
     * \param QWPang angle of the 1st QWP in degrees
     * \param QWP2ang angle of the 2nd QWP in degrees. defaults to 0 because of 2/3 WP cages
     */
    void movemotor(QString motor, double HWPang, double QWPang, double QWP2ang=0);

};

#endif // CAGECONTROL_H
