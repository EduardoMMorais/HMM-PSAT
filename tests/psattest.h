#ifndef PSATTEST_H
#define PSATTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "psat.h"

class PSATTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(PSATTest);
    CPPUNIT_TEST(testMoveable);
    CPPUNIT_TEST(testEqualOpClause);
    CPPUNIT_TEST(testEqualOpClausalFormula);
    CPPUNIT_TEST(testEqualOpConditionalFormula);
    CPPUNIT_TEST(testEqualOpPSATInstance);
    CPPUNIT_TEST(testDIMACSConversionNoChange);
    CPPUNIT_TEST(testDIMACSConversionNegated);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testMoveable();
    void testEqualOpClause();
    void testEqualOpClausalFormula();
    void testEqualOpConditionalFormula();
    void testEqualOpPSATInstance();
    void testDIMACSConversionNoChange();
    void testDIMACSConversionNegated();
};

#endif // PSATTEST_H
