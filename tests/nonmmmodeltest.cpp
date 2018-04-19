#include <utility>
#include <cmath>
#include <strstream>

#include "nonmmmodeltest.h"
#include "APSATSolver.h"

CPPUNIT_TEST_SUITE_REGISTRATION(NonMMModelTest);

static NonMarkovModel *model;
static vector<vector<mpq_class>> probWPT;
static vector<vector<mpq_class>> probTGT;
static vector<mpq_class> initProb;
void NonMMModelTest::setUp() {
    //Tag 0: Verb, Tag 1:Noum, Tag 2: Adj
    //Word "shop": 0, "buy": 1, "store": 2, "cheap": 3
    probWPT.push_back({mpq_class(2,5), mpq_class(2,5), mpq_class(1,5), mpq_class(0,1)});
    probWPT.push_back({mpq_class(1,3), mpq_class(1,10), mpq_class(1,2), mpq_class(1,10)});
    probWPT.push_back({mpq_class(1,10), mpq_class(1,10), mpq_class(1,10), mpq_class(7,10)});

    probTGT.push_back({mpq_class(1,10),mpq_class(1,2),mpq_class(2,5)});
    probTGT.push_back({mpq_class(1,3),mpq_class(1,3),mpq_class(2,5)});
    probTGT.push_back({mpq_class(2,5),mpq_class(2,5),mpq_class(1,5)});

    initProb = {mpq_class(1,3), mpq_class(1,2), mpq_class(1,5)};
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
                          make_pair<ConditionalFormula, mpq_class>(::Clause({1,2,3}),mpq_class(1,1)),
                          make_pair<ConditionalFormula, mpq_class>(::Clause({4,5,6}),mpq_class(1,1)),
                          make_pair<ConditionalFormula, mpq_class>(::Clause({7,8,9}),mpq_class(1,1)),
                          /* 2. Apenas uma tag por palavra */
                          make_pair<ConditionalFormula, mpq_class>(::Clause({-1,-2}),mpq_class(1,1)),
                          make_pair<ConditionalFormula, mpq_class>(::Clause({-1,-3}),mpq_class(1,1)),
                          make_pair<ConditionalFormula, mpq_class>(::Clause({-2,-3}),mpq_class(1,1)),
                          make_pair<ConditionalFormula, mpq_class>(::Clause({-4,-5}),mpq_class(1,1)),
                          make_pair<ConditionalFormula, mpq_class>(::Clause({-4,-6}),mpq_class(1,1)),
                          make_pair<ConditionalFormula, mpq_class>(::Clause({-5,-6}),mpq_class(1,1)),
                          make_pair<ConditionalFormula, mpq_class>(::Clause({-7,-8}),mpq_class(1,1)),
                          make_pair<ConditionalFormula, mpq_class>(::Clause({-7,-9}),mpq_class(1,1)),
                          make_pair<ConditionalFormula, mpq_class>(::Clause({-8,-9}),mpq_class(1,1)),
                          /* 3. Matriz de observação */
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(10,1),mpq_class(2,5)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(10,2),mpq_class(1,10)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(10,3),mpq_class(1,10)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(11,4),mpq_class(1,5)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(11,5),mpq_class(1,2)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(11,6),mpq_class(1,10)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(12,7),mpq_class(0,1)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(12,8),mpq_class(1,10)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(12,9),mpq_class(7,10)),
                          /* 4.Matriz de transição de estados*/
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(4,1),mpq_class(1,10)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(4,2),mpq_class(1,3)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(4,3),mpq_class(2,5)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(5,1),mpq_class(1,2)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(5,2),mpq_class(1,3)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(5,3),mpq_class(2,5)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(6,1),mpq_class(2,5)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(6,2),mpq_class(2,5)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(6,3),mpq_class(1,5)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(7,4),mpq_class(1,10)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(8,4),mpq_class(1,2)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(9,4),mpq_class(2,5)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(7,5),mpq_class(1,3)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(8,5),mpq_class(1,3)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(9,5),mpq_class(2,5)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(7,6),mpq_class(2,5)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(8,6),mpq_class(2,5)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(9,6),mpq_class(1,5)),
                          /*  5.Estados iniciais */
                          make_pair<ClausalFormula, mpq_class>(::Clause({1}),mpq_class(1,3)),
                          make_pair<ClausalFormula, mpq_class>(::Clause({2}),mpq_class(1,2)),
                          make_pair<ClausalFormula, mpq_class>(::Clause({3}),mpq_class(1,5)),
                          /*  6.Propriedade da independência (Janela 2)*/
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({10,11}),ClausalFormula({1,4})),mpq_class(2,25)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({10,11}),ClausalFormula({1,5})),mpq_class(1,5)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({10,11}),ClausalFormula({1,6})),mpq_class(1,25)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({10,11}),ClausalFormula({2,4})),mpq_class(1,50)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({10,11}),ClausalFormula({2,5})),mpq_class(1,20)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({10,11}),ClausalFormula({2,6})),mpq_class(1,100)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({10,11}),ClausalFormula({3,4})),mpq_class(1,50)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({10,11}),ClausalFormula({3,5})),mpq_class(1,20)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({10,11}),ClausalFormula({3,6})),mpq_class(1,100)),

                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({11,12}),ClausalFormula({4,7})),mpq_class(0,1)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({11,12}),ClausalFormula({4,8})),mpq_class(1,50)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({11,12}),ClausalFormula({4,9})),mpq_class(7,50)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({11,12}),ClausalFormula({5,7})),mpq_class(0,1)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({11,12}),ClausalFormula({5,8})),mpq_class(1,20)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({11,12}),ClausalFormula({5,9})),mpq_class(7,20)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({11,12}),ClausalFormula({6,7})),mpq_class(0,1)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({11,12}),ClausalFormula({6,8})),mpq_class(1,100)),
                          make_pair<ConditionalFormula, mpq_class>(ConditionalFormula(ClausalFormula({11,12}),ClausalFormula({6,9})),mpq_class(7,100)),
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
    vector<vector<mpq_class>> extra = problem.getDimacsExtraColumns();
    for (auto e: extra) {
        for (mpq_class c: e) {
            cout << c << " ";
        }
        cout << endl;
    }
    cout << "\nProbs: \n";
    mpq_class *probs = problem.getDimacsProbs();
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
    mpq_class probSentence = problem.getSolutionProbability(sentenceLits,PSatInstSolver.getSolverData());
    sentenceLits.splice(sentenceLits.begin(), model->getLiteralsTagging({0,1}));
    mpq_class probTaggingVerbNounAndSentence = problem.getSolutionProbability(sentenceLits,PSatInstSolver.getSolverData());

    CPPUNIT_ASSERT(fabs(probSentence - 0.0557) < 1.0e-12);
    CPPUNIT_ASSERT(fabs(probTaggingVerbNounAndSentence - 0.03) < 1.0e-12);
*/
    CPPUNIT_ASSERT(false);
    //Calculate
}
