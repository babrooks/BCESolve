#include "hybridentryreserve.hpp"

int main() {

  int numBids = 30;
  int numVals = 30;
  double weightOnOwnBid = 1;
  double reservePrice = .25;
  double entryFee = 0;

  HybridEntryReserve hera(numVals,
			  numBids,
			  weightOnOwnBid,
			  reservePrice,
			  entryFee);

  BCESolver solver(hera);
  
  solver.populate();
  
  // Minimize Revenue
  IloCplex cplex = solver.getCplex();
  // cplex.getObjective().setSense(IloObjective::Maximize);
  // cplex.getObjective().setExpr(.5*solver.getObjectiveFunction(0) + 
  // 			       .5*solver.getObjectiveFunction(1));
  cplex.getObjective().setSense(IloObjective::Minimize);
  cplex.getObjective().setExpr(solver.getObjectiveFunction(2));
  
  solver.solve();

  BCESolution data;
  solver.getSolution(data);
  
  cout << "Objective 0: " << cplex.getValue(solver.getObjectiveFunction(0)) << endl;
  cout << "Objective 1: " << cplex.getValue(solver.getObjectiveFunction(1)) << endl;
  cout << "Objective 2: " << cplex.getValue(solver.getObjectiveFunction(2)) << endl;

  stringstream fName;
  fName << "hybridauction_nv=" << numVals
	<< "_nb=" << numBids
	<< "_w=" << weightOnOwnBid
	<< "_rp=" << reservePrice 
	<< "_ef=" << entryFee << ".bce";
  string fNameStr = fName.str();
  const char * fNameC = fNameStr.c_str();
  BCESolution::save(data,fNameC);

}
