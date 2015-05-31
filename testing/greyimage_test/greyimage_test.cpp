
#include <QString>
#include <QtTest>

#include "greyimage.hpp"

class GreyImage_test : public QObject {
    Q_OBJECT

public:
    GreyImage_test();

private Q_SLOTS:
    void testReadFromGrey();
};

GreyImage_test::GreyImage_test() {
}

void GreyImage_test::testReadFromGrey() {

    Block block;
    GreyImage grey( 20, 10 );

    //  X  X  X  X
    //  X  X  1  2
    //  X  X  3  4
    grey[2][1] = 1;
    grey[2][2] = 2;
    grey[3][1] = 3;
    grey[3][2] = 4;

    grey.getBlock( block, 2, 1 );

    QVERIFY( block[0][0] == 1 );
    QVERIFY( block[0][1] == 2 );
    QVERIFY( block[1][0] == 3 );
    QVERIFY( block[1][1] == 4 );
}


QTEST_MAIN( GreyImage_test )

#include "greyimage_test.moc"
