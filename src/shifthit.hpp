#pragma once

#include <vector>

#include "shift.hpp"
#include "block.hpp"

class ShiftHit {
    public:
        explicit ShiftHit( Shift shift );
        ~ShiftHit();
        bool operator <(const ShiftHit& b ) const;
        bool operator >( const Shift& b ) const;
        void setBlocks( const std::vector<std::pair<Block,Block>>& blocks );
        std::vector<std::pair<Block,Block>>& getBlocks();
        std::vector<std::pair<Block,Block>> getGoodBlocks();
        bool looksGood() const;

        friend std::ostream& operator <<( std::ostream& stream, const ShiftHit& b );
        std::string toString() const;

        int x() const { return mMeanX; }
        int y() const { return mMeanY; }
        int radius() const { return mStandardDeviation; }
        int dx() const { return mShift.dx(); }
        int dy() const { return mShift.dy(); }

        // template<class T>
        // static double cosineSimilarity( const T& a, const T&b, double normA, double normB );

    private:
        void calculateStandardDeviation();

        Shift mShift;
        std::vector<std::pair<Block,Block>> mBlocks;

        // state checks
        bool mDataReceived;
        bool mMeanCalculated;

        // arithm average of position of blocks
        double mMeanX;
        double mMeanY;

        // stddev of position of blocks -> how spread is the hit
        double mStandardDeviation;
};
