#include <iostream>
#include "ClpSimplex.hpp"
#include "lp_lib.h"

#define INF 1e+10

template <class T>
int print_matrix(std::string name, int row, int col, T *mat) {
  std::cout << "[print_matrix] " << name << std::endl;
  for ( int r=0; r<row; r++ ) {
    std::cout << "   ";
    for ( int c=0; c<col; c++ ) {
      std::cout << " " << mat[c+r*col];
    }
    std::cout << std::endl;
  }
  return 0;
}

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
  int eus_lp_clp(int eq_dim, int ieq_dim, int state_dim, double* f, double* x, double* coeff, int _element_size, double* elements, long* _rowindices, long* _colindices, double* collower, double* colupper, double* rowlower, double* rowupper) {
    // std::cout << "[eus_lp_clp] start" << std::endl;
    int element_size = ( _element_size <= 0 ) ? ((eq_dim + ieq_dim) * state_dim) : _element_size;
    ClpSimplex model;
    int rowindices[element_size];
    int colindices[element_size];
    //
    if ( _element_size <= 0 ) {
      // std::cout << " -- auto row-col indices overwrite" << std::endl;
      // std::cout << " -- element_size:" << element_size << " = " << (eq_dim + ieq_dim) << " * " << state_dim << std::endl;
      for ( int row = 0; row<(eq_dim+ieq_dim) ; row++ ) { // not good
	for ( int col = 0 ; col<state_dim ; col++ ) {
	  // std::cout << " -- " << (row * state_dim + col) << " < " << element_size << std::endl;
	  rowindices[row * state_dim + col] = row;
	  colindices[row * state_dim + col] = col;
	}
      }
      // print_matrix("rowindices", ieq_dim, state_dim,  rowindices);
      // print_matrix("colindices", ieq_dim, state_dim,  colindices);
      // print_matrix("elements", ieq_dim, state_dim,  elements);
      // print_matrix("coeff", 1, state_dim,  coeff);
      // print_matrix("collower", 1, state_dim,  collower);
      // print_matrix("colupper", 1, state_dim,  colupper);
      // print_matrix("rowlower", 1, state_dim,  rowlower);
      // print_matrix("rowupper", 1, state_dim,  rowupper);
    } else {
      for ( int i=0 ; i<element_size ; i++ ) {
	rowindices[i] = _rowindices[i];
	colindices[i] = _colindices[i];
      }
    }
    //
    const CoinPackedMatrix matrix(false, rowindices, colindices, elements, element_size);
    //
    // std::cout << " -- start solve" << std::endl;
    model.loadProblem(matrix, collower, colupper, coeff, rowlower, rowupper);
    model.createStatus();
    model.primal();
    // std::cout << " -- ouput results" << std::endl;
    const double *colSolution = model.getColSolution();
    int ncol = model.numberColumns();
    for(int i=0; i<ncol; i++ ) x[i] = colSolution[i];
    f[0] = model.getObjValue();
    return ncol;
  }
}
