
#include <iostream>
#include <vector>

#include "block.hpp"
#include "fdimage.hpp"
#include "greyimage.hpp"
#include "dctsorter.hpp"
#include <log/log.hpp>

int main( int /*argc*/, char** /*argv*/ ) {

    LOG("Start");
    FDImage image;
    image.load( "cat.jpg" );

    DCTSorter sorter;
    sorter.setGrey( image.getGrey() );
    sorter.work();
    image.setGrey( sorter.getGrey() );

    image.save( "out.jpg" );
    LOG("End");

    return 0;
}

