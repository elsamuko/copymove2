#include "dctsorter.hpp"

DCTSorter::DCTSorter() {
}

void DCTSorter::setGrey( const GreyImage& grey ) {
    mGrey = grey;
}

void DCTSorter::work() {
    readGreyToBlocks();
    dctBlocks();
    sortBlocks();
    findDuplicates();
}

void DCTSorter::readGreyToBlocks() {

    const size_t width = mGrey.width();
    const size_t height = mGrey.height();
    const size_t h16 = height - 16 + 1;
    const size_t w16 = width  - 16 + 1;

    mBlocks = std::vector<Block>( h16 * w16 );
    int i = 0;

    // read
    for( size_t y = 0; y < h16; ++y ) {
        for( size_t x = 0; x < w16; ++x ) {
            mBlocks[i].setY( y );
            mBlocks[i].setX( x );
            mGrey.getBlock( mBlocks[i], x, y );
            i++;
        }
    }
}

void DCTSorter::dctBlocks() {

    for( auto & block : mBlocks ) {
        block.dct();
    }

}

// http://codereview.stackexchange.com/questions/22744/multi-threaded-sort
//template<class T>
//void parallel_sort(T* data, int len, int grainsize)
//{
//    if(len < grainsize) // Use grainsize instead of thread count so that we don't e.g. spawn 4 threads just to sort 8 elements.
//    {
//        std::sort(data, data + len, std::less<T>());
//    }
//    else
//    {
//        auto future = std::async(parallel_sort<T>, data, len/2, grainsize);

//        parallel_sort(data + len/2, len/2, grainsize); // No need to spawn another thread just to block the calling thread which would do nothing.

//        future.wait();

//        std::inplace_merge(data, data + len/2, data + len, std::less<T>());
//    }
//}

void DCTSorter::sortBlocks() {
    std::sort( mBlocks.begin(), mBlocks.end() );
}

void DCTSorter::findDuplicates() {
    Block& tmp = mBlocks[0];
    Shift shift;

    for( Block & b : mBlocks ) {
        if( tmp.manhattanDistance( b ) > Block::size ) {
            shift.setX( std::abs( b.x() - tmp.x() ) );
            shift.setY( std::abs( b.y() - tmp.y() ) );
            mShiftCount[ shift ]++;
        }

        tmp = b;
    }

}

