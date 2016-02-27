#include "allpayauction.hpp"

int main() {

  int numBids = 45;
  int numVals = 45;

  AllPayAuction apa(numVals,numBids);

  BCEGurobiSolver solver(apa);
  
  solver.populate();

  // Minimize Revenue by default.
  // Objective function 0 is player 0 payoff.
  // Objective function 1 is player 1 payoff.
  // Objective function 2 is seller revenue.
  solver.model.setObjective(solver.getObjectiveFunction(2),GRB_MINIMIZE);

  solver.solve();
  // solver.mapBoundary();

  BCESolution soln;

  solver.getSolution(soln);

  stringstream fName;
  fName << "allpayauction_nv=" << numVals
	<< "_nb=" << numBids
	<< "_nv=" << numVals << ".bce";
  string fNameStr = fName.str();
  const char * fNameC = fNameStr.c_str();
  BCESolution::save(soln,fNameC);

  return 0;

}
