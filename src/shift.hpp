#pragma once

#include <cmath>
#include <cstdlib>
#include <iostream>

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

private:
        int mDx;
        int mDy;
};
