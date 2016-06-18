
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
    QVERIFY( true );
}

QTEST_MAIN( Detection_test )

#include "detection_test.moc"
