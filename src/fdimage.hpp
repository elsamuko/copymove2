#ifndef FDIMAGE_H
#define FDIMAGE_H

#include <Magick++.h>

#include "block.hpp"
#include "greyimage.hpp"

class FDImage {
public:
    FDImage();
    FDImage( int width, int height );
    FDImage( const std::string filename );

public:
    static void Initialize();
    static bool initialized;

public:
    GreyImage getGrey();
    void setGrey( const GreyImage& grey );

    size_t width() const;
    size_t height() const;
    bool isNull() const;
    bool is16Bit() const;
    bool load( const std::string filename );
    bool save( const std::string filename, int quality = 95 );

private:
    /* the wrapped image */
    Magick::Image mImage;

    bool fileExists( const std::string& filename ) const;
};

#endif // FDIMAGE_H
