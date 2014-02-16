
TARGET = copymove2
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

macx:       PLATFORM=mac
win32:      PLATFORM=win
unix:!macx: PLATFORM=linux

MAIN_DIR = ../..
DESTDIR  = $${MAIN_DIR}/bin
PRI_DIR  = $${MAIN_DIR}/build/qmake
SRC_DIR  = $${MAIN_DIR}/src

include( $${PRI_DIR}/setup.pri )
include( $${PRI_DIR}/imagemagick.pri )
mac: include( $${PRI_DIR}/mac.pri )

SOURCES += $${SRC_DIR}/main.cpp \
    ../../src/shift.cpp
SOURCES += $${SRC_DIR}/block.cpp
HEADERS += $${SRC_DIR}/block.hpp \
    ../../src/shift.hpp
SOURCES += $${SRC_DIR}/greyimage.cpp
HEADERS += $${SRC_DIR}/greyimage.hpp
SOURCES += $${SRC_DIR}/dctsorter.cpp
HEADERS += $${SRC_DIR}/dctsorter.hpp

SOURCES += $${SRC_DIR}/ooura/shrtdct.cpp
HEADERS += $${SRC_DIR}/ooura/shrtdct.hpp

