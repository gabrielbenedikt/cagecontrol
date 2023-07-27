*-g++* {
            QMAKE_CXX = g++
            QMAKE_CFLAGS            += -Wall -Wextra -pipe -fdiagnostics-color=always -std=c++2a -Wl,-rpath,\'\$$ORIGIN\'
            QMAKE_CXXFLAGS          += -Wall -Wextra -pipe -fdiagnostics-color=always -std=c++2a -Wl,-rpath,\'\$$ORIGIN\'
            QMAKE_LFLAGS            += -Wall -Wextra -pipe -fdiagnostics-color=always -std=c++2a -Wl,-rpath,\'\$$ORIGIN\'
            QMAKE_CFLAGS_RELEASE    += -Werror -march=native -flto -O3 -fomit-frame-pointer
            QMAKE_CXXFLAGS_RELEASE  += -Werror -march=native -flto -O3 -fomit-frame-pointer
            QMAKE_LFLAGS_RELEASE    += -Werror -march=native -flto -O3 -fomit-frame-pointer
            QMAKE_CFLAGS_DEBUG    += -O0 -g
            QMAKE_CXXFLAGS_DEBUG  += -O0 -g
            QMAKE_LFLAGS_DEBUG    += -O0 -g
}

*-clang* {
            QMAKE_CFLAGS            += -Wall -Wextra -pipe -fcolor-diagnostics -fdiagnostics-fixit-info
            QMAKE_CXXFLAGS          += -Wall -Wextra -pipe -fcolor-diagnostics -fdiagnostics-fixit-info
            QMAKE_LFLAGS            += -Wall -Wextra -pipe -fcolor-diagnostics -fdiagnostics-fixit-info
            QMAKE_CFLAGS_RELEASE    += -Werror -march=native -O3 -fomit-frame-pointer #-flto #LTO needs llvm gold plugin
            QMAKE_CXXFLAGS_RELEASE  += -Werror -march=native -O3 -fomit-frame-pointer #-flto #LTO needs llvm gold plugin
            QMAKE_LFLAGS_RELEASE    += -Werror -march=native -O3 -fomit-frame-pointer #-flto #LTO needs llvm gold plugin
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
