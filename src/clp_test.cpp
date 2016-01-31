#include <iostream>
#include "ClpSimplex.hpp"
// #include "CoinLpIO.hpp"

// min -2.5 a + -5 b + -3.4 c
// subject to
//         2 a + 10 b + 4 c <= 425
//         6 a + 5  b + 8 c <= 400
//         7 a + 10 b + 8 c <= 600
// bounds
//         a >= 0
// 			    b >= 0
//         c >= 0
// end

#define INF 1e+10

int main( int argc, char **argv)
{
  ClpSimplex model;
  //
  const int rowindices[] = {0,1,2,0,1,2,0,1,2};
  const int colindices[] = {0,0,0,1,1,1,2,2,2};
  const double elements[] = {2,6,7,10,5,10,4,8,8};
  const CoinPackedMatrix matrix(false, rowindices, colindices, elements, 9);
  //
  const double collower[] = {0,0,0};
  const double colupper[] = {INF,INF,INF};
  const double coeff[] = {-2.5, -5, -3.4};
  const double rowlower[] = {-INF,-INF,-INF};
  const double rowupper[] = {425,400,600};
  //
  model.loadProblem(matrix, collower, colupper, coeff, rowlower, rowupper);
  model.createStatus();
  model.primal();
  //
  const double *colSolution = model.getColSolution();
  // int ncol = model.numberColumns();
  for(int i=0; i<3; i++ ) {
    std::cout << model.getColumnName(i)
              << ": " << colSolution[i] << std::endl;
  }

  return 0;
}

/*
int main( int argc, char **argv)
{
  ClpSimplex model;
  int status;
  char *lp_path;
  char lp_path_org[] = "test.lp";
  FILE *fp;
  CoinLpIO m;
  double epsilon = 1e-5;
  int ncol, nrow;
  //
  if ( argc >= 2 ) lp_path = argv[1];
  else lp_path = lp_path_org;

  //
  //status = model.readLp(lp_path);
  //
  fp = fopen(lp_path, "r");
  if(!fp) {
    std::cout << lp_path << " not found" << std::endl;
    return -1;
  }
  //
  m.readLp(fp, epsilon);
  fclose(fp);
  // set problem name
  // model.setStrParam(ClpProbName, m.getProblemName());
  // no errors
  model.loadProblem(*m.getMatrixByRow(), m.getColLower(), m.getColUpper(),
		    m.getObjCoefficients(), m.getRowLower(), m.getRowUpper());
  //
  ncol = model.numberColumns();
  nrow = model.numberRows();
  // std::cout << "getMatrixByRow" << std::endl;
  // std::cout << *m.getMatrixByRow() << std::endl;
  std::cout << "getColLower" << std::endl;
  for ( int i=0; i<ncol; i++ ) {
    std::cout << " " << (m.getColLower())[i];
  }
  std::cout << std::endl;
  std::cout << "getColUpper" << std::endl;
  for ( int i=0; i<ncol; i++ ) {
    std::cout << " " << (m.getColUpper())[i];
  }
  std::cout << std::endl;
  std::cout << "getRowLower" << std::endl;
  for ( int i=0; i<nrow; i++ ) {
    std::cout << " " << (m.getRowLower())[i];
  }
  std::cout << std::endl;
  std::cout << "getRowUpper" << std::endl;
  for ( int i=0; i<nrow; i++ ) {
    std::cout << " " << (m.getRowUpper())[i];
  }
  std::cout << std::endl;
  //
  // if (m.integerColumns()) {
  //   model.integerType_ = new char[numberColumns_];
  //   CoinMemcpyN(m.integerColumns(), model.numberColumns_, model.integerType_);
  // } else {
  //   integerType_ = NULL;
  // }
  model.createStatus();
  //
  // Perform the optimization
  model.primal();
  //
  // Print the solution
  const double *colSolution = model.getColSolution();
  for(int i=0; i<ncol; i++ ) {
    std::cout << model.getColumnName(i)
              << ": " << colSolution[i] << std::endl;
  }

  return 0;
}
*/
