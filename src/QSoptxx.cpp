#include <iostream>
#include "QSoptxx.h"

QSoptxx::QSoptxx(const vector<vector<mpq_class>>& columns, const vector<int>& cost, const vector<mpq_class>& rhs, int objsense): _cmatcnt(NULL),
    _cmatbeg(NULL), _cmatind(NULL), _cmatval(NULL), _sense(NULL), _colnames(NULL), _rownames(NULL), _obj(NULL), _rhs(NULL), _lower(NULL), _upper(NULL), _basis(NULL)
{
    QSexactStart(); // Inicializa variáveis estáticas. NÃO RELACIONADO COM ESTADO DO RESOLVEDOR
    QSexact_set_precision(128); // Seta precisão dos mpq para 128bits
    if (columns.size() > 0) {
        prepareVectors(columns, cost, rhs);
        _p = mpq_QSload_prob ("", _cols, _rows, _cmatcnt, _cmatbeg, _cmatind, _cmatval,
              objsense, _obj, _rhs, _sense, _lower, _upper, _colnames, _rownames);

        _problem_initialized = true;
    } else {
        _problem_initialized = false;
    }
}

void QSoptxx::dumpLP(const char* name) {
    mpq_QSwrite_prob (_p, name, "LP");
}

int QSoptxx::add_col(const vector<mpq_class>& col, const int cost) {
    mpq_t objective, lower, upper, *matval = new mpq_t[col.size()];
    int *ind = new int[col.size()];

    unsigned int cmatvalIdx = 0;
    for (unsigned i = 0; i < col.size(); i++) {
        const mpq_class& val = col[i];
        if (val != 0) {
            ind[cmatvalIdx] = i;
            mpq_init(matval[cmatvalIdx]);
            mpq_set(matval[cmatvalIdx], val.get_mpq_t());
            cmatvalIdx++;
        }
    }
    mpq_init(lower);
    mpq_init(upper);
    mpq_set(upper, mpq_ILL_MAXDOUBLE);
    mpq_init(objective);
    mpq_set_si(objective, cost, 1);
    int result = mpq_QSadd_col(_p, cmatvalIdx,
			   ind /* indices */,
			   matval, /* valores */
			   objective, lower, upper, NULL);
    if (!result) {
        _cols++;
    }
    delete[] matval;
    delete[] ind;
    return result;
}

void QSoptxx::prepareVectors(const vector<vector<mpq_class>>& columns, const vector<int>& cost, const vector<mpq_class>& rhs)
{
    cleanAuxVectors();
    _cols = columns.size();
    _cmatcnt = new int[_cols];
    _cmatbeg = new int[_cols];
    _colnames = new const char*[_cols];
    _obj = new mpq_t[_cols];
    _rows = rhs.size();
    _cmatind = new int[_cols * _rows]; // Could be smaller
    _cmatval = new mpq_t[_cols * _rows]; // Could be smaller
    _lower = new mpq_t[_cols];
    _upper = new mpq_t[_cols];
    unsigned int cmatvalIdx = 0;
    for (unsigned c=0; c<_cols; c++) {
        _colnames[c] = NULL;
        mpq_init(_obj[c]);
        mpq_set_si(_obj[c], cost[c], 1);

        _cmatbeg[c] = cmatvalIdx;
        unsigned int countVal = 0;
        for (unsigned i = 0; i < columns[c].size(); i++) {
            const mpq_class& val = columns[c][i];
            if (val != 0) {
                _cmatind[cmatvalIdx] = i;
                mpq_init(_cmatval[cmatvalIdx]);
                mpq_set(_cmatval[cmatvalIdx], val.get_mpq_t());
                countVal++;
                cmatvalIdx++;
            }
        }
        _cmatcnt[c] = countVal;
        mpq_init(_lower[c]);
        mpq_init(_upper[c]);
        mpq_set(_upper[c], mpq_ILL_MAXDOUBLE);
    }

    _rhs = new mpq_t[_rows];
    _rownames = new const char*[_rows];
    _sense = new char[_rows];
    for (unsigned i = 0; i < _rows; i++) {
        _sense[i]='E';
        mpq_init(_rhs[i]);
        mpq_set(_rhs[i], rhs[i].get_mpq_t());
        _rownames[i] = NULL;
    }

    // Debug vectors
    /*
    #define PRINT_VEC(x,y) std::cout << #x << ": "; for(int i=0; i<y; i++) std::cout << x[i]<<" "; std::cout << std::endl
    PRINT_VEC(_cmatcnt, _cols);
    PRINT_VEC(_cmatbeg, _cols);
    PRINT_VEC(_cmatind, _cols * _rows);
    PRINT_VEC(_cmatval, _cols * _rows);
    PRINT_VEC(_sense, _rows);
    PRINT_VEC(_obj, _cols);
    PRINT_VEC(_rhs, _rows);
    PRINT_VEC(_lower, _rows);
    PRINT_VEC(_upper, _rows);
    #undef PRINT_VEC
    */
}

int QSoptxx::solve() {
    if (!_problem_initialized) throw exception();
    int status;
    int ret = QSexact_solver(_p /* Problema */,
		 NULL /* mpq_t* para guardar solução primal */,
		 NULL /* solução dual */,
		 NULL /* QSBasis* base inicial e base final */,
		 DUAL_SIMPLEX /* Algoritmo */,
         &status);
    _basis = mpq_QSget_basis(_p);
    mpq_QSwrite_basis(_p,_basis,"base.lp");
    if (ret) return -1;
    return status;
}
mpq_class QSoptxx::getOptimalCost() {
    mpq_t cost;
    mpq_init(cost);
    mpq_QSget_objval(_p, &cost);
    mpq_class rVal(cost);
    mpq_clear(cost);
    return rVal;
}

void QSoptxx::cleanAuxVectors() {
    // TODO Desalocar mpq_t
    #define SAFE_DELETE(x) if (x != NULL) delete[] x
    SAFE_DELETE(_cmatcnt);
    SAFE_DELETE(_cmatbeg);
    SAFE_DELETE(_cmatind);
    SAFE_DELETE(_cmatval);
    SAFE_DELETE(_sense);
    SAFE_DELETE(_colnames);
    SAFE_DELETE(_rownames);
    SAFE_DELETE(_obj);
    SAFE_DELETE(_rhs);
    SAFE_DELETE(_lower);
    SAFE_DELETE(_upper);
    #undef SAFE_DELETE
}

void QSoptxx::getPrimalSolution(vector<mpq_class>& sol) const {
    /* Very inefficient...*/
    mpq_t* solucaoPrimal = new mpq_t[_cols];
    for (unsigned i=0; i<_cols; i++) {
        mpq_init(solucaoPrimal[i]);
    }
    mpq_QSget_x_array(_p, solucaoPrimal);
    sol.resize(_cols);
    for(unsigned i = 0; i < _cols; i++)
        sol[i] = mpq_class(solucaoPrimal[i]);
    for (unsigned i=0; i<_cols; i++) {
        mpq_clear(solucaoPrimal[i]);
    }
    delete[] solucaoPrimal;
}
void QSoptxx::getDualSolution(vector<mpq_class>& sol) const {
    /* Very inefficient...*/
    mpq_t* solucaoDual = new mpq_t[_rows];
    for (unsigned i=0; i<_rows; i++) {
        mpq_init(solucaoDual[i]);
    }
    mpq_QSget_pi_array(_p, solucaoDual);
    sol.resize(_rows);
    for(unsigned i = 0; i < _rows; i++)
        sol[i] = mpq_class(solucaoDual[i]);
    for (unsigned i=0; i<_rows; i++) {
        mpq_clear(solucaoDual[i]);
    }
    delete[] solucaoDual;
}

void QSoptxx::getReducedCostCoefs(vector<mpq_class>& coef) const {
    // Not the best solution, but works. It may select more columns than necessary, but it won't exclude columns
    getDualSolution(coef);
}

QSoptxx::~QSoptxx()
{
    cleanAuxVectors();
    mpq_QSfree_prob(_p);
    QSexactClear();
}
