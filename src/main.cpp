
#include <iostream>
#include <vector>

#include "block.hpp"
#include "fdimage.hpp"

int main( int /*argc*/, char** /*argv*/ ) {

    FDImage image;
    image.load( "cat.jpg" );


    Block b1;
    image.getBlock( b1, 222, 208);

    b1.dct();


    for( int i = 0; i< Block::size; ++i ) {
        for( int j = 0; j< Block::size; ++j ) {
            std::cout << b1[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

