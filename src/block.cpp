#include "block.hpp"

#include <cassert>
#include <sstream>
#include <array>

#include "ooura/shrtdct.hpp"
#include "log/log.hpp"
#include "scopeguard.hpp"

Block::Block( float color , size_t quality ) {
    mTransformed = false;
    mMeanCalculated = false;
    mDataReceived = false;
    mMean = 0.f;
    mStandardDeviation = 0.f;
    mQuantization = 20.f;
    mData = std::vector<std::vector<float>>( Block::size, std::vector<float>( Block::size, color ) );
    mFrequencies = std::vector<int>( Block::frequencies, 0 );
    mFrequencyNorm = 0;
    mPos.set( 0, 0 );
    mQuality = 0.9 + float( quality ) / 100.f;
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

    float max = 0.01;

    std::array<float, Block::frequencies> tmp;

    // get first 10 frequencies and cache them in mFrequencies
    for( int i = 0; i < Block::frequencies; ++i ) {
        int x = frequency_order[i][0];
        int y = frequency_order[i][1];
        float amp = this->mData[x][y];//(1.f + i); // TODO!
        max = std::max( max, std::abs( amp ) );
        tmp[i] = amp;
    }

    for( int i = 0; i < Block::frequencies; ++i ) {
        float f = std::round( tmp[i] * mQuantization / max );
        mFrequencies[i] = f;
        mFrequencyNorm += f * f;
    }

    mFrequencyNorm = std::sqrt( mFrequencyNorm );
}

void Block::dct() {
    STATE_CHECK( mTransformed );

    ooura::ddct<Block::size>( -1, mData );
    assignFrequencies();
}

void Block::idct() {
    assert( mTransformed );
    mTransformed = false;

    ooura::ddct<Block::size>( 1, mData );
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

std::vector<float>& Block::operator[]( const size_t pos ) {
    mDataReceived = true;
    return mData[pos];
}

const std::vector<float>& Block::operator[]( const size_t pos ) const {
    return mData[pos];
}

std::ostream& operator<< ( std::ostream& stream, const Block& b ) {

    if( b.interesting() ) {
        stream << "[+] [";
    } else {
        stream << "[-] [";
    }

    stream.width( 4 );
    stream << b.x() << ",";

    stream.width( 4 );
    stream << b.y() << "] ";


    stream.width( 3 );
    stream << std::round( b.mMean ) << " +- ";

    stream.width( 3 );
    stream << std::round( b.mStandardDeviation ) << " {";

    for( auto & f : b.mFrequencies ) {
        stream.width( 5 );
        stream << Block::roundBy<0>( f ) << ", ";
    }

    stream << " }" << std::endl;

    return stream;
}

std::string Block::toString() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

std::vector<std::vector<float> > Block::data() const {
    return mData;
}

//! \brief Set image data
void Block::setData( const std::vector<std::vector<float> >& data ) {
    assert( data.size() == Block::size );
    assert( data[0].size() == Block::size );
    mData = data;
    mTransformed = false;
}

void Block::setX( int x ) {
    mPos.setX( x );
}

int Block::x() const {
    return mPos.x();
}

void Block::setY( int y ) {
    mPos.setY( y );
}

int Block::y() const {
    return mPos.y();
}

PointI Block::pos() const {
    return mPos;
}

void Block::setPos( const PointI& pos ) {
    mPos = pos;
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
    assert( mMeanCalculated );

    float tmp = std::max( 1.f, mMean * 0.1f );
    return mStandardDeviation > tmp;
}

void Block::calculateStandardDeviation() {
    STATE_CHECK( mMeanCalculated );
    assert( mDataReceived );

    mMean = 0.f;
    mStandardDeviation = 0.f;
    float diff = 0.f;

    for( int y = 0; y < Block::size; ++y ) {
        for( int x = 0; x < Block::size; ++x ) {
            mMean += mData[x][y];
        }
    }

    mMean /= ( float )( Block::size * Block::size );

    for( int y = 0; y < Block::size; ++y ) {
        for( int x = 0; x < Block::size; ++x ) {
            diff = mData[x][y] - mMean;
            mStandardDeviation += diff * diff;
        }
    }

    mStandardDeviation /= ( float )( Block::size * Block::size );
    mStandardDeviation = std::sqrt( mStandardDeviation );
}
