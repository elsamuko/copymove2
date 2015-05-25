#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <vector>
#include <iostream>
#include <cmath>

class Block {
public:
    enum { size = 16, frequencies = 10 };
    Block();

    void dct();
    void idct();

    bool operator <( const Block& b ) const;
    bool operator >( const Block& b ) const;
    std::vector<double>& operator[]( const size_t pos );
    const std::vector<double>& operator[]( const size_t pos ) const;

    friend std::ostream& operator << ( std::ostream& stream, const Block& b );
    std::string toString() const;

    std::vector<std::vector<double>> data() const;
    void setData( const std::vector<std::vector<double>>& data );

    void setX( int x );
    int x() const;
    void setY( int y );
    int y() const;

    int manhattanDistance( const Block& that ) const;

    template<int from = 1, int to = 5>
    bool hasSimilarFreqs( Block& other );

    bool transformed() const;
    bool interesting() const;

    template< int digits = 4 >
    static double roundBy( double in );

private:
    void assignFrequencies();
    bool mTransformed;
    int mQuality;
    bool mInteresting;
    //! \brief X coordinate in image
    int mX;
    //! \brief Y coordinate in image
    int mY;
    std::vector<std::vector<double>> mData;
    std::vector<int> mFrequencies;
};

template<int from, int to>
bool Block::hasSimilarFreqs( Block &other ) {
    for( int i = from; i <= to; ++i ) {
        if( this->mFrequencies[i] != other.mFrequencies[i] ) {
            return false;
        }
    }
    return true;
}

constexpr int power10( int digits ) {
    return digits > 0 ? 10 * power10( digits - 1 ) : 1;
}

template< int digits >
double Block::roundBy( double in ) {
    in *= power10( digits );
    in = round( in );
    in /= power10( digits );
    return in;
}


#endif // BLOCK_HPP
