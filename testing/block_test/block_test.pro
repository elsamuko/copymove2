QT       += testlib

TARGET = block_test
CONFIG -= app_bundle
CONFIG += x86_64
TEMPLATE = app
DESTDIR = bin

macx: PLATFORM=mac
win32: PLATFORM=win
unix: !macx: PLATFORM=linux

MAIN_DIR=../..

PRI_DIR  = $${MAIN_DIR}/build/qmake
SRC_DIR  = $${MAIN_DIR}/src

include( $${PRI_DIR}/setup.pri )
macx: include( $${PRI_DIR}/mac.pri )

HEADERS += $${SRC_DIR}/block.hpp
SOURCES += $${SRC_DIR}/block.cpp

SOURCES += $${SRC_DIR}/ooura/shrtdct.cpp
HEADERS += $${SRC_DIR}/ooura/shrtdct.hpp

SOURCES += block_test.cpp

