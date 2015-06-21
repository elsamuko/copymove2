#pragma once

#include <vector>
#include <iostream>
#include <cmath>
#include <cassert>

#include "log/log.hpp"

class Block {
public:
    enum { size = 16, frequencies = 9 };
    explicit Block( float color = 0.f );

    void dct();
    void idct();

    bool operator <( const Block& b ) const;
    bool operator >( const Block& b ) const;
    std::vector<float>& operator[]( const size_t pos );
    const std::vector<float>& operator[]( const size_t pos ) const;

    friend std::ostream& operator << ( std::ostream& stream, const Block& b );
    std::string toString() const;

    std::vector<std::vector<float>> data() const;
    void setData( const std::vector<std::vector<float>>& data );

    void setX( int x );
    int x() const;
    void setY( int y );
    int y() const;

    int frequency( size_t position ) const;

    int manhattanDistance( const Block& that ) const;

    template<class T>
    static float cosineSimilarity( const T& a, const T&b, float normA, float normB );

    template<int from = 0, int to = 4>
    bool hasSimilarFreqs( const Block& other );

    bool transformed() const;
    bool interesting() const;
    void calculateStandardDeviation();

    template< int digits = 4 >
    static float roundBy( float in );

private:
    // get freq values from dct'd block
    void assignFrequencies();

    // state checks
    bool mDataReceived;
    bool mTransformed;
    bool mMeanCalculated;

    // quant steps of freqs
    float mQuantization;

    // arithm average of block ~ dct[0][0]
    float mMean;
    // stddev of block ~ contrast
    float mStandardDeviation; // contrast

    // precalculated norm for cosine similarity
    float mFrequencyNorm; // ||a||

    // x/y position in image
    int mX;
    int mY;

    // data at x/y
    std::vector<std::vector<float>> mData;
    std::vector<int> mFrequencies;
};

template<class T>
float Block::cosineSimilarity( const T& a, const T& b, float normA, float normB ) {

    assert( a.size() == b.size() );
    if( normA <= 0 ) return 0.f;
    if( normB <= 0 ) return 0.f;

    float dotP = 0;

    for( size_t i = 0; i < a.size(); ++i ) {
        dotP += a[i] * b[i];
    }

    float similarity = dotP/( normA * normB );
    return similarity;
}

// TODO: replace with cosine similarity
template<int from, int to>
bool Block::hasSimilarFreqs( const Block &other ) {
    assert( mTransformed );

    float similarity = cosineSimilarity( this->mFrequencies, other.mFrequencies, this->mFrequencyNorm, other.mFrequencyNorm );
    bool similar = similarity > 0.97;
    return similar;
}

constexpr int power10( int digits ) {
    return digits > 0 ? 10 * power10( digits - 1 ) : 1;
}

template< int digits >
float Block::roundBy( float in ) {
    in *= power10( digits );
    in = round( in );
    in /= power10( digits );
    return in;
}
