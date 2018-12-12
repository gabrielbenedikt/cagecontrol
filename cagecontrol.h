#ifndef CAGECONTROL_H
#define CAGECONTROL_H

#include "debug.h"
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

private:
    QSettings *settings;
    QTabWidget *tabs;
    QWidget *settingstab;
    QWidget *motorstab;
    QVector<QString> comports;

    Motor *redmotor;
    Motor *brownmotor;
    Motor *greenmotor;
    Motor *bluemotor;
    Motor *whitemotor;
    Motor *blackmotor;

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
    void moveredANG();
    void movebrownHV();
    void movebrownPM();
    void movebrownANG();
    void movegreenHV();
    void movegreenPM();
    void movegreenANG();
    void moveblueHV();
    void movebluePM();
    void moveblueANG();
    void movewhiteHV();
    void movewhitePM();
    void movewhiteANG();
    void moveblackHV();
    void moveblackPM();
    void moveblackANG();
};

#endif // CAGECONTROL_H
