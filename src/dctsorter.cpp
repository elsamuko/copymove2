#include <algorithm>
#include <utility>
#include <sstream>
#include <fstream>

#include "dctsorter.hpp"
#include "log/log.hpp"
#include "scopeguard.hpp"

DCTSorter::DCTSorter( size_t minHits ) :
    mMinHits( minHits ) {
}

void DCTSorter::reset() {
    mBlocks.clear();
    mShifts.clear();
    mShiftHits.clear();

    mGreyReceived       = false;
    mWorked             = false;
    mBlocksSet          = false;
    mDCTCalculated      = false;
    mBlocksSorted       = false;
    mDuplicatesSearched = false;
    mShiftsSorted       = false;
}

void DCTSorter::setGrey( const GreyImage& grey ) {
    this->reset();

    LOG( "Set grey..." );
    STATE_CHECK( mGreyReceived );

    mGrey = grey;
    mResult.from = GreyImage( grey.width(), grey.height() );
    mResult.to   = GreyImage( grey.width(), grey.height() );
    mImageSize.set( grey.width(), grey.height() );
}

GreyImage DCTSorter::getGrey() const {
    return mGrey;
}

DCTSorter::ShiftImages DCTSorter::getShiftImages() const {
    return mResult;
}

std::vector<ShiftHit> DCTSorter::getShiftHits() const {
    assert( mGreyReceived );
    assert( mWorked );
    assert( mBlocksSet );
    assert( mDCTCalculated );
    assert( mBlocksSorted );
    assert( mDuplicatesSearched );
    assert( mShiftsSorted );
    return mShiftHits;
}

void DCTSorter::work() {
    LOG( "Do work..." );
    STATE_CHECK( mWorked );

    readGreyToBlocks();
    // for( Block& b : mBlocks ) { LOG( b.toString() ); }
    dctBlocks();
    // debugBlocks();
    // for( Block& b : mBlocks ) { LOG( b.toString() ); }
    sortBlocks();
    // for( Block& b : mBlocks ) { LOG( b.toString() ); }
    searchDuplicates();
    sortShifts();
}

void DCTSorter::readGreyToBlocks() {
    LOG( "Read image..." );
    STATE_CHECK( mBlocksSet );

    const size_t width = mGrey.width();
    const size_t height = mGrey.height();
    const size_t hB = height - Block::size;
    const size_t wB = width  - Block::size;

    mBlocks = std::vector<Block>( hB * wB );
    std::atomic_int i( 0 );

    // read
    for( size_t y = 0; y < hB; ++y ) {
        for( size_t x = 0; x < wB; ++x ) {
            int current = i++;
            mThreadPool.add( [this, current, x, y] {
                mBlocks[current].setX( x );
                mBlocks[current].setY( y );
                mGrey.getBlock( mBlocks[current], x, y );
                mBlocks[current].calculateStandardDeviation();
            } );
        }
    }

    mThreadPool.waitForAllJobs();

    // some stats
    for( Block & block : mBlocks ) {
        mGrey[block.x() + Block::size / 2][block.y() + Block::size / 2] = block.interesting() ? 255 : 0;
    }
}

void DCTSorter::dctBlocks() {
    LOG( "DCT..." );
    STATE_CHECK( mDCTCalculated );

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
        mThreadPool.add( [thread, parts, this] {
            size_t from = thread* parts;
            size_t to   = ( thread + 1 ) * parts;
            LOG( std::to_string( from ) + " to " + std::to_string( to ) );

            for( size_t i = from; i < to; ++i ) {
                mBlocks[i].dct();
            }
        } );
    }

    if( rest > 0 ) {
        LOG( "rest : " + std::to_string( rest ) );
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        mThreadPool.add( [blocks, rest, this] {
            size_t from = blocks - rest;
            size_t to   = blocks;
            LOG( std::to_string( from ) + " to " + std::to_string( to ) );

            for( size_t i = from; i < to; ++i ) {
                mBlocks[i].dct();
            }
        } );
    }

    mThreadPool.waitForAllJobs();

    //    // some stats
    //    std::atomic_int max( 20 );
    //    int freq = 0;

    //    for( Block& block : mBlocks ) {
    //        max.store( std::max( max.load(), block.frequency( freq ) ) );
    //    }
    //    for( Block& block : mBlocks ) {
    //        mGrey[block.x()+Block::size/2][block.y()+Block::size/2] = ( max.load() + block.frequency( freq ) ) * 256 / max;
    //    }
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
    LOG( "Sorting blocks..." );
    STATE_CHECK( mBlocksSorted );

    std::sort( mBlocks.begin(), mBlocks.end() );
}

void DCTSorter::searchDuplicates() {
    LOG( "Searching duplicates..." );
    STATE_CHECK( mDuplicatesSearched );

    std::vector<Block>::iterator b = mBlocks.begin();
    std::vector<Block>::iterator c = b;

    for( ; b != mBlocks.end(); ++b ) {

        if( !b->interesting() ) {
            continue;
        }

        c = b + 1;

        if( c == mBlocks.end() ) {
            break;
        }

        while( b->hasSimilarFreqs( *c ) ) {

            if( c->interesting() && b->manhattanDistance( *c ) > ( 5 * Block::size ) ) {
                int dx = ( c->x() - b->x() );
                int dy = ( c->y() - b->y() );

                Shift shift( dx, dy );
                shift.quantize();

                // if already exists as negative shift
                if( mShifts.find( -shift ) != mShifts.end() ) {
                    mShifts[ -shift ].push_back( std::make_pair( c->pos(), b->pos() ) );
                } else {
                    mShifts[ shift ].push_back( std::make_pair( b->pos(), c->pos() ) );
                }
            }

            ++c;

            if( c == mBlocks.end() ) {
                break;
            }
        }

    }
}

void DCTSorter::debugBlocks() {
    LOG( "Writing debug info..." );
    std::ofstream ofs( "blocks.txt", std::ios::out );

    if( ofs.is_open() ) {
        for( Block & b : mBlocks ) {
            ofs << b;
        }
    }
}

void DCTSorter::sortShifts() {
    LOG( "Sorting shifts..." );
    STATE_CHECK( mShiftsSorted );

    mShiftHits.reserve( mShifts.size() );

    for( auto & count : mShifts ) {
        ShiftHit hit( count.first, mImageSize, mMinHits );
        hit.setBlocks( count.second );

        if( hit.looksGood() ) {
            mShiftHits.push_back( hit );
        }
    }

    std::sort( mShiftHits.rbegin(), mShiftHits.rend() );

    // set ranking
    int position = 0;

    for( ShiftHit & hit : mShiftHits ) {
        hit.setRanking( position );
        position++;
    }

    LOG( "Hits: " + std::to_string( position ) );


    // write result
    int size = mShiftHits.size();

    for( int i = 0; i < size ; ++i ) {
        Block white( 255 * ( size - i ) / size );
        ShiftHit& hit = mShiftHits[i];
        LOG( "Found " + hit.toString() );
        std::list<std::pair<PointI, PointI>>& pairs = hit.getBlocks();

        for( std::pair<PointI, PointI>& pair : pairs ) {
            mResult.from.setBlock( white, pair.first.x(), pair.first.y() );
            mResult.to.setBlock( white, pair.second.x(), pair.second.y() );
        }
    }
}
