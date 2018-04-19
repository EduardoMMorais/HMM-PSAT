#include <algorithm>
#include "psat.h"
#include "hmmmodel.h"

PSATInstance HMMModel::generatePSATInstance(const list<unsigned int> &sentence) {
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

    ClausalFormula sentenceFormula;
    for(wi = 0; wi < sentence.size(); wi++) {
        sentenceFormula.insert(getLiteralWord(wi, sentence.size()));
    }
    vector<Tag> tagset(sentence.size(), 0);
    do {
        ClausalFormula tagSetFormula;
        mpq_class sentenceProbabilityGivenTagSet(1,1),
            taggingProbability = getInitialProbability(tagset[0]);
        wi = 0;
        for(unsigned int word: sentence) {
            tagSetFormula.insert(getLiteralTagOfWord(tagset[wi], wi));
            sentenceProbabilityGivenTagSet *= getProbabilityWordPerTag(word, tagset[wi]);
            if (wi > 0)
                taggingProbability *= getProbabilityTagGivenTag(tagset[wi],tagset[wi-1]);
            wi++;
        }
        // 6. Propriedade da independência
        ConditionalFormula c(sentenceFormula, tagSetFormula);
        pi.emplace(c, sentenceProbabilityGivenTagSet);

        // 7.Propriedade de Markov + Matriz de transição de estados + Estados Iniciais
        pi.emplace(tagSetFormula, taggingProbability);

    } while(getNextTagging(tagset)); // Gera todos os taggings possíveis

    return pi;
}

Literal HMMModel::getLiteralTagOfWord(unsigned int tag, unsigned int word) const {
    return word * numOfTags + tag + 1;
}

Literal HMMModel::getLiteralWord(unsigned int word, unsigned int numOfWords) const {
    return numOfWords * numOfTags + word + 1;
}

list<Literal> HMMModel::getLiteralsSentence(unsigned int sentenceSize) {
    list<Literal> l;
    for (unsigned int i = 0; i<sentenceSize; i++)
        l.push_back(getLiteralWord(i, sentenceSize));
    return l;
}

list<Literal> HMMModel::getLiteralsTagging(vector<Tag> tags) {
    list<Literal> l;
    for (unsigned int i = 0; i<tags.size(); i++)
        l.push_back(getLiteralTagOfWord(tags[i], i));
    return l;
}

bool HMMModel::getNextTagging(vector<Tag> &tagset) const {
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
