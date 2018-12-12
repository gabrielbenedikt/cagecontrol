#include "cagecontrol.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cagecontrol w;
    w.show();

    return a.exec();
}
