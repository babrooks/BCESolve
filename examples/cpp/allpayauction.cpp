#include "allpayauction.hpp"

int main() {

  int numBids = 35;
  int numVals = 35;

  AllPayAuction apa(numVals,numBids);

  BCEGurobiSolver solver(apa);
  
  solver.populate();

  solver.model.setObjective(solver.getObjectiveFunction(2),GRB_MINIMIZE);
  solver.solve();

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
