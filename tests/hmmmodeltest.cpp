#include <utility>
#include <cmath>
#include <strstream>

#include "hmmmodeltest.h"
#include "APSATSolver.h"

CPPUNIT_TEST_SUITE_REGISTRATION(HMMModelTest);

static HMMModel *model;
static vector<vector<double>> probWPT;
static vector<vector<double>> probTGT;
static vector<double> initProb;
void HMMModelTest::setUp() {
    //Tag 0: Verb, Tag 1:Noum, Tag 2: Adj
    //Word "shop": 0, "buy": 1, "store": 2, "cheap": 3
    probWPT.push_back({0.4, 0.4, 0.2, 0.0});
    probWPT.push_back({0.3, 0.1, 0.5, 0.1});
    probWPT.push_back({0.1, 0.1, 0.1, 0.7});

    probTGT.push_back({0.1,0.5,0.4});
    probTGT.push_back({0.3,0.3,0.4});
    probTGT.push_back({0.4,0.4,0.2});

    initProb = {0.3, 0.5, 0.2};
    model = new HMMModel(3, &probWPT,&probTGT,&initProb);
}
void HMMModelTest::tearDown() {
    delete model;
}
void HMMModelTest::testPSATInstance() {
    list<unsigned int> sentence = {1,2};
    PSATInstance actual = model->generatePSATInstance(sentence);
    stringstream actualss, expectedss;
    actualss << actual;
    /* 1: t_00  2: t_01  3: t_02  4: t_10  5: t_11   6: t_12
     * 7: w_0   8: w_1 */
    PSATInstance expected({
                          /* 1. Toda palavra tem um tag */
                          make_pair<ConditionalFormula, double>(::Clause({1,2,3}),1.0),
                          make_pair<ConditionalFormula, double>(::Clause({4,5,6}),1.0),
                          /* 2. Apenas uma tag por palavra */
                          make_pair<ConditionalFormula, double>(::Clause({-1,-2}),1.0),
                          make_pair<ConditionalFormula, double>(::Clause({-1,-3}),1.0),
                          make_pair<ConditionalFormula, double>(::Clause({-2,-3}),1.0),
                          make_pair<ConditionalFormula, double>(::Clause({-4,-5}),1.0),
                          make_pair<ConditionalFormula, double>(::Clause({-4,-6}),1.0),
                          make_pair<ConditionalFormula, double>(::Clause({-5,-6}),1.0),
                          /* 3. Matriz de observação */
                          make_pair<ConditionalFormula, double>(ConditionalFormula(7,1),0.4),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(7,2),0.1),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(7,3),0.1),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(8,4),0.2),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(8,5),0.5),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(8,6),0.1),
                          /*  6.Propriedade da independência */
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({7,8}),ClausalFormula({1,4})),0.08),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({7,8}),ClausalFormula({1,5})),0.2),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({7,8}),ClausalFormula({1,6})),0.04),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({7,8}),ClausalFormula({2,4})),0.02),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({7,8}),ClausalFormula({2,5})),0.05),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({7,8}),ClausalFormula({2,6})),0.01),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({7,8}),ClausalFormula({3,4})),0.02),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({7,8}),ClausalFormula({3,5})),0.05),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({7,8}),ClausalFormula({3,6})),0.01),
                          /* 7.Propriedade de Markov + Matriz de transição de estados + Estados Iniciais */
                          make_pair<ConditionalFormula, double>(ClausalFormula({1,4}),0.03),
                          make_pair<ConditionalFormula, double>(ClausalFormula({1,5}),0.15),
                          make_pair<ConditionalFormula, double>(ClausalFormula({1,6}),0.12),
                          make_pair<ConditionalFormula, double>(ClausalFormula({2,4}),0.15),
                          make_pair<ConditionalFormula, double>(ClausalFormula({2,5}),0.15),
                          make_pair<ConditionalFormula, double>(ClausalFormula({2,6}),0.20),
                          make_pair<ConditionalFormula, double>(ClausalFormula({3,4}),0.08),
                          make_pair<ConditionalFormula, double>(ClausalFormula({3,5}),0.08),
                          make_pair<ConditionalFormula, double>(ClausalFormula({3,6}),0.04),
                          });
    expectedss << expected;
    CPPUNIT_ASSERT(actualss.str() == expectedss.str());
}

void HMMModelTest::testSolvePSATInstace() {
    list<unsigned int> sentence = {1,2};
    PSATInstance problem(model->generatePSATInstance(sentence));
    APSATSolver solver(problem.getDimacsNumVar(), const vector<mpq_class>& probs, const vector<vector<mpq_class>>& auxColumns, const ClausalFormula& gamma);
    /*PSAT PSatInstSolver(problem, 2);
    CPPUNIT_ASSERT(PSatInstSolver.solve());
    //Testa valor do resultado
    list<Literal> sentenceLits = model->getLiteralsSentence(sentence.size());
    double probSentence = problem.getSolutionProbability(sentenceLits,PSatInstSolver.getSolverData());
    sentenceLits.splice(sentenceLits.begin(), model->getLiteralsTagging({0,1}));
    double probTaggingVerbNounAndSentence = problem.getSolutionProbability(sentenceLits,PSatInstSolver.getSolverData());

    CPPUNIT_ASSERT(fabs(probSentence - 0.0557) < 1.0e-12);
    CPPUNIT_ASSERT(fabs(probTaggingVerbNounAndSentence - 0.03) < 1.0e-12);
*/
    //Calculate
}
