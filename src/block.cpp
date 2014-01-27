#include "block.hpp"

#include <cassert>

#include "ooura/shrtdct.hpp"

Block::Block() {
    mTransformed = false;
    mQuality = 10;
    mData = std::vector<std::vector<double>>( Block::size, std::vector<double>( Block::size, 0 ) );
    mFrequencies = std::vector<int>( Block::frequencies, 0 );
}

void Block::dct() {

    mTransformed = true;
    ooura::ddct16x16s( -1, mData );

    int x = 0;
    int y = 0;

    // order of the first ten frequencies:
    //  1  2  6  7
    //  3  5  8
    //  4  9
    // 10
    static int frequency_order[Block::frequencies][2] = {
        {0, 0}, {0, 1}, {1, 0}, {2, 0}, {1, 1},
        {0, 2}, {0, 3}, {1, 2}, {2, 1}, {3, 0}
    };

    // get first 10 frequencies and cache them in mFrequencies
    for( int i = 0; i < Block::frequencies; ++i ) {
        x = frequency_order[i][0];
        y = frequency_order[i][1];
        mFrequencies[i] = this->mData[x][y]/mQuality;
    }

}

void Block::idct() {
    ooura::ddct16x16s( 1, mData );
    mTransformed = false;
}

bool Block::operator <( const Block& that ) const {

    // check only frequencies 2-10
    for( int i = 1; i < Block::frequencies; ++i ) {

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

std::ostream& operator<< ( std::ostream& stream, const Block& b ) {
    for( auto& v : b.mData ) {
        for( auto& d : v ) {
            stream.width( 10 );
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

