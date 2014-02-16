#ifndef SHIFT_HPP
#define SHIFT_HPP

#include <cmath>

class Shift
{
    public:
        Shift();
        bool operator <( const Shift& b ) const;
        bool operator >( const Shift& b ) const;

        void setX( size_t x );
        void setY( size_t y );
private:
        size_t mX;
        size_t mY;
};

#endif // SHIFT_HPP
