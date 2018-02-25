#include "model.h"

double Model::getProbabilityWordPerTag(unsigned int word, unsigned int tag) const {
    return (*probabilityWordPerTag)[tag][word];
}

double Model::getInitialProbability(Tag tag) const {
    return (*initialProbability)[tag];
}

double Model::getProbabilityTagGivenTag(Tag t1, Tag told) const {
    return (*probabilityTagGivenTag)[told][t1];
}
