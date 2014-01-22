#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <vector>

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

    std::vector<std::vector<double>> data() const;
    void setData( const std::vector<std::vector<double>>& data );

    bool transformed() const;

private:
    bool mTransformed;
    std::vector<std::vector<double>> mData;
};


#endif // BLOCK_HPP
