#include <iostream>
#include "ClpSimplex.hpp"
#include "lp_lib.h"

#define INF 1e+10

extern "C" {
  int eus_lp_lpsolve(int eq_dim, int ieq_dim, int state_dim, double* f, double* x, double* objective, double* ieq_coeff, double* ieq_matrix, double* eq_coeff, double* eq_matrix) {
    // the first element of all row vectors must be zero;
    lprec *lp;
    lp = make_lp(eq_dim+ieq_dim, state_dim);
    set_obj_fn (lp, objective);
    for(int i=0; i<ieq_dim; i++) {
      add_constraint( lp, ieq_matrix + i*state_dim, LE, ieq_coeff[i] );
    }
    for(int i=0; i<eq_dim; i++) {
      add_constraint( lp, eq_matrix + i*state_dim, EQ, eq_coeff[i] );
    }
    set_verbose(lp, 0);
    set_presolve(lp, PRESOLVE_ROWS | PRESOLVE_COLS | PRESOLVE_LINDEP, get_presolveloops(lp));
    solve(lp);
    f[0] = get_objective(lp);
    return get_Norig_columns(lp);
  }
  //
  // int eus_lp_clp(double* f, double* x, int element_size, int* rowindices, int* colindices, double* elements, double* collower, double* colupper, double* coeff, double* rowlower, double* rowupper) {
  int eus_lp_clp(int eq_dim, int ieq_dim, int state_dim, double* f, double* x, int element_size, double* elements, long* _rowindices, long* _colindices, double* collower, double* colupper, double* coeff, double* rowlower, double* rowupper) {
    if ( ! _rowindices || ! _colindices ) element_size = (eq_dim + ieq_dim) * state_dim;
    ClpSimplex model;
    int rowindices[element_size];
    int colindices[element_size];
    const CoinPackedMatrix matrix(false, rowindices, colindices, elements, 9);
    //
    if ( ! _rowindices || ! _colindices ) {
      for ( int row = 0; row<eq_dim+ieq_dim ; row++ ) { // not good
	for ( int col = 0 ; col<state_dim ; col++ ) {
	  rowindices[row * state_dim + col] = row;
	  colindices[row * state_dim + col] = col;
	}
      }
    } else {
      for ( int i=0 ; i<element_size ; i++ ) {
	rowindices[i] = _rowindices[i];
	colindices[i] = _colindices[i];
      }
    }
    //
    model.loadProblem(matrix, collower, colupper, coeff, rowlower, rowupper);
    model.createStatus();
    model.primal();
    const double *colSolution = model.getColSolution();
    int ncol = model.numberColumns();
    for(int i=0; i<ncol; i++ ) x[i] = colSolution[i];
    f[0] = model.getObjValue();
    return ncol;
  }
}
