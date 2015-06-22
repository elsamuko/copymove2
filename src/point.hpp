#pragma once

#include <cmath>
#include <sstream>
#include <iomanip>

template<class T>
class Point {
    public:
        Point( T x = 0, T y = 0 ) : mX(x), mY(y) {}
        T x() const { return mX; }
        T y() const { return mY; }
        void set( const T x, const T y ) { mX = x; mY = y; }
        void setX( const T x ) { mX = x; }
        void setY( const T y ) { mY = y; }

        template<class U>
        friend std::ostream& operator <<( std::ostream& stream, const Point<U>& b );

        std::string toString() const;

        T distance( const Point<T>& p ) const {
            T dx = mX - p.x();
            T dy = mY - p.y();
            T distance = std::sqrt( dx*dx + dy*dy );
            return distance;
        }

    private:
        T mX;
        T mY;
};

template<class U>
std::ostream& operator <<(std::ostream &stream, const Point<U> &b) {

    stream << "["     << std::setw( 4 ) << std::round( b.mX );
    stream << ", "    << std::setw( 4 ) << std::round( b.mY );
    stream << "]";

    return stream;
}

template<class T>
std::string Point<T>::toString() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

using PointD = Point<double>;
using PointF = Point<float>;
using PointI = Point<float>;
