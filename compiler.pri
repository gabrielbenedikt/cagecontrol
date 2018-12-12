#
#compiler&linker flags
#
*-g++* {
            QMAKE_CXX = ccache g++

            QMAKE_CFLAGS            += -pipe -fdiagnostics-color=always
            QMAKE_CXXFLAGS          += -pipe -fdiagnostics-color=always
            QMAKE_LFLAGS            += -pipe -fdiagnostics-color=always -Wl,-rpath='${ORIGIN}'
            QMAKE_CFLAGS_RELEASE    += -march=native -flto -O3 -fomit-frame-pointer
            QMAKE_CXXFLAGS_RELEASE  += -march=native -flto -O3 -fomit-frame-pointer
            QMAKE_LFLAGS_RELEASE    += -march=native -flto -O3 -fomit-frame-pointer
            QMAKE_CFLAGS_DEBUG    += -O0 -g
            QMAKE_CXXFLAGS_DEBUG  += -O0 -g
            QMAKE_LFLAGS_DEBUG    += -O0 -g
}

*-clang* {
            QMAKE_CFLAGS            += -pipe -fcolor-diagnostics -fdiagnostics-fixit-info -
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
