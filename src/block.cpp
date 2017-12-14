#include "block.hpp"

#include <sstream>
#include <array>
#include <limits>

#include "ooura/shrtdct.hpp"
#include "log/log.hpp"
#include "scopeguard.hpp"

Block::Block( float color, size_t quality, bool initializeData ) {
    mTransformed = false;
    mMeanCalculated = false;
    mDataCleared = false;
    mDataReceived = false;
    mInterestingCalculated = false;
    mMean = 0.f;
    mStandardDeviation = 0.f;
    mQuantization = 20.f;

    if( initializeData ) {
        initData( color );
    }

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
    const static int frequency_order[Block::frequencies][2] = {
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
    assert( !mDataCleared );
    STATE_CHECK( mTransformed );

    ooura::ddct<Block::size>( -1, mData );
    assignFrequencies();
}

void Block::idct() {
    assert( !mDataCleared );
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
    assert( !mDataCleared );
    mDataReceived = true;
    return mData[pos];
}

const std::vector<float>& Block::operator[]( const size_t pos ) const {
    assert( !mDataCleared );
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
    stream << std::round( b.mStandardDeviation ) << " {";

    for( auto& f : b.mFrequencies ) {
        stream << Block::roundBy<0>( f ) << ", ";
    }

    // remove last ", "
    long pos = stream.tellp();
    stream.seekp( pos - 2 );

    stream << "}" << std::endl;

    return stream;
}

std::string Block::toString() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

std::vector<std::vector<float> > Block::data() const {
    assert( !mDataCleared );
    return mData;
}

//! \brief Set image data
void Block::setData( const std::vector<std::vector<float> >& data ) {
    assert( data.size() == Block::size );
    assert( data[0].size() == Block::size );
    mData = data;
    mTransformed = false;
    mDataCleared = false;
}

void Block::initData( float color ) {
    mData = std::vector<std::vector<float>>( Block::size, std::vector<float>( Block::size, color ) );
}

void Block::clearData() {
    STATE_CHECK( mDataCleared );
    mData.clear();
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

std::vector<uint8_t> Block::grey() {
    assert( !mData.empty() );

    std::vector<std::vector<float>> logarithmic = mData;

    for( int y = 0; y < Block::size; ++y ) {
        for( int x = 0; x < Block::size; ++x ) {
            logarithmic[x][y] = logf( mData[x][y] );
        }
    }

    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();

    for( int y = 0; y < Block::size; ++y ) {
        for( int x = 0; x < Block::size; ++x ) {
            min = std::min( min, logarithmic[x][y] );
            max = std::max( max, logarithmic[x][y] );
        }
    }

    float scale = 255.f/(max-min);


    std::vector<uint8_t> grey( Block::size * Block::size, 0 );

    for( int y = 0; y < Block::size; ++y ) {
        for( int x = 0; x < Block::size; ++x ) {
            grey[y * Block::size + x] = scale * (logarithmic[x][y]-min);
        }
    }

    return grey;
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

// debug function
float Block::cosineSimilarity( const Block& other ) {
    float similarity = Block::cosineSimilarity( this->mFrequencies, other.mFrequencies, this->mFrequencyNorm, other.mFrequencyNorm );
    return similarity;
}

bool Block::transformed() const {
    return mTransformed;
}

bool Block::interesting() const {
    assert( mInterestingCalculated );
    return mInteresting;
}

void Block::calculateStandardDeviation() {
    STATE_CHECK( mMeanCalculated );
    assert( mDataReceived );
    assert( !mDataCleared );

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

    // block is 'interesting' if it's not flat
    STATE_CHECK( mInterestingCalculated );
    float tmp = std::max( 1.f, mMean * 0.1f );
    mInteresting = mStandardDeviation > tmp;
}


bool Block::hasSimilarFreqs( const Block& other ) {
    assert( mTransformed );

    float similarity = cosineSimilarity( this->mFrequencies, other.mFrequencies, this->mFrequencyNorm, other.mFrequencyNorm );
    bool similar = similarity > mQuality;
    return similar;
}
