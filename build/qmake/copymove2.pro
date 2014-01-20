
TARGET = copymove2
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
DESTDIR = bin

macx:       PLATFORM=mac
win32:      PLATFORM=win
unix:!macx: PLATFORM=linux

MAIN_DIR = ../..
PRI_DIR  = $${MAIN_DIR}/build/qmake
SRC_DIR  = $${MAIN_DIR}/src

include( $${PRI_DIR}/setup.pri )
include( $${PRI_DIR}/image.pri )
mac: include( $${PRI_DIR}/mac.pri )

SOURCES += $${SRC_DIR}/main.cpp
SOURCES += $${SRC_DIR}/block.cpp
HEADERS += $${SRC_DIR}/block.hpp

SOURCES += $${SRC_DIR}/ooura/shrtdct.cpp
HEADERS += $${SRC_DIR}/ooura/shrtdct.hpp

