#include "fdimage.hpp"

#include <sstream>
#include <fstream>
#include <iostream>

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

void FDImage::cacheGreys() {

    size_t w = width();
    size_t h = height();

    mGreys = std::vector<std::vector<double>>( w, std::vector<double>( h ) );

    mImage.modifyImage();
    mImage.type( Magick::TrueColorType );

    // Request pixel region
    Magick::PixelPacket* pixel_cache = ( Magick::PixelPacket* ) mImage.getPixels( 0, 0, w, h );

    for( size_t i = 0; i < w; ++i ) {
        for( size_t j = 0; j < h; ++j ) {
            mGreys[i][j] = pixel_cache->green;
            ++pixel_cache;
        }
    }

    mImage.syncPixels();
}

void FDImage::setGreys() {

    size_t w = width();
    size_t h = height();

    mImage.modifyImage();
    mImage.type( Magick::TrueColorType );

    // Request pixel region
    Magick::PixelPacket* pixel_cache = ( Magick::PixelPacket* ) mImage.getPixels( 0, 0, w, h );

    for( size_t i = 0; i < w; ++i ) {
        for( size_t j = 0; j < h; ++j ) {
            pixel_cache->red = CLAMP<int, 0, 65535>( mGreys[i][j] );
            pixel_cache->green = CLAMP<int, 0, 65535>( mGreys[i][j] );
            pixel_cache->blue = CLAMP<int, 0, 65535>( mGreys[i][j] );
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
            this->cacheGreys();
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

        // write grey cache to magick image
        this->setGreys();

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
        std::cout << "save: Image is not valid" << std::endl;
    }

    return false;
}

//! \brief Request 16x16 block to grey cache
//! \param Block to fill
//! \param x vert. position
//! \param y horiz. position
void FDImage::getBlock( Block& block, int x, int y ) const {

    for( int i = 0; i < Block::size; ++i ) {
        for( int j = 0; j < Block::size; ++j ) {
            block[i][j] = mGreys[x+i][y+j];
        }
    }

}

//! \brief Write 16x16 block to grey cache
//! \param Block to insert
//! \param x vert. position
//! \param y horiz. position
void FDImage::setBlock( const Block& block, int x, int y ) {

    for( int i = 0; i < Block::size; ++i ) {
        for( int j = 0; j < Block::size; ++j ) {
            mGreys[x+i][y+j] = block[i][j];
        }
    }

}
