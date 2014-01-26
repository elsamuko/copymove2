#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <vector>
#include <iostream>

class Block {
public:
    enum { size = 16 };
    Block();

    void dct();
    void idct();

    bool operator <( const Block& b ) const;
    bool operator >( const Block& b ) const;
    std::vector<double>& operator[]( const size_t pos );
    const std::vector<double>& operator[]( const size_t pos ) const;

    friend std::ostream& operator << ( std::ostream& stream, const Block& b );

    std::vector<std::vector<double>> data() const;
    void setData( const std::vector<std::vector<double>>& data );

    bool transformed() const;

    template< int digits = 4 >
    static double roundBy( double in );

private:
    bool mTransformed;
    std::vector<std::vector<double>> mData;
};

constexpr int power10( int digits ) {
    return digits > 0 ? 10 * power10( digits - 1 ) : 1;
}

template< int digits >
double Block::roundBy( double in ) {
    in *= power10( digits );
    in = (int) in;
    in /= power10( digits );
    return in;
}


#endif // BLOCK_HPP
