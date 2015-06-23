#include "greyimage.hpp"

#include <sstream>
#include <fstream>
#include <iostream>

template <class T, int lower, int upper>
T CLAMP( T in ) {
    if( in < lower ) {
        return lower;
    }

    if( in > upper ) {
        return upper;
    }

    return in;
}

GreyImage::GreyImage( int width, int height ) {
    mWidth  = width;
    mHeight = height;
    mPixels = std::vector<std::vector<float>>( mWidth, std::vector<float>( mHeight, 0 ) );
}

size_t GreyImage::width() const {
    return mWidth;
}

std::vector<float>& GreyImage::operator[]( const size_t pos ) {
    return mPixels[pos];
}

const std::vector<float>& GreyImage::operator[]( const size_t pos ) const {
    return mPixels[pos];
}


size_t GreyImage::height() const {
    return mHeight;
}

GreyImage::GreyImage() {

}

//! \brief Request 16x16 block to grey cache
//! \param Block to fill
//! \param x vert. position
//! \param y horiz. position
void GreyImage::getBlock( Block& block, int x0, int y0 ) const {

    for( int y = 0; y < Block::size; ++y ) {
        for( int x = 0; x < Block::size; ++x ) {
            block[x][y] = mPixels[x0 + x][y0 + y];
        }
    }

}

//! \brief Write 16x16 block to grey cache
//! \param Block to insert
//! \param x vert. position
//! \param y horiz. position
void GreyImage::setBlock( const Block& block, int x0, int y0 ) {

    for( int y = 0; y < Block::size; ++y ) {
        for( int x = 0; x < Block::size; ++x ) {
            mPixels[x0 + x][y0 + y] = block[x][y];
        }
    }

}
