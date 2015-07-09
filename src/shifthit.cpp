#include <sstream>
#include <iomanip>
#include <cfloat>
#include <numeric>
#include <algorithm>
#include <tuple>

#include "shifthit.hpp"

ShiftHit::ShiftHit( Shift shift, PointI size , size_t minHits ) :
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
    assert( mMedianCalculated );
    // bool less = ( this->mHits.size() < that.mHits.size() );
    bool less = ( this->mWithinGeometricAverage < that.mWithinGeometricAverage );
    return less;
}

std::ostream& operator <<( std::ostream& stream, const ShiftHit& b ) {
    stream << "#"      << std::setw( 2 ) << std::round( b.mRanking );
    stream << "    ";
    stream << " ["     << std::setw( 4 ) << std::round( b.mMean.x() );
    stream << ", "     << std::setw( 4 ) << std::round( b.mMean.y() );
    stream << "] +- "  << std::setw( 4 ) << std::round( b.mStandardDeviation );
    stream << "    ";
    stream << " ["     << std::setw( 4 ) << std::round( b.mGeometricAverage.x() );
    stream << ", "     << std::setw( 4 ) << std::round( b.mGeometricAverage.y() );
    stream << "] +- "  << std::setw( 4 ) << std::round( b.mGeometricAverageDistance );
    stream << " +- "   << std::setw( 4 ) << std::round( b.mMinHitsAverageDistance );
    stream << "  dm "  << std::setw( 4 ) << std::round( b.mMean.distance( b.mGeometricAverage ) );
    stream << "  -> [" << std::setw( 5 ) << b.mShift.dx();
    stream << ", "     << std::setw( 5 ) << b.mShift.dy();
    stream << "] w/ "  << std::setw( 4 ) << b.mHits.size() << " hits";

    if( b.mVerbose ) {
        stream << ": { ";

        for( auto& fromTo : b.mHits ) {
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

void ShiftHit::setVerbosity( bool verbose ) {
    mVerbose = verbose;
}

std::tuple<PointF, float, float> ShiftHit::geometricMedian( const std::vector<PointF>& points, const size_t minHits ) {
    float currentMin = FLT_MAX;
    PointF result;
    std::vector<PointF> copy = points;

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

    // get distance from <minHits> nearest points
    std::sort( copy.begin(), copy.end(), [&result]( const PointF & a, const PointF & b ) {
        return a.distance( result ) < b.distance( result );
    } );

    if( copy.size() > minHits ) {
        copy.resize( minHits );
    }

    float distance = std::accumulate( copy.cbegin(), copy.cend(), 0.f, [&result]( const float sum, const PointF & p ) {
        return sum + p.distance( result );
    } );

    return std::make_tuple( result, currentMin / points.size(), distance / copy.size() );
}

void ShiftHit::calculateGeometricDistance() {
    assert( !mMedianCalculated );
    assert( mDataReceived );

    std::vector<PointF> points;
    points.reserve( mHits.size() );

    for( auto& fromTo : mHits ) {
        points.emplace_back( fromTo.first );
    }

    std::tuple<PointF, float, float> median = ShiftHit::geometricMedian( points, mMinHits );
    mGeometricAverage = std::get<0>( median );
    mGeometricAverageDistance = std::get<1>( median );
    mMinHitsAverageDistance = std::get<2>( median );

    mWithinGeometricAverage = std::accumulate( points.cbegin(), points.cend(), 0, [this]( const int sum, const PointF & p ) {
        if( mGeometricAverage.distance( p ) < mGeometricAverageDistance ) {
            return sum + 1;
        } else {
            return sum;
        }
    } );

    mMedianCalculated = true;
}

void ShiftHit::calculateStandardDeviation() {
    assert( !mMeanCalculated );
    assert( mDataReceived );

    mMean.set( 0.f, 0.f );
    mStandardDeviation = 0.f;
    PointF diff;

    for( auto& fromTo : mHits ) {
        mMean += fromTo.first;
    }

    mMean /= ( float )( mHits.size() );

    for( auto& fromTo : mHits ) {
        diff = PointF( fromTo.first ) - mMean;
        mStandardDeviation += diff.sqr();
    }

    mStandardDeviation /= ( float )( mHits.size() );
    mStandardDeviation = std::sqrt( mStandardDeviation );

    mMeanCalculated = true;
}

void ShiftHit::setBlocks( const std::list<std::pair<PointI, PointI> >& blocks ) {
    assert( !mDataReceived );
    mHits = blocks;
    mDataReceived = true;

    calculateGeometricDistance();
    calculateStandardDeviation();
}

std::list<std::pair<PointI, PointI> >& ShiftHit::getBlocks() {
    return mHits;
}

bool ShiftHit::looksGood() const {
    assert( mMeanCalculated );
    assert( mMedianCalculated );
    // bool centerCriterium = mMean.distance( mGeometricAverage ) < mImageSize.abs() / 8; // arith. mean and geometric median are close
    // bool spreadCriterium = mGeometricAverageDistance < mImageSize.abs() / 8;           // hit is not too spread
    bool clusterCriterium = mMinHitsAverageDistance < mImageSize.abs() / 16;           // best <minHits> hits are not too spread
    bool sizeCriterium   = mHits.size() > mMinHits;                                    // minimum of hits
    return /*centerCriterium && spreadCriterium &&*/ clusterCriterium && sizeCriterium;
}
