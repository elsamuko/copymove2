#pragma once

#include <string>

#define LEVEL_DEBUG   " Debug  "
#define LEVEL_INFO    " Info   "
#define LEVEL_WARNING " Warning"
#define LEVEL_ERROR   " Error  "

#define LOG(A)         logging::writeLog( LEVEL_INFO,    __FILE__, __LINE__, __FUNCTION__, A )
#define LOG_DEBUG(A)   logging::writeLog( LEVEL_DEBUG,   __FILE__, __LINE__, __FUNCTION__, A )
#define LOG_INFO(A)    logging::writeLog( LEVEL_INFO,    __FILE__, __LINE__, __FUNCTION__, A )
#define LOG_WARNING(A) logging::writeLog( LEVEL_WARNING, __FILE__, __LINE__, __FUNCTION__, A )
#define LOG_ERROR(A)   logging::writeLog( LEVEL_ERROR,   __FILE__, __LINE__, __FUNCTION__, A )

#define CHECK_QT_CONNECT( A ) if( !A ) { LOG_WARNING( "QObject::connect() failed" ); }


namespace logging {
    bool            writeLog( const char* level, const char* cfile, int line, const char* function, std::string content );
    std::string     logFilename();
}
