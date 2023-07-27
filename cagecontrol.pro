QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bin/cagecontrol
TEMPLATE = app

CONFIG(debug, debug|release) {
    CMAKE_BUILDTYPE = "DEBUG"
} else {
    CMAKE_BUILDTYPE = "RELEASE"
}

elliptec.target = libelliptecpp
elliptec.commands = echo "Building libelliptecpp.."; \
                    cmake -DCMAKE_INSTALL_PREFIX=$$OUT_PWD -DBUILD_EXAMPLES=OFF -DCMAKE_BUILD_TYPE=$$CMAKE_BUILDTYPE -B elliptecpp $$PWD/elliptecpp; \
                    $(MAKE) -C $$OUT_PWD/elliptecpp; \
                    $(MAKE) -C $$OUT_PWD/elliptecpp install; \
                    echo "Done building libelliptecpp.";

elliptec.depends =
pcbm.target = libpcbm
pcbm.commands = echo "Building libpcbm.."; \
                cmake -DCMAKE_INSTALL_PREFIX=$$OUT_PWD -DCMAKE_BUILD_TYPE=$$CMAKE_BUILDTYPE -B libpcbm $$PWD/libpcbm; \
                $(MAKE) -C $$OUT_PWD/libpcbm; \
                $(MAKE) -C $$OUT_PWD/libpcbm install; \
                echo "Done building libpcbm.";

pcbm.depends =
QMAKE_EXTRA_TARGETS += pcbm
QMAKE_EXTRA_TARGETS += elliptec
PRE_TARGETDEPS += libpcbm
PRE_TARGETDEPS += libelliptecpp



# compiler settings
include($$PWD/compiler.pri)

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++20
CONFIG(debug, debug|release) {
    LIBS += -L$$OUT_PWD/lib/ -lelliptecppd -lpcbmotord -lboost_system -lpthread
} else {
    LIBS += -L$$OUT_PWD/lib/ -lelliptecpp -lpcbmotor -lboost_system -lpthread
}
INCLUDEPATH += -I$$OUT_PWD/include

#
#Use git commit hash as version
#
version.target = version.h
version.depends = $$PWD/.git nevertruedependency #forces rebuild
nevertruedependency.CONFIG += recursive
QMAKE_EXTRA_TARGETS += nevertruedependency

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
    src/main.cpp \
    src/cagecontrol.cpp \
    src/motorwrapper.cpp \
    src/udplistener.cpp \
    src/helper.cpp \
    src/cqpushbutton.cpp

HEADERS += \
    src/cagecontrol.h \
    src/defines.h \
    src/debug.h \
    src/motorwrapper.h \
    src/version.h \
    src/udplistener.h \
    src/helper.h \
    src/cqpushbutton.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md \
    WPangles \
    scripts/doxygen_cagecontrol \
    scripts/createversion_linux.sh \
    scripts/createversion_win.bat \
    TODO.txt \

isEmpty(TARGET_EXT) {
    win32 {
        TARGET_CUSTOM_EXT = .exe
    }
    macx {
        TARGET_CUSTOM_EXT = .app
    }
} else {
    TARGET_CUSTOM_EXT = $${TARGET_EXT}
}

macx {
    DEPLOY_COMMAND = macdeployqt
}
win32{
    DEPLOY_COMMAND = windeployqt
}
win32{
    CONFIG( debug, debug|release ) {# debug
        DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/debug/$${TARGET}$${TARGET_CUSTOM_EXT}))
    } else {# release
        DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/release/$${TARGET}$${TARGET_CUSTOM_EXT}))
    }
}



#  # Uncomment the following line to help debug the deploy command when running qmake
#  warning($${DEPLOY_COMMAND} $${DEPLOY_TARGET})

# Use += instead of = if you use multiple QMAKE_POST_LINKs
QMAKE_POST_LINK += $${DEPLOY_COMMAND} $${DEPLOY_TARGET}
EXTRA_BINFILES += $$OUT_PWD/lib/*
EXTRA_OUT_FILES += bin/
for(FILE,EXTRA_BINFILES){
    QMAKE_POST_LINK += $$quote(cp $${FILE} $$EXTRA_OUT_FILES $$escape_expand(\\n\\t))
}
