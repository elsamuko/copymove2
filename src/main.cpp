
#include <iostream>
#include <vector>

#include "block.hpp"
#include "ioimage.hpp"
#include "greyimage.hpp"
#include "dctsorter.hpp"
#include <log/log.hpp>

int main( int argc, char** argv ) {

    if( argc < 2 ) {
        LOG_WARNING( "No argument, exiting...");
        return 1;
    }

    LOG("Start");
    IOImage image;
    std::string filename = argv[1];
    image.load( filename );

    int minHits = 20;

    DCTSorter sorter( minHits );
    sorter.setGrey( image.getGrey() );
    sorter.work();

    image.setGrey( sorter.getGrey() );
    image.save( "z_interesting.jpg" );

    DCTSorter::ShiftImages shifts = sorter.getShiftImages();
    image.setGrey( shifts.from );
    image.save( "y_from.png" );
    image.setGrey( shifts.to );
    image.save( "y_to.png" );

    image.load( filename );
    std::vector<ShiftHit> shiftHits = sorter.getShiftHits();
    for( ShiftHit& hit : shiftHits ) {
        image.drawHit( hit );
    }
    image.save( "z_result.jpg" );

    LOG("End");

    return 0;
}

