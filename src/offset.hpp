#ifndef OFFSET_HPP
#define OFFSET_HPP

#include "shift.hpp"

class Offset
{
    public:
        Offset();
        Shift getShift() const;
    private:
        size_t mX1;
        size_t mX2;
        size_t mY1;
        size_t mY2;
};

#endif // OFFSET_HPP
