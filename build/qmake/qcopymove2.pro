# Qt config
QT += core gui widgets

TARGET = qcopymove2
TEMPLATE = app

macx:       PLATFORM=mac
win32:      PLATFORM=win
unix:!macx: PLATFORM=linux
unix:!macx: CONFIG += linux

MAIN_DIR = ../..
DESTDIR  = $${MAIN_DIR}/bin
PRI_DIR  = $${MAIN_DIR}/build/qmake
SRC_DIR  = $${MAIN_DIR}/src

include( $${PRI_DIR}/setup.pri )
include( $${PRI_DIR}/imagemagick.pri )
include( $${PRI_DIR}/ui.pri )
include( $${PRI_DIR}/log.pri )
mac:   include( $${PRI_DIR}/mac.pri )
linux: include( $${PRI_DIR}/linux.pri )

# SOURCES += $${SRC_DIR}/main.cpp
SOURCES += $${SRC_DIR}/block.cpp
HEADERS += $${SRC_DIR}/block.hpp
SOURCES += $${SRC_DIR}/shift.cpp
HEADERS += $${SRC_DIR}/shift.hpp
SOURCES += $${SRC_DIR}/shifthit.cpp
HEADERS += $${SRC_DIR}/shifthit.hpp
SOURCES += $${SRC_DIR}/offset.cpp
HEADERS += $${SRC_DIR}/offset.hpp
SOURCES += $${SRC_DIR}/greyimage.cpp
HEADERS += $${SRC_DIR}/greyimage.hpp
SOURCES += $${SRC_DIR}/dctsorter.cpp
HEADERS += $${SRC_DIR}/dctsorter.hpp
SOURCES += $${SRC_DIR}/ooura/shrtdct.cpp
HEADERS += $${SRC_DIR}/ooura/shrtdct.hpp
SOURCES += $${SRC_DIR}/threadpool.cpp
HEADERS += $${SRC_DIR}/threadpool.hpp
SOURCES += $${SRC_DIR}/sorterparams.cpp
HEADERS += $${SRC_DIR}/sorterparams.hpp
