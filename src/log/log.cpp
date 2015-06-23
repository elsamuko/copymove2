#include "log.hpp"
#include "datetime.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <thread>
#include <mutex>
#include <cstdlib>
#if __APPLE__
#include <sys/stat.h>
#endif

static std::mutex mutex;

std::string logging::logFilename() {

#ifdef WIN32
    std::string filename( std::string( getenv( "temp" ) ) + "\\copymove.log" );
#elif __APPLE__
    std::string filename = std::string( getenv( "HOME" ) ) + "/Library/Caches/com.fd-imaging.copymove/copymove.log";
#else
    std::string filename( "/tmp/copymove.log" );
#endif // WIN32

    return filename;
}

static void rotateLog() {
    std::string filename = logging::logFilename();
    std::string backup1  = filename + ".1";
    std::string backup2  = filename + ".2";

    std::remove( backup2.c_str() );
    std::rename( backup1.c_str(),  backup2.c_str() );
    std::rename( filename.c_str(), backup1.c_str() );
}

void initLogFile() {
    static bool initialized = false;

    if( !initialized ) {
        initialized = true;

#if __APPLE__
        std::string cache = std::string( getenv( "HOME" ) ) + "/Library/Caches/com.fd-imaging.copymove";
        mkdir( cache.c_str(), 0777 );
#endif
        rotateLog();
    }
}

std::string threadId() {
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return ss.str().substr( 4, 4 );
}

std::string indent( const std::string& s, int count ) {
    std::stringstream ret;
    std::stringstream ss( s );
    std::string item;
    std::string whitespaces( count, ' ' );

    if( std::getline( ss, item ) ) {
        ret << item << std::endl;

        while( std::getline( ss, item ) ) {
            ret << whitespaces << item << std::endl;
        }
    }

    return ret.str();
}


bool logging::writeLog( const char* level, const char* cfile, int sourceline, const char* function, std::string content ) {

    std::stringstream logline;

    // time
    logline.width( 6 );
    logline << datetime::now();
    logline << level;
    // logline << " T" << threadId();

    std::string file( cfile );
#ifndef _WIN32
    file = file.substr( file.find_last_of( "/" ) + 1 );
#else
    file = file.substr( file.find_last_of( "\\" ) + 1 );
#endif // _WIN32
    file = file.substr( 0, file.find_last_of( "." ) );

    logline.width( 15 );
    logline << file << " ";

    logline.width( 30 );
    logline << function << "(";
    logline.width( 4 );
    logline.setf( std::ios::right );
    logline << sourceline << "): ";

    // indent paragraph by current size
    logline.seekg( 0, std::ios::end );
    logline << indent( content, logline.tellg() );

    std::lock_guard<std::mutex> lock( mutex );

    std::cout << logline.str();

    initLogFile();

    std::ofstream ofs( logFilename(), std::ios::out | std::ios::app );

    if( ofs.is_open() ) {
        ofs << logline.str();
        return true;
    } else {
        return false;
    }
}

