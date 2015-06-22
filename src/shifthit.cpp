#include "shifthit.hpp"
#include <sstream>
#include <iomanip>
#include <cfloat>

ShiftHit::ShiftHit( Shift shift ) :
    mShift( shift ),
    mDataReceived( false ),
    mMeanCalculated( false ),
    mVerbose( false ) {
}

ShiftHit::~ShiftHit() {

}

bool ShiftHit::operator <( const ShiftHit& that ) const {
    bool less = ( this->mBlocks.size() < that.mBlocks.size() );
    return less;
}

std::ostream& operator <<(std::ostream &stream, const ShiftHit &b) {
    stream << "#"     << std::setw( 2 ) << std::round( b.mRanking );
    stream << " ["    << std::setw( 4 ) << std::round( b.mMeanX );
    stream << ", "    << std::setw( 4 ) << std::round( b.mMeanY );
    stream << "] +- " << std::setw( 4 ) << std::round( b.mStandardDeviation );
    stream << " av. " << std::setw( 4 ) << std::round( b.mGeometricAverageDistance );
    stream << " -> [" << std::setw( 5 ) << b.mShift.dx();
    stream << ", "    << std::setw( 5 ) << b.mShift.dy();
    stream << "] w/ " << std::setw( 4 ) << b.mBlocks.size() << " hits";

    if( b.mVerbose ) {
        stream << ": { ";

        for( auto& fromTo : b.mBlocks ) {
            stream << "{";
            stream << fromTo.first.x() << ",";
            stream << fromTo.first.y() << "}, ";
        }
        stream << "}";
    }

    return stream;
}

std::string ShiftHit::toString() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

void ShiftHit::setVerbosity(bool verbose) {
    mVerbose = verbose;
}

std::pair<PointF, float> ShiftHit::geometricMedian( const std::list<PointF>& points ) {
    float currentMin = FLT_MAX;
    PointF result;

    for( const PointF& a : points ) {
        float squareSum = 0;
        for( const PointF& b : points ) {
            squareSum += a.distance( b );
        }
        if( squareSum < currentMin ) {
            currentMin = squareSum;
            result = a;
        }
    }
    return std::make_pair( result, currentMin/points.size() );
}

void ShiftHit::calculateStandardDeviation() {
    assert( !mMeanCalculated );
    assert( mDataReceived );
    mMean.set( 0.f, 0.f );
    mMeanX = 0.f;
    mMeanY = 0.f;
    mStandardDeviation = 0.f;
    float diffX = 0.f;
    float diffY = 0.f;

    std::list<PointF> points;

    for( auto& fromTo : mBlocks ) {
        points.emplace_back( fromTo.first.x(), fromTo.first.y() );
    }

    std::pair<PointF,float> median = ShiftHit::geometricMedian( points );

    mGeometricAverageDistance = median.second;

    mMeanX = median.first.x();
    mMeanY = median.first.y();

//    for( auto& fromTo : mBlocks ) {
//        mMeanX += fromTo.first.x();
//        mMeanY += fromTo.first.y();
//    }

//    mMeanX /= (float)(mBlocks.size());
//    mMeanY /= (float)(mBlocks.size());

    for( auto& fromTo : mBlocks ) {
        diffX = fromTo.first.x() - mMeanX;
        diffY = fromTo.first.y() - mMeanY;
        mStandardDeviation += diffX*diffX;
        mStandardDeviation += diffY*diffY;
    }

    mStandardDeviation /= (float)(mBlocks.size());
    mStandardDeviation = std::sqrt( mStandardDeviation );

    mMeanCalculated = true;
}

void ShiftHit::setBlocks(const std::list<std::pair<Block, Block> >& blocks ) {
    assert( !mDataReceived );
    mBlocks = blocks;
    mDataReceived = true;

    calculateStandardDeviation();
}

std::list<std::pair<Block, Block> >&ShiftHit::getBlocks() {
    return mBlocks;
}

bool ShiftHit::looksGood() const {
    assert( mMeanCalculated );
    return mBlocks.size() > 10 && mGeometricAverageDistance < 100; // TODO: Parameterize
}
