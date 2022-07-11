*-g++* {
            QMAKE_CXX = ccache g++
            QMAKE_CFLAGS            += -pipe -fdiagnostics-color=always -std=c++2a -Wl,-rpath='${ORIGIN}'
            QMAKE_CXXFLAGS          += -pipe -fdiagnostics-color=always -std=c++2a -Wl,-rpath='${ORIGIN}'
            QMAKE_LFLAGS            += -pipe -fdiagnostics-color=always -std=c++2a -Wl,-rpath='${ORIGIN}'
            QMAKE_CFLAGS_RELEASE    += -march=native -flto -O3 -fomit-frame-pointer -std=c++2a -Wl,-rpath='${ORIGIN}'
            QMAKE_CXXFLAGS_RELEASE  += -march=native -flto -O3 -fomit-frame-pointer -std=c++2a -Wl,-rpath='${ORIGIN}'
            QMAKE_LFLAGS_RELEASE    += -march=native -flto -O3 -fomit-frame-pointer -std=c++2a -Wl,-rpath='${ORIGIN}'
            QMAKE_CFLAGS_DEBUG    += -O0 -g -std=c++2a -Wl,-rpath='${ORIGIN}'
            QMAKE_CXXFLAGS_DEBUG  += -O0 -g -std=c++2a -Wl,-rpath='${ORIGIN}'
            QMAKE_LFLAGS_DEBUG    += -O0 -g -std=c++2a -Wl,-rpath='${ORIGIN}'
}

*-clang* {
            QMAKE_CFLAGS            += -pipe -fcolor-diagnostics -fdiagnostics-fixit-info
            QMAKE_CXXFLAGS          += -pipe -fcolor-diagnostics -fdiagnostics-fixit-info
            QMAKE_LFLAGS            += -pipe -fcolor-diagnostics -fdiagnostics-fixit-info
            QMAKE_CFLAGS_RELEASE    += -march=native -O3 -fomit-frame-pointer #-flto #LTO needs llvm gold plugin
            QMAKE_CXXFLAGS_RELEASE  += -march=native -O3 -fomit-frame-pointer #-flto #LTO needs llvm gold plugin
            QMAKE_LFLAGS_RELEASE    += -march=native -O3 -fomit-frame-pointer #-flto #LTO needs llvm gold plugin
            QMAKE_CFLAGS_DEBUG    += -O0 -g #-Weverything
            QMAKE_CXXFLAGS_DEBUG  += -O0 -g #-Weverything
            QMAKE_LFLAGS_DEBUG    += -O0 -g #-Weverything
}
msvc:QMAKE_CXXFLAGS += /Zi /DEBUG
msvc:QMAKE_LFLAGS += /INCREMENTAL:NO /DEBUG /OPT:REF /OPT:ICF
greaterThan(QT_MAJOR_VERSION, 4) {
    lessThan(QT_MINOR_VERSION, 7) {
        QMAKE_LFLAGS_WINDOWS += /SUBSYSTEM:WINDOWS,5.01
    }
}
