
#include <iostream>
#include <vector>

#include "block.hpp"
#include "ioimage.hpp"
#include "greyimage.hpp"
#include "dctsorter.hpp"
#include "cliparser.hpp"
#include "log/log.hpp"

int main( int argc, char** argv ) {

    // checks
    SorterParams params = cliparser::parseCLI( argc, argv );

    if( !params.valid() ) {
        LOG_WARNING( "Invalid params, exiting..." );
        return 1;
    }

    LOG( "Start" );
    IOImage image( params.filename() );

    if( image.isNull() ) {
        LOG_WARNING( "Invalid image, exiting..." );
        return 1;
    }


    // algorithm
    DCTSorter sorter;
    sorter.setParams( params );
    sorter.setGrey( image.getGrey() );
    sorter.work();


    // debug
    image.setGrey( sorter.getGrey() );
    image.save( "z_interesting.jpg" );
    DCTSorter::ShiftImages shifts = sorter.getShiftImages();
    image.setGrey( shifts.from );
    image.save( "y_from.png" );
    image.setGrey( shifts.to );
    image.save( "y_to.png" );


    // result
    image.load( params.filename() );
    std::vector<ShiftHit> shiftHits = sorter.getShiftHits();
    std::reverse( shiftHits.begin(), shiftHits.end() );

    for( ShiftHit & hit : shiftHits ) {
        image.drawHit( hit );
    }

    image.save( "z_result.jpg" );

    LOG( "End" );
    return 0;
}

