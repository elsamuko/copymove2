#include "shift.hpp"
#include <sstream>

Shift::Shift()
{
}

bool Shift::operator <( const Shift& that ) const {
    bool smaller = (this->mDx + this->mDy) < (that.mDx + that.mDy);
    return smaller;
}

bool Shift::operator >( const Shift& b ) const {
    return !( *this < b );
}

void Shift::setDx(int dx ) {
    mDx = dx/3;
}

void Shift::setDy(int dy ) {
    mDy = dy/3;
}

std::ostream& operator <<(std::ostream &stream, const Shift &b) {
    stream << "[" << b.mDx << ", " << b.mDy << "]";
    return stream;
}

std::string Shift::toString() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

