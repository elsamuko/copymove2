TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = bin
OBJECTS_DIR = tmp
MOC_DIR = tmp
UI_DIR = tmp
RCC_DIR = tmp

SOURCES += main.cpp

LIBS += -lMagick++ -lMagickCore
QMAKE_CXXFLAGS += -fopenmp -I/usr/include/ImageMagick
QMAKE_CXXFLAGS += -gdwarf-3

# speed
QMAKE_CXXFLAGS_RELEASE += -march=core2 -msse2 -O3 -finline
QMAKE_LFLAGS_RELEASE += -flto

