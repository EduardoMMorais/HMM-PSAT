#include "NonMarkovModel.h"

PSATInstance NonMarkovModel::generatePSATInstance(const list<unsigned int> &sentence) {
    PSATInstance pi;
    if (sentence.size() == 0) return pi;
    // 1. Toda palavra tem um tag
    for(unsigned int wi = 0; wi < sentence.size(); wi++) {
        ::Clause c;
        for (unsigned int ti = 0; ti < numOfTags; ti++) {
            c.insert(getLiteralTagOfWord(ti, wi));
        }
        pi.emplace(c, 1.0);
    }

    // 2. Apenas uma tag por palavra
    for(unsigned int wi = 0; wi < sentence.size(); wi++) {
        for(unsigned int ta = 0; ta < numOfTags; ta++) {
            for (unsigned int tb = ta + 1; tb < numOfTags; tb++) {
                ::Clause c;
                c.insert(-getLiteralTagOfWord(ta, wi));
                c.insert(-getLiteralTagOfWord(tb, wi));
                pi.emplace(c, 1.0);
            }
        }
    }

    // 3. Matriz de observação
    unsigned int wi = 0;
    for(unsigned int word: sentence) {
        for (unsigned int ti = 0; ti < numOfTags; ti++) {
            ConditionalFormula c(getLiteralWord(wi, sentence.size()) , getLiteralTagOfWord(ti, wi));
            pi.emplace(c, getProbabilityWordPerTag(word, ti));
        }
        wi++;
    }

    // 4. Matriz de transição de estados
    for(unsigned int wi = 1; wi < sentence.size(); wi++) {
        for(unsigned int ta = 0; ta < numOfTags; ta++) {
            for (unsigned int tb = 0; tb < numOfTags; tb++) {
                ConditionalFormula c(getLiteralTagOfWord(ta,wi),getLiteralTagOfWord(tb,wi-1));
                pi.emplace(c, getProbabilityTagGivenTag(ta, tb));
            }
        }
    }
    // 5. Estados iniciais
    for (unsigned int ta = 0; ta < numOfTags; ta++) {
        ::Clause c(getLiteralTagOfWord(ta,0));
        pi.emplace(c, getInitialProbability(ta));
    }

    unsigned int wws;
    for (wws = 0; wws <= sentence.size() - WINDOWSIZE; wws++) {
        ClausalFormula sentenceFormula;
        for(wi = wws; wi < wws + WINDOWSIZE; wi++) {
            sentenceFormula.insert(getLiteralWord(wi, sentence.size()));
        }

        vector<Tag> tagset(WINDOWSIZE, 0);
        do {
            ClausalFormula tagSetFormula;
            mpq_class sentenceProbabilityGivenTagSet = 1.0;
            list<unsigned int>::const_iterator wordIt = sentence.cbegin();
            for (unsigned int start_position = 0; start_position < wws; ++start_position) ++wordIt;
            for(wi = 0; wi < WINDOWSIZE; ++wi, ++wordIt) {
                tagSetFormula.insert(getLiteralTagOfWord(tagset[wi], wws + wi));
                sentenceProbabilityGivenTagSet *= getProbabilityWordPerTag(*wordIt, tagset[wi]);
            }
            // 6. Propriedade da independência
            ConditionalFormula c(sentenceFormula, tagSetFormula);
            pi.emplace(c, sentenceProbabilityGivenTagSet);

        } while(getNextTagging(tagset)); // Gera todos os taggings possíveis
    }

    return pi;
}

Literal NonMarkovModel::getLiteralTagOfWord(unsigned int tag, unsigned int word) const {
    return word * numOfTags + tag + 1;
}

Literal NonMarkovModel::getLiteralWord(unsigned int word, unsigned int numOfWords) const {
    return numOfWords * numOfTags + word + 1;
}

list<Literal> NonMarkovModel::getLiteralsSentence(unsigned int sentenceSize) {
    list<Literal> l;
    for (unsigned int i = 0; i<sentenceSize; i++)
        l.push_back(getLiteralWord(i, sentenceSize));
    return l;
}

list<Literal> NonMarkovModel::getLiteralsTagging(vector<Tag> tags) {
    list<Literal> l;
    for (unsigned int i = 0; i<tags.size(); i++)
        l.push_back(getLiteralTagOfWord(tags[i], i));
    return l;
}

bool NonMarkovModel::getNextTagging(vector<Tag> &tagset) const {
    if (tagset.empty())
        return false;

    unsigned int i = tagset.size() - 1;
    while(tagset[i] == numOfTags - 1) {
        tagset[i] = 0;
        if (i == 0)
            return false;
        i--;
    }
    tagset[i]++;
    return true;
}

/*SentenceCostCalculator::SentenceCostCalculator(PSolverData &sd, list<Literal> sl, PSATInstance &pi): _sd(sd), sentenceLiterals(sl), instance(pi) {
    costVector = new int[sd._nProb+1];
    for( unsigned int i=0; i < sd._nProb+1; i++ )
        costVector[i]=0;
}
SentenceCostCalculator::~SentenceCostCalculator() {
    delete[] costVector;
}
int *SentenceCostCalculator::calculateCostFunction(const Matrix &) {
    instance.solutionHasLiterals(sentenceLiterals,_sd,costVector);
    for(int column = 0; column <= instance.getDimacsNumPr(); column++) {
        costVector[column] = 1 - costVector[column];
    }
    return costVector;
}
int *SentenceCostCalculator::recalculateColumnExitBasis(const Matrix &basis, int) {
    return calculateCostFunction(basis);
}
*/
