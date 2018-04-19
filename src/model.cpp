#include "model.h"

mpq_class Model::getProbabilityWordPerTag(unsigned int word, unsigned int tag) const {
    return (*probabilityWordPerTag)[tag][word];
}

mpq_class Model::getInitialProbability(Tag tag) const {
    return (*initialProbability)[tag];
}

mpq_class Model::getProbabilityTagGivenTag(Tag t1, Tag told) const {
    return (*probabilityTagGivenTag)[told][t1];
}
