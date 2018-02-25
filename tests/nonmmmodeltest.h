#ifndef HMMMODELTEST_H_INCLUDED
#define HMMMODELTEST_H_INCLUDED

#include <cppunit/extensions/HelperMacros.h>

#include "NonMarkovModel.h"

class NonMMModelTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(NonMMModelTest);
    CPPUNIT_TEST(testPSATInstance);
    CPPUNIT_TEST(testSolveOPSATInstance);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testPSATInstance();
    void testSolveOPSATInstance();
};

#endif // HMMMODELTEST_H_INCLUDED
