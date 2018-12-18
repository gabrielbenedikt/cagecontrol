#include "cagecontrol.h"
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("CageControl");
    a.setOrganizationName("Walther_Group_UNIVIE");
    a.setOrganizationDomain("http://www.univie.ac.at/gruppe-walther/");
    a.setQuitOnLastWindowClosed(true);
    cagecontrol w;
    w.show();
    return a.exec();
}
