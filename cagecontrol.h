#ifndef CAGECONTROL_H
#define CAGECONTROL_H

#include <QMainWindow>

namespace Ui {
class cagecontrol;
}

class cagecontrol : public QMainWindow
{
    Q_OBJECT

public:
    explicit cagecontrol(QWidget *parent = nullptr);
    ~cagecontrol();

private:
    Ui::cagecontrol *ui;
};

#endif // CAGECONTROL_H
