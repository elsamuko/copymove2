#pragma once

#include "shift.hpp"

class Offset {
    public:
        Offset();
        Shift getShift() const;
    private:
        int mX1;
        int mX2;
        int mY1;
        int mY2;
};
