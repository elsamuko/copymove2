
#include <iostream>
#include <vector>

#include "block.hpp"
#include "ioimage.hpp"
#include "greyimage.hpp"
#include "dctsorter.hpp"
#include <log/log.hpp>

int main( int /*argc*/, char** /*argv*/ ) {

    LOG("Start");
    IOImage image;
    image.load( "cat.jpg" );

    DCTSorter sorter;
    sorter.setGrey( image.getGrey() );
    sorter.work();

    image.setGrey( sorter.getGrey() );
    image.save( "interesting.jpg" );

    DCTSorter::ShiftImages shifts = sorter.getShiftImages();
    image.setGrey( shifts.from );
    image.save( "z_from.jpg" );
    image.setGrey( shifts.to );
    image.save( "z_to.jpg" );

    image.load( "cat.jpg" );
    std::vector<ShiftHit> shiftHits = sorter.getShiftHits();
    for( ShiftHit& hit : shiftHits ) {
        image.drawHit( hit );
    }
    image.save( "z_result.jpg" );

    LOG("End");

    return 0;
}

