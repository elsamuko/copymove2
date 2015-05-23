#ifndef LOG_H
#define LOG_H

#include <string>

#define LEVEL_INFO    " Info   "
#define LEVEL_WARNING " Warning"
#define LEVEL_ERROR   " Error  "

#define LOG(A)         writeLog( LEVEL_INFO, __FILE__, __LINE__, __FUNCTION__, A )
#define LOG_INFO(A)    writeLog( LEVEL_INFO, __FILE__, __LINE__, __FUNCTION__, A )
#define LOG_WARNING(A) writeLog( LEVEL_WARNING, __FILE__, __LINE__, __FUNCTION__, A )
#define LOG_ERROR(A)   writeLog( LEVEL_ERROR, __FILE__, __LINE__, __FUNCTION__, A )
#define LOGQT(TYPE,A,FILE,LINE,FUNC) writeLog( TYPE, FILE, LINE, FUNC, A )

bool writeLog( const char* level, const char* file, int line, const char* function, std::string content );


#endif // LOG_H
