#include "offset.hpp"

Offset::Offset()
{
}

Shift Offset::getShift() const {
    Shift s;
    s.setDx( std::abs( mX2 - mX1 ) );
    s.setDy( std::abs( mY2 - mY1 ) );
    return s;
}
