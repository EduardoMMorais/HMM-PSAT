#ifndef APSATTEST_H
#define APSATTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "APSATSolver.h"

class APSATTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(APSATTest);
    CPPUNIT_TEST(testPSATSatisfiableInstance);
    CPPUNIT_TEST(testPSATUnsatisfiableInstance);
    CPPUNIT_TEST(testPSATConditionalProbDef);
    CPPUNIT_TEST(testPSATConditionalProbUndef);
    CPPUNIT_TEST(testPSATUnsatWithErrors);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testPSATSatisfiableInstance();
    void testPSATUnsatisfiableInstance();
    void testPSATConditionalProbDef();
    void testPSATConditionalProbUndef();
    void testPSATUnsatWithErrors();
    APSATSolver* PSatInst;
};

#endif // APSATTEST_H
