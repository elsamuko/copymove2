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
    std::string toString();
    friend std::ostream& operator << ( std::ostream& stream, const FDImage& p );
    static bool initialized;

public:
    void getBlock( Block& block, int x, int y );

    int  width() const;
    int  height() const;
    bool isNull() const;
    bool is16Bit() const;
    bool load( const std::string filename );
    bool save( const std::string filename, int quality = 95 );

    std::string colorSpace() const;
    std::string renderingIntent() const;
    std::string type() const;

private:
    /* the wrapped image */
    Magick::Image mImage;

    bool fileExists( const std::string& filename ) const;
};

#endif // FDIMAGE_H
