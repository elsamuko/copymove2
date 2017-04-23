
#include <sstream>
#include <iostream>

#include "ioimage.hpp"
#include "log/log.hpp"
#include "fileutils.hpp"

bool IOImage::initialized = false;

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
void IOImage::Initialize() {
    if( !IOImage::initialized ) {
        Magick::InitializeMagick( 0 );
        IOImage::initialized = true;
    }
}

IOImage::IOImage() {
    IOImage::Initialize();
}

IOImage::IOImage( int width, int height ) {
    IOImage::Initialize();
    mImage  = Magick::Image( Magick::Geometry( width, height ), Magick::Color() );
}

IOImage::IOImage( const std::string filename ) {
    IOImage::Initialize();
    this->load( filename );
}

size_t IOImage::width() const {
    if( mImage.isValid() ) {
        return mImage.columns();
    } else {
        return 0;
    }
}

size_t IOImage::height() const {
    if( mImage.isValid() ) {
        return mImage.rows();
    } else {
        return 0;
    }
}

//! \return true, if image is not valid
bool IOImage::isNull() const {
    return ! mImage.isValid();
}

//! \return true, if depth is 16 bit
bool IOImage::is16Bit() const {
    return mImage.depth() == 16;
}

GreyImage IOImage::getGrey() {

    size_t w = width();
    size_t h = height();

    GreyImage grey( w, h );

    mImage.modifyImage();
    mImage.type( Magick::TrueColorType );

    // Request pixel region
    Magick::PixelPacket* pixel_cache = ( Magick::PixelPacket* ) mImage.getPixels( 0, 0, w, h );

    for( size_t y = 0; y < h; ++y ) {
        for( size_t x = 0; x < w; ++x ) {
            grey[x][y] = pixel_cache->green / 256;
            ++pixel_cache;
        }
    }

    mImage.syncPixels();

    return grey;
}

void IOImage::setGrey( const GreyImage& grey ) {

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

void IOImage::drawHit( const ShiftHit& hit ) {

    int x = hit.x() + Block::size / 2;
    int y = hit.y() + Block::size / 2;

    std::list<Magick::Drawable> drawList;
    drawList.push_back( Magick::DrawableStrokeColor( "red" ) );
    drawList.push_back( Magick::DrawableFillColor( "transparent" ) );
    drawList.push_back( Magick::DrawableStrokeWidth( 1 ) );
    // drawList.push_back( Magick::DrawableCircle( x, y, x + hit.distance(),y ) );
    drawList.push_back( Magick::DrawableLine( x, y, x + hit.dx(), y + hit.dy() ) );

    std::string text = std::to_string( hit.ranking() );
    drawList.push_back( Magick::DrawableStrokeColor( "black" ) );
    drawList.push_back( Magick::DrawableFillColor( "lime" ) );
    drawList.push_back( Magick::DrawableRectangle( x - 5, y - 10, x + 3 + text.size() * 8, y + 10 ) );

    drawList.push_back( Magick::DrawableFillColor( "black" ) );
    drawList.push_back( Magick::DrawableStrokeWidth( 0 ) );
    drawList.push_back( Magick::DrawableText( x, y + 5, text ) );

    mImage.draw( drawList );
}

//! \brief Load image
//! \param filename e.g. "cat.jpg"
//! \returns true, if loaded successfully
//! \sa http://www.imagemagick.org/Magick++/Exception.html
bool IOImage::load( const std::string filename ) {

    if( fileutils::fileExists( filename ) ) {
        try {
            mImage.read( filename );
        } catch( Magick::Exception& error ) {
            LOG_ERROR( error.what() );
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
bool IOImage::save( const std::string filename, int quality ) {
    if( filename.empty() ) {
        LOG_ERROR( "Filename is empty" );
        return false;
    }

    if( mImage.isValid() ) {

        /* no alpha */
        mImage.matte( false );
        mImage.quality( quality );

        try {
            mImage.write( filename );
        } catch( Magick::Exception& error ) {
            LOG_ERROR( error.what() );
        }

        if( fileutils::fileExists( filename ) ) {
            return true;
        }
    } else {
        LOG( "Save: Image is not valid" );
    }

    return false;
}
