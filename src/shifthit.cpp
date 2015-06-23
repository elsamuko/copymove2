#include "shifthit.hpp"
#include <sstream>
#include <iomanip>
#include <cfloat>

ShiftHit::ShiftHit(Shift shift, PointI size , size_t minHits) :
    mShift( shift ),
    mImageSize( size ),
    mMinHits( minHits ),
    mDataReceived( false ),
    mMeanCalculated( false ),
    mMedianCalculated( false ),
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
    stream << " ["    << std::setw( 4 ) << std::round( b.mMean.x() );
    stream << ", "    << std::setw( 4 ) << std::round( b.mMean.y() );
    stream << "] +- " << std::setw( 4 ) << std::round( b.mStandardDeviation );
    stream << " ["    << std::setw( 4 ) << std::round( b.mGeometricAverage.x() );
    stream << ", "    << std::setw( 4 ) << std::round( b.mGeometricAverage.y() );
    stream << "] +- " << std::setw( 4 ) << std::round( b.mGeometricAverageDistance );
    stream << " dm "  << std::setw( 4 ) << std::round( b.mMean.distance( b.mGeometricAverage ) );
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

void ShiftHit::calculateGeometricDistance() {
    assert( !mMedianCalculated );
    assert( mDataReceived );

    std::list<PointF> points;

    for( auto& fromTo : mBlocks ) {
        points.emplace_back( fromTo.first.x(), fromTo.first.y() );
    }

    std::pair<PointF,float> median = ShiftHit::geometricMedian( points );
    mGeometricAverageDistance = median.second;
    mGeometricAverage = median.first;

    mMedianCalculated = true;
}

void ShiftHit::calculateStandardDeviation() {
    assert( !mMeanCalculated );
    assert( mDataReceived );

    mMean.set( 0.f, 0.f );
    mStandardDeviation = 0.f;
    PointF diff;

    for( auto& fromTo : mBlocks ) {
        mMean += fromTo.first.pos();
    }

    mMean /= (float)(mBlocks.size());

    for( auto& fromTo : mBlocks ) {
        diff = PointF( fromTo.first.pos() ) - mMean;
        mStandardDeviation += diff.sqr();
    }

    mStandardDeviation /= (float)(mBlocks.size());
    mStandardDeviation = std::sqrt( mStandardDeviation );

    mMeanCalculated = true;
}

void ShiftHit::setBlocks(const std::list<std::pair<Block, Block> >& blocks ) {
    assert( !mDataReceived );
    mBlocks = blocks;
    mDataReceived = true;

    calculateGeometricDistance();
    calculateStandardDeviation();
}

std::list<std::pair<Block, Block> >&ShiftHit::getBlocks() {
    return mBlocks;
}

bool ShiftHit::looksGood() const {
    assert( mMeanCalculated );
    assert( mMedianCalculated );
    // bool centerCriterium = mMean.distance( mGeometricAverage ) < 25;       // arith. mean and geometric median are close
    // bool spreadCriterium = mGeometricAverageDistance < mImageSize.abs()/8; // hit is not too spread
    bool sizeCriterium   = mBlocks.size() > mMinHits;                      // minimum of hits
    return /*centerCriterium && spreadCriterium &&*/ sizeCriterium;
}
