#include "shift.hpp"

Shift::Shift()
{
}

bool Shift::operator <( const Shift& that ) const {
    bool smaller = (this->mX + this->mY) < (that.mX + that.mY);
    return smaller;
}

bool Shift::operator >( const Shift& b ) const {
    return !( *this < b );
}

void Shift::setX( size_t x ) {
    mX = x;
}

void Shift::setY( size_t y ) {
    mY = y;
}

