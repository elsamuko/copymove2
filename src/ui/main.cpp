#include "mainwindow.hpp"
#include <QApplication>

int main( int argc, char* argv[] ) {

    QApplication::setOrganizationName( "FD Imaging" );
    QApplication::setOrganizationDomain( "fd-imaging.com" );
    QApplication::setApplicationName( "copymove2" );

    QApplication a( argc, argv );
    MainWindow w;
    w.show();

    return a.exec();
}
