#include "hybridentryreserve.hpp"
#include "bcegurobisolver.hpp"

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

  BCEGurobiSolver solver(hera);
  
  solver.populate();
  
  // cplex.getObjective().setSense(IloObjective::Minimize);
  // cplex.getObjective().setExpr(solver.getObjectiveFunction(2));
  // GRBLinExpr objective = 0;
  // objective += .5 * solver.getObjectiveFunction(0);
  // objective += .5 * solver.getObjectiveFunction(1);
  // solver.model.setObjective(objective,GRB_MAXIMIZE);
  solver.model.setObjective(solver.getObjectiveFunction(2),GRB_MINIMIZE);  
  solver.model.update();

  solver.solve();

  BCESolution data;
  solver.getSolution(data);
  
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
