
LIBS += -framework CoreServices \
        -framework Carbon \
        -framework ApplicationServices

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.10

QMAKE_CXXFLAGS_RELEASE += -msse2 -Ofast -finline -ffast-math -funsafe-math-optimizations

QMAKE_CXXFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON += -Wall -Wno-unknown-pragmas
