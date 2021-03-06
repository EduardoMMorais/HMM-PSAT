#ifndef PSAT_H_INCLUDED
#define PSAT_H_INCLUDED

#include <set>
#include <map>
#include <vector>
#include <list>
#include <gmpxx.h>

#include "c++11_warning.h"

using namespace std;
class PSolverData;
typedef int Literal;

class Clause : public set<Literal> {
public:
    using set<Literal>::set; //Inherit the base class' constructors
    Clause(): set()  {};
    Clause(Literal c): set {c} {};
};

const extern ::Clause TrueClause;

class ClausalFormula : public set<::Clause> {
public:
    using set<::Clause>::set; //Inherit the base class' constructors
    ClausalFormula(const ::Clause &c): set({c}) {};
    ClausalFormula(): set() {};
};

const extern ClausalFormula TrueFormula;

class ConditionalFormula : public pair<ClausalFormula, ClausalFormula> {
public:
    using pair<ClausalFormula, ClausalFormula>::pair; //Inherit the base class' constructors
    ConditionalFormula(const ClausalFormula &c): pair(c, TrueFormula) {};
    ConditionalFormula(const ::Clause &c): pair(ClausalFormula(c), TrueFormula) {};
    ConditionalFormula(const ::Clause &c1, const ::Clause &c2): pair(ClausalFormula(c1), ClausalFormula(c2)) {};
};

class PSATInstance : public map<ConditionalFormula, mpq_class> {
public:
    using map<ConditionalFormula, mpq_class>::map; //Inherit the base class' constructors
    int getDimacsNumPr();
    int getDimacsNumVar();
    int getDimacsNumCl();
    mpq_class* getDimacsProbs();
    const ClausalFormula& getDimacsGamma();
    vector<vector<mpq_class>>& getDimacsExtraColumns();
    mpq_class getSolutionProbability(const list<Literal>& literals, PSolverData& psd);
    void solutionHasLiterals(const list<Literal>& literals, PSolverData& psd, int* columns);
private:
    void convertToDIMACS();
    int countVariables(const ConditionalFormula &cf);
    int countVariables(const ClausalFormula &clf);
    int countVariables(const ::Clause &cl);
    void addConditional(const ConditionalFormula &cf, mpq_class prob, map<Literal, mpq_class>& varsThatNeedProb);
    Literal getRemmapedLiteral(const Literal orig) const {return literalMap.at(orig);}
    Literal makeLiteral(const ClausalFormula& clf);
    Literal makeLiteral(const ::Clause&);
    void addToGamma(const ClausalFormula& clf);
    void remapVariables(map<Literal, mpq_class> &varsThatNeedProb);
    void normalizeExtraColumns();
    Literal makeLiteralConjunction(Literal l1, Literal l2);
    bool convertedToDIMACS = false;
    ClausalFormula gamma;
    int numProb;
    int numVar;
    int numCl;
    mpq_class* probs;
    vector<vector<mpq_class>> extraColumns;
    vector<map<int, mpq_class>> extraColumnsTmp;
    map<Literal, Literal> literalMap;
};

std::ostream& operator<< (std::ostream& stream, const PSATInstance& psat);
std::ostream& operator<< (std::ostream& stream, const ClausalFormula& cf);
std::ostream& operator<< (std::ostream& stream, const ::Clause& cl);
#endif // PSAT_H_INCLUDED
