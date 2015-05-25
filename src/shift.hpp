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

        void setX( size_t x );
        void setY( size_t y );
        friend std::ostream& operator << ( std::ostream& stream, const Shift& b );
        std::string toString() const;

private:
        size_t mX;
        size_t mY;
};

#endif // SHIFT_HPP
