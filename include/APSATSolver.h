#ifndef APSATSOLVER_H
#define APSATSOLVER_H
#include <vector>
#include <list> //TEMPORARY
#include <gmpxx.h>
#include <yices.h>
#include "psat.h"

using namespace std;
class APSATSolver
{
    public:
        APSATSolver(unsigned int nVars, const vector<mpq_class>& probs, const vector<vector<mpq_class>>& auxColumns, const ClausalFormula& gamma);
        ~APSATSolver();
        void setProbabilities(const vector<mpq_class>& probs);
        void setGamma(const ClausalFormula& gamma);
        void setAuxColumns(const vector<vector<mpq_class>>& auxColumns);
        void setAuxColumnsCost(const vector<int>& auxColumnsCost);
        void setNumVars(unsigned int);
        bool solve();
        mpq_class oSolve();
    protected:
        void generateNextColumn(vector<mpq_class>& gencol, const vector<mpq_class>& dualSoltion, context_t* ctx);
        term_t createGammaTerm();
    private:
        unsigned int _numVars;
        vector<mpq_class> _probabilities;
        vector<vector<mpq_class>> _auxiliaryColumns;
        vector<int> _auxiliaryColumnsCost;
        ClausalFormula _gamma;
        vector<term_t> _yices_vars;
};

#endif // APSATSOLVER_H
