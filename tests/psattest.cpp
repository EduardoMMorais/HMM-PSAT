#include "psattest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(PSATTest);

void PSATTest::setUp() {}
void PSATTest::tearDown() {}
void PSATTest::testMoveable() {
    CPPUNIT_ASSERT(std::is_move_assignable<PSATInstance>());
    CPPUNIT_ASSERT(std::is_move_assignable<ConditionalFormula>());
    CPPUNIT_ASSERT(std::is_move_assignable<Clause>());
}

void PSATTest::testEqualOpClause() {
    Clause c1({1,2,3,5}), c2({5,3,2,1});
    CPPUNIT_ASSERT(c1 == c2);
}

void PSATTest::testEqualOpClausalFormula() {
    ClausalFormula c1({{1,3,5},{2,4,6}}),
        c2({{6,4,2},{5,3,1}});
    CPPUNIT_ASSERT(c1 == c2);
}

void PSATTest::testEqualOpConditionalFormula() {
    ConditionalFormula c1({{1,3,5},{2,4,6}}, {{7,9,-1},{8,-2,-4}}),
        c2({{5,3,1},{6,4,2}}, {{-1,9,7},{-4,-2,8}});
    CPPUNIT_ASSERT(c1 == c2);
}

void PSATTest::testEqualOpPSATInstance() {
    PSATInstance c1({
                    make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({{1,3,5},{2,4,6}}),ClausalFormula({{7,9,-1},{8,-2,-4}})),0.08),
                    make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({{7,9,-1},{8,-2,-4}}),ClausalFormula({{1,3,5},{2,4,6}})),0.7),
                    }),
        c2({
        make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({{7,9,-1},{8,-2,-4}}),ClausalFormula({{1,3,5},{2,4,6}})),0.7),
        make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({{1,3,5},{2,4,6}}),ClausalFormula({{7,9,-1},{8,-2,-4}})),0.08),
        });
    CPPUNIT_ASSERT(c1 == c2);
}

void PSATTest::testDIMACSConversionNoChange() {
    PSATInstance c({
                   make_pair<ConditionalFormula, double>(ConditionalFormula(Clause({1,2})),1),
                   make_pair<ConditionalFormula, double>(ConditionalFormula(Clause({1,3})),1),
                   make_pair<ConditionalFormula, double>(ConditionalFormula(Clause({2,3})),1),
                   make_pair<ConditionalFormula, double>(ConditionalFormula(Clause({1})),0.7),
                   make_pair<ConditionalFormula, double>(ConditionalFormula(Clause({2})),0.7),
                   make_pair<ConditionalFormula, double>(ConditionalFormula(Clause({3})),0.7),
                   });
    double *probs = c.getDimacsProbs();
    CPPUNIT_ASSERT(probs[0] == 1);
    CPPUNIT_ASSERT(probs[1] == 0.7);
    CPPUNIT_ASSERT(probs[2] == 0.7);
    CPPUNIT_ASSERT(probs[3] == 0.7);
    delete[] probs;
    CPPUNIT_ASSERT(c.getDimacsGamma() == ClausalFormula({{1,2},{1,3},{2,3}}));
    CPPUNIT_ASSERT(c.getDimacsExtraColumns().size() == 0);
    CPPUNIT_ASSERT(c.getDimacsNumCl() == 3);
    CPPUNIT_ASSERT(c.getDimacsNumPr() == 3);
    CPPUNIT_ASSERT(c.getDimacsNumVar() == 3);
}

void PSATTest::testDIMACSConversionNegated() {
    PSATInstance c({
                   make_pair<ConditionalFormula, double>(ConditionalFormula(Clause({1,2})),1),
                   make_pair<ConditionalFormula, double>(ConditionalFormula(Clause({1,3})),1),
                   make_pair<ConditionalFormula, double>(ConditionalFormula(Clause({2,3})),1),
                   make_pair<ConditionalFormula, double>(ConditionalFormula(Clause({-1})),0.3),
                   make_pair<ConditionalFormula, double>(ConditionalFormula(Clause({2})),0.6),
                   make_pair<ConditionalFormula, double>(ConditionalFormula(Clause({3})),0.7),
                   });
    double *probs = c.getDimacsProbs();
    CPPUNIT_ASSERT(probs[0] == 1);
    CPPUNIT_ASSERT(probs[1] == 0.6);
    CPPUNIT_ASSERT(probs[2] == 0.7);
    CPPUNIT_ASSERT(probs[3] == 0.3);
    delete[] probs;
    CPPUNIT_ASSERT(c.getDimacsGamma() == ClausalFormula({{4,1},{4,2},{1,2},{-4,-3},{3,4}}));
    CPPUNIT_ASSERT(c.getDimacsExtraColumns().size() == 0);
    CPPUNIT_ASSERT(c.getDimacsNumCl() == 5);
    CPPUNIT_ASSERT(c.getDimacsNumPr() == 3);
    CPPUNIT_ASSERT(c.getDimacsNumVar() == 4);
}
