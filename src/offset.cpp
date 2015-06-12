#include "offset.hpp"

Offset::Offset()
{
}

Shift Offset::getShift() const {
    Shift s( mX2 - mX1, mY2 - mY1 );
    return s;
}
