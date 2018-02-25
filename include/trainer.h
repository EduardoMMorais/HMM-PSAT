#ifndef TRAINER_H
#define TRAINER_H

#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include "c++11_warning.h"
#include "parser.h"
#include "model.h"
#include "gmpxx.h"

#define SMOOTHING_FACTOR 1
using namespace std;
class Trainer
{
    public:
        Trainer();
        virtual ~Trainer();
        void train(Parser& p, float TestTrainRatio);
        unsigned int getWordCode(const string &word);
        vector<vector<mpq_class>> probabilityWordPerTag;
        vector<vector<mpq_class>> probabilityTagGivenTag;
        vector<mpq_class> initialProbability;
        list<list<string>> testSentences;
        list<list<Tag>> testSentencesStandard;
        unsigned int trainedSentences;
        unsigned int totalSentences;
        unsigned int numberOfWords = 0;

        vector<string> tagset;
    protected:

    private:
        Tag getTagCode(const string& tag);
        unordered_map<string, Tag> tagCodeMap;
        map<string, unsigned int> wordCodeMap;
};

#endif // TRAINER_H
