#pragma once

#include <list>

#include "shift.hpp"
#include "block.hpp"
#include "point.hpp"

class ShiftHit {
    public:
        explicit ShiftHit() {}
        explicit ShiftHit( Shift shift, PointI size, size_t minHits );
        ~ShiftHit();
        bool operator <( const ShiftHit& b ) const;
        bool operator >( const Shift& b ) const;
        void setBlocks( const std::list<std::pair<PointI, PointI> >& blocks );
        std::list<std::pair<PointI, PointI> >& getBlocks();
        bool looksGood() const;

        friend std::ostream& operator <<( std::ostream& stream, const ShiftHit& b );
        std::string toString() const;

        int x() const {
            assert( mMedianCalculated );
            return mMedian.GeometricAverage.x();
        }
        int y() const {
            assert( mMedianCalculated );
            return mMedian.GeometricAverage.y();
        }
        PointI mean() const {
            assert( mMeanCalculated );
            return mMean;
        }
        int radius() const {
            assert( mMeanCalculated );
            return mStandardDeviation;
        }
        int distance() const {
            assert( mMedianCalculated );
            return mMedian.GeometricAverageDistance;
        }
        int distanceMinHits() const {
            assert( mMedianCalculated );
            return mMedian.MinHitsAverageDistance;
        }
        size_t hits() const {
            return mHits.size();
        }
        PointI imageSize() const {
            return mImageSize;
        }
        int dx() const {
            return mShift.dx();
        }
        int dy() const {
            return mShift.dy();
        }
        void setVerbosity( bool verbose );

        void setRanking( int position ) {
            mRanking = position;
        }
        int ranking() const {
            return mRanking;
        }

        struct Median {
            Median() : GeometricAverage( PointF( 0, 0 ) ), GeometricAverageDistance( 0.F ), MinHitsAverageDistance( 0.F ) {}
            Median( PointF gavg, float dist, float distmin ) : GeometricAverage( gavg ), GeometricAverageDistance( dist ), MinHitsAverageDistance( distmin ) {}
            PointF GeometricAverage;
            float GeometricAverageDistance;
            float MinHitsAverageDistance;
        };

        static Median geometricMedian( const std::vector<PointF>& points, const size_t minHits );

    private:
        void calculateStandardDeviation();
        void calculateGeometricDistance();

        Shift mShift;
        PointI mImageSize;
        std::list<std::pair<PointI, PointI>> mHits; // from -> to
        size_t mMinHits;

        // state checks
        bool mDataReceived;
        bool mMeanCalculated;
        bool mMedianCalculated;

        // debug info
        bool mVerbose;
        int mRanking;

        // arith average + stddev
        PointF mMean;
        float mStandardDeviation;

        // geometric median + distance
        Median mMedian;
        size_t mWithinGeometricAverage;
};
