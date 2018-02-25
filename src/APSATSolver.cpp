#include <iostream>
#include <cstdint>
#include "APSATSolver.h"
#include "QSoptxx.h"

APSATSolver::APSATSolver(unsigned int nVars, const vector<mpq_class>& probs, const vector<vector<mpq_class>>& auxColumns, const ClausalFormula& gamma):
    _numVars(nVars),
    _gamma(gamma) {
    setProbabilities(probs);
    setAuxColumns(auxColumns);
    
    yices_init();
    }

void APSATSolver::setProbabilities(const vector<mpq_class>& probs) {
    _probabilities.resize(_numVars+1);
    _probabilities[0] = 1;
    for(unsigned int i = 0; i<_numVars; i++) {
        _probabilities[i+1] = probs[i];
    }
}

void APSATSolver::setAuxColumns(const vector<vector<mpq_class>>& auxColumns) {
    _auxiliaryColumns.clear();
    _auxiliaryColumns.resize(auxColumns.size());
    for(unsigned int col = 0; col < auxColumns.size(); col++) {
        _auxiliaryColumns[col].resize(_numVars+1);
        _auxiliaryColumns[col][0] = 1;
        for(unsigned int i = 0; i<_numVars; i++) {
            _auxiliaryColumns[col][i+1] = auxColumns[col][i];
        }
    }

}

void APSATSolver::setNumVars(unsigned int numVars) {
    _numVars = numVars;
}

void APSATSolver::setGamma(const ClausalFormula& gamma) {
    _gamma  = gamma;
}

term_t APSATSolver::createGammaTerm() {
    type_t bool_type = yices_bool_type();
    _yices_vars.resize(_numVars);
    term_t *clauses = new term_t[_gamma.size()];
    for(unsigned i = 0; i<_numVars; i++) {
        _yices_vars[i] = yices_new_uninterpreted_term(bool_type);
        #ifndef NDEBUG
        char name[4];
        name[0] = 'x';
        name[1] = '0' + (i / 10);
        name[2] = '0' + (i % 10);
        name[3] = '\0';
        yices_set_term_name(_yices_vars[i], name);
        #endif
    }

    unsigned ci = 0;
    for (const Clause &cl: _gamma) {
        term_t *clVars = new term_t[cl.size()];
        unsigned i = 0;
        for (const Literal &l: cl) {
            if (l > 0)
                clVars[i] = _yices_vars[l-1];
            else
                clVars[i] = yices_not(_yices_vars[(-l)-1]);
            i++;
        }
        clauses[ci] = yices_and(cl.size(), clVars);
        ci++;
        delete[] clVars;
    }
  
    term_t term = yices_or(_gamma.size(), clauses);
    #ifndef NDEBUG
    cout << "Term :";
    yices_pp_term(stdout, term, 80, 20, 0);
    #endif
    delete[] clauses;
    return term;
}

bool APSATSolver::solve() {
    bool isSAT = false, solving = true;
    unsigned int nIter = 0;

    _columnId = 0;

    vector<vector<mpq_class>> columns(_auxiliaryColumns);
    vector<int> cost(_auxiliaryColumns.size(), 0);
    // Basic columns
    for(unsigned int i=0; i <= _numVars; i++) {
        vector<mpq_class> col(_numVars+1);
        for(unsigned int j=0; j <= i; j++) {
            col[j] = 1;
        }
        columns.push_back(col);
        cost.push_back(1);
    }

    ctx_config_t *config = yices_new_config();
    yices_default_config_for_logic(config, "QF_LRA");
    yices_set_config(config, "mode", "push-pop");
    context_t *ctx = yices_new_context(config);
    yices_free_config(config);

    int32_t yi_code;
    yi_code = yices_assert_formula(ctx, createGammaTerm());
    if (yi_code < 0) {
        cerr << "Error asserting gamma: " << yices_error_code() << endl;
        yices_print_error(stderr);
        solving = false;
    }
    yices_push(ctx);
    yices_push(ctx);

    mpq_class lastcost(columns.size()+1);
    QSoptxx qs(columns, cost, _probabilities);
    while (solving) {
        #ifndef NDEBUG
        cout << "ITER: " << nIter << endl;
        #endif
        if( qs.solve() != QS_LP_OPTIMAL ) {
            solving = false;
            cerr << "Linear programming problem unfeasible. Dumping to dump.lp" << endl;
            qs.dumpLP("dump.lp");
            break;
        }
        mpq_class cost(qs.getOptimalCost());
        if (cost > lastcost) {
            solving = false;
            cerr << "Cost increased! Last iteration: " << lastcost << " Now: " << cost << endl;
            break;
        }
        #ifndef NDEBUG
        cout << "COST: " << cost << endl;
        #endif
        if (cost == 0) {
            isSAT = true;
            solving = false;
            break;
        }
        // Generate column
        vector<mpq_class> gencol, reducedCostCoefs;
        qs.getReducedCostCoefs(reducedCostCoefs);
        generateNextColumn(gencol, reducedCostCoefs, ctx);
        if (gencol.size() > 0)
            qs.add_col(gencol, 0);
        else
            solving = false;
        lastcost = cost;
        nIter++;
        #ifndef NDEBUG
        cout << endl;
        #endif
    }

    yices_free_context(ctx);
    return isSAT;
}

void APSATSolver::generateNextColumn(vector<mpq_class>& gencol, const vector<mpq_class>& reducedCostCoefs, context_t* ctx) {
    gencol.clear();
    // O custo da coluna atual que estamos gerando é 0 ou 1 dependendo se ela satisfaz ou não a fórmula que queremos otimizar
    // Precisa inserir a fórmula que queremos otimizar como um termo no Yices
    term_t *vars = new term_t[reducedCostCoefs.size()];
    vars[0] = yices_mpq(reducedCostCoefs[0].get_mpq_t());
    for(unsigned i=1; i<reducedCostCoefs.size(); i++) {
        vars[i] = yices_ite(_yices_vars[i-1], yices_mpq(reducedCostCoefs[i].get_mpq_t()), yices_zero());
    }
    term_t eq = yices_arith_geq0_atom(yices_sum(reducedCostCoefs.size(), vars)) ;
    #ifndef NDEBUG
    cout << "Negative cost:\n";
    yices_pp_term(stdout, eq, 80, 20, 0);
    #endif
    int32_t yi_code;
    yi_code = yices_assert_formula(ctx, eq);
    if (yi_code < 0) {
        cerr << "Error asserting inequality: " << yices_error_code() << endl;
        yices_print_error(stderr);
        return;
    }
    delete[] vars;

    if (yices_check_context(ctx, NULL) == STATUS_SAT) {
        model_t* model;
        model = yices_get_model(ctx, true);
        
        #ifndef NDEBUG
        cout << "Model:\n";
        yices_pp_model(stdout, model, 80, 4, 0); // print the model
        #endif

        gencol.resize(_numVars+1);
        gencol[0] = 1;
        term_t *current_model = new term_t[_numVars];
        for (unsigned i=0; i<_numVars; i++) {
            int32_t val;
            yices_get_bool_value(model, _yices_vars[i], &val);
            gencol[i+1] = val;
            if (val)
                current_model[i] = _yices_vars[i];
            else
                current_model[i] = yices_not(_yices_vars[i]);
        }
        yices_pop(ctx);
        yices_assert_formula(ctx, yices_not(yices_and(_numVars, current_model)));
        yices_push(ctx);
        delete[] current_model;
    }
}

mpq_class APSATSolver::oSolve() {
    return mpq_class(0.5);
}

APSATSolver::~APSATSolver() {
    yices_exit();
}
