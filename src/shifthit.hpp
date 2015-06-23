#pragma once

#include <list>

#include "shift.hpp"
#include "block.hpp"
#include "point.hpp"

class ShiftHit {
    public:
        explicit ShiftHit( Shift shift, PointI size, size_t minHits );
        ~ShiftHit();
        bool operator <(const ShiftHit& b ) const;
        bool operator >( const Shift& b ) const;
        void setBlocks( const std::list<std::pair<Block,Block>>& blocks );
        std::list<std::pair<Block,Block>>& getBlocks();
        bool looksGood() const;

        friend std::ostream& operator <<( std::ostream& stream, const ShiftHit& b );
        std::string toString() const;

        int x() const { return mGeometricAverage.x(); }
        int y() const { return mGeometricAverage.y(); }
        int radius() const { assert( mMeanCalculated ); return mStandardDeviation; }
        int distance() const { assert( mMeanCalculated ); return mGeometricAverageDistance; }
        int dx() const { return mShift.dx(); }
        int dy() const { return mShift.dy(); }
        void setVerbosity( bool verbose );

        void setRanking( int position ) { mRanking = position; }
        int ranking() const { return mRanking; }

        static std::pair<PointF,float> geometricMedian( const std::list<PointF>& points );

    private:
        void calculateStandardDeviation();
        void calculateGeometricDistance();

        Shift mShift;
        PointI mImageSize;
        std::list<std::pair<Block,Block>> mBlocks;
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
        PointF mGeometricAverage;
        float mGeometricAverageDistance;
};
