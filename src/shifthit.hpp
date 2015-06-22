#pragma once

#include <list>

#include "shift.hpp"
#include "block.hpp"
#include "point.hpp"

class ShiftHit {
    public:
        explicit ShiftHit( Shift shift );
        ~ShiftHit();
        bool operator <(const ShiftHit& b ) const;
        bool operator >( const Shift& b ) const;
        void setBlocks( const std::list<std::pair<Block,Block>>& blocks );
        std::list<std::pair<Block,Block>>& getBlocks();
        bool looksGood() const;

        friend std::ostream& operator <<( std::ostream& stream, const ShiftHit& b );
        std::string toString() const;

        int x() const { return mMeanX; }
        int y() const { return mMeanY; }
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

        Shift mShift;
        std::list<std::pair<Block,Block>> mBlocks;

        // state checks
        bool mDataReceived;
        bool mMeanCalculated;

        // debug state
        bool mVerbose;

        int mRanking;

        // arithm average of position of blocks
        PointF mMean;
        float mMeanX;
        float mMeanY;

        // stddev of position of blocks -> how spread is the hit
        float mStandardDeviation;

        // average distance of all hits to center hit
        float mGeometricAverageDistance;
};
