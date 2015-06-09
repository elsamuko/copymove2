#include <algorithm>
#include <utility>

#include "dctsorter.hpp"
#include "log/log.hpp"
#include <sstream>

DCTSorter::DCTSorter() :
    mMaxHits( 5 ) {
}

void DCTSorter::setGrey( const GreyImage& grey ) {
    mGrey = grey;
    mResult.from = GreyImage( grey.width(), grey.height() );
    mResult.to   = GreyImage( grey.width(), grey.height() );
}

GreyImage DCTSorter::getGrey() const {
    return mGrey;
}

DCTSorter::ShiftImages DCTSorter::getShifts() const {
    return mResult;
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
    const size_t hB = height - Block::size;
    const size_t wB = width  - Block::size;

    mBlocks = std::vector<Block>( hB * wB );
    std::atomic_int i(0);

    // read
    for( size_t y = 0; y < hB; ++y ) {
        for( size_t x = 0; x < wB; ++x ) {
            int current = i++;
            mThreadPool.add( [this,current,x,y] {
                mBlocks[current].setX( x );
                mBlocks[current].setY( y );
                mGrey.getBlock( mBlocks[current], x, y );
                mBlocks[current].calculateStandardDeviation();
            });
        }
    }

    mThreadPool.waitForAllJobs();
}

void DCTSorter::dctBlocks() {
    LOG("DCT...");

    size_t blocks  = mBlocks.size();
    size_t threads = mThreadPool.size();
    size_t parts   = blocks / threads;
    size_t rest    = blocks % threads;

    LOG( "blocks  : " + std::to_string( blocks ) );
    LOG( "threads : " + std::to_string( threads ) );
    LOG( "parts   : " + std::to_string( parts ) );
    LOG( "rest    : " + std::to_string( rest ) );

    for( size_t thread = 0; thread < threads; ++thread ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        mThreadPool.add( [thread,parts,this] {
            size_t from = thread*parts;
            size_t to   = (thread+1)*parts;
            LOG( std::to_string( from ) + " to " + std::to_string( to ) );
            for( size_t i = from; i<to; ++i ) {
                mBlocks[i].dct();
            }
        });
    }

    if( rest > 0 ) {
        LOG( "rest : " + std::to_string( rest ) );
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        mThreadPool.add( [blocks,rest,this] {
            size_t from = blocks - rest;
            size_t to   = blocks;
            LOG( std::to_string( from ) + " to " + std::to_string( to ) );
            for( size_t i = from; i<to; ++i ) {
                mBlocks[i].dct();
            }
        });
    }

    mThreadPool.waitForAllJobs();


    // some stats
    std::atomic_int max( 20 );
    int freq = 0;

    for( Block& block : mBlocks ) {
        max.store( std::max( max.load(), block.frequency( freq ) ) );
    }
    for( Block& block : mBlocks ) {
        mGrey[block.x()+Block::size/2][block.y()+Block::size/2] = ( max.load() + block.frequency( freq ) ) * 256 / max;
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

void DCTSorter::findDuplicates() {
    LOG("Collecting shifts...");

    std::vector<Block>::iterator tmp = mBlocks.begin();
    std::vector<Block>::iterator b = tmp + 1;
    Shift shift;

    for( ; b!= mBlocks.end(); ++b ) {

        if( !b->interesting() ) {
            tmp = b;
            continue;
        }

        std::vector<Block>::iterator c = b;

        if( tmp->hasSimilarFreqs( *c ) ) {
            if( tmp->manhattanDistance( *c ) > 10 * Block::size ) {
                size_t dx = ( c->x() - tmp->x() );
                size_t dy = ( c->y() - tmp->y() );

                shift.setDx( dx );
                shift.setDy( dy );

                mShifts[ shift ].push_back( std::make_pair(*tmp,*c) );
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

void DCTSorter::sortShifts() {
    LOG("Sorting shifts...");

    std::vector<std::pair<int,Shift>> sorted;
    sorted.reserve( mShifts.size() );

    for( auto& count : mShifts ) {
        sorted.push_back( std::make_pair( count.second.size(), count.first ) );
    }

    std::sort( sorted.begin(), sorted.end() );

    Block white;
    for( int y = 0; y<Block::size; ++y ){
        for( int x = 0; x<Block::size; ++x ){
            white[x][y] = 255;
        }
    }

    int i = sorted.size();

    for( int k = i; k > i-mMaxHits ; --k ) {
        std::pair<int,Shift>& count = sorted[k-1];
        std::vector<std::pair<Block,Block>>& pairs = mShifts[count.second];
        for( std::pair<Block,Block>& pair : pairs ) {
            mResult.from.setBlock( white, pair.first.x(), pair.first.y() );
            mResult.to.setBlock( white, pair.second.x(), pair.second.y() );
        }
        LOG( count.second.toString() + " : " + std::to_string( count.first )  + " @ " + mOffsets[count.second].toString() );
    }

}
