
#include <iostream>
#include <vector>

#include "block.hpp"
#include "fdimage.hpp"
#include "greyimage.hpp"
#include "dctsorter.hpp"

int main( int /*argc*/, char** /*argv*/ ) {

    FDImage image;
    image.load( "cat.jpg" );

    DCTSorter sorter;
    sorter.setGrey( image.getGrey() );
    sorter.work();

    image.save( "out.jpg" );

    return 0;
}

