#ifndef SHIFT_HPP
#define SHIFT_HPP

#include <cmath>
#include <cstdlib>
#include <iostream>

class Shift
{
    public:
        Shift();
        bool operator <( const Shift& b ) const;
        bool operator >( const Shift& b ) const;

        void setDx( int dx );
        void setDy( int dy );
        friend std::ostream& operator << ( std::ostream& stream, const Shift& b );
        std::string toString() const;

private:
        int mDx;
        int mDy;
};

#endif // SHIFT_HPP
