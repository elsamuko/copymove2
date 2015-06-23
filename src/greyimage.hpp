#pragma once

#include "block.hpp"

class GreyImage {
    public:
        GreyImage();
        GreyImage( int width, int height );

    public:
        void getBlock( Block& block, int x0, int y0 ) const;
        void setBlock( const Block& block, int x0, int y0 );

        std::vector<float>& operator[]( const size_t pos );
        const std::vector<float>& operator[]( const size_t pos ) const;

        size_t width() const;
        size_t height() const;

    private:
        /* the wrapped image */
        std::vector<std::vector<float>> mPixels;
        size_t mWidth;
        size_t mHeight;
};
