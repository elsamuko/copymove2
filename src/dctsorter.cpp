#include <algorithm>
#include <utility>

#include "dctsorter.hpp"
#include "log/log.hpp"
#include <sstream>

DCTSorter::DCTSorter() :
    mMinimum( 20 ) {
}

void DCTSorter::setGrey( const GreyImage& grey ) {
    mGrey = grey;
}

GreyImage DCTSorter::getGrey() const {
    return mGrey;
}

void DCTSorter::work() {
    readGreyToBlocks();
    // for( Block& b : mBlocks ) { LOG( b.toString() ); }
    dctBlocks();
    // for( Block& b : mBlocks ) { LOG( b.toString() ); }
    sortBlocks();
    // for( Block& b : mBlocks ) { LOG( b.toString() ); }
    findDuplicates();
    sortShifts();

}

void DCTSorter::readGreyToBlocks() {
    LOG("Read image...");

    const size_t width = mGrey.width();
    const size_t height = mGrey.height();
    const size_t h8 = height - Block::size + 1;
    const size_t w8 = width  - Block::size + 1;

    mBlocks = std::vector<Block>( h8 * w8 );
    int i = 0;

    // read
    for( size_t y = 0; y < h8; ++y ) {
        for( size_t x = 0; x < w8; ++x ) {
            mBlocks[i].setY( y );
            mBlocks[i].setX( x );
            mGrey.getBlock( mBlocks[i], x, y );
            i++;
        }
    }
}

void DCTSorter::dctBlocks() {
    LOG("DCT...");

    int max = 1;
    int freq = 1;

    for( Block& block : mBlocks ) {
        block.dct();
        max = std::max( max, block.frequency( freq ) );
    }

    for( Block& block : mBlocks ) {
        mGrey[block.x()+Block::size/2][block.y()+Block::size/2] = block.frequency( freq ) * 65536 / max;
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
    LOG("Sorting...");
    std::sort( mBlocks.begin(), mBlocks.end() );
}

void DCTSorter::sortShifts() {

    std::vector<std::pair<int,Shift>> sorted;
    sorted.reserve( mShifts.size() );

    for( auto& count : mShifts ) {
        sorted.push_back( std::make_pair( count.second, count.first ) );
    }

    std::sort( sorted.begin(), sorted.end() );

    for( auto& count : sorted ) {
        if( count.first > mMinimum ) {
            LOG( count.second.toString() + " : " + std::to_string( count.first )  + " @ " + mOffsets[count.second].toString() );
        }
    }

}

void DCTSorter::findDuplicates() {
    LOG("Collecting shifts...");

    std::vector<Block>::iterator tmp = mBlocks.begin();
    std::vector<Block>::iterator b = tmp + 1;
    Shift shift;

    for( ; b!= mBlocks.end(); ++b ) {

        std::vector<Block>::iterator c = b;

        while( tmp->hasSimilarFreqs( *c ) ) {
            if( tmp->manhattanDistance( *c ) > Block::size ) {
                size_t dx = std::abs( c->x() - tmp->x() );
                size_t dy = std::abs( c->y() - tmp->y() );

                shift.setDx( dx );
                shift.setDy( dy );

                mShifts[ shift ]++;
                mOffsets[ shift ] = *c;
            }
            ++c;
            if( c == mBlocks.end()) {
                break;
            }
        }

        tmp = b;
    }
}

