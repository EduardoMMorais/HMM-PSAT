#ifndef QSOPTXX_H
#define QSOPTXX_H

#include <vector>
#include <gmpxx.h>
extern "C" {
#include "QSopt_ex.h"
}

using namespace std;

class QSoptxx
{
    public:
        QSoptxx(const vector<vector<mpq_class>>& columns, const vector<int>& cost, const vector<mpq_class>& rhs, int objsense = QS_MIN);
        virtual ~QSoptxx();
        int solve();
        void dumpLP(const char* name);
        mpq_class getOptimalCost();
        int add_col(const vector<mpq_class>& col, const int cost);
        void getPrimalSolution(vector<mpq_class>& sol) const;
        void getDualSolution(vector<mpq_class>& sol) const;
        void getReducedCostCoefs(vector<mpq_class>& coefs) const;

        //vector<mpq_class> getSolution();
    protected:
        mpq_QSprob _p;
    private:
        void cleanAuxVectors();
        void prepareVectors(const vector<vector<mpq_class>>& columns, const vector<int>& cost, const vector<mpq_class>& rhs);

        bool _problem_initialized;
        // Temporary variables
        int* _cmatcnt; // Quantidade de variáveis por coluna
        int* _cmatbeg; // Posição em cmatval que começa essa coluna
        int* _cmatind; // A qual linha se refere cada coluna
        mpq_t* _cmatval; // Base por coluna (cmatval[x] = A[i][j], x=i + j*L)
        char* _sense; // 'L' <=, 'E' =
        const char ** _colnames;
        const char ** _rownames;
        mpq_t* _obj; // Vetor de custo
        mpq_t* _rhs; // RHS
        mpq_t* _lower; // Limite inferior (mpq_ILL_MINDOUBLE para infinito)
        mpq_t* _upper; // Limite superior (mpq_ILL_MAXDOUBLE para infinito)
        unsigned int _rows;
        unsigned int _cols;
        QSbasis *_basis;
};

#endif // QSOPTXX_H
