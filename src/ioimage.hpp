#pragma once

#include <memory>

#include "block.hpp"
#include "greyimage.hpp"
#include "shifthit.hpp"

namespace Magick {
    class Image;
}

class IOImage {
    public:
        IOImage();
        IOImage( int width, int height );
        IOImage( const std::string filename );

    public:
        static void Initialize();
        static bool initialized;

    public:
        GreyImage getGrey();
        void setGrey( const GreyImage& grey );

        void drawHit( const ShiftHit& hit );

        size_t width() const;
        size_t height() const;
        bool isNull() const;
        bool is16Bit() const;
        bool load( const std::string filename );
        bool save( const std::string filename, int quality = 95 );

    private:
        /* the wrapped image */
        std::shared_ptr<Magick::Image> mImage;
};
