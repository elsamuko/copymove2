#include <fstream>

#include "fileutils.hpp"

bool fileutils::fileExists( const std::string& filename ) {
    std::ifstream file( filename.c_str() );
    return file.is_open();
    // destructor closes file
}
