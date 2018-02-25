#include "QSoptxxTest.h"
#include "QSoptxx.h"

CPPUNIT_TEST_SUITE_REGISTRATION(QSoptxxTest);

#ifndef makeRational
#define makeRational(NUM,DEN) mpq_class((NUM),(DEN))
#endif

void QSoptxxTest::setUp() {
}
void QSoptxxTest::tearDown() {
}
void QSoptxxTest::testSimpleLP() {
    /*
    Primal
    min - 3 x - 2 y - 4 z
    3/2 x +     y + 1/2 z = 6
    5/2 x + 1/2 y         = 5

    Sol: x = 2 y = 0 z = 6

    Dual
    max 6 x + 5 y
    3/2 x + 5/2 y <= -3
        x + 1/2 y <= -2
    1/2 x         <= -4
    Dual: x = -8  y = 18/5
    */
    vector<vector<mpq_class>> columns({{makeRational(3,2),makeRational(5,2)},{makeRational(1,1), makeRational(1,2)},{makeRational(1,2), makeRational(0,1)}});
    vector<int> cost({-3,-2,-4});
    vector<mpq_class> rhs({makeRational(6,1),makeRational(5,1)});
    QSoptxx qs(columns, cost, rhs);
    int result = qs.solve();
    CPPUNIT_ASSERT(result==QS_LP_OPTIMAL);
    CPPUNIT_ASSERT(qs.getOptimalCost() == -30);

    vector<mpq_class> primal, dual;
    qs.getPrimalSolution(primal);
    qs.getDualSolution(dual);
    CPPUNIT_ASSERT(primal.size()==3);
    CPPUNIT_ASSERT(primal[0]==2);
    CPPUNIT_ASSERT(primal[1]==0);
    CPPUNIT_ASSERT(primal[2]==6);

    CPPUNIT_ASSERT(dual.size()==2);
    CPPUNIT_ASSERT(dual[0]==-8);
    CPPUNIT_ASSERT(dual[1]==makeRational(18,5));
}
