
#include <iostream>
#include <vector>

#include "block.hpp"

int main( int /*argc*/, char** /*argv*/ ) {

    std::vector<std::vector<double> > d1 = std::vector<std::vector<double>>( 16, std::vector<double>( 16 ) );
    std::vector<std::vector<double> > d2 = std::vector<std::vector<double>>( 16, std::vector<double>( 16 ) );

    d1[0][0] = 3;
    d2[0][0] = 4;

    Block b1;
    b1.setData( d1 );
    Block b2;
    b2.setData( d2 );

    std::cout << ( b2 < b1 ) << std::endl;

    return 0;
}

