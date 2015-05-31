#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <vector>
#include <iostream>
#include <cmath>
#include <cassert>

class Block {
public:
    enum { size = 8, frequencies = 9 };
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

    int frequency( size_t position ) const;

    int manhattanDistance( const Block& that ) const;

    template<class T>
    static double cosineSimilarity( const T& a, const T&b );

    template<int from = 0, int to = 4>
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

template<class T>
double Block::cosineSimilarity( const T& a, const T& b ) {

    assert( a.size() == b.size() );

    double dotP = 0;
    double sumA = 0;
    double sumB = 0;

    for( size_t i = 0; i < a.size(); ++i ) {
        dotP += a[i] * b[i];
        sumA += a[i] * a[i];
        sumB += b[i] * b[i];
    }

    double similarity = dotP/( std::sqrt( sumA ) * std::sqrt( sumB ) );
    return similarity;
}

// TODO: replace with cosine similarity
template<int from, int to>
bool Block::hasSimilarFreqs( Block &other ) {
    assert( mTransformed );

    bool similar = cosineSimilarity( this->mFrequencies, other.mFrequencies ) > 0.9;
    return similar;
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
