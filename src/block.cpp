#include "block.hpp"

#include <cassert>
#include <sstream>
#include <array>

#include "ooura/shrtdct.hpp"

Block::Block() {
    mTransformed = false;
    mQuality = 20;
    mData = std::vector<std::vector<double>>( Block::size, std::vector<double>( Block::size, 0 ) );
    mFrequencies = std::vector<int>( Block::frequencies, 0 );
    mX = 0;
    mY = 0;
}

void Block::assignFrequencies() {
    // order of the used frequencies:
    //  X  X  X  X
    //  X  1  3  9
    //  X  2  4  6
    //  X  8  5  7
    static int frequency_order[Block::frequencies][2] = {
        {1, 1}, // 1
        {1, 2}, // 2
        {2, 1}, // 3
        {2, 2}, // 4
        {2, 3}, // 5
        {3, 2}, // 6
        {3, 3}, // 7
        {1, 3}, // 8
        {3, 1}, // 9
    };

    int x = 0;
    int y = 0;
    double amp = 0;
    double max = 0.01;

    std::array<double,Block::frequencies> tmp;

    // get first 10 frequencies and cache them in mFrequencies
    for( int i = 0; i < Block::frequencies; ++i ) {
        x = frequency_order[i][0];
        y = frequency_order[i][1];
        amp = this->mData[x][y];
        max = std::max( max, std::abs( amp ) );
        tmp[i] = amp;
    }

    for( int i = 0; i < Block::frequencies; ++i ) {
        mFrequencies[i] = std::round( tmp[i] * 8.f / max );
    }
}

void Block::dct() {
    assert( !mTransformed );
    mTransformed = true;
    ooura::ddct8x8s( -1, mData );

    assignFrequencies();
}

void Block::idct() {
    assert( mTransformed );
    mTransformed = false;
    ooura::ddct8x8s( 1, mData );
}

bool Block::operator <( const Block& that ) const {

    for( int i = 0; i < Block::frequencies; ++i ) {

        if( this->mFrequencies[i] < that.mFrequencies[i] ) {
            return true;
        }

        if( this->mFrequencies[i] > that.mFrequencies[i] ) {
            return false;
        }
    }

    return false;
}

bool Block::operator >( const Block& b ) const {
    return !( *this < b );
}

std::vector<double>& Block::operator[]( const size_t pos ) {
    return mData[pos];
}

const std::vector<double>& Block::operator[]( const size_t pos ) const {
    return mData[pos];
}

std::string Block::toString() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

std::ostream& operator<< ( std::ostream& stream, const Block& b ) {
    stream << "[" ;

    stream.width( 3 );
    stream << b.x() << ",";

    stream.width( 3 );
    stream << b.y() << "] ";

    for( auto& f : b.mFrequencies ) {
        stream.width( 5  );
        stream << Block::roundBy<0>( f ) << " ";
    }

    stream << std::endl;

//    for( auto & v : b.mData ) {
//        for( auto & d : v ) {
//            stream.width( 10 );
//            stream << Block::roundBy( d ) << " ";
//        }

//        stream << std::endl;
//    }

    return stream;
}

std::vector<std::vector<double> > Block::data() const {
    return mData;
}

//! \brief Set image data
void Block::setData( const std::vector<std::vector<double> >& data ) {
    assert( data.size() == Block::size );
    assert( data[0].size() == Block::size );
    mData = data;
    mTransformed = false;
}

void Block::setX( int x ) {
    mX = x;
}

int Block::x() const {
    return mX;
}

void Block::setY( int y ) {
    mY = y;
}

int Block::y() const {
    return mY;
}

int Block::frequency( size_t position ) const {
    assert( mTransformed );
    assert( position < mFrequencies.size() );
    return mFrequencies[position];
}

int Block::manhattanDistance( const Block& that ) const {
    int dx = std::abs( this->x() - that.x() );
    int dy = std::abs( this->y() - that.y() );
    return dx + dy;
}

bool Block::transformed() const {
    return mTransformed;
}

bool Block::interesting() const {
    return mInteresting;
}
