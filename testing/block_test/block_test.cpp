
#include <QString>
#include <QtTest>
#include <QFileInfo>

#include "block.hpp"

class Block_test : public QObject {
    Q_OBJECT

public:
    Block_test();

private Q_SLOTS:
    void testComparison();
    void testDCT();
};

Block_test::Block_test() {
}

void Block_test::testComparison() {

    Block b1;
    Block b2;

    b1[1][0] = 3;
    b2[1][0] = 4;

    QVERIFY( b2 > b1 );

    b2[1][0] = 2;
    QVERIFY( b2 < b1 );
}

void Block_test::testDCT() {

    Block b1;
    b1[2][1] = 1;
    b1.dct();

    // compare with Octave:
    // clear; data=zeros(16,16); data(2,3)=1; dct2(data)
    QVERIFY( Block::roundBy<7>( b1[2][1] ) == 0.0664559 );
    QVERIFY( Block::roundBy<7>( b1[2][2] ) == 0.0577425 );
    QVERIFY( Block::roundBy<7>( b1[2][3] ) == 0.0440563 );
    QVERIFY( Block::roundBy<7>( b1[15][15] ) == -0.0171049 );
}

QTEST_MAIN( Block_test )

#include "block_test.moc"
