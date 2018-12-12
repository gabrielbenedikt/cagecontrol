#include "cagecontrol.h"
#include "ui_cagecontrol.h"

cagecontrol::cagecontrol(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::cagecontrol)
{
    ui->setupUi(this);
}

cagecontrol::~cagecontrol()
{
    delete ui;
}
