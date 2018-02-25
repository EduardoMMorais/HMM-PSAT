#include "apsattest.h"
#include "psat.h"

CPPUNIT_TEST_SUITE_REGISTRATION(APSATTest);

#ifndef makeRational
#define makeRational(NUM,DEN) mpq_class((NUM),(DEN))
#endif

void APSATTest::setUp()
{
    vector<mpq_class> probs({makeRational(7,10), makeRational(7,10), makeRational(7,10)});
    ClausalFormula gamma={{1,2}, {1,3}, {2,3}};
    PSatInst = new APSATSolver(3, probs, {}, gamma);
}

void APSATTest::tearDown()
{
    delete PSatInst;
}

void APSATTest::testPSATSatisfiableInstance()
{
    vector<mpq_class> probs({makeRational(7,10), makeRational(7,10), makeRational(7,10)});
    PSatInst->setProbabilities(probs);
    CPPUNIT_ASSERT(PSatInst->solve());
}

void APSATTest::testPSATUnsatisfiableInstance()
{
    vector<mpq_class> probs({makeRational(3,5), makeRational(3,5), makeRational(3,5)});
    PSatInst->setProbabilities(probs);
    CPPUNIT_ASSERT(!PSatInst->solve());
}

void APSATTest::testPSATConditionalProbDef()
{
    /* P(A|B) = 4/7 => P(A^B) = 4/10 = 2/5 */
    PSatInst->setNumVars(4);
    vector<mpq_class> probs({makeRational(7,10), makeRational(7,10), makeRational(7,10), makeRational(2, 5)});
    PSatInst->setProbabilities(probs);
    ClausalFormula gamma={
        {1,2}, {1,3}, {2,3}, /* Usual gamma */
        {-4,1}, {-4,2}, {-1,-2,4} /* D <-> A^B */
        };
    PSatInst->setGamma(gamma);
    CPPUNIT_ASSERT(PSatInst->solve());
}

void APSATTest::testPSATConditionalProbUndef()
{
    /* P(A|B^C) = 0.25 */
    PSatInst->setNumVars(5);
    vector<mpq_class> probs({makeRational(7,10), makeRational(7,10), makeRational(7,10), 0, 0});
    PSatInst->setProbabilities(probs);
    ClausalFormula gamma={
        {1,2}, {1,3}, {2,3}, /* Usual gamma */
        {-4,1}, {-4,5}, {-1,-5,4}, /* D <-> A^E */
        {-5,2}, {-5,3}, {-2,-3,5}, /* E <-> B^C */
        };
    PSatInst->setGamma(gamma);
    vector<vector<mpq_class>> auxVarColumn = {{0,0,0,makeRational(-1,4),-1}};
    PSatInst->setAuxColumns(auxVarColumn);
    CPPUNIT_ASSERT(PSatInst->solve());
}

/*class ErrorCostCalculator: public CostFunctionCalculator {
    private:
        PSolverData &_sd;
        int *costVector;
    public:
        ErrorCostCalculator(PSolverData &sd): _sd(sd) {
            costVector = new int[sd._nProb+1];
            for( unsigned int i=0; i < sd._nProb+1; i++ )
                costVector[i]=0;
        }
        ~ErrorCostCalculator() {
            delete[] costVector;
        }
        int *calculateCostFunction(const Matrix &) {
            for(unsigned int i=0; i < _sd._nProb+1; i++ )
                if (_sd.getColumnExtra(i) > -1)
                    costVector[i]=1;
            return costVector;
        }
        int *recalculateColumnExitBasis(const Matrix &basis, int) {
            return calculateCostFunction(basis);
        }
};*/

void APSATTest::testPSATUnsatWithErrors()
{
/*    makeRational probs[3] = {makeRational(3,5), makeRational(3,5), makeRational(3,5)};
    ClausalFormula gamma={{1,2}, {1,3}, {2,3}};
    vector<vector<makeRational>> errorColumns = {
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1},
        {0,0,0,-1},
        {0,0,-1,0},
        {0,-1,0,0}};
    PSatInst = new PSAT(3 /*nVar, 3 /*nProb, 3 /* nClause , probs, 5 /*precision, errorColumns, gamma);
    ErrorCostCalculator costFunction(PSatInst->getSolverData());
    double cost = PSatInst->oSolve(costFunction);
    CPPUNIT_ASSERT(cost == makeRational(1,5));*/
    CPPUNIT_ASSERT(false);
}
