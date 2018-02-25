#ifndef NONMARKOVMODEL_H
#define NONMARKOVMODEL_H
#include "c++11_warning.h"
#include "model.h"
#include "APSATSolver.h"

#define WINDOWSIZE 2

/*class SentenceCostCalculator : public CostFunctionCalculator {
    private:
        PSolverData &_sd;
        int *costVector;
        list<Literal> sentenceLiterals;
        PSATInstance &instance;
    public:
        SentenceCostCalculator(PSolverData &sd, list<Literal> sl, PSATInstance &pi);
        ~SentenceCostCalculator();
        int *calculateCostFunction(const Matrix &basis);
        int *recalculateColumnExitBasis(const Matrix &basis, int column);
};*/

class NonMarkovModel : public Model
{
    public:
        PSATInstance generatePSATInstance(const list<unsigned int> &sentence);
        using Model::Model; //Inherit the base class' constructors
        list<Literal> getLiteralsSentence(unsigned int sentenceSize);
        list<Literal> getLiteralsTagging(vector<Tag> tags);
    protected:
    private:
        Literal getLiteralWord(unsigned int word, unsigned int numOfWords) const;
        Literal getLiteralTagOfWord(Tag tag, unsigned int word) const;
        bool getNextTagging(vector<Tag> &tagset) const;

    friend class NonMMModelTest;
};

#endif // NONMARKOVMODEL_H
