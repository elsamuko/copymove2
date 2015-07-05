#include "datetime.hpp"

#include <iostream>
#include <sstream>
#include <chrono>
#include <mutex>

#define HAS_STD_PUT_TIME 0

static std::mutex mutex;

std::string datetime::now() {

    std::stringstream now;

    auto tp = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>( tp.time_since_epoch() );
    size_t modulo = ms.count() % 1000;

    time_t seconds = std::chrono::duration_cast<std::chrono::seconds>( ms ).count();

#if HAS_STD_PUT_TIME
    now << std::put_time( std::localtime( &seconds ), "%T." );
#else

    char buffer[10]; // holds "21:31:42"

    // localtime() is not threadsafe
    std::lock_guard<std::mutex> lock( mutex );

    if( strftime( buffer, 10, "%H:%M:%S.", localtime( &seconds ) ) ) {
        now << buffer;
    }

#endif // HAS_STD_PUT_TIME

    // ms
    now.fill( '0' );
    now.width( 3 );
    now << modulo;

    return now.str();
}

int datetime::compilationYear() {
    std::tm tm;
    strptime( __DATE__, "%b %d %Y", &tm );
    return 1900 + tm.tm_year;
}
