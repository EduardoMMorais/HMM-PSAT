#include <iostream>
#include "c++11_warning.h"
#include "psat.h"
#include "parser.h"
#include "trainer.h"

using namespace std;

void tagWithMostProb(Trainer &trainer);

int main()
{
    setlocale( LC_ALL, "" );
    //Parser parser("data/brown.txt");
    Parser parser("data/all2.txt");
    //Parser parser("data/test.txt");
    Trainer trainer;
    cout << "Treinando ..." << endl;
    trainer.train(parser, 0.1);
    /*cout << "Tagset: ";
    for (unsigned int i = 0; i < trainer.tagset.size(); i++) {
        cout << i << ' ' << trainer.tagset[i] << ' ' << endl;
    }*/
    cout << endl;
    cout << "Total " << trainer.tagset.size() << " tags" <<endl;
    cout << endl;

    cout << "Most probable tag: " << endl;
    tagWithMostProb(trainer);
    cout << "NonMarkovian: " << endl;
    return 0;
}

Tag getMostProbableTag(Trainer &t, unsigned int w) {
    mpq_class prob(0);
    Tag bestTag = 0;
    for(Tag i = 0; i<t.probabilityWordPerTag.size(); i++) {
        if (w >= t.probabilityWordPerTag[i].size())
            continue;
        if (t.probabilityWordPerTag[i][w] > prob) {
            bestTag = i;
            prob = t.probabilityWordPerTag[i][w];
        }
    }
    return bestTag;
}

void tagWithMostProb(Trainer &trainer) {
    unsigned int acertosTag = 0, acertosSentence = 0, totalTags = 0, totalSentences = 0;
    list<list<Tag>>::const_iterator goldStandardIterator = trainer.testSentencesStandard.cbegin();
    for (const list<string> &sentence: trainer.testSentences) {
        const list<Tag> &goldStandard = *goldStandardIterator;
        list<string>::const_iterator sentWordIt = sentence.cbegin();
        bool sentenceCorreta = true;
        for(const Tag& gs: goldStandard) {
            totalTags++;
            Tag mostProb = getMostProbableTag(trainer, trainer.getWordCode(*sentWordIt));
            if (gs == mostProb)
                acertosTag++;
            else
                sentenceCorreta = false;
            ++sentWordIt;
        }
        if (sentenceCorreta) {
            ++acertosSentence;
        }
        ++totalSentences;
        ++goldStandardIterator;
    }

    cout << "Total de acertos (tags): " << acertosTag << " / " << totalTags << "(" << ((double)acertosTag/totalTags) << ")" << endl;
    cout << "Total de acertos (senteces): " << acertosSentence << " / " << totalSentences << "(" << ((double)acertosSentence/totalSentences) << ")" << endl;
}
