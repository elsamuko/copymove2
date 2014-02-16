#ifndef GREYIMAGE_H
#define GREYIMAGE_H

#include "block.hpp"

class GreyImage {
public:
    GreyImage();
    GreyImage( int width, int height );

public:
    void getBlock( Block& block, int x, int y ) const;
    void setBlock( const Block& block, int x, int y );

    std::vector<double>& operator[]( const size_t pos );
    const std::vector<double>& operator[]( const size_t pos ) const;

    size_t width() const;
    size_t height() const;

private:
    /* the wrapped image */
    std::vector<std::vector<double>> mPixels;
    size_t mWidth;
    size_t mHeight;
};

#endif // GREYIMAGE_H
