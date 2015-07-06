#include <QApplication>

#ifdef _WIN32
#include <regex>
#endif // _WIN32

#include "mainwindow.hpp"
#include "log/log.hpp"


static inline void customMessageHandler( QtMsgType type, const QMessageLogContext& context, const QString& message ) {

    const char* cfile     = context.file ? context.file : "unknown";
    const char* cfunction = context.function ? context.function : "a::unknown()";

    // remove class and arguments
    std::string function( cfunction );
#ifdef _WIN32
    function = std::regex_replace( function, std::regex( "\\<.*\\>.*" ), "[]{}" ); // shorten lambdas
    function = std::regex_replace( function, std::regex( "\\(.*\\)" ), "" );       // clean arguments
    function = std::regex_replace( function, std::regex( ".*\\s{1}" ), "" );       // clean return values
    function = std::regex_replace( function, std::regex( ".*::" ), "" );           // clean namespaces
#endif // _WIN32

    switch( type ) {
        case QtDebugMsg:
            logging::writeLog( LEVEL_INFO, cfile, context.line, function.c_str(), message.toStdString() );
            break;

        case QtWarningMsg:
            logging::writeLog( LEVEL_WARNING, cfile, context.line, function.c_str(), message.toStdString() );
            break;

        case QtCriticalMsg:
            logging::writeLog( LEVEL_ERROR, cfile, context.line, function.c_str(), message.toStdString() );
            break;

        case QtFatalMsg:
            logging::writeLog( LEVEL_ERROR, cfile, context.line, function.c_str(), message.toStdString() );
            logging::writeLog( LEVEL_ERROR, cfile, context.line, function.c_str(), "Aborting..." );
    }

    if( type == QtFatalMsg ) {
        abort();
    }
}

int main( int argc, char* argv[] ) {
    qInstallMessageHandler( ::customMessageHandler );

    QApplication::setOrganizationName( "FD Imaging" );
    QApplication::setOrganizationDomain( "fd-imaging.com" );
    QApplication::setApplicationName( "copymove2" );

    QApplication a( argc, argv );
    MainWindow w;
    w.show();

    return a.exec();
}
