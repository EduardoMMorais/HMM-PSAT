#include <cstdlib>
#include <ctime>
#include <cmath>
#include "trainer.h"

Trainer::Trainer()
{
    //ctor
}

void Trainer::train(Parser& p, float TestTrainRatio)
{
    int randNum, maxTestRandValue = (int) round((float)RAND_MAX * TestTrainRatio);
    bool sentenceForTest = false;
    list<string> currentTestSentence;
    list<Tag> currentTestSentenceStandard;
    pair<string, string> wordTag;

    unordered_map<unsigned int,unordered_map<Tag,unsigned int>> countWordPerTag;
    unordered_map<Tag,unordered_map<Tag, unsigned int>> countTagPerTag;
    unordered_map<Tag, unsigned int> initialCount;
    unordered_map<Tag, unsigned int> countTag, countTagPrev;
    unsigned int countInitial = 0;
    int previousTagInSentence = -1;
    srand(time(0));
    trainedSentences = 0;
    totalSentences = 0;
    randNum = rand();
    if (randNum <= maxTestRandValue) sentenceForTest = true;
    else sentenceForTest = false;
    do {
        wordTag = p.getNextToken();
        string &word = wordTag.first, &tag = wordTag.second;
        if (p.status != EOF) {
            Tag tagCode = getTagCode(tag);
            unsigned int wordCode = getWordCode(word);

            if (!sentenceForTest) {
                countTag[tagCode]++;
                countWordPerTag[wordCode][tagCode]++;
                if (previousTagInSentence == -1) {
                    countInitial++;
                    initialCount[tagCode]++;
                } else {
                    countTagPrev[previousTagInSentence]++;
                    countTagPerTag[previousTagInSentence][tagCode]++;
                }
            } else {
                currentTestSentence.push_back(word);
                currentTestSentenceStandard.push_back(tagCode);
            }

            if (p.status != EOL) {
                previousTagInSentence = tagCode;
            }
        }

        if (p.status == EOL || p.status == EOF) {
            if (!sentenceForTest)
                trainedSentences++;
            totalSentences++;

            if (!currentTestSentence.empty()) {
                testSentences.push_back(currentTestSentence);
                testSentencesStandard.push_back(currentTestSentenceStandard);
                currentTestSentence.clear();
                currentTestSentenceStandard.clear();
            }
            previousTagInSentence = -1;
            randNum = rand();
            if (randNum <= maxTestRandValue) sentenceForTest = true;
            else sentenceForTest = false;
        }
    } while (p.status != EOF);

    // Calcula probabilidades com smoothing
    numberOfWords = countWordPerTag.size();
    probabilityWordPerTag.resize(tagset.size());
    for(size_t pt = 0; pt < tagset.size(); pt++) {
        probabilityWordPerTag[pt].resize(numberOfWords);
        for(size_t t = 0; t < numberOfWords; t++) {
            probabilityWordPerTag[pt][t]=mpq_class(countWordPerTag[t][pt] + SMOOTHING_FACTOR, (int)(countTag[pt] + SMOOTHING_FACTOR * numberOfWords));
            probabilityWordPerTag[pt][t].canonicalize();
        }
    }


    //Tagset
    probabilityTagGivenTag.resize(tagset.size());
    for(size_t pt = 0; pt < tagset.size(); pt++) {
        probabilityTagGivenTag[pt].resize(tagset.size());
        for(size_t t = 0; t < tagset.size(); t++) {
            probabilityTagGivenTag[pt][t]=mpq_class(countTagPerTag[pt][t] + SMOOTHING_FACTOR, (int)(countTagPrev[pt] + SMOOTHING_FACTOR * tagset.size()));
            probabilityTagGivenTag[pt][t].canonicalize();
        }
    }

    //Tagset
    initialProbability.resize(tagset.size());
    for(size_t t = 0; t < tagset.size(); t++) {
        initialProbability[t]=mpq_class(initialCount[t] + SMOOTHING_FACTOR, (int)(countInitial + SMOOTHING_FACTOR * tagset.size()));
        initialProbability[t].canonicalize();
    }
}

Tag Trainer::getTagCode(const string& tag) {
    auto code = tagCodeMap.find(tag);
    if (code == tagCodeMap.end()) {
        tagset.push_back(tag);
        auto result = tagCodeMap.insert({tag,tagset.size()-1});
        code = result.first;
    }
    return code->second;
}

unsigned int Trainer::getWordCode(const string &word) {
    auto code = wordCodeMap.find(word);
    if (code == wordCodeMap.end()) {
        numberOfWords++;
        auto result = wordCodeMap.insert({word,numberOfWords-1});
        code = result.first;
    }
    return code->second;
}

Trainer::~Trainer()
{
    //dtor
}
