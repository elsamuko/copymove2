#include "block.hpp"

#include <cassert>

#include "ooura/shrtdct.hpp"

Block::Block() {
    mTransformed = false;
    mData = std::vector<std::vector<double>>( Block::size, std::vector<double>( Block::size, 0 ) );
}

void Block::dct() {
    ooura::ddct16x16s( 1, mData );
    mTransformed = true;
}

void Block::idct() {
    ooura::ddct16x16s( -1, mData );
    mTransformed = false;
}

bool Block::operator <( const Block& that ) const {
    bool b = this->mData[1][0] < that.mData[1][0];
    return b;
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

std::ostream& operator<< ( std::ostream& stream, const Block& b ) {
    for( auto& v : b.mData ) {
        for( auto& d : v ) {
            stream.width( 7 );
            stream << Block::roundBy( d ) << " ";
        }
        stream << std::endl;
    }
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

bool Block::transformed() const {
    return mTransformed;
}

