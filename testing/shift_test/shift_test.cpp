
#include <QString>
#include <QtTest>

#include<fstream>

#include "shift.hpp"
#include "log/log.hpp"

class Shift_test : public QObject {
    Q_OBJECT

public:
    Shift_test();

private Q_SLOTS:
    void testMinus();
};

Shift_test::Shift_test() {
}

void Shift_test::testMinus() {
    Shift pos( 3, 4 );
    QVERIFY( pos.dx() == 3 );
    QVERIFY( pos.dy() == 4 );

    pos = -pos;
    QVERIFY( pos.dx() == -3 );
    QVERIFY( pos.dy() == -4 );
}

QTEST_MAIN( Shift_test )

#include "shift_test.moc"
