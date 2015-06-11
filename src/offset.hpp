#ifndef OFFSET_HPP
#define OFFSET_HPP

#include "shift.hpp"

class Offset
{
    public:
        Offset();
        Shift getShift() const;
    private:
        int mX1;
        int mX2;
        int mY1;
        int mY2;
};

#endif // OFFSET_HPP
