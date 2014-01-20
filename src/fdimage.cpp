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


std::string FDImage::colorSpace() const {
    switch( mImage.colorSpace() ) {
    case Magick::UndefinedColorspace:
        return "UndefinedColorspace";

    case Magick::RGBColorspace:
        return "RGBColorspace";

    case Magick::GRAYColorspace:
        return "GRAYColorspace";

    default:
        return "UndefinedColorspace";
    }
}

std::string FDImage::renderingIntent() const {
    switch( mImage.renderingIntent() ) {
    case Magick::UndefinedIntent:
        return "UndefinedIntent";

    case Magick::SaturationIntent:
        return "SaturationIntent";

    case Magick::PerceptualIntent:
        return "PerceptualIntent";

    case Magick::AbsoluteIntent:
        return "AbsoluteIntent";

    case Magick::RelativeIntent:
        return "RelativeIntent";

    default:
        return "UndefinedIntent";
    }
}

std::string FDImage::type() const {
    switch( mImage.type() ) {
    case Magick::UndefinedType:
        return "UndefinedType";

    case Magick::BilevelType:
        return "BilevelType";

    case Magick::GrayscaleType:
        return "GrayscaleType";

    case Magick::GrayscaleMatteType:
        return "GrayscaleMatteType";

    case Magick::PaletteType:
        return "PaletteType";

    case Magick::PaletteMatteType:
        return "PaletteMatteType";

    case Magick::TrueColorType:
        return "TrueColorType";

    case Magick::TrueColorMatteType:
        return "TrueColorMatteType";

    case Magick::ColorSeparationType:
        return "ColorSeparationType";

    case Magick::ColorSeparationMatteType:
        return "ColorSeparationMatteType";

    case Magick::OptimizeType:
        return "OptimizeType";

    default:
        return "UndefinedType";
    }
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

    std::ifstream file( filename.c_str() );

    if( file ) {
        mImage.read( filename );
    } else {
        std::cout << "Image " << filename << " is not valid" << std::endl;
    }
}

int FDImage::width() const {
    if( mImage.isValid() ) {
        return mImage.columns();
    } else {
        return 0;
    }
}

int FDImage::height() const {
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

//! \brief Load image
//! \param filename e.g. "cat.jpg"
//! \returns true, if loaded successfully
//! \sa http://www.imagemagick.org/Magick++/Exception.html
bool FDImage::load( const std::string filename ) {

    if( fileExists( filename ) ) {

        try {
            try {
                mImage.read( filename );
            } catch( Magick::WarningCoder& warning ) {
                std::cerr << "Coder Warning: " << warning.what() << std::endl;
            } catch( Magick::Warning& warning ) {
                std::cerr << "Warning: " << warning.what() << std::endl;
            } catch( Magick::ErrorFileOpen& error ) {
                std::cerr << "Error: " << error.what() << std::endl;
            }
        } catch( std::exception& error ) {
            std::cerr << "Caught C++ STD exception: " << error.what() << std::endl;
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
    if( mImage.isValid() ) {

        /* no alpha */
        mImage.matte( false );

        mImage.quality( quality );
        mImage.write( filename );

        if( fileExists( filename ) ) {
            return true;
        }
    } else {
        std::cout << "save: Image is not valid" << std::endl;
    }

    return false;
}

//! \brief debug info
std::string FDImage::toString() {
    std::stringstream s;
    s << *this;
    return s.str();
}

std::ostream& operator << ( std::ostream& out, const FDImage& f ) {

    out << " ******** FDImage ******** " << std::endl;
    out << "  filename:   " << f.mImage.fileName() << std::endl;
    out << "  width :     " << f.mImage.columns() << std::endl;
    out << "  height:     " << f.mImage.rows() << std::endl;
    out << "  depth:      " << f.mImage.depth() << std::endl;
    out << "  colorspace: " << f.colorSpace() << std::endl;
    out << "  intent:     " << f.renderingIntent() << std::endl;
    out << "  type:       " << f.type() << std::endl;
    out << "  quantize:   " << f.mImage.quantizeColors() << std::endl;
    out << " ************************* ";

    return out;
}

//! \brief Request 16x16 block pixels
//! \param Block to fill
//! \param x vert. position
//! \param y horiz. position
void FDImage::getBlock( Block& block, int x, int y ) {
    mImage.modifyImage();
    mImage.type( Magick::TrueColorType );

    // Request pixel region
    Magick::PixelPacket* pixel_cache = ( Magick::PixelPacket* ) mImage.getPixels( x, y, Block::size, Block::size );

    for( int i = 0; i< Block::size; ++i ) {
        for( int j = 0; j< Block::size; ++j ) {
            block[i][j] = pixel_cache->green;
            ++pixel_cache;
        }
    }

    mImage.syncPixels();
}
