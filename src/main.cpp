
#include <iostream>
#include <vector>

#include "block.hpp"
#include "fdimage.hpp"

int main( int /*argc*/, char** /*argv*/ ) {

    FDImage image;
    image.load( "cat.jpg" );

    Block b1;

    for( size_t i = 0; i < image.width(); i += Block::size ) {
        for( size_t j = 0; j < image.height(); j += Block::size ) {
            image.getBlock( b1, i, j);
            b1.dct();
            // b1.idct();
            image.setBlock( b1, i, j);
        }
    }

    image.save( "out.jpg" );

    return 0;
}

