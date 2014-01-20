
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
};

Block_test::Block_test() {
}

void Block_test::testComparison() {

    Block b1;
    Block b2;

    b1[0][0] = 3;
    b2[0][0] = 4;

    QVERIFY( b2 > b1 );

    b2[0][0] = 2;
    QVERIFY( b2 < b1 );
}

QTEST_MAIN( Block_test )

#include "block_test.moc"
