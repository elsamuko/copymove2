
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

    DCTSorter::ShiftImages shifts = sorter.getShifts();
    image.setGrey( shifts.from );
    image.save( "z_from.jpg" );
    image.setGrey( shifts.to );
    image.save( "z_to.jpg" );

    GreyImage grey = sorter.getGrey();
    image.setGrey( grey );
    image.save( "grey.jpg" );

    LOG("End");

    return 0;
}

