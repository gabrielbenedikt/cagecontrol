#-------------------------------------------------
#
# Project created by QtCreator 2018-12-12T11:30:32
#
#-------------------------------------------------

QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bin/cagecontrol
TEMPLATE = app

# compiler settings
include($$PWD/compiler.pri)

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

#
#Use git commit hash as version
#
version.target = version.h
#version.depends = $$PWD/.git nevertruedependency #forces rebuild
nevertruedependency.CONFIG += recursive
#QMAKE_EXTRA_TARGETS += nevertruedependency

unix {
    !macx {
        version.commands = cd $$PWD ; ./createversion_linux.sh
        nevertruedependency.commands = echo ""
    }
}

win32 {
    version.commands = cd $$PWD & createversion_win.bat
    nevertruedependency.commands = echo
}

macx {
    #no mac to test.
    #version.commands = $$PWD/
    #nevertruedependency.commands = echo
}

SOURCES += \
        main.cpp \
        cagecontrol.cpp \
    motor.cpp \
    udplistener.cpp \
    helper.cpp

HEADERS += \
        cagecontrol.h \
    defines.h \
    debug.h \
    version.h \
    createversion_win.bat \
    motor.h \
    udplistener.h \
    helper.h

#FORMS += \
#        cagecontrol.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    doxygen_cagecontrol \
    createversion_linux.sh \
    TODO.txt
