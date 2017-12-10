
#include <QString>
#include <QtTest>

#include "block.hpp"
#include "ioimage.hpp"
#include "greyimage.hpp"
#include "log/log.hpp"

class Detection_test : public QObject {
        Q_OBJECT

    public:
        Detection_test();

    private Q_SLOTS:
        void testSimilar();
};

Detection_test::Detection_test() {
}

void Detection_test::testSimilar() {

    GreyImage greyA = IOImage( "sample_1A.jpg" ).getGrey();
    GreyImage greyB = IOImage( "sample_1B.jpg" ).getGrey();

    Block blockA;
    Block blockB;

    greyA.getBlock( blockA, 0, 0 );
    greyB.getBlock( blockB, 0, 0 );

    blockA.dct();
    blockB.dct();

    blockA.calculateStandardDeviation();
    blockB.calculateStandardDeviation();

    LOG( blockA.toString() );
    LOG( blockB.toString() );

    bool isSimilar = blockA.hasSimilarFreqs( blockB );
    QVERIFY( isSimilar );
}

QTEST_MAIN( Detection_test )

#include "detection_test.moc"
