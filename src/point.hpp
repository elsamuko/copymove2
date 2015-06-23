#pragma once

#include <cmath>
#include <sstream>
#include <iomanip>

template<class T>
class Point {
    public:
        Point( T x = T(), T y = T() ) : mX( x ), mY( y ) {}
        template<class U>
        Point( Point<U> that ) : mX( that.x() ), mY( that.y() ) {}
        T x() const { return mX; }
        T y() const { return mY; }
        void set( const T x, const T y ) { mX = x; mY = y; }
        void setX( const T x ) { mX = x; }
        void setY( const T y ) { mY = y; }

        template<class U>
        friend std::ostream& operator <<( std::ostream& stream, const Point<U>& b );

        std::string toString() const;

        T abs() const {
            return std::sqrt( sqr() );
        }

        T sqr() const {
            return mX*mX + mY*mY;
        }

        template<class U>
        Point& operator+= ( const Point<U>& that ) {
            this->mX += that.x();
            this->mY += that.y();
            return *this;
        }

        template<class U>
        Point& operator-= ( const Point<U>& that ) {
            this->mX -= that.x();
            this->mY -= that.y();
            return *this;
        }

        template<class U>
        Point& operator*= ( const Point<U>& that ) {
            this->mX *= that.x();
            this->mY *= that.y();
            return *this;
        }

        template<class U>
        Point& operator*= ( const U in ) {
            this->mX *= in;
            this->mY *= in;
            return *this;
        }

        template<class U>
        Point& operator/= ( const U in ) {
            this->mX /= in;
            this->mY /= in;
            return *this;
        }

        T distance( const Point<T>& that ) const {
            T dx = mX - that.mX;
            T dy = mY - that.mY;
            T distance = std::sqrt( dx*dx + dy*dy );
            return distance;
        }

    private:
        T mX;
        T mY;
};

template<class T, class U>
inline Point<T> operator+( const Point<T>& a, const Point<U>& b ) {
    Point<T> result( a );
    result += b;
    return result;
}

template<class T, class U>
inline Point<T> operator-( const Point<T>& a, const Point<U>& b ) {
    Point<T> result( a );
    result -= b;
    return result;
}

template<class T, class U>
inline Point<T> operator*( const Point<T>& a, const Point<U>& b )  {
    Point<T> result( a );
    result *= b;
    return result;
}

template<class T, class U>
inline Point<T> operator*( const Point<T>& a,  const U b )  {
    Point<T> result( a );
    result *= b;
    return result;
}

template<class T, class U>
inline Point<T> operator*( const U a, const Point<T>& b )  {
    Point<T> result( b );
    result *= a;
    return result;
}

template<class T, class U>
inline Point<T> operator/( const Point<T>& a, const Point<T>& b ) {
    return ( a * ( 1 / b ) );
}

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

using PointF = Point<float>;
using PointI = Point<int>;
