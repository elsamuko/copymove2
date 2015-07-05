# Qt config
QT += core gui widgets

TARGET = CopyMove2
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
include( $${PRI_DIR}/ui.pri )
include( $${PRI_DIR}/log.pri )
include( $${PRI_DIR}/common.pri )
include( $${PRI_DIR}/version.pri )
mac:   include( $${PRI_DIR}/mac.pri )
linux: include( $${PRI_DIR}/linux.pri )

RESOURCES += $${MAIN_DIR}/resources/resources.qrc

mac: ICON = $${MAIN_DIR}/resources/icons/logo.icns
mac: QMAKE_POST_LINK += macdeployqt $${DESTDIR}/$${TARGET}.$${TEMPLATE};
mac: QMAKE_POST_LINK += $${MAIN_DIR}/scripts/fix_qt_frameworks_mac.sh $${DESTDIR}/$${TARGET}.$${TEMPLATE};

