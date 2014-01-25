#include <iostream>
#include <vector>
#include <sstream>

#include <Magick++.h>

template <class T, int lower, int upper>
T CLAMP( T in ) {
    if( in < lower ) {
        return lower;
    }

    if( in >= upper ) {
        return upper;
    }

    return in;
}

std::vector<int8_t> getPixels( Magick::Image* image ) {

    image->modifyImage();
    image->type( Magick::TrueColorType );

    int w = image->columns();
    int h = image->rows();
    int offset = w*h;
    std::vector<int8_t> pixels( 3 * w * h );


    Magick::PixelPacket* pixel_cache = ( Magick::PixelPacket* ) image->getPixels( 0, 0, w, h );

    int pos = 0;
    int i = 0;

    for( int x = 0; x < w; x++ ) {
        for( int y = 0; y < h; y++ ) {
            pos = ( w*y  + x );
            pixels[i]            = CLAMP<int, 0, 256>( pixel_cache[pos].red   >> 8 ) + 1;
            pixels[i + offset]   = CLAMP<int, 0, 256>( pixel_cache[pos].green >> 8 ) + 1;
            pixels[i + 2*offset] = CLAMP<int, 0, 256>( pixel_cache[pos].blue  >> 8 ) + 1;
            i++;
        }
    }

    image->syncPixels();
    return pixels;
}

std::string toString( const std::vector<int8_t>& pixels, const int w, const int h, const std::string& name ) {
    std::stringstream ss;
    ss << "# Created by image2octave" << std::endl;
    ss << "# name:  " << name << std::endl;
    ss << "# type:  matrix" << std::endl;
    ss << "# ndims: 3" << std::endl;
    ss << h << " " << w << " 3" << std::endl;

    for( size_t i = 0; i < pixels.size(); ++i ) {
        ss << (int) pixels[i] << std::endl;
    }

    return ss.str();
}

int main( int argc, char *argv[] ) {

    if( argc != 2 ) {
        std::cerr << "Usage:" << std::endl;
        std::cerr << "    image2octave [image] > out.mat" << std::endl;
        return 1;
    }

    std::string filename( argv[1] );
    std::string basename = filename.substr( 0, filename.find_last_of( '.' ) );
    Magick::InitializeMagick( 0 );

    Magick::Image image( filename );
    int w = image.columns();
    int h = image.rows();

    std::vector<int8_t> pixels = getPixels( &image );

    std::string output = toString( pixels, w,h, basename );

    std::cout << output << std::endl;

    return 0;
}

