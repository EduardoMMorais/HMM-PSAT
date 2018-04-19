#include <utility>
#include <cmath>
#include <strstream>

#include "hmmmodeltest.h"
#include "APSATSolver.h"

CPPUNIT_TEST_SUITE_REGISTRATION(HMMModelTest);

static HMMModel *model;
static vector<vector<mpq_class>> probWPT;
static vector<vector<mpq_class>> probTGT;
static vector<mpq_class> initProb;
void HMMModelTest::setUp() {
    //Tag 0: Verb, Tag 1:Noum, Tag 2: Adj
    //Word "shop": 0, "buy": 1, "store": 2, "cheap": 3
    probWPT.push_back({mpq_class(2,5), mpq_class(2,5), mpq_class(1,5), mpq_class(0,1)});
    probWPT.push_back({mpq_class(3,10), mpq_class(1,10), mpq_class(1,2), mpq_class(1,10)});
    probWPT.push_back({mpq_class(1,10), mpq_class(1,10), mpq_class(1,10), mpq_class(7,10)});

    probTGT.push_back({mpq_class(1,10),mpq_class(1,2),mpq_class(2,5)});
    probTGT.push_back({mpq_class(3,10),mpq_class(3,10),mpq_class(2,5)});
    probTGT.push_back({mpq_class(2,5),mpq_class(2,5),mpq_class(1,5)});

    initProb = {mpq_class(3,10), mpq_class(1,2), mpq_class(1,5)};
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
                          make_pair<ConditionalFormula, mpq_class>(::Clause({1,2,3}),mpq_class(1,1)),
                          make_pair<ConditionalFormula, mpq_class>(::Clause({4,5,6}),mpq_class(1,1)),
                          /* 2. Apenas uma tag por palavra */
                          make_pair<ConditionalFormula, mpq_class>(::Clause({-1,-2}),mpq_class(1,1)),
                          make_pair<ConditionalFormula, mpq_class>(::Clause({-1,-3}),mpq_class(1,1)),
                          make_pair<ConditionalFormula, mpq_class>(::Clause({-2,-3}),mpq_class(1,1)),
                          make_pair<ConditionalFormula, mpq_class>(::Clause({-4,-5}),mpq_class(1,1)),
                          make_pair<ConditionalFormula, mpq_class>(::Clause({-4,-6}),mpq_class(1,1)),
                          make_pair<ConditionalFormula, mpq_class>(::Clause({-5,-6}),mpq_class(1,1)),
                          /* 3. Matriz de observação */
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(7,1),mpq_class(2,5)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(7,2),mpq_class(1,10)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(7,3),mpq_class(1,10)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(8,4),mpq_class(1,5)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(8,5),mpq_class(1,2)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(8,6),mpq_class(1,10)),
                          /*  6.Propriedade da independência */
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({7,8}),ClausalFormula({1,4})),mpq_class(2,25)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({7,8}),ClausalFormula({1,5})),mpq_class(1,5)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({7,8}),ClausalFormula({1,6})),mpq_class(1,25)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({7,8}),ClausalFormula({2,4})),mpq_class(1,50)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({7,8}),ClausalFormula({2,5})),mpq_class(1,20)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({7,8}),ClausalFormula({2,6})),mpq_class(1,100)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({7,8}),ClausalFormula({3,4})),mpq_class(1,50)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({7,8}),ClausalFormula({3,5})),mpq_class(1,20)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({7,8}),ClausalFormula({3,6})),mpq_class(1,100)),
                          /* 7.Propriedade de Markov + Matriz de transição de estados + Estados Iniciais */
                          make_pair<ConditionalFormula, mpq_class>(ClausalFormula({1,4}),mpq_class(3,100)),
                          make_pair<ConditionalFormula, mpq_class>(ClausalFormula({1,5}),mpq_class(3,20)),
                          make_pair<ConditionalFormula, mpq_class>(ClausalFormula({1,6}),mpq_class(3,25)),
                          make_pair<ConditionalFormula, mpq_class>(ClausalFormula({2,4}),mpq_class(3,20)),
                          make_pair<ConditionalFormula, mpq_class>(ClausalFormula({2,5}),mpq_class(3,20)),
                          make_pair<ConditionalFormula, mpq_class>(ClausalFormula({2,6}),mpq_class(1,5)),
                          make_pair<ConditionalFormula, mpq_class>(ClausalFormula({3,4}),mpq_class(2,25)),
                          make_pair<ConditionalFormula, mpq_class>(ClausalFormula({3,5}),mpq_class(2,25)),
                          make_pair<ConditionalFormula, mpq_class>(ClausalFormula({3,6}),mpq_class(1,25)),
                          });
    expectedss << expected;
    CPPUNIT_ASSERT(actualss.str() == expectedss.str());
}

void HMMModelTest::testSolvePSATInstace() {
    list<unsigned int> sentence = {1,2};
    PSATInstance problem(model->generatePSATInstance(sentence));
    //APSATSolver solver(problem.getDimacsNumVar(), const vector<mpq_class>& probs, const vector<vector<mpq_class>>& auxColumns, const ClausalFormula& gamma);
    CPPUNIT_ASSERT(false);
    //
    /*PSAT PSatInstSolver(problem, 2);
    CPPUNIT_ASSERT(PSatInstSolver.solve());
    //Testa valor do resultado
    list<Literal> sentenceLits = model->getLiteralsSentence(sentence.size());
    mpq_class probSentence = problem.getSolutionProbability(sentenceLits,PSatInstSolver.getSolverData());
    sentenceLits.splice(sentenceLits.begin(), model->getLiteralsTagging({0,1}));
    mpq_class probTaggingVerbNounAndSentence = problem.getSolutionProbability(sentenceLits,PSatInstSolver.getSolverData());

    CPPUNIT_ASSERT(fabs(probSentence - 0.0557) < 1.0e-12);
    CPPUNIT_ASSERT(fabs(probTaggingVerbNounAndSentence - 0.03) < 1.0e-12);
*/
    //Calculate
}
