
#include <QString>
#include <QtTest>
#include <QFileInfo>

#include<fstream>

#include "block.hpp"
#include "sample.hpp"
#include "fdimage.hpp"
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
    // clear; data=zeros(8,8); data(2,3)=1; dct2(data)
    QVERIFY( Block::roundBy<7>( b1[2][1] ) == -0.0795474 );
    QVERIFY( Block::roundBy<7>( b1[2][2] ) == -0.0366117 );
    QVERIFY( Block::roundBy<7>( b1[2][3] ) == 0.0186645 );
    QVERIFY( Block::roundBy<7>( b1[7][7] ) == -0.1154849 );

    b1.idct();
    QVERIFY( Block::roundBy<7>( b1[2][1] ) == 1 );
    QVERIFY( Block::roundBy<7>( b1[2][2] ) == 0 );
}

void Block_test::testSort() {

    std::vector<Block> blocks( 3 );
    blocks[0][0][0] = 65536;
    blocks[0].setX( 4 );

    for( Block& b : blocks ) {
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

    FDImage image;
    image.load( "sample.png" );
    QVERIFY( ! image.isNull() );

    GreyImage grey = image.getGrey();
    Block b;
    grey.getBlock( b, 0, 0);

    LOG( b.toString() );
    b.dct();
    LOG( b.toString() );

    // -6   0  -1  -4  -3  -8   5  -0   3
    QVERIFY( b.frequency(0) == -6 );
    QVERIFY( b.frequency(1) ==  0 );
    QVERIFY( b.frequency(2) == -1 );
    QVERIFY( b.frequency(3) == -4 );
    QVERIFY( b.frequency(4) == -3 );
    QVERIFY( b.frequency(5) == -8 );
    QVERIFY( b.frequency(6) ==  5 );
    QVERIFY( b.frequency(7) ==  0 );
    QVERIFY( b.frequency(8) ==  3 );
}


QTEST_MAIN( Block_test )

#include "block_test.moc"
