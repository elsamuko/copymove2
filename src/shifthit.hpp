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

        friend std::ostream& operator <<( std::ostream& stream, const ShiftHit& b );
        std::string toString() const;

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
