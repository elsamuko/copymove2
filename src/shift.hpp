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

        void setDx( size_t dx );
        void setDy( size_t dy );
        friend std::ostream& operator << ( std::ostream& stream, const Shift& b );
        std::string toString() const;

private:
        size_t mDx;
        size_t mDy;
};

#endif // SHIFT_HPP
