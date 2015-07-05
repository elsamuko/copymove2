
#include <QString>
#include <QtTest>

#include<fstream>

#include "point.hpp"
#include "log/log.hpp"

class Point_test : public QObject {
        Q_OBJECT

    public:
        Point_test();

    private Q_SLOTS:
        void testMinus();
        void testPlus();
        void testMulti();
};

Point_test::Point_test() {
}

void Point_test::testMinus() {
    PointI p1( 3, 4 );
    QVERIFY( p1.x() == 3 );
    QVERIFY( p1.y() == 4 );

    PointI p2( 5, 1 );
    PointI p3 = p1 - p2;
    QVERIFY( p3.x() == -2 );
    QVERIFY( p3.y() == 3 );
}

void Point_test::testPlus() {
    PointI p1( 3, 4 );
    QVERIFY( p1.x() == 3 );
    QVERIFY( p1.y() == 4 );

    PointI p2( 5, 1 );
    PointI p3 = p1 + p2;
    QVERIFY( p3.x() == 8 );
    QVERIFY( p3.y() == 5 );
}

void Point_test::testMulti() {
    PointI p1( 3, 4 );
    QVERIFY( p1.x() == 3 );
    QVERIFY( p1.y() == 4 );

    PointI p3 = p1 * 4;
    QVERIFY( p3.x() == 12 );
    QVERIFY( p3.y() == 16 );

    PointI p4 = p1 * p3;
    QVERIFY( p4.x() == 36 );
    QVERIFY( p4.y() == 64 );
}

QTEST_MAIN( Point_test )

#include "point_test.moc"
