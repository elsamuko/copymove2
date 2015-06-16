#include "shifthit.hpp"
#include <sstream>

ShiftHit::ShiftHit( Shift shift ) :
    mShift( shift ),
    mDataReceived( false ),
    mMeanCalculated( false ) {
}

ShiftHit::~ShiftHit() {

}

bool ShiftHit::operator <( const ShiftHit& that ) const {
    bool less = ( this->mBlocks.size() < that.mBlocks.size() );
    return less;
}

std::ostream& operator <<(std::ostream &stream, const ShiftHit &b) {
    stream << "[" << b.mMeanX << ", " << b.mMeanY << "] +- " << b.mStandardDeviation;
    stream << " w/ " << b.mBlocks.size() << " hits";
    return stream;
}

std::string ShiftHit::toString() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

void ShiftHit::calculateStandardDeviation() {
    assert( !mMeanCalculated );
    assert( mDataReceived );
    mMeanX = 0.f;
    mMeanY = 0.f;
    mStandardDeviation = 0.f;
    double diffX = 0.f;
    double diffY = 0.f;

    for( auto& fromTo : mBlocks ) {
        mMeanX += fromTo.first.x();
        mMeanY += fromTo.first.y();
    }

    mMeanX /= (double)(mBlocks.size());
    mMeanY /= (double)(mBlocks.size());

    for( auto& fromTo : mBlocks ) {
        diffX = fromTo.first.x() - mMeanX;
        diffY = fromTo.first.y() - mMeanY;
        mStandardDeviation += diffX*diffX;
        mStandardDeviation += diffY*diffY;
    }

    mStandardDeviation /= (double)(mBlocks.size());
    mStandardDeviation = std::sqrt( mStandardDeviation );

    mMeanCalculated = true;
}

void ShiftHit::setBlocks(const std::vector<std::pair<Block, Block> >& blocks ) {
    assert( !mDataReceived );
    mBlocks = blocks;
    mDataReceived = true;

    calculateStandardDeviation();
}

std::vector<std::pair<Block, Block> >&ShiftHit::getBlocks() {
    return mBlocks;
}

