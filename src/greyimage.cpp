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
    mPixels = std::vector<std::vector<double>>( mWidth, std::vector<double>( mHeight, 0 ) );
}

size_t GreyImage::width() const {
    return mWidth;
}

std::vector<double>& GreyImage::operator[]( const size_t pos ) {
    return mPixels[pos];
}

const std::vector<double>& GreyImage::operator[]( const size_t pos ) const {
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
void GreyImage::getBlock( Block& block, int x, int y ) const {

    for( int i = 0; i < Block::size; ++i ) {
        for( int j = 0; j < Block::size; ++j ) {
            block[i][j] = mPixels[x + i][y + j];
        }
    }

}

//! \brief Write 16x16 block to grey cache
//! \param Block to insert
//! \param x vert. position
//! \param y horiz. position
void GreyImage::setBlock( const Block& block, int x, int y ) {

    for( int i = 0; i < Block::size; ++i ) {
        for( int j = 0; j < Block::size; ++j ) {
            mPixels[x + i][y + j] = block[i][j];
        }
    }

}
