#include "fdimage.hpp"

#include <sstream>
#include <fstream>
#include <iostream>

#include <log/log.hpp>

bool FDImage::initialized = false;

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

//! \brief Initializes Magick, needed before any image operations
void FDImage::Initialize() {
    if( !FDImage::initialized ) {
        Magick::InitializeMagick( 0 );
        FDImage::initialized = true;
    }
}

FDImage::FDImage() {
    FDImage::Initialize();
}

FDImage::FDImage( int width, int height ) {
    FDImage::Initialize();
    mImage  = Magick::Image( Magick::Geometry( width, height ), Magick::Color() );
}

FDImage::FDImage( const std::string filename ) {
    FDImage::Initialize();
    this->load( filename );
}

size_t FDImage::width() const {
    if( mImage.isValid() ) {
        return mImage.columns();
    } else {
        return 0;
    }
}

size_t FDImage::height() const {
    if( mImage.isValid() ) {
        return mImage.rows();
    } else {
        return 0;
    }
}

//! \return true, if image is not valid
bool FDImage::isNull() const {
    return ! mImage.isValid();
}

//! \return true, if depth is 16 bit
bool FDImage::is16Bit() const {
    return mImage.depth() == 16;
}

bool FDImage::fileExists( const std::string& filename ) const {
    std::ifstream file( filename.c_str() );
    return file.is_open();
    // destructor closes file
}

GreyImage FDImage::getGrey() {

    size_t w = width();
    size_t h = height();

    GreyImage grey( w, h );

    mImage.modifyImage();
    mImage.type( Magick::TrueColorType );

    // Request pixel region
    Magick::PixelPacket* pixel_cache = ( Magick::PixelPacket* ) mImage.getPixels( 0, 0, w, h );

    for( size_t y = 0; y < h; ++y ) {
        for( size_t x = 0; x < w; ++x ) {
            grey[x][y] = pixel_cache->green/256;
            ++pixel_cache;
        }
    }

    mImage.syncPixels();

    return grey;
}

void FDImage::setGrey( const GreyImage& grey ) {

    size_t w = width();
    size_t h = height();

    mImage.modifyImage();
    mImage.type( Magick::TrueColorType );

    // Request pixel region
    Magick::PixelPacket* pixel_cache = ( Magick::PixelPacket* ) mImage.getPixels( 0, 0, w, h );

    for( size_t y = 0; y < h; ++y ) {
        for( size_t x = 0; x < w; ++x ) {
            pixel_cache->red = 256 * CLAMP<int, 0, 256>( grey[x][y] );
            pixel_cache->green = 256 * CLAMP<int, 0, 256>( grey[x][y] );
            pixel_cache->blue = 256 * CLAMP<int, 0, 256>( grey[x][y] );
            ++pixel_cache;
        }
    }

    mImage.syncPixels();
}

//! \brief Load image
//! \param filename e.g. "cat.jpg"
//! \returns true, if loaded successfully
//! \sa http://www.imagemagick.org/Magick++/Exception.html
bool FDImage::load( const std::string filename ) {

    if( fileExists( filename ) ) {
        try {
            mImage.read( filename );
        } catch( Magick::Exception& error ) {
            std::cerr << "Error: " << error.what() << std::endl;
        }

        if( mImage.isValid() ) {
            return true;
        }
    }

    return false;
}

//! \brief Save image
//! \param filename e.g. "cat.jpg"
//! \param quality, default is 95
//! \returns true, if saved successfully
bool FDImage::save( const std::string filename, int quality ) {
    if( filename.empty() ) {
        std::cerr << "Filename is empty";
        return false;
    }

    if( mImage.isValid() ) {

        /* no alpha */
        mImage.matte( false );
        mImage.quality( quality );

        try {
            mImage.write( filename );
        } catch( Magick::Exception& error ) {
            std::cerr << "Error: " << error.what() << std::endl;
        }

        if( fileExists( filename ) ) {
            return true;
        }
    } else {
        LOG( "Save: Image is not valid" );
    }

    return false;
}
