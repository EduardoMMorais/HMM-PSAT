#ifndef HMMMODELTEST_H_INCLUDED
#define HMMMODELTEST_H_INCLUDED

#include <cppunit/extensions/HelperMacros.h>

#include "hmmmodel.h"

class HMMModelTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(HMMModelTest);
    CPPUNIT_TEST(testPSATInstance);
    //CPPUNIT_TEST(testPSATInstance1);
    CPPUNIT_TEST(testSolvePSATInstace);
    //CPPUNIT_TEST(testSolvePureHMMInstace);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testPSATInstance();
    //void testPSATInstance1();
    void testSolvePSATInstace();
};

#endif // HMMMODELTEST_H_INCLUDED
