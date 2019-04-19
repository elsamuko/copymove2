TARGET = copymove2-cli
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

macx:       PLATFORM=mac
win32:      PLATFORM=win
unix:!macx: PLATFORM=linux
unix:!macx: CONFIG += linux

MAIN_DIR = ..
PRI_DIR  = $${MAIN_DIR}/qmake/include
SRC_DIR  = $${MAIN_DIR}/src

include( $${PRI_DIR}/setup.pri )
DESTDIR  = $${MAIN_DIR}/bin/$${COMPILE_MODE}

CONFIG += ioimage
include( $${PRI_DIR}/imagemagick.pri )
include( $${PRI_DIR}/log.pri )
include( $${PRI_DIR}/common.pri )
include( $${PRI_DIR}/version.pri )
mac:   include( $${PRI_DIR}/mac.pri )
linux: include( $${PRI_DIR}/linux.pri )
win32: CONFIG += static
win32: include( $${PRI_DIR}/win.pri )

include( $${PRI_DIR}/tclap.pri )
HEADERS += $${SRC_DIR}/cliparser.hpp
SOURCES += $${SRC_DIR}/cliparser.cpp
SOURCES += $${SRC_DIR}/main.cpp

win32 {
    RC_FILE = $${MAIN_DIR}/resources/win/copymove2-cli.rc
}

linux {
    # reduce binary size
    CONFIG(release, debug|release) { QMAKE_POST_LINK += strip --strip-all $${DESTDIR}/$${TARGET}; }

    # search in lib first for missing shared libraries
    QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/lib
}
