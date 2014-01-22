#ifndef FDIMAGE_H
#define FDIMAGE_H

#include <Magick++.h>

#include "block.hpp"

class FDImage {
public:
    FDImage();
    FDImage( int width, int height );
    FDImage( const std::string filename );

public:
    static void Initialize();
    static bool initialized;

public:
    void getBlock( Block& block, int x, int y ) const;
    void setBlock( const Block& block, int x, int y );

    size_t width() const;
    size_t height() const;
    bool isNull() const;
    bool is16Bit() const;
    bool load( const std::string filename );
    bool save( const std::string filename, int quality = 95 );

private:
    /* the wrapped image */
    Magick::Image mImage;
    std::vector<std::vector<double>> mGreys; // cache the luma values

    bool fileExists( const std::string& filename ) const;
    void cacheGreys();
    void setGreys();
};

#endif // FDIMAGE_H
