#include <iostream>
#include <cassert>
#include "psat.h"

const ::Clause TrueClause;
const ClausalFormula TrueFormula({});

std::ostream& operator<< (std::ostream& stream, const PSATInstance& psat) {
    for (const pair<ConditionalFormula, mpq_class> &condform: psat) {
        stream << "P(" << condform.first.first;
        if (!condform.first.second.empty())
            stream << " | " << condform.first.second;
        stream << ") = " << condform.second << "\n";
    }
    return stream;
}

std::ostream& operator<< (std::ostream& stream, const ClausalFormula& cf) {
    for (ClausalFormula::const_iterator it = cf.cbegin(); it != cf.cend(); ++it) {
        if (it != cf.cbegin())
            stream << " ∧ ";
        stream << *it;
    }
    return stream;
}

std::ostream& operator<< (std::ostream& stream, const ::Clause& cl) {
    stream << "(";
    for (::Clause::const_iterator it = cl.cbegin(); it != cl.cend(); ++it) {
        if (it != cl.cbegin())
            stream << " ∨ ";

        if (*it < 0)
            stream << "¬" << -(*it);
        else
            stream << (*it);
    }
    stream << ")";
    return stream;
}

int PSATInstance::getDimacsNumPr() {
    if (!convertedToDIMACS)
        convertToDIMACS();
    return numProb;
}
int PSATInstance::getDimacsNumVar()  {
    if (!convertedToDIMACS)
        convertToDIMACS();
    return numVar;
}
int PSATInstance::getDimacsNumCl() {
    if (!convertedToDIMACS)
        convertToDIMACS();
    return numCl;
}
mpq_class* PSATInstance::getDimacsProbs() {
    if (!convertedToDIMACS)
        convertToDIMACS();
    return probs;
}
const ClausalFormula& PSATInstance::getDimacsGamma() {
    if (!convertedToDIMACS)
        convertToDIMACS();
    return gamma;
}

vector<vector<mpq_class>>& PSATInstance::getDimacsExtraColumns() {
    if (!convertedToDIMACS)
        convertToDIMACS();
    return extraColumns;
}

void PSATInstance::convertToDIMACS() {
    map<Literal, mpq_class> varsThatNeedProb;
    // Count variables
    numVar = 0;
    for(auto it = cbegin(); it != cend(); it++) {
        int c = countVariables(it->first);
        if (c > numVar)
            numVar = c;
    }

    // Convert everything to Atomic Normal Form
    for(auto it = cbegin(); it != cend(); it++) {
        const ConditionalFormula &cf = it->first;
        if (cf.second.size() > 0) {
            addConditional(cf, it->second, varsThatNeedProb);
        } else {
            if (it->second != 1) {
                Literal l1 = makeLiteral(cf.first);
                varsThatNeedProb.emplace(l1, it->second);
            } else {
                addToGamma(cf.first);
            }
        }
    }
    numCl = gamma.size();
    numProb = varsThatNeedProb.size();
    probs = new mpq_class[numProb+1];
    remapVariables(varsThatNeedProb); // The vars that need prob NEED to come first...
    normalizeExtraColumns(); //We didn't know how many vars the extra columns needed
    convertedToDIMACS = true;
}

void PSATInstance::normalizeExtraColumns() {
    for(auto extraColumn: extraColumnsTmp) {
        vector<mpq_class> newColumn(numProb+1, 0);
        for(auto colPosition: extraColumn){
            newColumn[literalMap[colPosition.first]] = colPosition.second;
        }
        extraColumns.push_back(newColumn);
    }
    extraColumnsTmp.clear();
}

void PSATInstance::remapVariables(map<Literal, mpq_class> &varsThatNeedProb) {
    int varNum=1;
    probs[0] = 1;
    for(auto var: varsThatNeedProb) {
        literalMap.emplace(var.first,varNum);
        probs[varNum] = var.second;
        varNum++;
    }

    //Remap gamma
    ClausalFormula remapedGamma;
    for (auto it = gamma.begin(); it != gamma.end(); it ++) {
        ::Clause remapedClause;
        for (Literal l: *it) {
            Literal litOnMap;
            if (l < 0)
                litOnMap = -l;
            else
                litOnMap = l;
            if (literalMap.find(litOnMap) == literalMap.end()) {
                literalMap.emplace(litOnMap,varNum++);
            }
            if (l<0)
                remapedClause.insert(-literalMap[litOnMap]);
            else
                remapedClause.insert(literalMap[litOnMap]);
        }
        remapedGamma.insert(remapedClause);
    }
    gamma.swap(remapedGamma);
}

void PSATInstance::addToGamma(const ClausalFormula& clf) {
    for (const ::Clause& c: clf) {
        gamma.insert(c);
    }
}

int PSATInstance::countVariables(const ConditionalFormula &cf) {
    int c1 = countVariables(cf.first);
    int c2 = countVariables(cf.second);
    if (c1 > c2)
        return c1;
    return c2;
}

int PSATInstance::countVariables(const ClausalFormula &clf) {
    Literal maxLit=0;
    for (const ::Clause &cl: clf) {
        int c = countVariables(cl);
        if (c > maxLit)
            maxLit = c;
    }
    return maxLit;
}

int PSATInstance::countVariables(const ::Clause &cl) {
    Literal maxLit=0;
    for (const Literal lit: cl) {
        if (lit > maxLit)
            maxLit = lit;
    }
    return maxLit;
}

void PSATInstance::addConditional(const ConditionalFormula &cf, mpq_class prob, map<Literal, mpq_class>& varsThatNeedProb) {
    Literal l1 = makeLiteral(cf.first),
        l2 = makeLiteral(cf.second),
        l3 = makeLiteralConjunction(l1, l2);

    extraColumnsTmp.push_back({{l3,-prob},{l2, -1}});
    varsThatNeedProb.emplace(l3, 0);
    varsThatNeedProb.emplace(l2, 0);
}

Literal PSATInstance::makeLiteralConjunction(Literal l1, Literal l2) {
    Literal newLit = ++numVar;
    gamma.insert({{-newLit,l1}});
    gamma.insert({{-newLit,l2}});
    gamma.insert({{-l1,-l2,newLit}});
    return newLit;
}

Literal PSATInstance::makeLiteral(const ClausalFormula& clf) {
    if (clf.size() == 1)
        return makeLiteral(*(clf.cbegin()));
    else {
        Literal newLit = ++numVar;
        // X -> A ^ B (X -> A && X->B)
        for (const ::Clause& c: clf) {
            ::Clause newClause;
            newClause.insert(-newLit);
            for (const Literal l: c) {
                newClause.insert(l);
            }
            gamma.insert(newClause);
        }

        // A ^ B -> X (~A v ~B x X)
        vector<Literal> lits;
        for (const ::Clause& c: clf) {
            lits.push_back(makeLiteral(c));
        }
        ::Clause newClause;
        for (const Literal l: lits) {
            newClause.insert(-l);
        }
        newClause.insert(newLit);
        gamma.insert(newClause);
        return newLit;
    }
}

Literal PSATInstance::makeLiteral(const ::Clause& cl) {
    if (cl.size() == 1) {
        if (*(cl.cbegin()) < 0) {
            Literal negLit = *(cl.cbegin()), newLit = ++numVar;
            gamma.insert(::Clause({-newLit,negLit}));
            gamma.insert(::Clause({-negLit,newLit}));
            return newLit;
        }
        return *(cl.cbegin());
    } else {
        Literal newLit = ++numVar;
        // X -> A v B (~X v A v B)
        ::Clause newClause;
        newClause.insert(-newLit);
        for (const Literal l: cl) {
            newClause.insert(l);
        }
        gamma.insert(newClause);

        // A v B -> X (~A v X && ~B v X)
        for (const Literal l: cl) {
            ::Clause newClause;
            newClause.insert(-l);
            newClause.insert(newLit);
            gamma.insert(newClause);
        }
        return newLit;
    }
}

mpq_class PSATInstance::getSolutionProbability(const list<Literal>& literals, PSolverData& psd) {
    double prob = 0.0;
/*    double **extendedBasis = psd._models;
    const vector<double> &basicSolution = psd._basicSolution;

    //Calculate sentence and tagging Prob
    for(int column = 0; column <= getDimacsNumPr(); column++) {
        if (!extendedBasis[column]) continue;
        if (extendedBasis[column][0] != 1) continue;
        // Sentence
        bool hasAllLiterals = true;
        for (const Literal l : literals ) {
            Literal lRemapped = getRemmapedLiteral(l);
                if (! extendedBasis[column][lRemapped]) {
                    hasAllLiterals = false;
                    break;
                }
        }
        if (!hasAllLiterals)
            continue;

        prob += basicSolution[column];
    }*/
    return prob;
}

void PSATInstance::solutionHasLiterals(const list<Literal>& literals, PSolverData& psd, int* columns) {
    /*double **extendedBasis = psd._models;

    for(int column = 0; column <= getDimacsNumPr(); column++) {
        if (!extendedBasis[column]) continue;
        if (extendedBasis[column][0] != 1) continue;
        // Sentence
        bool hasAllLiterals = true;
        for (const Literal l : literals ) {
            Literal lRemapped = getRemmapedLiteral(l);
                if (! extendedBasis[column][lRemapped]) {
                    hasAllLiterals = false;
                    break;
                }
        }
        if (!hasAllLiterals)
            columns[column] = 1;
        else
            columns[column] = 0;
    }*/
}

