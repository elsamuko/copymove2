#include "offset.hpp"

Offset::Offset()
{
}

Shift Offset::getShift() const {
    Shift s;
    s.setDx( mX2 - mX1 );
    s.setDy( mY2 - mY1 );
    return s;
}
