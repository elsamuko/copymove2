#include <algorithm>
#include <utility>
#include <sstream>
#include <fstream>

#include "dctsorter.hpp"
#include "log/log.hpp"
#include "scopeguard.hpp"

#define IF_STOPPED_RETURN if( mState != DCTSorter::Running ) { LOG( "Stopped" ); return; }

DCTSorter::DCTSorter() {
    reset( true );
}

DCTSorter::~DCTSorter() {
    this->stop();
}

void DCTSorter::reset( bool hard ) {

    if( hard ) {
        mBlocks.clear();
        mShifts.clear();
        mShiftHits.clear();
        mState = DCTSorter::Idling;
    }

    mGreyReceived       = false;
    mWorked             = false;
    mBlocksSet          = false;
    mDCTCalculated      = false;
    mBlocksSorted       = false;
    mDuplicatesSearched = false;
    mShiftsSorted       = false;
}

void DCTSorter::setProgress( size_t percentage ) {
    LOG( std::to_string( percentage ) + "%" );

    if( mProgressCallback ) {
        mProgressCallback( percentage );
    }
}

void DCTSorter::setGrey( const GreyImage& grey ) {
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

void DCTSorter::setParams( const SorterParams& params ) {
    mParams = params;
    LOG( "Params: " + params.toString() );
    assert( mParams.valid() );
}

void DCTSorter::work() {
    IF_STOPPED_RETURN;
    LOG( "Do work..." );
    STATE_CHECK( mWorked );

    setProgress( 10 );
    readGreyToBlocks();
    IF_STOPPED_RETURN;

    setProgress( 70 );
    // debugBlocks();
    sortBlocks();
    IF_STOPPED_RETURN;

    setProgress( 80 );
    searchDuplicates();
    IF_STOPPED_RETURN;

    setProgress( 90 );
    sortShifts();
    setProgress( 99 );
}

void DCTSorter::start() {
    mState = DCTSorter::Running;
}

void DCTSorter::stop() {
    mState = DCTSorter::Stopping;
    LOG( "Stop requested" );
    mThreadPool.suspend();
    this->reset( false );
    mState = DCTSorter::Idling;
}

void DCTSorter::resume() {
    mThreadPool.resume();
}

bool DCTSorter::stopped() const {
    return mState != DCTSorter::Running;
}

void DCTSorter::setProgressCallback( const std::function<void( size_t )>& callback ) {
    mProgressCallback = callback;
}

void DCTSorter::readGreyToBlocks() {
    LOG( "Read image..." );
    STATE_CHECK( mBlocksSet );
    STATE_CHECK( mDCTCalculated );

    const size_t width = mGrey.width();
    const size_t height = mGrey.height();
    const size_t hB = height - Block::size;
    const size_t wB = width  - Block::size;
    mBlocks = std::vector<Block>( hB * wB, Block( 0.f, mParams.quality(), false ) );

    // send small progress steps from 10-70 %
    int size = hB * wB;
    std::atomic_int pos( 0 );
    std::function<void()> step( [&pos, &size, this] {
        if( ( pos++ % 25000 ) == 0 ) {
            float progress = ( float ) pos.load() / size;
            int percent = 10 + progress * 60.f;
            setProgress( percent );
        }
    } );

    // read
    bool ok = true;

    for( size_t y = 0; y < hB; ++y ) {
        for( size_t x = 0; x < wB; ++x ) {
            IF_STOPPED_RETURN;
            int current = y * wB + x;
            ok &= mThreadPool.add( [this, current, x, y, &step] {
                Block block;
                block.setX( x );
                block.setY( y );
                block.setQuality( mParams.quality() );
                block.initData(); // create data first here...
                mGrey.getBlock( block, x, y );
                block.calculateStandardDeviation();
                block.dct();
                block.clearData(); // ...and clear it right back for less memory consumption
                IF_STOPPED_RETURN;
                mBlocks[current] = block;
                step();
            } );
        }
    }

    LOG( "Waiting..." );
    mThreadPool.waitForAllJobs();
    IF_STOPPED_RETURN;

    // some stats
    if( ok ) {
        for( Block& block : mBlocks ) {
            IF_STOPPED_RETURN;
            mGrey[block.x() + Block::size / 2][block.y() + Block::size / 2] = block.interesting() ? 255 : 0;
        }
    }
}

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
        for( Block& b : mBlocks ) {
            ofs << b;
        }
    }
}

void DCTSorter::sortShifts() {
    LOG( "Sorting shifts..." );
    STATE_CHECK( mShiftsSorted );

    mShiftHits.reserve( mShifts.size() );

    for( auto& count : mShifts ) {
        ShiftHit hit( count.first, mImageSize, mParams.minimalHits() );
        hit.setBlocks( count.second );

        if( hit.looksGood() ) {
            mShiftHits.push_back( hit );
        }
    }

    std::sort( mShiftHits.rbegin(), mShiftHits.rend() );

    // set ranking
    int position = 0;

    for( ShiftHit& hit : mShiftHits ) {
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
