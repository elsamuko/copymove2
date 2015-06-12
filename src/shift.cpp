#include "shift.hpp"
#include <sstream>

Shift::Shift( int dx, int dy ) :
    mDx( dx), mDy( dy ) {
}

bool Shift::operator <( const Shift& that ) const {
    bool smaller = (this->mDx + this->mDy) < (that.mDx + that.mDy);
    return smaller;
}

bool Shift::operator >( const Shift& b ) const {
    return !( *this < b );
}

void Shift::setDx(int dx ) {
    mDx = dx;
}

void Shift::setDy(int dy ) {
    mDy = dy;
}

int Shift::dx() const {
    return mDx;
}

int Shift::dy() const {
    return mDy;
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

Shift Shift::operator -() {
    Shift s( -mDx, -mDy );
    return s;
}

