TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

unix:!macx: CONFIG += linux

DESTDIR = bin
OBJECTS_DIR = tmp
MOC_DIR = tmp
UI_DIR = tmp
RCC_DIR = tmp

SOURCES += main.cpp
MAIN_DIR = ../..
PRI_DIR  = $${MAIN_DIR}/build/qmake
SRC_DIR  = $${MAIN_DIR}/src
include( $${PRI_DIR}/setup.pri )

include( $${PRI_DIR}/imagemagick.pri )
mac:   include( $${PRI_DIR}/mac.pri )
linux: include( $${PRI_DIR}/linux.pri )

