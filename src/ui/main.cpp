#include "mainwindow.hpp"
#include <QApplication>

#ifdef _WIN32
#   include <regex>
#endif

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
            LOGQT( LEVEL_INFO, message.toStdString(), cfile, context.line, function.c_str() );
            break;

        case QtWarningMsg:
            LOGQT( LEVEL_WARNING, message.toStdString(), cfile, context.line, function.c_str() );
            break;

        case QtCriticalMsg:
            LOGQT( LEVEL_ERROR, message.toStdString(), cfile, context.line, function.c_str() );
            break;

        case QtFatalMsg:
            LOGQT( LEVEL_ERROR, message.toStdString(), cfile, context.line, function.c_str() );
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
