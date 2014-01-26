
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
    b1[1][2] = 1;
    b1.dct();

    double fromOoura = b1[1][2];
    double fromOctave = 0.0664559;

    QVERIFY( Block::roundBy<7>( fromOoura ) == fromOctave );
}

QTEST_MAIN( Block_test )

#include "block_test.moc"
