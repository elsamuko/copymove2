#include "shift.hpp"
#include <sstream>

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

std::ostream& operator <<(std::ostream &stream, const Shift &b) {
    stream << "[" << b.mX << ", " << b.mY << "]";
}

std::string Shift::toString() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

