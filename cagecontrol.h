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

class cagecontrol : public QMainWindow
{
    Q_OBJECT

public:
    explicit cagecontrol(QWidget *parent = nullptr);
    ~cagecontrol();

private slots:
    void updatesettings(double d);
    void updateUI();

public slots:
    /*!
     * \brief moveHV
     * \param color
     */
    void slot_moveHV(QString color);
    /*!
     * \brief movePM
     * \param color
     */
    void slot_movePM(QString color);
    /*!
     * \brief moveLR
     * \param color
     */
    void slot_moveLR(QString color);
    /*!
     * \brief movemotors
     * \param color
     */
    void slot_movemotors(QString color, double HWPang, double QWPang);
private:
    int udpport;
    QSettings *settings;                                //!<
    UDPlistener *udplistener;
    QTabWidget *tabs;                                   //!<
    QWidget *settingstab;
    QWidget *motorstab;
    QVector<QString> comports;                          //!<Vector containing available serial ports names ports

    Motor *redmotor;
    Motor *brownmotor;
    Motor *greenmotor;
    Motor *bluemotor;
    Motor *whitemotor;
    Motor *blackmotor;

    /*WIP - cleaner code*/
    /*Even nicer: put everything in motor class*/
    QVector<Motor*> motors;//motor
    QVector<QString> motorName;//motor
    QVector<QDoubleSpinBox> HWP0sp;//ui
    QVector<QDoubleSpinBox> QWP0sp;//ui
    QVector<int> HWPmnum;//motor
    QVector<int> QWPmnum;//motor
    QVector<double> HWP0;//motor
    QVector<double> QWP0;//motor
    QVector<double> HWPcust;//motor
    QVector<double> QWPcust;//motor
    //UI
    QVector<QGroupBox*> uiMotorGroupBoxes;

    void setupUI(QGridLayout *layout);
    void openmotors();

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

    void motorGB(QGroupBox *gb,QString id);
    void initconnections();
    void movemotor(QString motor, double HWPang, double QWPang);

    //Note: there must be a better way to do this...
    void moveredHV();
    void moveredPM();
    void moveredLR();
    void moveredANG();
    void movebrownHV();
    void movebrownPM();
    void movebrownLR();
    void movebrownANG();
    void movegreenHV();
    void movegreenPM();
    void movegreenLR();
    void movegreenANG();
    void moveblueHV();
    void movebluePM();
    void moveblueLR();
    void moveblueANG();
    void movewhiteHV();
    void movewhitePM();
    void movewhiteLR();
    void movewhiteANG();
    void moveblackHV();
    void moveblackPM();
    void moveblackLR();
    void moveblackANG();
    void moveallhv();
    void moveallpm();
    void movealllr();
    void moveallarb();
};

#endif // CAGECONTROL_H
