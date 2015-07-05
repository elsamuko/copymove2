
#include <QString>
#include <QtTest>
#include <QFileInfo>

#include<fstream>

#include "block.hpp"
#include "sample.hpp"
#include "ioimage.hpp"
#include "greyimage.hpp"
#include "log/log.hpp"

class Block_test : public QObject {
        Q_OBJECT

    public:
        Block_test();

    private Q_SLOTS:
        void testComparison();
        void testDCT();
        void testSort();
        void testOctave();
};

Block_test::Block_test() {
}

void Block_test::testComparison() {

    Block b1;
    Block b2;

    b2[0][0] = 65536;
    b2.dct();

    QVERIFY( b1 < b2 );
}

void Block_test::testDCT() {

    Block b1;
    b1[2][1] = 1;
    b1.dct();

    // compare with Octave:
    if( Block::size == 8 ) {
        // clear; data=zeros(8,8); data(2,3)=1; dct2(data)
        QVERIFY( Block::roundBy<7>( b1[2][1] ) == -0.0795474 );
        QVERIFY( Block::roundBy<7>( b1[2][2] ) == -0.0366117 );
        QVERIFY( Block::roundBy<7>( b1[2][3] ) == 0.0186645 );
        QVERIFY( Block::roundBy<7>( b1[7][7] ) == -0.1154849 );
    } else if( Block::size == 16 ) {
        // clear; data=zeros(16,16); data(2,3)=1; dct2(data)
        QVERIFY2( Block::roundBy<7>( b1[2][1] ) == 0.0664559f, std::to_string( Block::roundBy<7>( b1[2][1] ) ).c_str() );
        QVERIFY( Block::roundBy<7>( b1[2][2] ) == 0.0577425f );
        QVERIFY( Block::roundBy<7>( b1[2][3] ) == 0.0440563f );
        QVERIFY( Block::roundBy<7>( b1[15][15] ) == -0.0171049f );
    }

    b1.idct();
    QVERIFY( Block::roundBy<7>( b1[2][1] ) == 1 );
    QVERIFY( Block::roundBy<7>( b1[2][2] ) == 0 );
}

void Block_test::testSort() {

    std::vector<Block> blocks( 3 );
    blocks[0][0][0] = 65536;
    blocks[0].setX( 4 );

    for( Block & b : blocks ) {
        b.dct();
    }

    std::sort( blocks.begin(), blocks.end() );

    QVERIFY( blocks[2].x() == 4 );

}

void Block_test::testOctave() {
    std::ofstream file( "sample.png", std::ofstream::binary );

    if( file.is_open() ) {
        file.write( ( const char* ) sample_png, sizeof( sample_png ) );
        file.close();
    }

    IOImage image;
    image.load( "sample.png" );
    QVERIFY( ! image.isNull() );

    GreyImage grey = image.getGrey();
    Block b;
    grey.getBlock( b, 0, 0 );
    b.calculateStandardDeviation();

    LOG( b.toString() );
    b.dct();
    LOG( b.toString() );

    if( Block::size == 8 ) {
        // -20    2  -17   -3   -8    0   -6   13   12
        QVERIFY( b.frequency( 0 ) == -20 );
        QVERIFY( b.frequency( 1 ) ==   2 );
        QVERIFY( b.frequency( 2 ) == -17 );
        QVERIFY( b.frequency( 3 ) ==  -3 );
        QVERIFY( b.frequency( 4 ) ==  -8 );
        QVERIFY( b.frequency( 5 ) ==   0 );
        QVERIFY( b.frequency( 6 ) ==  -6 );
        QVERIFY( b.frequency( 7 ) ==  13 );
        QVERIFY( b.frequency( 8 ) ==  12 );
    } else if( Block::size == 16 ) {
        // 11  -20    1  -12    1   -7    3    3   -9
        QVERIFY( b.frequency( 0 ) ==  11 );
        QVERIFY( b.frequency( 1 ) == -20 );
        QVERIFY( b.frequency( 2 ) ==   1 );
        QVERIFY( b.frequency( 3 ) == -12 );
        QVERIFY( b.frequency( 4 ) ==   1 );
        QVERIFY( b.frequency( 5 ) ==  -7 );
        QVERIFY( b.frequency( 6 ) ==   3 );
        QVERIFY( b.frequency( 7 ) ==   3 );
        QVERIFY( b.frequency( 8 ) ==  -9 );
    }
}


QTEST_MAIN( Block_test )

#include "block_test.moc"
