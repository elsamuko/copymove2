
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
    std::vector<std::vector<double> > d1 = std::vector<std::vector<double>>( 16, std::vector<double>( 16 ) );
    std::vector<std::vector<double> > d2 = std::vector<std::vector<double>>( 16, std::vector<double>( 16 ) );

    d1[0][0] = 3;
    d2[0][0] = 4;

    Block b1;
    b1.setData( d1 );
    Block b2;
    b2.setData( d2 );

    QVERIFY( b2 > b1 );

    d2[0][0] = 2;
    b2.setData( d2 );
    QVERIFY( b2 < b1 );
}


QTEST_MAIN( Block_test )

#include "block_test.moc"
