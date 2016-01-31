#include <iostream>
#include "lp_lib.h"

// max: 2.5 x1 + 5 x2 + 3.4 x3;
// C1: 2 x1 + 10 x2 + 4 x3 <= 425;
// C2: 6 x1 + 5 x2 + 8 x3 <= 400;
// C3: 7 x1 + 10 x2 + 8 x3 <= 600;
// # test.sh
// Value of objective function: 266.25
// Actual values of the variables:
// x1                             30
// x2                             34
// x3                           6.25

#define eq_dim 0
#define ieq_dim 3
#define state_dim 3


int main( ) {
  double x[state_dim+1];
  double objective[state_dim+1] = {0, -2.5, -5.0, -3.4};
  double ieq_coeff[ieq_dim] = {425, 400, 600};
  double ieq_matrix[ieq_dim][state_dim+1]
    = {{0,2,10,4}, {0,6,5,8}, {0, 7, 10, 8}};
  double eq_coeff[eq_dim]; // = {30};
  double eq_matrix[eq_dim][state_dim+1];
  //
  lprec *lp;
  //
  lp = make_lp(eq_dim+ieq_dim, state_dim);
  set_obj_fn (lp, objective);
  for(int i=0; i<ieq_dim; i++) {
    add_constraint( lp, ieq_matrix[i], LE, ieq_coeff[i] );
  }
  for(int i=0; i<eq_dim; i++) {
    add_constraint( lp, eq_matrix[i], EQ, eq_coeff[i] );
  }
  //
  set_verbose(lp, 0);
  set_presolve(lp, PRESOLVE_ROWS | PRESOLVE_COLS | PRESOLVE_LINDEP, get_presolveloops(lp));
  solve(lp);
  //
  std::cout << "f : " << get_objective(lp) << std::endl;
  std::cout << "x :"  ;
  double col0 = get_Norig_columns(lp);
  double row0 = get_Norig_rows(lp);
  for(int i = 1; i <= col0; i++) {
    double _x = get_var_primalresult(lp, row0 + i);
    std::cout << " " << _x ;
  }
  std::cout << std::endl;
}
