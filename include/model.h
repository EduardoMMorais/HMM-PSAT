#ifndef MODEL_H
#define MODEL_H

#include <list>
#include <vector>
#include <string>
#include "c++11_warning.h"
#include "psat.h"

typedef unsigned int Tag;

class Model
{
    public:
        Model(unsigned int t, vector<vector<double>> *probWPT,
              vector<vector<double>> *probTGT, vector<double> *initProb):
                  numOfTags(t), probabilityWordPerTag(probWPT), probabilityTagGivenTag(probTGT), initialProbability(initProb) {};
        virtual PSATInstance generatePSATInstance(const list<unsigned int> &sentence)=0;
        virtual list<Literal> getLiteralsSentence(unsigned int sentenceSize)=0;
        virtual list<Literal> getLiteralsTagging(vector<unsigned int> tags)=0;
        virtual ~Model() {};
    protected:
        unsigned int numOfTags;
        double getProbabilityWordPerTag(unsigned int word, Tag tag) const;
        double getInitialProbability(Tag tag) const;
        double getProbabilityTagGivenTag(Tag t1, Tag t2) const;
    private:
        vector<vector<double>> *probabilityWordPerTag;
        vector<vector<double>> *probabilityTagGivenTag;
        vector<double> *initialProbability;
};

#endif // MODEL_H
