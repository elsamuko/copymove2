#include "Image.hpp"

#include <sstream>
#include <fstream>
#include <iostream>

bool Image::initialized = false;

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


std::string Image::colorSpace() const {
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

std::string Image::renderingIntent() const {
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

std::string Image::type() const {
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
void Image::Initialize() {

    if( !Image::initialized ) {
        Magick::InitializeMagick( 0 );
        Image::initialized = true;
    }

}

void Image::Destroy() {
    Image::initialized = false;
}

Image::Image() {
    Image::Initialize();
}

Image::Image( int width, int height ) {
    Image::Initialize();
    mImage  = Magick::Image( Magick::Geometry( width, height ), Magick::Color() );
}

Image::Image( const std::string filename ) {
    Image::Initialize();

    std::ifstream file( filename.c_str() );

    if( file ) {
        mImage.read( filename );
    } else {
        std::cout << "Image " << filename << " is not valid" << std::endl;
    }
}

int Image::width() const {
    if( mImage.isValid() ) {
        return mImage.columns();
    } else {
        return 0;
    }
}

int Image::height() const {
    if( mImage.isValid() ) {
        return mImage.rows();
    } else {
        return 0;
    }
}

//! \return true, if image is not valid
bool Image::isNull() const {
    return ! mImage.isValid();
}

//! \return true, if depth is 16 bit
bool Image::is16Bit() const {
    return mImage.depth() == 16;
}

bool Image::fileExists( const std::string& filename ) const {
    std::ifstream file( filename.c_str() );
    return file.is_open();
    // destructor closes file
}

//! \brief Load image
//! \param filename e.g. "cat.jpg"
//! \returns true, if loaded successfully
//! \sa http://www.imagemagick.org/Magick++/Exception.html
bool Image::load( const std::string filename ) {
    if( fileExists( filename ) ) {

        try {
            try {
                // Try reading image file
                mImage.read( filename );
            } catch( Magick::WarningCoder& warning ) {
                // Process coder warning while loading file (e.g. TIFF warning)
                // Maybe the user will be interested in these warnings (or not).
                // If a warning is produced while loading an image, the image
                // can normally still be used (but not if the warning was about
                // something important!)
                std::cerr << "Coder Warning: " << warning.what() << std::endl;
            } catch( Magick::Warning& warning ) {
                // Handle any other Magick++ warning.
                std::cerr << "Warning: " << warning.what() << std::endl;
            } catch( Magick::ErrorFileOpen& error ) {
                // Process Magick++ file open error
                std::cerr << "Error: " << error.what() << std::endl;
            }
        } catch( std::exception& error ) {
            // Process any other exceptions derived from standard C++ exception
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
//! \param quality, default is 75
//! \returns true, if saved successfully
bool Image::save( const std::string filename, int quality ) {
    if( mImage.isValid() ) {
        /* doesn't work, because Magick ignores this: */
        // mImage.attribute("software","FD Imaging - Black Silk");

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
std::string Image::toString() {
    std::stringstream s;
    s << *this;
    return s.str();
}

std::ostream& operator << ( std::ostream& out, const Image& f ) {
    out << " ******** Image ******** " << std::endl;
    out << "  filename:   " << f.mImage.fileName() << std::endl;
    out << "  width :     " << f.mImage.columns() << std::endl;
    out << "  height:     " << f.mImage.rows() << std::endl;
    out << "  depth:      " << f.mImage.depth() << std::endl;
    out << "  colorspace: " << f.colorSpace() << std::endl;
    out << "  intent:     " << f.renderingIntent() << std::endl;
    out << "  type:       " << f.type() << std::endl;
    out << "  quantize:   " << f.mImage.quantizeColors() << std::endl;
    out << " ************************* ";

    // std::auto_ptr<ImageStatistics> stats( new ImageStatistics );
    // f.mImage.statistics( stats.get() );

    return out;
}

//! \brief Request all pixels
//! \param data RGBA pixel buffer to fill
//! \param is16Bit if true, offset is ushort
void Image::getImage( void* data, bool hasAlpha, bool is16Bit ) {
    mImage.modifyImage();
    mImage.type( Magick::TrueColorType );

    // Request pixel region
    int w = mImage.columns();
    int h = mImage.rows();
    Magick::PixelPacket* pixel_cache = ( Magick::PixelPacket* ) mImage.getPixels( 0, 0, w, h );

    if( is16Bit ) {
        uint16_t* bits = ( uint16_t* ) data;

        for( int i = 0; i < h * w; i++ ) {
            *bits++ = pixel_cache->red;
            *bits++ = pixel_cache->green;
            *bits++ = pixel_cache->blue;

            if( hasAlpha ) {
                *bits++ = pixel_cache->opacity;
            }

            pixel_cache++;
        }
    } else {
        uint8_t* bits = ( uint8_t* ) data;

        for( int i = 0; i < h * w; i++ ) {
            *bits++ = CLAMP<int, 0, 255>( pixel_cache->red   / 256 );
            *bits++ = CLAMP<int, 0, 255>( pixel_cache->green / 256 );
            *bits++ = CLAMP<int, 0, 255>( pixel_cache->blue  / 256 );

            if( hasAlpha ) {
                *bits++ = pixel_cache->opacity;
            }

            pixel_cache++;
        }
    }

    mImage.syncPixels();
}

//! \brief Request all pixels
//! \param data RGBA pixel buffer to read from
//! \param is16Bit if true, offset is ushort
void Image::setImage( void* data, bool hasAlpha, bool is16Bit ) {
    mImage.modifyImage();
    mImage.type( Magick::TrueColorType );

    // Request pixel region
    int w = mImage.columns();
    int h = mImage.rows();
    Magick::PixelPacket* pixel_cache = mImage.getPixels( 0, 0, w, h );

    if( is16Bit ) {
        uint16_t* bits = ( uint16_t* ) data;

        for( int i = 0; i < h * w; i++ ) {
            pixel_cache->red     = *bits++;
            pixel_cache->green   = *bits++;
            pixel_cache->blue    = *bits++;

            if( hasAlpha ) {
                pixel_cache->opacity = *bits++;
            } else {
                pixel_cache->opacity = 65535;
            }

            pixel_cache++;
        }
    } else {
        uint8_t* bits = ( uint8_t* ) data;

        for( int i = 0; i < h * w; i++ ) {
            pixel_cache->red     = 256 * *bits++;
            pixel_cache->green   = 256 * *bits++;
            pixel_cache->blue    = 256 * *bits++;

            if( hasAlpha ) {
                pixel_cache->opacity = *bits++;
            } else {
                pixel_cache->opacity = 255;
            }

            pixel_cache++;
        }
    }

    mImage.syncPixels();
}
