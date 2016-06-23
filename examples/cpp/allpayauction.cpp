#include "allpayauction.hpp"

int main() {

  int numBids = 45;
  int numVals = 45;

  AllPayAuction apa(numVals,numBids);

  BCESolver solver(apa);

  try {
  solver.populate();
  }
  catch(BCEException &e) {
    cout << e.getMessage() << endl;
  }
  
  // Minimize Revenue by default.
  // Objective function 0 is player 0 payoff.
  // Objective function 1 is player 1 payoff.
  // Objective function 2 is seller revenue.
  vector<double> objWeights(3);
  objWeights[2]=-1;
  
  try {
    solver.solve(objWeights);
  }
  catch(BCEException &e) {
    cout << e.getMessage() << endl;
  }
  // solver.mapBoundary();

  BCESolution soln;

  solver.getSolution(soln);

  stringstream fName;
  fName << "allpayauction_nv=" << numVals
	<< "_nb=" << numBids << ".bce";
  string fNameStr = fName.str();
  const char * fNameC = fNameStr.c_str();
  BCESolution::save(soln,fNameC);

  return 0;

}
