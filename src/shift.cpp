#include "shift.hpp"
#include <sstream>

Shift::Shift( int dx, int dy ) :
    mDXY( dx, dy ) {
}

bool Shift::operator <( const Shift& that ) const {

    if( this->mDXY.y() < that.mDXY.y() ) return true;
    if( this->mDXY.y() > that.mDXY.y() ) return false;

    return this->mDXY.x() < that.mDXY.x();
}

bool Shift::operator >( const Shift& b ) const {
    return !( *this < b );
}

void Shift::setDx(int dx ) {
    mDXY.setX( dx );
}

void Shift::setDy(int dy ) {
    mDXY.setY( dy );
}

int Shift::dx() const {
    return mDXY.x();
}

int Shift::dy() const {
    return mDXY.y();
}

std::ostream& operator <<(std::ostream &stream, const Shift &b) {
    stream << "[" << b.mDXY.x() << ", " << b.mDXY.y() << "]";
    return stream;
}

std::string Shift::toString() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

Shift Shift::operator -() {
    Shift s( -mDXY.x(), -mDXY.y() );
    return s;
}

void Shift::quantize() {
    if( std::abs( mDXY.x() ) > 200 ) {
        mDXY.setX( Shift::quantize<10>( mDXY.x() ) );
    } else {
        mDXY.setX( Shift::quantize<5>( mDXY.x() ) );
    }

    if( std::abs( mDXY.y() ) > 200 ) {
        mDXY.setY( Shift::quantize<10>( mDXY.y() ) );
    } else {
        mDXY.setY( Shift::quantize<5>( mDXY.y() ) );
    }
}

