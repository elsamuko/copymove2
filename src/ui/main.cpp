#include <QApplication>

#include "mainwindow.hpp"
#include "log/log.hpp"

int main( int argc, char* argv[] ) {
    qInstallMessageHandler( logging::customMessageHandler );

    QApplication::setOrganizationName( "FD Imaging" );
    QApplication::setOrganizationDomain( "fd-imaging.com" );
    QApplication::setApplicationName( "copymove2" );

    QApplication a( argc, argv );
    MainWindow w;
    w.show();

    return a.exec();
}
