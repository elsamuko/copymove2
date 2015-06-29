#include "sorterconnection.hpp"

SorterConnection::SorterConnection( QObject* parent ) :
    QObject( parent ) {
}

void SorterConnection::setImage( QImage image ) {
    mImage = image;
}

void SorterConnection::slotRun( SorterParams params ) {
    LOG( "Run requested" );
    GreyImage grey = this->getGrey();
    mSorter.setGrey( grey );
    mSorter.setParams( params );
    mSorter.work();
    mShiftHits = mSorter.getShiftHits();

    auto begin = mShiftHits.cbegin();
    auto end   = mShiftHits.cend();

    emit signalDone( begin, end );
}

GreyImage SorterConnection::getGrey() const {
    const uchar* bits   = mImage.constBits();
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
