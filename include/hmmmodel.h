#ifndef HMMMODEL_H
#define HMMMODEL_H
#include "c++11_warning.h"
#include "model.h"


class HMMModel : public Model
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

    friend class HMMModelTest;
};

#endif // HMMMODEL_H
