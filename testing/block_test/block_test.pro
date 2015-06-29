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
include( $${PRI_DIR}/imagemagick.pri )
include( $${PRI_DIR}/log.pri )
macx: include( $${PRI_DIR}/mac.pri )
unix: !macx: include( $${PRI_DIR}/linux.pri )

HEADERS += $${SRC_DIR}/block.hpp
SOURCES += $${SRC_DIR}/block.cpp
HEADERS += $${SRC_DIR}/greyimage.hpp
SOURCES += $${SRC_DIR}/greyimage.cpp
SOURCES += $${SRC_DIR}/ooura/shrtdct.cpp
HEADERS += $${SRC_DIR}/ooura/shrtdct.hpp
SOURCES += $${SRC_DIR}/shift.cpp
HEADERS += $${SRC_DIR}/shift.hpp
SOURCES += $${SRC_DIR}/fileutils.cpp
HEADERS += $${SRC_DIR}/fileutils.hpp

SOURCES += block_test.cpp
HEADERS += sample.hpp

