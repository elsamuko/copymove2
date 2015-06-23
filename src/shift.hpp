#pragma once

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "point.hpp"

class Shift
{
    public:
        Shift( int dx, int dy );
        bool operator <( const Shift& b ) const;
        bool operator >( const Shift& b ) const;

        void setDx( int dx );
        void setDy( int dy );
        int dx() const;
        int dy() const;

        friend std::ostream& operator <<( std::ostream& stream, const Shift& b );
        std::string toString() const;
        Shift operator -();

        template<int i>
        static int quantize( int in ) {
            return int( i * std::round( float( in ) / i ) );
        }

        void quantize();

private:
        PointI mDXY;
};
