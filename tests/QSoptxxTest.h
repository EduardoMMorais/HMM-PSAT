#ifndef QSOPTXXTEST_H
#define QSOPTXXTEST_H

#include <cppunit/extensions/HelperMacros.h>

class QSoptxxTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(QSoptxxTest);
    CPPUNIT_TEST(testSimpleLP);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testSimpleLP();
};

#endif // QSOPTXXTEST_H
