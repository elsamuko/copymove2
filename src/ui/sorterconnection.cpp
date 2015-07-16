
#include <QtConcurrent/QtConcurrent>

#include "sorterconnection.hpp"
#include "scopeguard.hpp"

#define IF_STOPPED_RETURN if( mSorter.stopped() ) { LOG( "Stopped" ); return; }

SorterConnection::SorterConnection( QObject* parent ) :
    QObject( parent ) {
}

void SorterConnection::setImage( QImage image ) {
    mImage = image;
}

void SorterConnection::slotRun( SorterParams params ) {

    std::unique_lock<std::mutex> lock( mStateLock, std::try_to_lock );

    if( !lock.owns_lock() ) {
        LOG( "Can't lock state" );
        return;
    }

    if( mImage.isNull() ) {
        emit signalReset();
        LOG( "No image" );
        return;
    }

    if( !params.valid() ) {
        emit signalReset();
        LOG( "Invalid params" );
        return;
    }

    mSorter.reset( true );
    mSorter.start();
    mSorter.setParams( params );

    mWhenFinished = QtConcurrent::run( [this] {

        LOG( "Run requested" );
        emit signalProgress( 1 );

        GreyImage grey = this->getGrey();
        mSorter.setGrey( grey );
        emit signalProgress( 3 );
        IF_STOPPED_RETURN;

        mSorter.setProgressCallback( [this]( size_t progress ) {
            emit signalProgress( progress );
        } );

        mSorter.work();
        emit signalProgress( 100 );
        IF_STOPPED_RETURN;

        mShiftHits = mSorter.getShiftHits();
        IF_STOPPED_RETURN;

        auto begin = mShiftHits.cbegin();
        auto end   = mShiftHits.cend();

        emit signalDone( begin, end );
    } );
}

void SorterConnection::slotStop() {
    std::unique_lock<std::mutex> lock( mStateLock, std::try_to_lock );

    if( !lock.owns_lock() ) {
        LOG( "Can't lock state" );
        return;
    }

    LOG( "Stop requested" );
    mSorter.stop();
    mWhenFinished.waitForFinished();
    mSorter.resume();
    emit signalReset();
    LOG( "Stopped" );
}

GreyImage SorterConnection::getGrey() const {
    const uchar* bits = mImage.constBits();
    GreyImage grey( mImage.width(), mImage.height() );

    for( int y = 0; y < mImage.height(); y++ ) {
        for( int x = 0; x < mImage.width(); x++ ) {
            bits++; // b
            grey[x][y] = *bits;
            bits++; // g
            bits++; // r
            bits++; // a
        }
    }

    return grey;
}
