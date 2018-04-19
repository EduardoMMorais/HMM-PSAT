#ifndef MODEL_H
#define MODEL_H

#include <list>
#include <vector>
#include <string>
#include <gmpxx.h>
#include "c++11_warning.h"
#include "psat.h"

typedef unsigned int Tag;

class Model
{
    public:
        Model(unsigned int t, vector<vector<mpq_class>> *probWPT,
              vector<vector<mpq_class>> *probTGT, vector<mpq_class> *initProb):
                  numOfTags(t), probabilityWordPerTag(probWPT), probabilityTagGivenTag(probTGT), initialProbability(initProb) {};
        virtual PSATInstance generatePSATInstance(const list<unsigned int> &sentence)=0;
        virtual list<Literal> getLiteralsSentence(unsigned int sentenceSize)=0;
        virtual list<Literal> getLiteralsTagging(vector<unsigned int> tags)=0;
        virtual ~Model() {};
    protected:
        unsigned int numOfTags;
        mpq_class getProbabilityWordPerTag(unsigned int word, Tag tag) const;
        mpq_class getInitialProbability(Tag tag) const;
        mpq_class getProbabilityTagGivenTag(Tag t1, Tag t2) const;
    private:
        vector<vector<mpq_class>> *probabilityWordPerTag;
        vector<vector<mpq_class>> *probabilityTagGivenTag;
        vector<mpq_class> *initialProbability;
};

#endif // MODEL_H
