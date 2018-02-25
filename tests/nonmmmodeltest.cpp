#include <utility>
#include <cmath>
#include <strstream>

#include "nonmmmodeltest.h"
#include "APSATSolver.h"

CPPUNIT_TEST_SUITE_REGISTRATION(NonMMModelTest);

static NonMarkovModel *model;
static vector<vector<double>> probWPT;
static vector<vector<double>> probTGT;
static vector<double> initProb;
void NonMMModelTest::setUp() {
    //Tag 0: Verb, Tag 1:Noum, Tag 2: Adj
    //Word "shop": 0, "buy": 1, "store": 2, "cheap": 3
    probWPT.push_back({0.4, 0.4, 0.2, 0.0});
    probWPT.push_back({0.3, 0.1, 0.5, 0.1});
    probWPT.push_back({0.1, 0.1, 0.1, 0.7});

    probTGT.push_back({0.1,0.5,0.4});
    probTGT.push_back({0.3,0.3,0.4});
    probTGT.push_back({0.4,0.4,0.2});

    initProb = {0.3, 0.5, 0.2};
    model = new NonMarkovModel(3, &probWPT,&probTGT,&initProb);
}
void NonMMModelTest::tearDown() {
    delete model;
}
void NonMMModelTest::testPSATInstance() {
    list<unsigned int> sentence = {1,2,3};
    PSATInstance actual = model->generatePSATInstance(sentence);
    stringstream actualss, expectedss;
    actualss << actual;
    /* 1: t_00  2: t_01  3: t_02  4: t_10  5: t_11   6: t_12   7: t_20   8: t_21   9: t_22
     * 10: w_0   11: w_1   12: w_2 */
    PSATInstance expected({
                          /* 1. Toda palavra tem um tag */
                          make_pair<ConditionalFormula, double>(::Clause({1,2,3}),1.0),
                          make_pair<ConditionalFormula, double>(::Clause({4,5,6}),1.0),
                          make_pair<ConditionalFormula, double>(::Clause({7,8,9}),1.0),
                          /* 2. Apenas uma tag por palavra */
                          make_pair<ConditionalFormula, double>(::Clause({-1,-2}),1.0),
                          make_pair<ConditionalFormula, double>(::Clause({-1,-3}),1.0),
                          make_pair<ConditionalFormula, double>(::Clause({-2,-3}),1.0),
                          make_pair<ConditionalFormula, double>(::Clause({-4,-5}),1.0),
                          make_pair<ConditionalFormula, double>(::Clause({-4,-6}),1.0),
                          make_pair<ConditionalFormula, double>(::Clause({-5,-6}),1.0),
                          make_pair<ConditionalFormula, double>(::Clause({-7,-8}),1.0),
                          make_pair<ConditionalFormula, double>(::Clause({-7,-9}),1.0),
                          make_pair<ConditionalFormula, double>(::Clause({-8,-9}),1.0),
                          /* 3. Matriz de observação */
                          make_pair<ConditionalFormula, double>(ConditionalFormula(10,1),0.4),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(10,2),0.1),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(10,3),0.1),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(11,4),0.2),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(11,5),0.5),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(11,6),0.1),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(12,7),0.0),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(12,8),0.1),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(12,9),0.7),
                          /* 4.Matriz de transição de estados*/
                          make_pair<ConditionalFormula, double>(ConditionalFormula(4,1),0.1),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(4,2),0.3),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(4,3),0.4),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(5,1),0.5),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(5,2),0.3),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(5,3),0.4),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(6,1),0.4),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(6,2),0.4),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(6,3),0.2),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(7,4),0.1),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(8,4),0.5),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(9,4),0.4),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(7,5),0.3),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(8,5),0.3),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(9,5),0.4),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(7,6),0.4),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(8,6),0.4),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(9,6),0.2),
                          /*  5.Estados iniciais */
                          make_pair<ClausalFormula, double>(::Clause({1}),0.3),
                          make_pair<ClausalFormula, double>(::Clause({2}),0.5),
                          make_pair<ClausalFormula, double>(::Clause({3}),0.2),
                          /*  6.Propriedade da independência (Janela 2)*/
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({10,11}),ClausalFormula({1,4})),0.08),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({10,11}),ClausalFormula({1,5})),0.2),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({10,11}),ClausalFormula({1,6})),0.04),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({10,11}),ClausalFormula({2,4})),0.02),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({10,11}),ClausalFormula({2,5})),0.05),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({10,11}),ClausalFormula({2,6})),0.01),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({10,11}),ClausalFormula({3,4})),0.02),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({10,11}),ClausalFormula({3,5})),0.05),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({10,11}),ClausalFormula({3,6})),0.01),

                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({11,12}),ClausalFormula({4,7})),0.0),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({11,12}),ClausalFormula({4,8})),0.02),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({11,12}),ClausalFormula({4,9})),0.14),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({11,12}),ClausalFormula({5,7})),0.0),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({11,12}),ClausalFormula({5,8})),0.05),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({11,12}),ClausalFormula({5,9})),0.35),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({11,12}),ClausalFormula({6,7})),0.0),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({11,12}),ClausalFormula({6,8})),0.01),
                          make_pair<ConditionalFormula, double>(ConditionalFormula(ClausalFormula({11,12}),ClausalFormula({6,9})),0.07),
                          });
    expectedss << expected;
    CPPUNIT_ASSERT(actualss.str() == expectedss.str());
}

void NonMMModelTest::testSolveOPSATInstance() {
    //list<unsigned int> sentence = {1,2,3};
    list<unsigned int> sentence = {1,2};
    PSATInstance problem = model->generatePSATInstance(sentence);

/*
    cout << "\nExtra Columns: \n";
    vector<vector<double>> extra = problem.getDimacsExtraColumns();
    for (auto e: extra) {
        for (double c: e) {
            cout << c << " ";
        }
        cout << endl;
    }
    cout << "\nProbs: \n";
    double *probs = problem.getDimacsProbs();
    for (int i = 0; i < problem.getDimacsNumPr(); i++) {
        cout << "P(" << (i+1) << ") = " << probs[i] << endl;
    }
    cout << "\nGamma: \n";
    cout << problem.getDimacsGamma() << endl;

    PSAT PSatInstSolver(problem, 2);
    SentenceCostCalculator costFunction(PSatInstSolver.getSolverData(), model->getLiteralsSentence(sentence.size()), problem);
    CPPUNIT_ASSERT(PSatInstSolver.oSolve(costFunction) < 1.0e-12); //Dá UNSAT ou fica parado no plateau*/
    //Testa valor do resultado
    /*
    list<Literal> sentenceLits = model->getLiteralsSentence(sentence.size());
    double probSentence = problem.getSolutionProbability(sentenceLits,PSatInstSolver.getSolverData());
    sentenceLits.splice(sentenceLits.begin(), model->getLiteralsTagging({0,1}));
    double probTaggingVerbNounAndSentence = problem.getSolutionProbability(sentenceLits,PSatInstSolver.getSolverData());

    CPPUNIT_ASSERT(fabs(probSentence - 0.0557) < 1.0e-12);
    CPPUNIT_ASSERT(fabs(probTaggingVerbNounAndSentence - 0.03) < 1.0e-12);
*/
    CPPUNIT_ASSERT(false);
    //Calculate
}
